#ifndef __APP_JSON__
#define __APP_JSON__

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

typedef struct app_connection_t{
    std::string ip;
    std::string port;
    std::string mode;
}app_connection_t;

typedef struct app_config{
    std::string csv_path;
    bool csv_auto_save;
    bool telemetry_auto_get_config;
    double last_login_time;
    app_connection_t app_connection;
    std::vector<std::string> activeTabs;
}app_config;

#ifdef __APP_JSON_IMPLEMENTATION__

template <>
bool CheckJson(const app_connection_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("ip")){
        JSON_LOG_FUNC("app_connection_t MISSING FIELD: ip"); 
        check = false;
    }
    if(!doc.HasMember("port")){
        JSON_LOG_FUNC("app_connection_t MISSING FIELD: port"); 
        check = false;
    }
    if(!doc.HasMember("mode")){
        JSON_LOG_FUNC("app_connection_t MISSING FIELD: mode"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const app_connection_t& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("ip", rapidjson::Value().SetString(obj.ip.c_str(), obj.ip.size(), alloc), alloc);
    out.AddMember("port", rapidjson::Value().SetString(obj.port.c_str(), obj.port.size(), alloc), alloc);
    out.AddMember("mode", rapidjson::Value().SetString(obj.mode.c_str(), obj.mode.size(), alloc), alloc);
}
template<>
void Deserialize(app_connection_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("ip")){
        JSON_LOG_FUNC("app_connection_t MISSING FIELD: ip"); 
    }else{
    obj.ip = std::string(doc["ip"].GetString(), doc["ip"].GetStringLength());
    }
    if(!doc.HasMember("port")){
        JSON_LOG_FUNC("app_connection_t MISSING FIELD: port"); 
    }else{
    obj.port = std::string(doc["port"].GetString(), doc["port"].GetStringLength());
    }
    if(!doc.HasMember("mode")){
        JSON_LOG_FUNC("app_connection_t MISSING FIELD: mode"); 
    }else{
    obj.mode = std::string(doc["mode"].GetString(), doc["mode"].GetStringLength());
    }
}

template <>
bool CheckJson(const app_config& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("csv_path")){
        JSON_LOG_FUNC("app_config MISSING FIELD: csv_path"); 
        check = false;
    }
    if(!doc.HasMember("csv_auto_save")){
        JSON_LOG_FUNC("app_config MISSING FIELD: csv_auto_save"); 
        check = false;
    }
    if(!doc.HasMember("telemetry_auto_get_config")){
        JSON_LOG_FUNC("app_config MISSING FIELD: telemetry_auto_get_config"); 
        check = false;
    }
    if(!doc.HasMember("last_login_time")){
        JSON_LOG_FUNC("app_config MISSING FIELD: last_login_time"); 
        check = false;
    }
    if(!doc.HasMember("app_connection")){
        JSON_LOG_FUNC("app_config MISSING FIELD: app_connection"); 
        check = false;
    }
    if(!doc.HasMember("activeTabs")){
        JSON_LOG_FUNC("app_config MISSING FIELD: activeTabs"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const app_config& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("csv_path", rapidjson::Value().SetString(obj.csv_path.c_str(), obj.csv_path.size(), alloc), alloc);
    out.AddMember("csv_auto_save", rapidjson::Value().SetBool(obj.csv_auto_save), alloc);
    out.AddMember("telemetry_auto_get_config", rapidjson::Value().SetBool(obj.telemetry_auto_get_config), alloc);
    out.AddMember("last_login_time", rapidjson::Value().SetDouble(obj.last_login_time), alloc);
    {
        rapidjson::Value v;
        Serialize(v, obj.app_connection, alloc);
        out.AddMember("app_connection", v, alloc);
    }
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.activeTabs.size(); i++){
        	v0.PushBack(rapidjson::Value().SetString(obj.activeTabs[i].c_str(), obj.activeTabs[i].size(), alloc), alloc);
    	}
    	out.AddMember("activeTabs", v0, alloc);
    }
}
template<>
void Deserialize(app_config& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("csv_path")){
        JSON_LOG_FUNC("app_config MISSING FIELD: csv_path"); 
    }else{
    obj.csv_path = std::string(doc["csv_path"].GetString(), doc["csv_path"].GetStringLength());
    }
    if(!doc.HasMember("csv_auto_save")){
        JSON_LOG_FUNC("app_config MISSING FIELD: csv_auto_save"); 
    }else{
        obj.csv_auto_save = doc["csv_auto_save"].GetBool();
    }
    if(!doc.HasMember("telemetry_auto_get_config")){
        JSON_LOG_FUNC("app_config MISSING FIELD: telemetry_auto_get_config"); 
    }else{
        obj.telemetry_auto_get_config = doc["telemetry_auto_get_config"].GetBool();
    }
    if(!doc.HasMember("last_login_time")){
        JSON_LOG_FUNC("app_config MISSING FIELD: last_login_time"); 
    }else{
        obj.last_login_time = doc["last_login_time"].GetDouble();
    }
    if(!doc.HasMember("app_connection")){
        JSON_LOG_FUNC("app_config MISSING FIELD: app_connection"); 
    }else{
        Deserialize(obj.app_connection, doc["app_connection"]);
    }
    if(!doc.HasMember("activeTabs")){
        JSON_LOG_FUNC("app_config MISSING FIELD: activeTabs"); 
    }else{
	obj.activeTabs.resize(doc["activeTabs"].Size());
	for(rapidjson::SizeType i = 0; i < doc["activeTabs"].Size(); i++){
		obj.activeTabs[i] = doc["activeTabs"][i].GetString();
	}
    }
}
template<>
void Deserialize(app_config& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("csv_path")){
        JSON_LOG_FUNC("app_config MISSING FIELD: csv_path"); 
    }else{
    obj.csv_path = std::string(doc["csv_path"].GetString(), doc["csv_path"].GetStringLength());
    }
    if(!doc.HasMember("csv_auto_save")){
        JSON_LOG_FUNC("app_config MISSING FIELD: csv_auto_save"); 
    }else{
        obj.csv_auto_save = doc["csv_auto_save"].GetBool();
    }
    if(!doc.HasMember("telemetry_auto_get_config")){
        JSON_LOG_FUNC("app_config MISSING FIELD: telemetry_auto_get_config"); 
    }else{
        obj.telemetry_auto_get_config = doc["telemetry_auto_get_config"].GetBool();
    }
    if(!doc.HasMember("last_login_time")){
        JSON_LOG_FUNC("app_config MISSING FIELD: last_login_time"); 
    }else{
        obj.last_login_time = doc["last_login_time"].GetDouble();
    }
    if(!doc.HasMember("app_connection")){
        JSON_LOG_FUNC("app_config MISSING FIELD: app_connection"); 
    }else{
        Deserialize(obj.app_connection, doc["app_connection"]);
    }
    if(!doc.HasMember("activeTabs")){
        JSON_LOG_FUNC("app_config MISSING FIELD: activeTabs"); 
    }else{
	obj.activeTabs.resize(doc["activeTabs"].Size());
	for(rapidjson::SizeType i = 0; i < doc["activeTabs"].Size(); i++){
		obj.activeTabs[i] = doc["activeTabs"][i].GetString();
	}
    }
}

template<>
std::string StructToString(const app_config& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const app_config& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, app_config& out)
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
bool LoadStruct(app_config& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const app_config& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

#endif //__APP_JSON_IMPLEMENTATION__

#endif // __JSON_LOADER__