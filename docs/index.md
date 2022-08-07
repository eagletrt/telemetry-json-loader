# JSON Loader
This project allows generating c++ json loader classes from json files.  
Given json as input it detects all fields (structs, vectors ...) and generates c++ code for loading them.
The json library used is [rapidjson](git@github.com:Tencent/rapidjson.git)

# Usage
Input files are located under **json_input**, each subfolder will later represent a .h file.  
Each .json file will be parsed and will be added to the corresponding .h file. These .json file are called Main Structs, and some functions generated supports only Main Structs.
Nested structs are called Secondary structs, and cannot be loaded directly.

The generated code is placed in **out** or in branch build.

# Functions generated
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


// If this is a main struct:
// T is a struct
// J is a rapidjson::Document or a rapidjson::Value
template<class T>
bool LoadStruct(T& out, const std::string& path);
template<class T>
void SaveStruct(const T& obj, const std::string& path);

// If main struct
template<class T>
std::string StructToString(const T& obj);
template<class T>
std::string StructToStringPretty(const T& obj);
template<class T>
bool StringToStruct(const std::string& obj_str, T& obj);
~~~


**LoadStruct** and **SaveStruct** are directly json to file or file to json.
**StructToString** and **StringToStruct** are json to string or string to json (used in telemetry for exchanging messages).
**StructToStringPretty** is json to string with pretty formatting.
**Serialize**, **Deserialize** and **CheckJson** are internal functions.