#include <stdint.h>

/******************************************************************************
 * Input Command
 ******************************************************************************/

typedef struct
{
    uint8_t ucW;
    uint8_t ucA;
    uint8_t ucS;
    uint8_t ucD;
} InputCommand_t;

/******************************************************************************
 * Motor Command
 ******************************************************************************/

typedef struct
{
    int16_t sLeftSpeed;
    int16_t sRightSpeed;
} MotorCommand_t;

/******************************************************************************
 * Sensor Data
 ******************************************************************************/

typedef struct
{
    uint16_t usBattery;

    int16_t sAccelX;
    int16_t sAccelY;
    int16_t sAccelZ;
} SensorData_t;

/******************************************************************************
 * Telemetry
 ******************************************************************************/

typedef struct
{
    SensorData_t xSensor;

    int16_t sLeftMotor;
    int16_t sRightMotor;
} Telemetry_t;

/******************************************************************************
 * Helper Functions
 ******************************************************************************/

static inline void vMotorCommandInit(
    MotorCommand_t *pxCommand)
{
    pxCommand->sLeftSpeed = 0;
    pxCommand->sRightSpeed = 0;
}

static inline void vTelemetryInit(
    Telemetry_t *pxTelemetry)
{
    pxTelemetry->sLeftMotor = 0;
    pxTelemetry->sRightMotor = 0;

    pxTelemetry->xSensor.usBattery = 0;

    pxTelemetry->xSensor.sAccelX = 0;
    pxTelemetry->xSensor.sAccelY = 0;
    pxTelemetry->xSensor.sAccelZ = 0;
}