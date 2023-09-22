# JSON Loader
This project allows generating c++ json loader classes from json files.  
Given json as input it detects all fields (structs, vectors ...) and generates c++ code for loading them.
The json library used is [rapidjson](git@github.com:Tencent/rapidjson.git)

# Usage
Input files are located under **json_input**, each subfolder will later represent a .h file.  
Each .json file will be parsed and will be added to the corresponding .h file. These .json file are called Main Structs, and some functions generated supports only Main Structs.
Nested structs are called Secondary structs, and cannot be loaded directly.

The generated code is placed in **out** or in branch build.

## Struct Generation
Supported types are:

- int
- double
- string
- bool
- vectors
- objects
- and vector of objects (also multiple levels of nesting)

### Simple JSON
This JSON:
~~~JSON
{
    "Messages": 351245,
    "Average_Frequency_Hz": 3078,
    "Duration_seconds": 114.093
}
~~~
Will generate the following c++ struct:
~~~c++
typedef struct stat_json{
    int Messages;
    int Average_Frequency_Hz;
    double Duration_seconds;
}stat_json;
~~~
Where stat_json is the filename used in the code generator.  

### Complex JSON
This JSON:
~~~JSON
{
    "type": "telemetry_status",
    "timestamp": 0.0,
    "data": 1,
    "msgs_per_second": [
        {
            "device": "name",
            "count": 1
        }
    ],
    "camera_status": "fail",
    "cpu_process_load": 10
}
~~~
Will generate the following c++ structs:
~~~c++
typedef struct msgs_per_second_o{
    std::string device;
    int count;
}msgs_per_second_o;

typedef struct telemetry_status{
    std::string type;
    double timestamp;
    int data;
    std::vector<msgs_per_second_o> msgs_per_second;
    std::string camera_status;
    int cpu_process_load;
}telemetry_status;
~~~

## Logging
If the application is using logging functions the JSON loader exposes a macro to define the log function that will be used to log messages, for example when checking JSON.  
Default log function is:
~~~c++
#ifndef JSON_LOG_FUNC
#define JSON_LOG_FUNC(msg) std::cout << msg << std::endl;
#endif
~~~

## Functions

The so called main structs are the struct of the first level of nesting.  These are the only one that can use LoadJson, SaveJson, StructToString, and StringToStruct.

~~~c++
// T is a struct
// J is a rapidjson::Document or a rapidjson::Value
template<class T, class J>
bool CheckJson(const T& obj, const J& doc);

// T is a struct
// J is a rapidjson::Document or a rapidjson::Value
template<class T, class J>
void Deserialize(T& obj, J& doc);
template<class T>
void Serialize(rapidjson::Document& out, const T& obj);
template<class T>
void Serialize(rapidjson::Value& out, const T& obj, rapidjson::Document::AllocatorType& alloc);

// ---- Main structs only ---- //
// T is a struct
// J is a rapidjson::Document or a rapidjson::Value
template<class T>
bool LoadStruct(T& out, const std::string& path);
template<class T>
void SaveStruct(const T& obj, const std::string& path);

template<class T>
std::string StructToString(const T& obj);
template<class T>
bool StringToStruct(const std::string& obj_str, T& obj);
~~~

- **CheckJson** checks if all the required fields are present in the JSON file, doesn't throw any exceptions but returns false if any of the required fields is missing. The missing fields are logged using ```JSON_LOG_FUNC```.
- **LoadStruct** loads a JSON file and deserializes it into a struct. Before loading the JSON file it checks if it is valid. If it is not valid it logs the error using ```JSON_LOG_FUNC```. If the JSON has some missing fileds it will still load all the valid one.
- **SaveStruct** saves a struct into a JSON file.
- **StructToString** converts a struct to a string.
- **StringToStruct** converts a JSON formatted string to a struct, as always checks the JSON before deserializing.
- **Deserialize** deserializes a JSON file into a struct.
- **Serialize** serializes a struct into a JSON file.
