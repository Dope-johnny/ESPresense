#include "AnalogRead.h"

#include <Arduino.h>

#include "defaults.h"
#include "globals.h"
#include "mqtt.h"

namespace AnalogRead {
float smooth_A0_voltage;
unsigned long A0PreviousMillis = 0;
int sensorInterval = 500;
//bool initialized = false;

void Setup() {
    pinMode(GPIO_NUM_0, INPUT);
}

void Loop()
{
    //if (!initialized) return;

    float A0_voltage = 0.001 * analogReadMilliVolts(GPIO_NUM_0);

    if (smooth_A0_voltage)
        smooth_A0_voltage = 0.1 * (A0_voltage - smooth_A0_voltage) + smooth_A0_voltage; // LPF with time frame of 10
    else
        smooth_A0_voltage = A0_voltage;

    if (A0PreviousMillis == 0 || millis() - A0PreviousMillis >= sensorInterval) {
        pub((roomsTopic + "/a0_voltage").c_str(), 0, 1, String(smooth_A0_voltage).c_str());
        A0PreviousMillis = millis();
    }
}

bool SendDiscovery() {
    return sendSensorDiscovery("A0 Voltage", EC_NONE, "voltage", "V");
}

} // namespace AnalogRead
