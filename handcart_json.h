#ifndef __HANDCART_JSON__
#define __HANDCART_JSON__

#include <string>
#include <vector>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>


#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#ifndef JSON_LOG_FUNC
#define JSON_LOG_FUNC(msg) std::cout << msg << std::endl;
#endif

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


#ifndef __JSON_LOADER_DEFINITION__
#define __JSON_LOADER_DEFINITION__
static void LoadJSON(rapidjson::Document& out, const std::string& path){
    std::ifstream f(path);
    std::stringstream buffer;
    buffer << f.rdbuf();
    out.Parse(buffer.str().c_str());
}
static void SaveJSON(const rapidjson::Document& doc, const std::string& path){
    char writeBuffer[65536];
    FILE* fp = fopen(path.c_str(), "w");
    rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
    doc.Accept(writer);
    fclose(fp);
}
#endif // __JSON_LOADER_DEFINITION__

typedef struct handcart_settings_t{
    uint64_t fan_speed;
    double target_voltage;
    double max_current_out;
    double max_current_in;
}handcart_settings_t;

#ifdef __HANDCART_JSON_IMPLEMENTATION__

template <>
bool CheckJson(const handcart_settings_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("fan_speed")){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: fan_speed"); 
        check = false;
    }
    if(!doc.HasMember("target_voltage")){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: target_voltage"); 
        check = false;
    }
    if(!doc.HasMember("max_current_out")){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: max_current_out"); 
        check = false;
    }
    if(!doc.HasMember("max_current_in")){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: max_current_in"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const handcart_settings_t& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("fan_speed", rapidjson::Value().SetUint64(obj.fan_speed), alloc);
    out.AddMember("target_voltage", rapidjson::Value().SetDouble(obj.target_voltage), alloc);
    out.AddMember("max_current_out", rapidjson::Value().SetDouble(obj.max_current_out), alloc);
    out.AddMember("max_current_in", rapidjson::Value().SetDouble(obj.max_current_in), alloc);
}
template<>
void Deserialize(handcart_settings_t& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("fan_speed") || !doc["fan_speed"].IsUint64()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: fan_speed"); 
    }else{
        obj.fan_speed = doc["fan_speed"].GetUint64();
    }
    if(!doc.HasMember("target_voltage") || !doc["target_voltage"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: target_voltage"); 
    }else{
        obj.target_voltage = doc["target_voltage"].GetDouble();
    }
    if(!doc.HasMember("max_current_out") || !doc["max_current_out"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: max_current_out"); 
    }else{
        obj.max_current_out = doc["max_current_out"].GetDouble();
    }
    if(!doc.HasMember("max_current_in") || !doc["max_current_in"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: max_current_in"); 
    }else{
        obj.max_current_in = doc["max_current_in"].GetDouble();
    }
}
template<>
void Deserialize(handcart_settings_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("fan_speed") || !doc["fan_speed"].IsUint64()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: fan_speed"); 
    }else{
        obj.fan_speed = doc["fan_speed"].GetUint64();
    }
    if(!doc.HasMember("target_voltage") || !doc["target_voltage"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: target_voltage"); 
    }else{
        obj.target_voltage = doc["target_voltage"].GetDouble();
    }
    if(!doc.HasMember("max_current_out") || !doc["max_current_out"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: max_current_out"); 
    }else{
        obj.max_current_out = doc["max_current_out"].GetDouble();
    }
    if(!doc.HasMember("max_current_in") || !doc["max_current_in"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: max_current_in"); 
    }else{
        obj.max_current_in = doc["max_current_in"].GetDouble();
    }
}

template<>
std::string StructToString(const handcart_settings_t& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const handcart_settings_t& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, handcart_settings_t& out)
{
    rapidjson::Document doc;
    rapidjson::ParseResult ok = doc.Parse(obj_str.c_str(), obj_str.size());
    if(!ok)
        return false;
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}

template<>
bool LoadStruct(handcart_settings_t& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const handcart_settings_t& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

#endif //__HANDCART_JSON_IMPLEMENTATION__

#endif // __JSON_LOADER__