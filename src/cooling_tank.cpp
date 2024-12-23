/*
Cooling Tank Main Run file with Program Execution
MENU includes the following :
1. Run Program - key 'R'
2. Display Fan and Pump States - key 'D'
3. Run Tests with cassert - key 'T'
4. Exit with write to PLC and gtest test cases - key 'X'

PLC COMMUNICATIONS
PLC COMMS used - libplctag C library - assumed communication with CLX ControlLogix PLC. 
PLC Control - a CLX tag is used in the plc for 0/1 indication in case of system critical (temp > TEMP_ALARM)

With cpppo installed (pip install cpppo) Run the following in a terminal
python3 -m cpppo.server.enip --print clx_alarm_tag=DINT -a 127.0.0.1
python3 -m cpppo.server.enip.client -v --print clx_alarm_tag=25 -a 127.0.0.1 -r 20

- Dhruv Sharma
*/

#include <iostream>
#include "cooling_tank.h"
#include "libplctag.h"
#include <vector>
#include <gtest/gtest.h>
#include "tests.h"

// Set global variables
const int DATA_TIMEOUT = 5000;
int pump_warning_indication = 0;
int fan_warning_indication = 0;
float temp_sensor_input_IN = 90.0;
bool ignition_switch = 1;               // Set to 1 to enable fan_start & pump_start

//Main Loop
int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    std::vector<float> temp_inputs;     //to save all temperature input arguments 

    Cooling_Tank tank;                  //object of cooling tank class

    char user_in;                       //user input during runtime
    float current_temp = 0;                 //temp value checker
    float last_temp_value = 0;              //last value to check the state at the end and send warning to PLC

    //communicate to PLC using <libplctag> library
    int32_t plc_status;
    int32_t plc_read;
    int status_tag_length = 1;
    std::string plc_ip_addr = "127.0.0.1";
    std::string status_tag_str_read = "clx_alarm_tag";

    // Loop through each setpoint argument passed
    for (int i = 1; i < argc; ++i) {
        try {
            // Convert the string argument to an integer or float
            float setpoint = std::stof(argv[i]); // Use std::stof for floats, std::stoi for integers
            std::cout << "Setpoint " << i << ": " << setpoint << std::endl;
            temp_inputs.push_back(setpoint);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << argv[i] << " is not a valid number." << std::endl;
        }
    }

    // Initialize PLC Path for communication 
    std::string tag_status_path_read = std::string("protocol=ab_eip&gateway=") + 
        plc_ip_addr + std::string("&path=1,0&cpu=Controllogix&elem_size=4&elem_count=") + 
            std::to_string(status_tag_length) + std::string("&name=") + status_tag_str_read;

    // Program Execution
    do{
        std::cout<<"\nPress 'r' to run program; 'd' to Display Fan/Pump State; 't' for tests and 'x' to exit with gtest\n";
        std::cin>>user_in;      //user input
        
        if(user_in == 'r' || user_in == 'R'){       //when in run mode
            if(!temp_inputs.empty()){\
                // Iterate over the vector of arguments provided
                for(auto temp_itr : temp_inputs){
                    last_temp_value = temp_itr;
                    std::cout<<"\nTemp = "<<temp_itr<<"\n";
                    tank.updateSystem(temp_itr, ignition_switch);
                    temp_inputs.pop_back();
                }
            }else{
                // If no arguments - generate random floats
                std::cout<<"\nNo input temperature registered generating random values\n";
                current_temp = tank.temp_simulate(0, temp_sensor_input_IN);
                last_temp_value = current_temp;
                std::cout<<"\nTemp = "<<current_temp<<"\n";
                tank.updateSystem(current_temp, ignition_switch);
            }
        }
        else if(user_in == 't' || user_in == 'T'){
            // Run Set Tests
            std::cout<<"\nRunning tests\n";
            tank.testControlPump();
            tank.testControlFan();
            tank.testSafetyCheck();
        }
        else if(user_in == 'd' || user_in == 'D'){
            // See current states for the pump and fan
            std::cout<<"\n Pump start = "<<tank.getPumpState()<<"\n";
            std::cout<<"\n Fan start = "<<tank.getFanState()<<"\n";
        }
        else{
            // Not a valid input
            std::cout<<"\nNot a valid input\n";
        }

    }while(user_in!='x');       //Exit
    std::cout<<"\nProgram Exiting\n";

    // Setting PLC tag value on CLX - ControlLogix Ethernet IP
    plc_status = plc_tag_create(tag_status_path_read.c_str(), DATA_TIMEOUT);    //create tag

    // Read tag value before setting it
    int read_plc_data = plc_tag_get_int32(plc_status, 0);       
    std::cout<<"\nTag current value = "<<read_plc_data<<"\n";

    // Write to the tag value and read it again
    std::cout<<"\nChecking for the last system state = "<<last_temp_value<<"\n";
    int write_plc_data = plc_tag_set_int32(plc_status, 0, tank.Safety_Control(last_temp_value));        //1 = HIGH; 0 = LOW
    read_plc_data = plc_tag_get_int32(plc_status, 0);
    std::cout<<"\nTag current value = "<<read_plc_data<<"\n";

    plc_tag_destroy(plc_status);        //destroy tag and clear memory

    return RUN_ALL_TESTS();
}
