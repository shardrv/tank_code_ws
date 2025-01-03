/*
Cooling Tank Main Header file with function definitions
class Cooling Tank has Fan and Pump Control individual options 
to control true/false states of the devices
Update System updates these states by receiving 0/1/2 integer values
0 - No action and Pump/Fan OFF
1 - Operating Range Pump/Fan ON
2 - High temp range Pump/Fan ON

This also includes separate Safety Check for Over Temp

Added cassert test cases for individual test logging.

- Dhruv Sharma
*/

#ifndef _cooling_tank_h 
#define _cooling_tank_h 

#define KP 2.0
#define KI 0.1
#define KD 0.05

#include <iostream>
#include <random>
#include <cassert>

// Added setpoint values
// User can make changes here according to requirements
const float PUMP_SETPOINT = 20;
const float HI_PUMP_SETPOINT = 60;
const float FAN_SETPOINT = 20;
const float HI_FAN_SETPOINT = 60;
const float TEMP_ALARM = 80;

float previousError = 0;
float integral = 0;

// Cooling Tank Class
class Cooling_Tank{
    float temp_sensor_input_IN = 90.0;
    bool fan_start = false;
    bool pump_start = false;
    float test_coolant_temp = 0.0;

    public:
        int Fan_Control(float temp);
        int Pump_Control(float temp);
        int Safety_Control(float temp);
        float temp_simulate(float a, float b);
        void updateSystem(float curr_temp, bool ignition);
        bool getPumpState();
        bool getFanState();
        void testControlPump();
        void testControlFan();
        void testSafetyCheck();
        void setIgnitionSwitch(bool state);
};

// Get Current Pump State
bool Cooling_Tank::getPumpState(){
    return pump_start;
}

// Get Current Fan State
bool Cooling_Tank::getFanState(){
    return fan_start;
}

// Control Fan using bits 0/1/2
int Cooling_Tank::Fan_Control(float temp){

    if(temp >= HI_FAN_SETPOINT){
        return 2;
    }
    else if(temp < HI_FAN_SETPOINT && temp >= FAN_SETPOINT){
        return 1;
    }
    return 0;
}

// Control Pump using bits 0/1/2
int Cooling_Tank::Pump_Control(float temp){
    if(temp >= HI_PUMP_SETPOINT){
        return 2;
    }
    else if(temp < HI_PUMP_SETPOINT && temp >= PUMP_SETPOINT){
        return 1;
    }
    return 0;
}

// Safety check Over temp
int Cooling_Tank::Safety_Control(float temp){
    // Safety Control not to damage system
    if(Fan_Control(temp) * Pump_Control(temp) == 4){
            std::cout<<"\nSystem Critical - Pump and Fan turned ON\n";
            fan_start = true;
            pump_start = true;
            return 1;
    }
    return 0;
}

// Simulate temp values in case no user inputs provided
float Cooling_Tank::temp_simulate(float a, float b) {
    // Random number generator
    std::random_device rd;                     // Obtain a random seed from the system
    std::mt19937 gen(rd());                    // Mersenne Twister engine, initialized with the random seed
    std::uniform_real_distribution<> dist(a, b); // Uniform distribution between a and b

    // Generate a random floating-point number in the range [a, b]
    return dist(gen);
}

// Execution of system with current values of temp and enable/disable fan/pump
void Cooling_Tank::updateSystem(float current_temp, bool ignition_switch){
    if(ignition_switch){
        switch(Pump_Control(current_temp)){
            case 0:
                std::cout<<"\nTemp under control - no action\n";
                pump_start = false;
                break;

            case 1:
                std::cout<<"\nTemp has reached an operating value; pump ON\n";
                pump_start = true;
                break;

            case 2:
                std::cout<<"\nTemp has reached a warning value high high; pump ON\n";
                pump_start = true;
                break;
        };

        switch(Fan_Control(current_temp)){
            case 0:
                std::cout<<"\nTemp under control - no action\n";
                fan_start = false;
                break;

            case 1:
                std::cout<<"\nTemp has reached an operating value; pump ON\n";
                fan_start = true;
                break;

            case 2:
                std::cout<<"\nTemp has reached a warning value high high; pump ON\n";
                fan_start = true;
                break;
        };
    }
    else{
        std::cout<<"\nIgnition OFF - no action\n";
    }
}


// Table-driven test function for controlling the pump
void Cooling_Tank::testControlPump() {
    struct TestCase {
        float inputTemp;
        int expectedPumpStatus;
    };

    // Define test cases for controlling the pump
    TestCase testCases[] = {
        {15.0, 0},  // Temperature below MIN_COOLANT_TEMP
        {40.0, 1},   // Temperature above MIN_COOLANT_TEMP
        {65.0, 2}    // Temperature sending alarm
    };

    // Run the tests
    for (const auto& testCase : testCases) {
        test_coolant_temp = testCase.inputTemp;
        int pump_status = Pump_Control(test_coolant_temp);
        assert(pump_status == testCase.expectedPumpStatus && "Pump control test failed!");
    }

    std::cout << "controlPump tests passed.\n";
}

// Table-driven test function for controlling the fan
void Cooling_Tank::testControlFan() {
    struct TestCase {
        float inputTemp;
        int expectedFanStatus;
    };

    // Define test cases for controlling the fan
    TestCase testCases[] = {
        {15.0, 0},  // Temperature below FAN_START_TEMP
        {65.0, 2},   // Temperature above FAN_START_TEMP
        {55.0, 1},  // Temperature just below operating range
        {85.0, 2}    // Temperature above FAN_START_TEMP
    };

    // Run the tests
    for (const auto& testCase : testCases) {
        test_coolant_temp = testCase.inputTemp;
        int fan_status = Fan_Control(test_coolant_temp);
        assert(fan_status == testCase.expectedFanStatus && "Fan control test failed!");
    }

    std::cout << "controlFan tests passed.\n";
}

// Table-driven test function for safety check
void Cooling_Tank::testSafetyCheck() {
    struct TestCase {
        float inputTemp;
        int expectedOverTempFlag;
    };

    // Define test cases for safety check
    TestCase testCases[] = {
        {75.0, 1},  // Temperature below MAX_COOLANT_TEMP
        {80.0, 1},  // Temperature exactly MAX_COOLANT_TEMP
        {85.0, 1}    // Temperature above MAX_COOLANT_TEMP
    };

    // Run the tests
    for (const auto& testCase : testCases) {
        test_coolant_temp = testCase.inputTemp;
        int over_temperature = Safety_Control(test_coolant_temp);
        assert(over_temperature == testCase.expectedOverTempFlag && "Safety check test failed!");
    }

    std::cout << "safetyCheck tests passed.\n";
}

#endif
