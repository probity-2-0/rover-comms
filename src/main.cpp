#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// ============================================================================
// --- FORWARD DECLARATION STRUCTURES (Aligned across Nodes A, B, and C) ---
// ============================================================================
typedef struct __attribute__((packed)) {
    int id;           // 1 for Node A
    int joyX;         
    int joyY;         
    bool btnState;    
    int intensity;    // Target PWM value (0-255)
} tx_message_t;

// ============================================================================
// --- BTS7960 MOTOR PIN CONFIGURATIONS ---
// ============================================================================
// Back Right Wheel
#define BR_RPWM 21
#define BR_LPWM 22

// Middle Right + Front Right Wheels
#define RR_RPWM 23
#define RR_LPWM 25

// Middle Left + Back Left Wheels
#define LR_RPWM 16
#define LR_LPWM 17

// Front Left Wheel
#define FL_RPWM 27
#define FL_LPWM 26

// --- PWM Architecture Allocation Setup ---
#define MOTOR_FREQ     10000 // 10kHz ultra-sonic frequency removes driver hum
#define MOTOR_RES      8     // 8-bit resolution matches intensity limits (0-255)

// Unique Hardware Channels (Every individual pin gets a dedicated channel)
#define CH_BR_R  0
#define CH_BR_L  1
#define CH_RR_R  2
#define CH_RR_L  3
#define CH_LR_R  4
#define CH_LR_L  5
#define CH_FL_R  6
#define CH_FL_L  7

// Joystick Axis Threshold Profiles
#define JOY_CENTER    1875
#define JOY_DEADZONE  200  // Ignore jitter around the center point

// Authorized Middleman Node B MAC Address
uint8_t macB[] = {0x68, 0x09, 0x47, 0x5d, 0x14, 0xbc};

// FreeRTOS Inter-Task Data Pipeline Queue Handle
QueueHandle_t motorQueue = NULL;

// ============================================================================
// --- COMPONENT MOTOR CONTROL FUNCTIONS (Variable PWM Driven) ---
// ============================================================================

void updateDriveTrain(int brR, int brL, int rrR, int rrL, int lrR, int lrL, int flR, int flL) {
    ledcWrite(CH_BR_R, brR); ledcWrite(CH_BR_L, brL);
    ledcWrite(CH_RR_R, rrR); ledcWrite(CH_RR_L, rrL);
    ledcWrite(CH_LR_R, lrR); ledcWrite(CH_LR_L, lrL);
    ledcWrite(CH_FL_R, flR); ledcWrite(CH_FL_L, flL);
}

void stopMotors() {
    updateDriveTrain(0, 0, 0, 0, 0, 0, 0, 0);
}

// Drive states adapt automatically to the speed intensity requested by Node A
void moveForward(int speed) {
    updateDriveTrain(speed, 0, speed, 0, speed, 0, speed, 0);
}

void moveBackward(int speed) {
    updateDriveTrain(0, speed, 0, speed, 0, speed, 0, speed);
}

void pivotLeft(int speed) {
    // Right side forward, Left side backward
    updateDriveTrain(speed, 0, speed, 0, 0, speed, 0, speed);
}

void pivotRight(int speed) {
    // Left side forward, Right side backward
    updateDriveTrain(0, speed, 0, speed, speed, 0, speed, 0);
}

// ============================================================================
// --- NETWORK DATA INTERCEPT HOOK (Fixed for older framework support) ---
// ============================================================================
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingDataBytes, int len) {
    // Security validation: Only accept packets originating from Node B
    if (memcmp(mac, macB, 6) != 0) return; 
    
    if (len == sizeof(tx_message_t)) {
        tx_message_t receivedInput;
        memcpy(&receivedInput, incomingDataBytes, sizeof(tx_message_t));
        // Instantly push to processing loop to prevent hardware interrupt starvation
        xQueueSendFromISR(motorQueue, &receivedInput, NULL);
    }
}


// ============================================================================
// --- FREERTOS ASYNCHRONOUS ACTUATION TASK ---
// ============================================================================
void vTaskMotorController(void *pvParameters) {
    tx_message_t input;
    
    for (;;) {
        // Blocks efficiently with 0% CPU consumption until Node B forwards a packet
        if (xQueueReceive(motorQueue, &input, portMAX_DELAY) == pdPASS) {
            
            int speed = input.intensity; // Scaled 0-255 PWM value
            int xOffset = input.joyX - JOY_CENTER;
            int yOffset = input.joyY - JOY_CENTER;

            // Direct hardware fallback: if stick is at rest, turn off all drivers
            if (speed == 0 || (abs(xOffset) < JOY_DEADZONE && abs(yOffset) < JOY_DEADZONE)) {
                stopMotors();
                continue;
            }

            // Direction parsing algorithm based on maximum absolute deflection vector
            if (abs(yOffset) >= abs(xOffset)) {
                // Dominant vertical axis movement
                if (yOffset > 0) {
                    moveBackward(speed); // Adapt orientation bounds if inverted
                    Serial.printf("[DRIVE] Backward Speed: %d\n", speed);
                } else {
                    moveForward(speed);
                    Serial.printf("[DRIVE] Forward Speed: %d\n", speed);
                }
            } else {
                // Dominant horizontal axis movement
                if (xOffset > 0) {
                    pivotRight(speed);
                    Serial.printf("[DRIVE] Pivoting Right Speed: %d\n", speed);
                } else {
                    pivotLeft(speed);
                    Serial.printf("[DRIVE] Pivoting Left Speed: %d\n", speed);
                }
            }
        }
    }
}

// ============================================================================
// --- CORE SYSTEM SETUP ---
// ============================================================================
void setup() {
    Serial.begin(115200);

    // Bind Pins to PWM Generation Channels
    ledcAttachPin(BR_RPWM, CH_BR_R); ledcAttachPin(BR_LPWM, CH_BR_L);
    ledcAttachPin(RR_RPWM, CH_RR_R); ledcAttachPin(RR_LPWM, CH_RR_L);
    ledcAttachPin(LR_RPWM, CH_LR_R); ledcAttachPin(LR_LPWM, CH_LR_L);
    ledcAttachPin(FL_RPWM, CH_FL_R); ledcAttachPin(FL_LPWM, CH_FL_L);

    // Initialise PWM Profiles globally across the pin registers
    ledcSetup(CH_BR_R, MOTOR_FREQ, MOTOR_RES); ledcSetup(CH_BR_L, MOTOR_FREQ, MOTOR_RES);
    ledcSetup(CH_RR_R, MOTOR_FREQ, MOTOR_RES); ledcSetup(CH_RR_L, MOTOR_FREQ, MOTOR_RES);
    ledcSetup(CH_LR_R, MOTOR_FREQ, MOTOR_RES); ledcSetup(CH_LR_L, MOTOR_FREQ, MOTOR_RES);
    ledcSetup(CH_FL_R, MOTOR_FREQ, MOTOR_RES); ledcSetup(CH_FL_L, MOTOR_FREQ, MOTOR_RES);

    // Emergency physical state isolation at startup
    stopMotors();

    // Boot Up Networking Layer
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("CRITICAL: ESP-NOW Architecture Failed to Init.");
        return;
    }

#if defined(ESP_IDF_VERSION_MAJOR) && ESP_IDF_VERSION_MAJOR >= 2
    esp_now_register_recv_cb(OnDataRecv);
#else
    esp_now_register_recv_cb(OnDataRecv);
#endif

    // Bind middleman Peer Node B profile for validation security filters
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, macB, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);

    // Allocate Task Queue Memory Buffer Size
    motorQueue = xQueueCreate(10, sizeof(tx_message_t));
    if (motorQueue == NULL) {
        Serial.println("CRITICAL: Queue Allocation Matrix Space Blown.");
        return;
    }

    // Spawn Multithread Processing Loops Pinned into Core 1
    xTaskCreatePinnedToCore(
        vTaskMotorController, 
        "BTS7960DriveEngine", 
        3072, 
        NULL, 
        3, // Highest priority to handle movement immediately
        NULL, 
        1
    );

    Serial.println("Node C Real-Time Variable Speed Receiver Initialized Successfully.");
}

void loop() {
    vTaskDelete(NULL); 
}