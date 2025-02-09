#include <arduino-timer.h>
#include <HardwareSerial.h>
#include <ModbusRTU.h>

#include "rs485.h"
#include "WS_GPIO.h"

#ifndef SENSOR_ID
#define SENSOR_ID 1
#endif

bool time_delay_done(void *arg);
bool timeout_while_reading_modbus(void *arg);
void update_temperature_and_humidity(struct sensor_context*);

void *rate_limit_task;
auto rate_limit_timer = timer_create_default();
void *modbus_timeout_task;
auto modbus_timeout_timer = timer_create_default();
HardwareSerial rs485Serial(1);
ModbusRTU mb;

enum mb_state {
    READY_TO_REQUEST,
    WAITING_FOR_RESPONSE,
    WAITING_FOR_TIME_DELAY,
};

struct sensor_context
{
    uint8_t slave_id;
    enum mb_state mb_state;
    uint16_t inputRegs[2]; // temperature & humidity values read from XY-MD01 device
    int relative_humidity_percent_x10;
    int temperature_degC_x10;
    int milliseconds_to_delay_between_readings;
};

static sensor_context context =
{
    .slave_id = SENSOR_ID,
    .mb_state = READY_TO_REQUEST,
    .inputRegs = { 0 },
    .relative_humidity_percent_x10 = 0,
    .temperature_degC_x10 = 0,
    .milliseconds_to_delay_between_readings = 250,
};

bool time_delay_done(void *arg)
{
    struct sensor_context * const c = (struct sensor_context *)arg;
    printf("t = %d, rh = %d\r\n", c->temperature_degC_x10, c->relative_humidity_percent_x10);
    c->mb_state = READY_TO_REQUEST;
    return true;
}

bool timeout_while_reading_modbus(void *arg)
{
    printf("timeout_while_reading_modbus\r\n");
    ((struct sensor_context *)arg)->mb_state = READY_TO_REQUEST;
    return true;
}

bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data)
{
    modbus_timeout_timer.cancel(modbus_timeout_task);
    if (event != Modbus::EX_SUCCESS)
    {
        printf("Error response from RS485 / ModbusRTU; event = 0x%08X\r\n", event);
        context.mb_state = READY_TO_REQUEST;
    }
    else
    {
        update_temperature_and_humidity(&context);
        context.mb_state = WAITING_FOR_TIME_DELAY;
        rate_limit_task = rate_limit_timer.in(context.milliseconds_to_delay_between_readings, time_delay_done, &context);
    }
    return true;
}

void rs485_init()
{
    rs485Serial.begin(9600, SERIAL_8N1, RXD1, TXD1);
    mb.begin(&rs485Serial);
    mb.client();
}

void rs485_task()
{
    rate_limit_timer.tick();
    modbus_timeout_timer.tick();
    mb.task();
    switch (context.mb_state)
    {
        default:
            [[fallthrough]];
        case READY_TO_REQUEST:
            rate_limit_timer.cancel(rate_limit_task);
            //printf("Requesting temperature & humidity readings\r\n");
            mb.readIreg(context.slave_id, 1, context.inputRegs, 2, cb);
            context.mb_state = WAITING_FOR_RESPONSE;
            modbus_timeout_task = modbus_timeout_timer.in(2000, timeout_while_reading_modbus, &context);
            break;
        case WAITING_FOR_RESPONSE:
            break;
        case WAITING_FOR_TIME_DELAY:
            break;
    }
}

void update_temperature_and_humidity(struct sensor_context *c)
{
    c->temperature_degC_x10 = c->inputRegs[0];
    c->relative_humidity_percent_x10 = c->inputRegs[1];
    //printf("Updated temperature & humidity %d %d\r\n", c->temperature_degC_x10, c->relative_humidity_percent_x10);
}

int get_relative_humidity_percent_x10()
{
    return context.relative_humidity_percent_x10;
}

int get_temperature_degC_x10()
{
    return context.temperature_degC_x10;
}