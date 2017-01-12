# rockets-irec-sradaltimeter

Code to run on the embedded processor of the SSI SRAD altimeter.

I'm sorry this readme doesn't have spellcheck so gg.

### Hardware overview
For information on the hardware running on the 

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

This details all of the flight transitions and what the criteria are for passing through. 
 - 1a => 1b: Transition occurs once startup is complete and all sensors initialized. Deterministic.
 - 1b => 1c: The altimeter arms when it determines that it is pointing upwards and has been stable for a set amount of time. Non deterministic, and will likely be determined by set thresholds for both being stable and pointing upwards.
 - 1c => 2a: Transition will occur when the altimeter undergoes an acceleration above a set threshold for a set amount of time. Thresholds will be determined from previous flight data.
 - 2a => 2b: Transition occurs when acceleration transitions from positive to negative for a set amount of time.
 - 2b => 2c: Occurs when the altimeter determines that altitude has reached it's maximum and is now starting to decrease. On this transition, an ejection charge is triggered for the drogue chute.
 - 2c => 2d: Occurs when the altimeter has fallen to a set altitude where the main should be deployed (nominally around 700ft). On this transition, an ejection charge is triggered for the main chute.
 - 2d => 3a: Occurs after the altimeter sees a sharp acceleration spike, followed by minimal movement.
 

### Events

The processor manages multiple tasks without the implementation of a full RTOS through the use of events that are triggered by interrupts.

Events are stored in the `uint8_t events` variable. Each bit of the `events` variable can flag an event that signals a task to be processed in the next iteration of the main loop. Each event is enumerated as a single byte with a 1 in a position corresponding to the flagged bit. For example, the rightmost bit of `events` signals to execute the main update loop, which is named `EVENT_UPDATE` and enumerated as `0b00000001` After a task is processed, it is unscheduled. 

The main event flags are triggered interrupt timers. These timers interrupt on constant intervals, and call a function that simply edits the interrupt bit in the `events` variable.

### Code Organization

#### Classes

`Altimeter` - this is the primary class for the board and contains all of the other classes as members. Everything is initialized in this class

`Flight_Data` - this class stores the raw data values that are read from each sensor. It also also contains the methods for logging to SD card and has a member object that is the Kalman Filter object. Supports reading sensors at different frequencies. Does not store history, but probably will be updated in the future so that maybe the past 100 or so values are kept in ram.


`Flight_Sensors` - this class is used for accessing all of the sensors and reading GPIO inputs. Is kept separate from Flight_Data class, as use of this object can be easily swapped out for one for a Hardware-In-The-Loop test object

`Flight_Events` - Object of this class contains the 4 internal interrupt timers, as well as any other pin-driven interrupts, that can be used to time events properly. 

`Altitude_Kalman_Filter` - Custom Kalman filter class for determining altitude. Stores all the relevant constants for the filter as well as the previous filter value. Contains all methods needed for filter update. 

#### Members of `Altimeter` class

flight_data: Object of type Flight_Data that stores all of the relevant data values for the flight. 
flight_sensors: Object of type Flight_Sensors that interfaces with all sensors
flight_events: Object of type Flight_Events that contains the 4 interrupt timers

flight_state: Integer that contains the enumerated flight state values
*more to add below* 


#### Non-Class Files

`S-ALT_REV0.h` - header file for the board. Contains all pin definitions and any pcb layout specific values
`Flight_Configurations.h` - header file that defines enumerated list and structures or various things within the project.

### Implementation Details

#### SD Data storage

In the root directory of the SD card, a new folder should be generated to store files for each flight, numbered incrementally as `flight_[number]`. So, if the altimeter boots up and sees that the folders `\fight_1` and `\flight_2`, it should create a folder `flight_3` to log data in. 

Within the folder for the flight, the altimeter should create a .csv file for each sensor source. So, it will contain `MMA.csv`, `BNO.csv`, `BMP.csv`, `esense.csv`, and `gps.csv` (and any others that I may have forgotten). Each line of each sensor file will be of the format: `[time stamp], [data field 1], [data field 2], ... `. Sensor readings are kept in separate files for better organization and to support writing sensor data at different frequencies (if it turn out that opening/closing files is time consuming, then things will be logged in a single file with an indicator for which sensor it was on each line).

For normal operations the altimeter should not log data to the SD card until after it reaches the `liftoff` state (to prevent excessive folder generation when the altimeter is simply just powered on). However, in the testing phase for data collection purposes, it should start logging data immediately. 

#### RAM data storage

To keep a history of sensor information in the ram, each sensors should have circular array of variable length so store readings in. The storage should support the following functionality:
- Changing the length of the array for each sensor
- Store data at a fraction of the frequency it is sampled at (so if the accelerometer is sampled at 50Hz, it can be stored in ram at only say 10Hz, so that a longer history can be kept without taking up too much memory). However it should still be able to store at the same frequency that it is sampled if desired
- methods for retrieving data given the sensor, and the nearest number of milliseconds/microseconds in the past it was logged.

