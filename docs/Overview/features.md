# Features
All the features that this repo implemented!


## Logging
The most important feature is to be able to log all data coming from sensors in the car.  
The raw format, saving every CANbus message as it is, prepending the timestamp.  
The parsed format, saving directly parsed messages to .CSV files (one for each sensor).  
Extra information, like date/time, pilot who drove the car, circuit on which was racing on, race type, a specific race configuration, etc...
Lastly has been added the onboard video logging, saving a synced video with low resolution and framerate to reduce video size. This helps a lot to recognize which specific race was and mostly reconstruct how the race gone.

## CSV
When the race session ends, from a raw file (only CANbus messages) is really important to be able to generate files containing usable data from the sensors.  
The most common and simple way to save this kind of data is with CSV files (comma separated values).  
CSV files are simple to use to be imported and to be procecced.  
To analyze car performances the team uses Matlab which is kinda slow loading CSV files, a better format for that specific application is .mat, in this repo under python folder there is a script wich converts CSV files to .mat files.

## Connection
Telemetry can transmit data over wireless connection, it uses Websocket (most used) and MQTT, in the future it will also support radio transmission.  
The connection is used to send status messages to the clients and to receive configurations from the clients.  
The configurations are used to change the behavior of the telemetry, to enable or disable CSV parsing, enable GPSs, etc...

## Realtime Data
The data is transmitted in realtime, so the user can see the data as it is being received.  
Sensor data are serialized using Google protobuffers, the serialization and deserialization is implemented in canlib.  
It can be configured the send rate and the downsample, so reducing the messages sent to the user.

## Report
Really cool feature.  
Automatic report generator, from a race session generates a PDF file containing plots and basic information about that race, this is really usefull to have a quick analisys of the sensor data, easy to check if sensors were working properly.