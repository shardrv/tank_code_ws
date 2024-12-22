#include <iostream>
#include <random>
#include <cassert>

const int HI_PUMP_SETPOINT = 20;
const int HI_HI_PUMP_SETPOINT = 60;
const int HI_FAN_SETPOINT = 20;
const int HI_HI_FAN_SETPOINT = 60;
const int TEMP_ALARM = 80;

class Cooling_Tank{
    int temp_sensor_input_IN = 90;
    float level_switch_value ;
    bool fan_start;
    bool pump_start;
    int test_coolant_temp;

    public:
        int Fan_Control(int temp);
        int Pump_Control(int temp);
        int Safety_Control(int temp);
        int temp_simulate();
        void updateSystem(int curr_temp, bool ignition);
        void testControlPump();
        void testControlFan();
        void testSafetyCheck();
};

int Cooling_Tank::Fan_Control(int temp){

    if(temp >= HI_HI_FAN_SETPOINT){
        return 2;
    }
    else if(temp < HI_HI_FAN_SETPOINT && temp >= HI_FAN_SETPOINT){
        return 1;
    }
    return 0;
}

int Cooling_Tank::Pump_Control(int temp){
    if(temp >= HI_HI_PUMP_SETPOINT){
        return 2;
    }
    else if(temp < HI_HI_PUMP_SETPOINT && temp >= HI_PUMP_SETPOINT){
        return 1;
    }
    return 0;
}

int Cooling_Tank::Safety_Control(int temp){
    if(Fan_Control(temp) * Pump_Control(temp) >= 4){
            std::cout<<"\nSystem Critical - Pump and Fan turned ON\n";
            fan_start = true;
            pump_start = true;
            return 1;
    }
    return 0;
}

int Cooling_Tank::temp_simulate(){
    return std::rand()%temp_sensor_input_IN;
}

void Cooling_Tank::updateSystem(int current_temp, bool ignition_switch){
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
        int inputTemp;
        int expectedPumpStatus;
    };

    // Define test cases for controlling the pump
    TestCase testCases[] = {
        {15, 0},  // Temperature below MIN_COOLANT_TEMP
        {40, 1},   // Temperature above MIN_COOLANT_TEMP
        {90, 2}    // Temperature exactly MIN_COOLANT_TEMP
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
        int inputTemp;
        int expectedFanStatus;
    };

    // Define test cases for controlling the fan
    TestCase testCases[] = {
        {15, 0},  // Temperature below FAN_START_TEMP
        {65, 2},   // Temperature above FAN_START_TEMP
        {55, 1},  // Temperature below FAN_STOP_TEMP
        {85, 2}    // Temperature above FAN_START_TEMP
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
        int inputTemp;
        int expectedOverTempFlag;
    };

    // Define test cases for safety check
    TestCase testCases[] = {
        {75, 1},  // Temperature below MAX_COOLANT_TEMP
        {80, 1},  // Temperature exactly MAX_COOLANT_TEMP
        {85, 1}    // Temperature above MAX_COOLANT_TEMP
    };

    // Run the tests
    for (const auto& testCase : testCases) {
        test_coolant_temp = testCase.inputTemp;
        int over_temperature = Safety_Control(test_coolant_temp);
        assert(over_temperature == testCase.expectedOverTempFlag && "Safety check test failed!");
    }

    std::cout << "safetyCheck tests passed.\n";
}
