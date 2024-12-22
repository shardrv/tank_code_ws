#include <iostream>
#include "cooling_tank.h"
#include "libplctag.h"

const int DATA_TIMEOUT = 5000;

bool ignition_switch = 1;

int pump_warning_indication = 0;
int fan_warning_indication = 0;

int main(){

    Cooling_Tank tank;

    char user_in;
    int current_temp;
    int32_t plc_status;
    int32_t plc_read;

    int status_tag_length = 1;
    std::string plc_ip_addr = "127.0.0.1";
    std::string status_tag_str_read = "clx_tag";

    std::string tag_status_path_read = std::string("protocol=ab_eip&gateway=") + 
        plc_ip_addr + std::string("&path=1,0&cpu=Controllogix&elem_size=4&elem_count=") + 
            std::to_string(status_tag_length) + std::string("&name=") + status_tag_str_read;


    plc_status = plc_tag_create(tag_status_path_read.c_str(), DATA_TIMEOUT);
    std::cout<<"here = "<<plc_status;

    int read_plc_data = plc_tag_get_int32(plc_status, 0);
    std::cout<<"\nTag = "<<read_plc_data<<"\n";

    plc_tag_destroy(plc_status);

    do{
        std::cout<<"\nPress any key to start and 'x' to exit\n";
        std::cin>>user_in;

        current_temp = tank.temp_simulate();

        std::cout<<"\nTemp = "<<current_temp<<"\n";

        tank.updateSystem(current_temp, ignition_switch);

    }while(user_in!='x');

    tank.testControlPump();
    tank.testControlFan();
    tank.testSafetyCheck();


    return 0;
}
