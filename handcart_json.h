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
    double target_voltage;
    bool fans_override;
    double fans_speed;
    double acc_charge_current;
    double grid_max_current;
    uint64_t status;
}handcart_settings_t;

#ifdef __HANDCART_JSON_IMPLEMENTATION__

template <>
bool CheckJson(const handcart_settings_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("target_voltage")){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: target_voltage"); 
        check = false;
    }
    if(!doc.HasMember("fans_override")){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: fans_override"); 
        check = false;
    }
    if(!doc.HasMember("fans_speed")){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: fans_speed"); 
        check = false;
    }
    if(!doc.HasMember("acc_charge_current")){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: acc_charge_current"); 
        check = false;
    }
    if(!doc.HasMember("grid_max_current")){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: grid_max_current"); 
        check = false;
    }
    if(!doc.HasMember("status")){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: status"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const handcart_settings_t& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("target_voltage", rapidjson::Value().SetDouble(obj.target_voltage), alloc);
    out.AddMember("fans_override", rapidjson::Value().SetBool(obj.fans_override), alloc);
    out.AddMember("fans_speed", rapidjson::Value().SetDouble(obj.fans_speed), alloc);
    out.AddMember("acc_charge_current", rapidjson::Value().SetDouble(obj.acc_charge_current), alloc);
    out.AddMember("grid_max_current", rapidjson::Value().SetDouble(obj.grid_max_current), alloc);
    out.AddMember("status", rapidjson::Value().SetUint64(obj.status), alloc);
}
template<>
void Deserialize(handcart_settings_t& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("target_voltage") || !doc["target_voltage"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: target_voltage"); 
    }else{
        obj.target_voltage = doc["target_voltage"].GetDouble();
    }
    if(!doc.HasMember("fans_override") || !doc["fans_override"].IsBool()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: fans_override"); 
    }else{
        obj.fans_override = doc["fans_override"].GetBool();
    }
    if(!doc.HasMember("fans_speed") || !doc["fans_speed"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: fans_speed"); 
    }else{
        obj.fans_speed = doc["fans_speed"].GetDouble();
    }
    if(!doc.HasMember("acc_charge_current") || !doc["acc_charge_current"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: acc_charge_current"); 
    }else{
        obj.acc_charge_current = doc["acc_charge_current"].GetDouble();
    }
    if(!doc.HasMember("grid_max_current") || !doc["grid_max_current"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: grid_max_current"); 
    }else{
        obj.grid_max_current = doc["grid_max_current"].GetDouble();
    }
    if(!doc.HasMember("status") || !doc["status"].IsUint64()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: status"); 
    }else{
        obj.status = doc["status"].GetUint64();
    }
}
template<>
void Deserialize(handcart_settings_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("target_voltage") || !doc["target_voltage"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: target_voltage"); 
    }else{
        obj.target_voltage = doc["target_voltage"].GetDouble();
    }
    if(!doc.HasMember("fans_override") || !doc["fans_override"].IsBool()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: fans_override"); 
    }else{
        obj.fans_override = doc["fans_override"].GetBool();
    }
    if(!doc.HasMember("fans_speed") || !doc["fans_speed"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: fans_speed"); 
    }else{
        obj.fans_speed = doc["fans_speed"].GetDouble();
    }
    if(!doc.HasMember("acc_charge_current") || !doc["acc_charge_current"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: acc_charge_current"); 
    }else{
        obj.acc_charge_current = doc["acc_charge_current"].GetDouble();
    }
    if(!doc.HasMember("grid_max_current") || !doc["grid_max_current"].IsDouble()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: grid_max_current"); 
    }else{
        obj.grid_max_current = doc["grid_max_current"].GetDouble();
    }
    if(!doc.HasMember("status") || !doc["status"].IsUint64()){
        JSON_LOG_FUNC("handcart_settings_t MISSING FIELD: status"); 
    }else{
        obj.status = doc["status"].GetUint64();
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