# rockets-irec-sradaltimeter

Code to run on the embedded processor of the SRAD altimeter.

### Flight States
The altimeter operates in discrete states that dictate the majority of the actions of the processor. However it is not in statemachine as the entirety of the unit is not determined solely by state and input.

#### States
1. Pre-Launch  
 - 1a. Startup  
 - 1b. Idle
 - 1c. Armed
2. Launch
 - 2a. Liftoff
 - 2b. Coast 
 - 2c. Descent under Drogue 
 - 2d. Descent under Main
3. Landed
 - 3a. Awaiting Recovery
 - 3b. Recovered 

#### Transitions 

This details all of the flight transitions and what the criteria are for passing though. To be completed later.
1a  => 1b: 

### Events

The processor manages multiple tasks without the implementation of a full RTOS through the use of events that are triggered by interrupts.

Events are stored in the `uint8_t events` variable. Each bit of the `events` variable can flag an event that signals a task to be processed in the next iteration of the main loop. Each event is enumerated as a single byte with a 1 in a position corrisponding to the flagged bit. For example, the rightmost bit of `events` signals to exectute the main update loop, which is named `EVENT_UPDATE` and enumerated as `0b00000001` After a task is processed, it is unschedualed. 

The main event flags are triggered interupt timers. These timers inturrupt on constant intervals, and call a function that simply edits the inturrupt bit in the `events` variable.

### Code Organization

#### Classes

`Flight_Data` - this class stores the raw data values that are read from each sensor. It also also contains the methods for logging to SD card and has a member object that is the Kalman Filter object. Supports reading sensors at different frequencies. Does not store history.

`Flight_Sensors` - this class is used for accessing all of the sensors and reading GPIO inputs. Is kept separate from Flight_Data class, as use of this object can be easily swapped out for one for a Hardware-In-The-Loop test object

`Flight_Events` - Object of this class contains the 4 internal interrupt timers, as well as any other pin-driven interrupts, that can be used to time events properly. 

`Altitude_Kalman_Filter` - Custom kalman filter class for determining altitude. Stores all the relevant constants for the filter as well as the previous filter value. Contains all methods needed for filter update. 

#### Global Variables and Objects

flight_data: Object of type Flight_Data that stores all of the relevant data values for the flight. 
flight_sensors: Object of type Flight_Sensors that interfaces with all sensors
flight_state: Integer that contains the enumerated flight state values

#### Non-Class Files

`S-ALT_REV0.h` - header file for the board. Contains all pin definitions and any layout specific values
`Flight_Configurations.h` - header file that defines enumerated list and structures or various things within the board.


















