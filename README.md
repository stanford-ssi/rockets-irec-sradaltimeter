# rockets-irec-sradaltimeter
Code to run on the embedded processor of the SRAD altimeter

Flight States
The altimeter operates in discrete states that dictate the majority of the actions of the processor. However it is not in statemachine as the entirety of the unit is not determined solely by state and input.

State:
Pre-Launch 
Startup
Idle
Armed
Launch
Liftoff
Coast 
Descent under Drogue 
Descent under Main
Landed
Awaiting Recovery
Recovered 

Transitions 
This details all of the flight transitions and what the criteria are for passing though. To be completed later.
1a  => 1b: 

Code Organization
Classes
Flight_Data - this class stores the raw data values that are read from each sensor. It also also contains the methods for logging to SD card and has a member object that is the Kalman Filter object. Supports reading sensors at different frequencies. Does not store history.

Flight_Sensors - this class is used for accessing all of the sensors and reading GPIO inputs. Is kept separate from Flight_Data class, as use of this object can be easily swapped out for one for a Hardware-In-The-Loop test object

Flight_Events - Object of this class contains the 4 internal interrupt timers, as well as any other pin-driven interrupts, that can be used to time events properly. 

Altitude_Kalman_Filter - Custom kalman filter class for determining altitude. Stores all the relevant constants for the filter as well as the previous filter value. Contains all methods needed for filter update. 

Global Variables and Objects

flight_data: Object of type Flight_Data that stores all of the relevant data values for the flight. 
flight_sensors: Object of type Flight_Sensors that interfaces with all sensors
flight_state: Integer that contains the enumerated flight state values


Files
S-ALT_REV0.h header file for the board. Contains all pin definitions and any layout specific values


















