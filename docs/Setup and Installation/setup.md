# Telemetry Usage

## Running
To run check if the device you are using has a CAN interface:
~~~bash
ifconfig
~~~
In the infos displayed make sure **can0**/**can1**/**vcan0** device is shown, if not: if you want to test on an actual CAN interface run `bash canSetup.sh`, if you want to use a symulated interface run `bash vcanSetup.sh`.

Then to run the actual telemetry use:
~~~
./bin/telemetry
~~~
The script will log lots of informations, check for eventual errors and contact the developer for help.

***
**Telemetry generates a debug file in which all the debug/warn/errors are logged.**  
This file is in home folder: **~/telemetry_debug.log**.
***

## Configurations
At the first telemetry run, it generates some .json files that can be used to configure how the code acts.  
All configs are in home folder (**~**).  
The most important is **fenice_telemetry_config.json**.  
### fenice_telemetry_config.json
| field | type | meaning |
| ----- | ---- |:------- |
| can_devices | vector of objects | Each object must contain: <br> - sock: name of the interface (can0 ...) <br> - name: primary/secondary |  
| gps_devices | vector of objects | each entry corresponds to a gps device, objects must have these fields:<br> - addr: port/file name <br> - mode port/file <br> - enabled boolean |
| generate_csv | bool | enables generation of .csv <br> files while telemetry logs |
| camera_enable | bool | enables video logging from raspicam |
| connection_enabled | bool | enables communication over <br> internet (websockets) |
| connection_send_rate | int | milliseconds between each <br> sensor data ws message |
| connection_send_sensor_data | bool | enables sending parsed sensor data over ws |
| connection_downsample | bool | enables downsample of <br> sensors to reduce packet size |
| connection_downsample_mps | int | mps -> messages per second -> maximum number of messages added in each packet |
| connection | object | fields:<br> - ip <br> - port <br> - mode WEBSOCKET or ZEROMQ or MQTT to define wich protocol is used |

***example***
~~~json
{
    "can_devices": [
        {
            "sock": "vcan0",
            "name": "primary"
        },
        {
            "sock": "vcan1",
            "name": "secondary"
        }
    ],
    "gps_devices": [
      {
        "addr": "/dev/ttyACM1",
            "mode": "port",
            "enabled": true
        }
    ],
    "generate_csv": true,
    "camera_enable": true,
    "connection_downsample": false,
    "connection_downsample_mps": 40,
    "connection_enabled": true,
    "connection_send_rate": 100,
    "connection_send_sensor_data": true,
    "connection": {
        "ip": "telemetry-server.herokuapp.com",
        "port": "",
        "mode": "WEBSOCKET"
    }
}
~~~

### session_config.json
Configures a race session. This informations are used to generate folder names for each log session.

| field | type | meaning |
| ----- | ---- | ------- |
| Circuit | string | name of the race circuit |
| Pilot | string | name of the pilot |
| Race | string | type of race ( acceleration/skidpad/trakdrive etc) |
| Configuration | string | current race configuration, for example test at 40 Km/h or car power map 40 % etc |
| Date | string | date of the test gg_mm_yyyy |
| Time | string | time of the test HH:MM:SS |

**example**
~~~json
{
  "Circuit": "Vadena",
  "Pilot": "Mirco",
  "Race": "Half Skidpad (velocità costante)",
  "Configuration": "Bassa - 40%",
  "Date": "21_03_2022",
  "Time": "00:12:49"
}
~~~

When telemetry starts logging a folder is generated to contain all the current log infos/files.  
The folder name is constructed like:
~~~
~/logs/<date>/<race> [<configuration>] <incremental number>/
~~~
***Example:***
~~~
~/logs/14_03_2022/Straight line [40 km h] 7
~~~
Where *Straight line* is field **Race**, *40 km h* is **Configuration** and 7 is the 7th logging session whith same configurations.

## Starting
Using the telemetry [application](https://github.com/eagletrt/telemetry-app).

Or can be started with a CAN message defined in canlib:
~~~
SET_TLM_STATUS
~~~

The can message can be sent from an onboard device, in our case the steering wheel has a button that sends start/stop messages to telemetry.

## Output
The data that telemetry produces can be found in:
~~~
~/logs/<date>/<race> [<configuration>]
~~~
Folder names are explained [here](#sessionconfigjson).  
Files that telemetry produces:  
**Raw**:  
- candump.log: contains all raw CAN messages.  
- gps_n.log: n is the index of the device (index in the vector of gps devices defined in [telemetry_config.json](#telemetryconfigjson)), contains raw strings coming from GPS device.  

**Stats**:  
- CAN_Info.json: json formatted file containing the session infos like Race, Pilot, Configuration and some extra informations about CAN messages: number and frequency during the log session.  
- gps_n.json: contains date and time of the log and informations about gps messages: number and frequency.

**CSV**:  
If generate_csv option is enabled, in a subfolder are located a bunch of .csv files, one for each sensor logged.  
The files are named as the sensor. The first line is a header, contains the column value name. Every other line is the actual sensor value.  

**Example**  
Pedals.csv contains the sensors values of the "pedals".  
*Header*  
~~~
timestamp,throttle1,throttle2,brake_front,brake_rear,
~~~
*Couple of lines*  
~~~
1637591203.904133,28.000000,68.000000,1.246000,1.112000,
1637591203.913102,30.000000,67.000000,1.246000,1.112000,
1637591203.914190,30.000000,67.000000,1.328000,1.084000,
1637591203.923164,29.000000,67.000000,1.328000,1.084000,
1637591203.924087,29.000000,67.000000,1.302000,1.084000,
1637591203.933217,29.000000,67.000000,1.302000,1.084000,
1637591203.934093,29.000000,67.000000,1.328000,1.112000,
1637591203.943089,30.000000,67.000000,1.328000,1.112000,
1637591203.944154,30.000000,67.000000,1.246000,1.138000,
1637591203.953122,31.000000,65.000000,1.246000,1.138000,
1637591203.954210,31.000000,65.000000,1.246000,1.138000,
1637591203.963181,33.000000,65.000000,1.246000,1.138000,
1637591203.964085,33.000000,65.000000,1.328000,1.138000,
~~~
Split each line by comma and the result is a vector of values, each column value refers to the column name defined in the header.