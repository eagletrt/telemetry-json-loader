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

typedef struct post_processing_t{
    std::string last_path;
    uint64_t last_sample_freq;
    std::vector<std::string> no_resample_fields;
}post_processing_t;

typedef struct popup_t{
    std::string id;
    std::string cause;
    bool open;
    uint64_t threshold;
    uint64_t count;
    uint64_t duration;
}popup_t;

typedef struct filter_t{
    std::string message_str;
    std::string field_str;
    std::string operator_;
    double value;
}filter_t;

typedef struct events_o{
    filter_t filter;
    popup_t popup;
}events_o;

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
    std::vector<std::string> last_connection_ips;
    std::vector<events_o> events;
    uint64_t last_open_mode;
    post_processing_t post_processing;
}app_config;

#ifdef __APP_JSON_IMPLEMENTATION__

template <>
bool CheckJson(const post_processing_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("last_path")){
        JSON_LOG_FUNC("post_processing_t MISSING FIELD: last_path"); 
        check = false;
    }
    if(!doc.HasMember("last_sample_freq")){
        JSON_LOG_FUNC("post_processing_t MISSING FIELD: last_sample_freq"); 
        check = false;
    }
    if(!doc.HasMember("no_resample_fields")){
        JSON_LOG_FUNC("post_processing_t MISSING FIELD: no_resample_fields"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const post_processing_t& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("last_path", rapidjson::Value().SetString(obj.last_path.c_str(), obj.last_path.size(), alloc), alloc);
    out.AddMember("last_sample_freq", rapidjson::Value().SetUint64(obj.last_sample_freq), alloc);
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.no_resample_fields.size(); i++){
        	v0.PushBack(rapidjson::Value().SetString(obj.no_resample_fields[i].c_str(), obj.no_resample_fields[i].size(), alloc), alloc);
    	}
    	out.AddMember("no_resample_fields", v0, alloc);
    }
}
template<>
void Deserialize(post_processing_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("last_path") && doc["last_path"].IsString()){
        JSON_LOG_FUNC("post_processing_t MISSING FIELD: last_path"); 
    }else{
        obj.last_path = std::string(doc["last_path"].GetString(), doc["last_path"].GetStringLength());
    }
    if(!doc.HasMember("last_sample_freq") && doc["last_sample_freq"].IsUint64()){
        JSON_LOG_FUNC("post_processing_t MISSING FIELD: last_sample_freq"); 
    }else{
        obj.last_sample_freq = doc["last_sample_freq"].GetUint64();
    }
    if(!doc.HasMember("no_resample_fields") && doc["no_resample_fields"].IsObject()){
        JSON_LOG_FUNC("post_processing_t MISSING FIELD: no_resample_fields"); 
    }else{
		obj.no_resample_fields.resize(doc["no_resample_fields"].Size());
		for(rapidjson::SizeType i = 0; i < doc["no_resample_fields"].Size(); i++){
				obj.no_resample_fields[i] = doc["no_resample_fields"][i].GetString();
		}
    }
}

template <>
bool CheckJson(const popup_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("id")){
        JSON_LOG_FUNC("popup_t MISSING FIELD: id"); 
        check = false;
    }
    if(!doc.HasMember("cause")){
        JSON_LOG_FUNC("popup_t MISSING FIELD: cause"); 
        check = false;
    }
    if(!doc.HasMember("open")){
        JSON_LOG_FUNC("popup_t MISSING FIELD: open"); 
        check = false;
    }
    if(!doc.HasMember("threshold")){
        JSON_LOG_FUNC("popup_t MISSING FIELD: threshold"); 
        check = false;
    }
    if(!doc.HasMember("count")){
        JSON_LOG_FUNC("popup_t MISSING FIELD: count"); 
        check = false;
    }
    if(!doc.HasMember("duration")){
        JSON_LOG_FUNC("popup_t MISSING FIELD: duration"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const popup_t& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("id", rapidjson::Value().SetString(obj.id.c_str(), obj.id.size(), alloc), alloc);
    out.AddMember("cause", rapidjson::Value().SetString(obj.cause.c_str(), obj.cause.size(), alloc), alloc);
    out.AddMember("open", rapidjson::Value().SetBool(obj.open), alloc);
    out.AddMember("threshold", rapidjson::Value().SetUint64(obj.threshold), alloc);
    out.AddMember("count", rapidjson::Value().SetUint64(obj.count), alloc);
    out.AddMember("duration", rapidjson::Value().SetUint64(obj.duration), alloc);
}
template<>
void Deserialize(popup_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("id") && doc["id"].IsString()){
        JSON_LOG_FUNC("popup_t MISSING FIELD: id"); 
    }else{
        obj.id = std::string(doc["id"].GetString(), doc["id"].GetStringLength());
    }
    if(!doc.HasMember("cause") && doc["cause"].IsString()){
        JSON_LOG_FUNC("popup_t MISSING FIELD: cause"); 
    }else{
        obj.cause = std::string(doc["cause"].GetString(), doc["cause"].GetStringLength());
    }
    if(!doc.HasMember("open") && doc["open"].IsBool()){
        JSON_LOG_FUNC("popup_t MISSING FIELD: open"); 
    }else{
        obj.open = doc["open"].GetBool();
    }
    if(!doc.HasMember("threshold") && doc["threshold"].IsUint64()){
        JSON_LOG_FUNC("popup_t MISSING FIELD: threshold"); 
    }else{
        obj.threshold = doc["threshold"].GetUint64();
    }
    if(!doc.HasMember("count") && doc["count"].IsUint64()){
        JSON_LOG_FUNC("popup_t MISSING FIELD: count"); 
    }else{
        obj.count = doc["count"].GetUint64();
    }
    if(!doc.HasMember("duration") && doc["duration"].IsUint64()){
        JSON_LOG_FUNC("popup_t MISSING FIELD: duration"); 
    }else{
        obj.duration = doc["duration"].GetUint64();
    }
}

template <>
bool CheckJson(const filter_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("message_str")){
        JSON_LOG_FUNC("filter_t MISSING FIELD: message_str"); 
        check = false;
    }
    if(!doc.HasMember("field_str")){
        JSON_LOG_FUNC("filter_t MISSING FIELD: field_str"); 
        check = false;
    }
    if(!doc.HasMember("operator_")){
        JSON_LOG_FUNC("filter_t MISSING FIELD: operator_"); 
        check = false;
    }
    if(!doc.HasMember("value")){
        JSON_LOG_FUNC("filter_t MISSING FIELD: value"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const filter_t& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("message_str", rapidjson::Value().SetString(obj.message_str.c_str(), obj.message_str.size(), alloc), alloc);
    out.AddMember("field_str", rapidjson::Value().SetString(obj.field_str.c_str(), obj.field_str.size(), alloc), alloc);
    out.AddMember("operator_", rapidjson::Value().SetString(obj.operator_.c_str(), obj.operator_.size(), alloc), alloc);
    out.AddMember("value", rapidjson::Value().SetDouble(obj.value), alloc);
}
template<>
void Deserialize(filter_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("message_str") && doc["message_str"].IsString()){
        JSON_LOG_FUNC("filter_t MISSING FIELD: message_str"); 
    }else{
        obj.message_str = std::string(doc["message_str"].GetString(), doc["message_str"].GetStringLength());
    }
    if(!doc.HasMember("field_str") && doc["field_str"].IsString()){
        JSON_LOG_FUNC("filter_t MISSING FIELD: field_str"); 
    }else{
        obj.field_str = std::string(doc["field_str"].GetString(), doc["field_str"].GetStringLength());
    }
    if(!doc.HasMember("operator_") && doc["operator_"].IsString()){
        JSON_LOG_FUNC("filter_t MISSING FIELD: operator_"); 
    }else{
        obj.operator_ = std::string(doc["operator_"].GetString(), doc["operator_"].GetStringLength());
    }
    if(!doc.HasMember("value") && doc["value"].IsDouble()){
        JSON_LOG_FUNC("filter_t MISSING FIELD: value"); 
    }else{
        obj.value = doc["value"].GetDouble();
    }
}

template <>
bool CheckJson(const events_o& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("filter")){
        JSON_LOG_FUNC("events_o MISSING FIELD: filter"); 
        check = false;
    }
    if(!doc.HasMember("popup")){
        JSON_LOG_FUNC("events_o MISSING FIELD: popup"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const events_o& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    {
        rapidjson::Value v;
        Serialize(v, obj.filter, alloc);
        out.AddMember("filter", v, alloc);
    }
    {
        rapidjson::Value v;
        Serialize(v, obj.popup, alloc);
        out.AddMember("popup", v, alloc);
    }
}
template<>
void Deserialize(events_o& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("filter") && doc["filter"].IsObject()){
        JSON_LOG_FUNC("events_o MISSING FIELD: filter"); 
    }else{
        Deserialize(obj.filter, doc["filter"]);
    }
    if(!doc.HasMember("popup") && doc["popup"].IsObject()){
        JSON_LOG_FUNC("events_o MISSING FIELD: popup"); 
    }else{
        Deserialize(obj.popup, doc["popup"]);
    }
}

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
    if(!doc.HasMember("ip") && doc["ip"].IsString()){
        JSON_LOG_FUNC("app_connection_t MISSING FIELD: ip"); 
    }else{
        obj.ip = std::string(doc["ip"].GetString(), doc["ip"].GetStringLength());
    }
    if(!doc.HasMember("port") && doc["port"].IsString()){
        JSON_LOG_FUNC("app_connection_t MISSING FIELD: port"); 
    }else{
        obj.port = std::string(doc["port"].GetString(), doc["port"].GetStringLength());
    }
    if(!doc.HasMember("mode") && doc["mode"].IsString()){
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
    if(!doc.HasMember("last_connection_ips")){
        JSON_LOG_FUNC("app_config MISSING FIELD: last_connection_ips"); 
        check = false;
    }
    if(!doc.HasMember("events")){
        JSON_LOG_FUNC("app_config MISSING FIELD: events"); 
        check = false;
    }
    if(!doc.HasMember("last_open_mode")){
        JSON_LOG_FUNC("app_config MISSING FIELD: last_open_mode"); 
        check = false;
    }
    if(!doc.HasMember("post_processing")){
        JSON_LOG_FUNC("app_config MISSING FIELD: post_processing"); 
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
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.last_connection_ips.size(); i++){
        	v0.PushBack(rapidjson::Value().SetString(obj.last_connection_ips[i].c_str(), obj.last_connection_ips[i].size(), alloc), alloc);
    	}
    	out.AddMember("last_connection_ips", v0, alloc);
    }
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.events.size(); i++){
        	rapidjson::Value new_obj;
        	Serialize(new_obj, obj.events[i], alloc);
        	v0.PushBack(new_obj, alloc);
    	}
    	out.AddMember("events", v0, alloc);
    }
    out.AddMember("last_open_mode", rapidjson::Value().SetUint64(obj.last_open_mode), alloc);
    {
        rapidjson::Value v;
        Serialize(v, obj.post_processing, alloc);
        out.AddMember("post_processing", v, alloc);
    }
}
template<>
void Deserialize(app_config& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("csv_path") && doc["csv_path"].IsString()){
        JSON_LOG_FUNC("app_config MISSING FIELD: csv_path"); 
    }else{
        obj.csv_path = std::string(doc["csv_path"].GetString(), doc["csv_path"].GetStringLength());
    }
    if(!doc.HasMember("csv_auto_save") && doc["csv_auto_save"].IsBool()){
        JSON_LOG_FUNC("app_config MISSING FIELD: csv_auto_save"); 
    }else{
        obj.csv_auto_save = doc["csv_auto_save"].GetBool();
    }
    if(!doc.HasMember("telemetry_auto_get_config") && doc["telemetry_auto_get_config"].IsBool()){
        JSON_LOG_FUNC("app_config MISSING FIELD: telemetry_auto_get_config"); 
    }else{
        obj.telemetry_auto_get_config = doc["telemetry_auto_get_config"].GetBool();
    }
    if(!doc.HasMember("last_login_time") && doc["last_login_time"].IsDouble()){
        JSON_LOG_FUNC("app_config MISSING FIELD: last_login_time"); 
    }else{
        obj.last_login_time = doc["last_login_time"].GetDouble();
    }
    if(!doc.HasMember("app_connection") && doc["app_connection"].IsObject()){
        JSON_LOG_FUNC("app_config MISSING FIELD: app_connection"); 
    }else{
        Deserialize(obj.app_connection, doc["app_connection"]);
    }
    if(!doc.HasMember("activeTabs") && doc["activeTabs"].IsObject()){
        JSON_LOG_FUNC("app_config MISSING FIELD: activeTabs"); 
    }else{
		obj.activeTabs.resize(doc["activeTabs"].Size());
		for(rapidjson::SizeType i = 0; i < doc["activeTabs"].Size(); i++){
				obj.activeTabs[i] = doc["activeTabs"][i].GetString();
		}
    }
    if(!doc.HasMember("last_connection_ips") && doc["last_connection_ips"].IsObject()){
        JSON_LOG_FUNC("app_config MISSING FIELD: last_connection_ips"); 
    }else{
		obj.last_connection_ips.resize(doc["last_connection_ips"].Size());
		for(rapidjson::SizeType i = 0; i < doc["last_connection_ips"].Size(); i++){
				obj.last_connection_ips[i] = doc["last_connection_ips"][i].GetString();
		}
    }
    if(!doc.HasMember("events") && doc["events"].IsObject()){
        JSON_LOG_FUNC("app_config MISSING FIELD: events"); 
    }else{
		obj.events.resize(doc["events"].Size());
		for(rapidjson::SizeType i = 0; i < doc["events"].Size(); i++){
				Deserialize(obj.events[i], doc["events"][i]);
		}
    }
    if(!doc.HasMember("last_open_mode") && doc["last_open_mode"].IsUint64()){
        JSON_LOG_FUNC("app_config MISSING FIELD: last_open_mode"); 
    }else{
        obj.last_open_mode = doc["last_open_mode"].GetUint64();
    }
    if(!doc.HasMember("post_processing") && doc["post_processing"].IsObject()){
        JSON_LOG_FUNC("app_config MISSING FIELD: post_processing"); 
    }else{
        Deserialize(obj.post_processing, doc["post_processing"]);
    }
}
template<>
void Deserialize(app_config& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("csv_path") && doc["csv_path"].IsString()){
        JSON_LOG_FUNC("app_config MISSING FIELD: csv_path"); 
    }else{
        obj.csv_path = std::string(doc["csv_path"].GetString(), doc["csv_path"].GetStringLength());
    }
    if(!doc.HasMember("csv_auto_save") && doc["csv_auto_save"].IsBool()){
        JSON_LOG_FUNC("app_config MISSING FIELD: csv_auto_save"); 
    }else{
        obj.csv_auto_save = doc["csv_auto_save"].GetBool();
    }
    if(!doc.HasMember("telemetry_auto_get_config") && doc["telemetry_auto_get_config"].IsBool()){
        JSON_LOG_FUNC("app_config MISSING FIELD: telemetry_auto_get_config"); 
    }else{
        obj.telemetry_auto_get_config = doc["telemetry_auto_get_config"].GetBool();
    }
    if(!doc.HasMember("last_login_time") && doc["last_login_time"].IsDouble()){
        JSON_LOG_FUNC("app_config MISSING FIELD: last_login_time"); 
    }else{
        obj.last_login_time = doc["last_login_time"].GetDouble();
    }
    if(!doc.HasMember("app_connection") && doc["app_connection"].IsObject()){
        JSON_LOG_FUNC("app_config MISSING FIELD: app_connection"); 
    }else{
        Deserialize(obj.app_connection, doc["app_connection"]);
    }
    if(!doc.HasMember("activeTabs") && doc["activeTabs"].IsObject()){
        JSON_LOG_FUNC("app_config MISSING FIELD: activeTabs"); 
    }else{
		obj.activeTabs.resize(doc["activeTabs"].Size());
		for(rapidjson::SizeType i = 0; i < doc["activeTabs"].Size(); i++){
				obj.activeTabs[i] = doc["activeTabs"][i].GetString();
		}
    }
    if(!doc.HasMember("last_connection_ips") && doc["last_connection_ips"].IsObject()){
        JSON_LOG_FUNC("app_config MISSING FIELD: last_connection_ips"); 
    }else{
		obj.last_connection_ips.resize(doc["last_connection_ips"].Size());
		for(rapidjson::SizeType i = 0; i < doc["last_connection_ips"].Size(); i++){
				obj.last_connection_ips[i] = doc["last_connection_ips"][i].GetString();
		}
    }
    if(!doc.HasMember("events") && doc["events"].IsObject()){
        JSON_LOG_FUNC("app_config MISSING FIELD: events"); 
    }else{
		obj.events.resize(doc["events"].Size());
		for(rapidjson::SizeType i = 0; i < doc["events"].Size(); i++){
				Deserialize(obj.events[i], doc["events"][i]);
		}
    }
    if(!doc.HasMember("last_open_mode") && doc["last_open_mode"].IsUint64()){
        JSON_LOG_FUNC("app_config MISSING FIELD: last_open_mode"); 
    }else{
        obj.last_open_mode = doc["last_open_mode"].GetUint64();
    }
    if(!doc.HasMember("post_processing") && doc["post_processing"].IsObject()){
        JSON_LOG_FUNC("app_config MISSING FIELD: post_processing"); 
    }else{
        Deserialize(obj.post_processing, doc["post_processing"]);
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