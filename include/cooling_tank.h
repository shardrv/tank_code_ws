#include <iostream>
#include <random>
#include <cassert>

const float HI_PUMP_SETPOINT = 20;
const float HI_HI_PUMP_SETPOINT = 60;
const float HI_FAN_SETPOINT = 20;
const float HI_HI_FAN_SETPOINT = 60;
const float TEMP_ALARM = 80;

class Cooling_Tank{
    float temp_sensor_input_IN = 90.0;
    float level_switch_value = 0.0;
    bool fan_start = false;
    bool pump_start = true;
    float test_coolant_temp = 0.0;

    public:
        int Fan_Control(float temp);
        int Pump_Control(float temp);
        int Safety_Control(float temp);
        float temp_simulate(float a, float b);
        void updateSystem(float curr_temp, bool ignition);
        void testControlPump();
        void testControlFan();
        void testSafetyCheck();
};

int Cooling_Tank::Fan_Control(float temp){

    if(temp >= HI_HI_FAN_SETPOINT){
        return 2;
    }
    else if(temp < HI_HI_FAN_SETPOINT && temp >= HI_FAN_SETPOINT){
        return 1;
    }
    return 0;
}

int Cooling_Tank::Pump_Control(float temp){
    if(temp >= HI_HI_PUMP_SETPOINT){
        return 2;
    }
    else if(temp < HI_HI_PUMP_SETPOINT && temp >= HI_PUMP_SETPOINT){
        return 1;
    }
    return 0;
}

int Cooling_Tank::Safety_Control(float temp){
    if(Fan_Control(temp) * Pump_Control(temp) >= 4){
            std::cout<<"\nSystem Critical - Pump and Fan turned ON\n";
            fan_start = true;
            pump_start = true;
            return 1;
    }
    return 0;
}

float Cooling_Tank::temp_simulate(float a, float b) {
    // Random number generator
    std::random_device rd;                     // Obtain a random seed from the system
    std::mt19937 gen(rd());                    // Mersenne Twister engine, initialized with the random seed
    std::uniform_real_distribution<> dist(a, b); // Uniform distribution between a and b

    // Generate a random floating-point number in the range [a, b]
    return dist(gen);
}

void Cooling_Tank::updateSystem(float current_temp, bool ignition_switch){
    if(ignition_switch){
        switch(Pump_Control(current_temp)){
            case 0:
                std::cout<<"\nTemp under control - no action\n";
                pump_start = false;
                break;

            case 1:
                std::cout<<"\nTemp has reached a warning value high; pump ON\n";
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
                std::cout<<"\nTemp has reached a warning value high; pump ON\n";
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
        {90.0, 2}    // Temperature exactly MIN_COOLANT_TEMP
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
        {55.0, 1},  // Temperature below FAN_STOP_TEMP
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
