# Cooling Tank Operation


## Requirements
- cpppo library -  For PLC simulation
```
pip install cpppo
```

- libplctag library - For PLC communication
```
git clone https://github.com/libplctag/libplctag.git
cd libplctag
mkdir -p build
cd build
cmake ..
make
sudo make install
```

- gtest library - for testing framework



## File details

#### cooling_tank.cpp
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


#### cooling_tank.h

Cooling Tank Main Header file with function definitions
class Cooling Tank has Fan and Pump Control individual options 
to control true/false states of the devices
Update System updates these states by receiving 0/1/2 integer values
0 - No action and Pump/Fan OFF
1 - Operating Range Pump/Fan ON
2 - High temp range Pump/Fan ON

This also includes separate Safety Check for Over Temp

Added cassert test cases for individual test logging.


#### tests.h

Test Cases using GTEST library




## PLC Communication

In separate terminals run the following 

```
python3 -m cpppo.server.enip --print clx_alarm_tag=DINT -a 127.0.0.1

python3 -m cpppo.server.enip.client -v --print clx_alarm_tag=0 -a 127.0.0.1 -r 20
```

This will run a client to simulate a PLC ControlLogix with tagname clx_alarm_tag
and run a client to update it initially with 0. This will now be updated by the program.


## Running the program 

```
Navigate to tank_code_ws
Create a build folder
Run cmake 
Run make
Run ./cooling_tank
```

## HOW TO RUN 

```
cd tank_code_ws
mkdir -p build
cd build
cmake ..
make
./cooling_tank
```
