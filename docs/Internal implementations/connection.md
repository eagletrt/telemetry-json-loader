# Implement New Connection Type

## Introduction

To create a new connection type you should just implement all virtual functions in the abstract ```Connection``` class, that is in the ```inc/connection.h``` file.

Every message has to be of type ```Generic Message```.  
The structure of this struct is show here below:
    ```
    struct GenericMessage {
        string topic;
        string payload;
    };
    ```

To generalize the connection type you have to implement a class that will contain all the things you'll need for the connection, such as the context and the socket. The class you're going to create must inherit from ```GeneralSocket``` class, a void class that has been created in the ```inc/connection.h``` file. Once you've done so, you can create your connection class, that will inherit from ```Connection``` (the abstract class mentioned above).

## Variables list

All variables are ```protected``` and can be accessed from the derived class.  The following variables are available:

- These variables are needed to set up the connection. You can only set them by calling the ```Connection```'s init function, by passing to it them all.
    ```
    string address; // FORMAT EXAMPLE: 127.0.0.1
    string port;    // FORMAT EXAMPLE: 8080
    int openMode;   // FORMAT EXAMPLE: PUB/SUB (it's an enum)
    ```

- This is the object mentioned above, where should be stored your custom socket class.
    ```
    GeneralSocket* socket;
    ```

- These variables are checked inside the Connection class, so you have to manage them.  
    For example:
    * the ```open = true``` when the connection has started;  
    * the ```done = true``` when you close the connection;  
    * ```new_data``` should never be modify, because is managed by the ```Connection``` class.

    ```
    bool done;
    bool open;
    bool new_data;
    ```

- These variables are used in the ```Connection``` class to manage the reading and the writing operation.
The ```mtx``` is used to lock the variables, so you can't read or write them at the same time; the ```cv``` is used to wait for the other thread to finish the operation; the ```buff_send``` is used to store the data that will be sent.
    ```
    mutex mtx;
    condition_variable cv;
    queue<message> buff_send;
    ```

## Methods to implement

Here below the list of all the needed methods:

- The **constructor** should only call the ```Connection```'s contrsuctor.
    ```
    // in the .cpp file
    [YOUR_CLASS]::[YOUR_CLASS]() : Connection() {
        // code here will be executed before the Connection constructor
    }
    ```
- The **destructor** should only call the ```Connection```'s destructor and delete all pointers and other allocated variables.
    ```
    // in the .cpp file
    [YOUR_CLASS]::~[YOUR_CLASS]() {
        // code here will be executed before the Connection destructor
        delete [VARIABLE_NAME];
    }
    ```
- The **close connection** function does not need parameter, because all connection items you'll need should be in the ```GeneralSocket``` item.
In this function you have to manage the connection closing.
    ```
    void closeConnection();
    ```
- The **subscribe** function should manage the subscriber's subscription to a spcified topic
    ```
    void subscribe(const string& topic);
    ```
- The **subscribe** function should manage the subscriber's unsubscription to a specified topic.
    ```
    void unsubscribe(const string& topic);
    ```
- The **send message** function should take a ```message``` type message and **only** send it.
    ```
    void sendMessage(const message& msg);
    ```
- The **receive message** function should call the receive function of your connection and save the arguments in the given ```message``` type message.
    ```
    void receiveMessage(message& msg);
    ```
- The **start pub** function have to start a ```PUB``` connection and then **must** create a thread calling the ```pubLoop();``` function and return the thread.
    ```
    thread* startPub();
    ```
- The **start sub** function have to start a ```SUB``` connection and then **must** create a thread calling the ```subLoop();``` function and return the thread.
    ```
    thread* startSub();
    ```

## Already implemented methods (in the Connection class)

##### Public methods
- The **init** function sets the connection's variables.  
As told before, to set the connection's variables you have to call this  function.
    ```
    void init(const string& address, const string& port, const int& openMode);
    ```
- The **set data** will create a ```message``` type message and insert it in the queue.
    ```
    void setData(string id, string data);
    ```
- The **start** function will start the connection using the parameter given to the **init** function.
    ```
    thread* start();
    ```
##### Protected methods
- This function is called by the **start pub** function.
It will loop and check if the queue is not empty, if there's at least one message, it will call your **send message** function.
    ```
    void pubLoop();
    ```
- This function is called by the **start sub** function.
It will loop and check if there are messages to receive by calling your **receive message** function.
The received message will be used by ```clbk_on_message```.
    ```
    void subLoop();
    ```
- This function will reset the connection's variables.
    ```
    void reset();
    ```
- This function will stop and close the connection calling your **close connection** function.
    ```
    void stop();
    ```
- This function will clear the queue.
    ```
    void clearData();
    ```
##### Callbacks
###### Set callbacks
- This function will set the given function to the ```clbk_on_open``` variable.
```
void add_on_open(function<void()>);
```
- This function will set the given function to the ```clbk_on_close``` variable.
```
void add_on_close(function<void(const int& code)>);
```
- This function will set the given function to the ```clbk_on_error``` variable.
```
void add_on_error(function<void(const int& code, const string& msg)>);
```
- This function will set the given function to the ```clbk_on_message``` variable.
```
void add_on_message(function<void(const message&)>);
```
- This function will set the given function to the ```clbk_on_subscribe``` variable.
```
void add_on_subscribe(function<void(const string&)>);
```
- This function will set the given function to the ```clbk_on_unsubscribe``` variable.
```
void add_on_unsubscribe(function<void(const string&)>);
```
###### Use callbacks
- This function will call the ```clbk_on_open``` callback.
It have to be called when the connection is opened.
    ```
    void on_open();
    ```
- This function will call the ```clbk_on_close``` callback.
It have to be called when the connection is closed (also when fatal errors occur).
The given code should be 0 if there's no error, otherwise it should be the error code.
    ```
    void on_close(const int& code);
    ```
- This function will call the ```clbk_on_error``` callback.
It have to be called when an error occurs (remember to handle errors).
It need the error code and the error message.
    ```
    void on_error(const int& code, const string& msg);
    ```
- This function will call the ```clbk_on_message``` callback.
It's used in the **sub loop** function.
It take the received message as a parameter.
    ```
    void on_message(const message& msg);
    ```
- This function will call the ```clbk_on_subscribe``` callback.
It have to be called when a subscription is done.
It take the topic as a parameter.
    ```
    void on_subscribe(const string& topic);
    ```
- This function will call the ```clbk_on_unsubscribe``` callback.
It have to be called when an unsubscription is done.
It take the topic as a parameter.
    ```
    void on_unsubscribe(const string& topic);
    ```

## Usage

### Create a connection
1. Create a connection object.
It will init connection variables.
    ```
    // the name should represent either the publisher or the subscriber
    [YOUR_CLASS] name;
    ```
2. Init the connection.
    ```
    name.init("localhost", "1883", PUB);

    // or

    name.init("localhost", "1883", SUB);
    ```
3. Set up the callbacks.
    ```
    // where [CALLBACK] is either open, close, error, message, subscribe, unsubscribe
    name.add_on_[CALLBACK]([&]() {
        // do something
    });
    ```
4. Start the connection.
It will automatically run the **pub loop** or the **sub loop**, depending on the connection's mode setted in the **init** function.
    ```
    thread* thread_name = name.start();
    ```
5. Subscribe/unsubscribe to/from a topic or set data.
    ```
    name.subscribe("topic");

    // or

    name.unsubscribe("topic");

    // or

    // the topic should be the ID of the message
    name.setData("topic", "data");
    ```
6. Close the connection.
    ```
    name.closeConnection();
    ```

### Examples
An example of the connection could be seen at ```src/zmq_connection.cpp```, with the associated header at ```inc/zmq_connection.h```.  
A working test example could be seen at ```scripts/testZMQ/test.cpp```. It's a simple connection that will publish and subscribe to a topic.  
The example describes a ZMQ working connection between a publisher and a subscriber comunicating between two different threads.