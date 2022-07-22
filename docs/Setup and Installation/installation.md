# Installation

Is a large project so it needs some time to be installed properly.  
Documentation is always not exaustive enough but I'll try my best.  
This installation procedure is tested and written only for Linux, this is because is developed on Linux and the telemetry software is running on a RaspberryPi, sooo...

Clone the repo with
~~~
git clone --recursive https://github.com/eagletrt/telemetry.git
~~~

## Requirements
- Required cmake and other stuff to compile the project:  
  ~~~
  sudo apt install build-essential cmake can-utils libboost-iostreams-dev libboost-system-dev libboost-filesystem-dev
  ~~~

- Required Google Protobuf
  ~~~
  sudo apt-get install libprotobuf-dev libprotoc-dev
  ~~~

- Websocketpp  
  ~~~
  git clone git://github.com/zaphoyd/websocketpp.git  
  cd websocketpp  
  cmake .  
  sudo make install  
  ~~~

- GNU PLOT
  Sorry read the docs to install it
  ~~~
  git clone https://github.com/dstahlke/gnuplot-iostream.git
  ~~~

- Raspi Cam
  ~~~
  git clone https://github.com/rmsalinas/raspicam
  cd raspicam
  mkdir build
  cd build
  cmake ..
  make
  sudo make install
  sudo ldconfig
  ~~~

- LibHaru
  libharu is a submodule in this repo but needs a couple of commands to work properly.  
  ~~~
  cd thirdparty/libharu
  buildconf.sh
  ./configure
  ~~~

- OpenCV  
  This is the latest added so please wait for the installation, OpenCV is a HUGE library but for this project only a tiny part is required. This guide will avoid installing the whole library.

## Building

~~~bash
cd Protobuffer
mkdir cpp
chmod +X compile
./compile
~~~

Once all the requirements are installed:
~~~bash
mkdir build
cd build
cmake ..
make
~~~
Most of the times running the cmake and make commands generates lots of errors, please contact the [developer](https://github.com/Pippo98) to be helped and to allow him to integrate this giude with possible solutions.

All the generated binaries are located in **bin** folder.