/*
Test Cases using GTEST library
- Dhruv Sharma
*/


#include <iostream>
#include <gtest/gtest.h>
#include "cooling_tank.h"

// Unit test for the CoolingLoop class
class CoolingLoopTest : public ::testing::Test {
protected:
    Cooling_Tank cooling_loop;
};

TEST_F(CoolingLoopTest, TestPumpAndFanControlWithIgnitionOff) {

    // Test result when the ignition is OFF
    bool setIgnitionSwitch = false;
    float setCoolantTemp = 25;
    
    cooling_loop.updateSystem(setCoolantTemp, setIgnitionSwitch);
    ASSERT_EQ(cooling_loop.getPumpState(), false);
    ASSERT_EQ(cooling_loop.getFanState(), false);
}

TEST_F(CoolingLoopTest, TestPumpAndFanControlWithIgnitionOn) {
    bool setIgnitionSwitch = true;

    // Test if the temperature is below the minimum safe threshold
    float setCoolantTemp = 5;
    
    ASSERT_EQ(cooling_loop.Pump_Control(setCoolantTemp), 0);
    ASSERT_EQ(cooling_loop.Fan_Control(setCoolantTemp), 0);

    // Test if the temperature is between the safe range
    setCoolantTemp = 25;
    
    ASSERT_EQ(cooling_loop.Pump_Control(setCoolantTemp), 1);
    ASSERT_EQ(cooling_loop.Fan_Control(setCoolantTemp), 1);
    
    // Test if the temperature exceeds the max safe threshold
    setCoolantTemp = 75;
    
    ASSERT_EQ(cooling_loop.Pump_Control(setCoolantTemp), 2);
    ASSERT_EQ(cooling_loop.Fan_Control(setCoolantTemp), 2);
}

TEST_F(CoolingLoopTest, TestTemperatureExceedMaxSafeTemp) {
    bool setIgnitionSwitch = true;

    // Test if the temperature exceeds max temp
    float setCoolantTemp = 95.0;  // Exceeds max temp
        
    ASSERT_EQ(cooling_loop.Pump_Control(setCoolantTemp), 2);
    ASSERT_EQ(cooling_loop.Fan_Control(setCoolantTemp), 2);
    ASSERT_EQ(cooling_loop.Safety_Control(setCoolantTemp), 1);
}
