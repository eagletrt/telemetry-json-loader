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

typedef struct fields_a{
    std::string message;
    std::string field;
}fields_a;

typedef struct axis_a{
    std::string axis_name;
    std::vector<fields_a> fields;
}axis_a;

typedef struct custom_plots_a{
    std::string plot_name;
    std::vector<axis_a> axis;
}custom_plots_a;

typedef struct color_o{
    double r;
    double g;
    double b;
    double a;
}color_o;

typedef struct json_trigger_o{
    std::string message;
    std::string field;
    uint64_t comparator;
    double value;
    color_o color;
}json_trigger_o;

typedef struct events_a{
    json_trigger_o json_trigger;
}events_a;

typedef struct post_proc_o{
    uint64_t last_sample_freq;
    std::vector<std::string> include_items;
    std::vector<std::string> exclude_items;
}post_proc_o;

typedef struct saved_confs_a{
    std::string name;
    std::string ip;
    std::string port;
    std::string mode;
}saved_confs_a;

typedef struct app_connection_o{
    std::string ip;
    std::string port;
    std::string mode;
    std::vector<saved_confs_a> saved_confs;
}app_connection_o;

typedef struct paths_a{
    std::string key;
    std::string value;
}paths_a;

typedef struct generic_key_value_a{
    std::string key;
    std::string value;
}generic_key_value_a;

typedef struct app_config_t{
    std::vector<generic_key_value_a> generic_key_value;
    std::vector<paths_a> paths;
    bool csv_auto_save;
    bool telemetry_auto_get_config;
    double last_login_time;
    app_connection_o app_connection;
    std::vector<std::string> activeTabs;
    std::vector<std::string> last_connection_ips;
    post_proc_o post_proc;
    std::vector<events_a> events;
    std::vector<custom_plots_a> custom_plots;
    uint64_t theme;
    uint64_t last_open_mode;
}app_config_t;

typedef struct user_data_t{
    std::string username;
    std::string token;
    std::string refresh_token;
    double expiry;
    uint64_t role;
}user_data_t;

#ifdef __APP_JSON_IMPLEMENTATION__

template <>
bool CheckJson(const fields_a& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("message")){
        JSON_LOG_FUNC("fields_a MISSING FIELD: message"); 
        check = false;
    }
    if(!doc.HasMember("field")){
        JSON_LOG_FUNC("fields_a MISSING FIELD: field"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const fields_a& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("message", rapidjson::Value().SetString(obj.message.c_str(), obj.message.size(), alloc), alloc);
    out.AddMember("field", rapidjson::Value().SetString(obj.field.c_str(), obj.field.size(), alloc), alloc);
}
template<>
void Deserialize(fields_a& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("message") || !doc["message"].IsString()){
        JSON_LOG_FUNC("fields_a MISSING FIELD: message"); 
    }else{
        obj.message = std::string(doc["message"].GetString(), doc["message"].GetStringLength());
    }
    if(!doc.HasMember("field") || !doc["field"].IsString()){
        JSON_LOG_FUNC("fields_a MISSING FIELD: field"); 
    }else{
        obj.field = std::string(doc["field"].GetString(), doc["field"].GetStringLength());
    }
}

template <>
bool CheckJson(const axis_a& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("axis_name")){
        JSON_LOG_FUNC("axis_a MISSING FIELD: axis_name"); 
        check = false;
    }
    if(!doc.HasMember("fields")){
        JSON_LOG_FUNC("axis_a MISSING FIELD: fields"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const axis_a& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("axis_name", rapidjson::Value().SetString(obj.axis_name.c_str(), obj.axis_name.size(), alloc), alloc);
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.fields.size(); i++){
        	rapidjson::Value new_obj;
        	Serialize(new_obj, obj.fields[i], alloc);
        	v0.PushBack(new_obj, alloc);
    	}
    	out.AddMember("fields", v0, alloc);
    }
}
template<>
void Deserialize(axis_a& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("axis_name") || !doc["axis_name"].IsString()){
        JSON_LOG_FUNC("axis_a MISSING FIELD: axis_name"); 
    }else{
        obj.axis_name = std::string(doc["axis_name"].GetString(), doc["axis_name"].GetStringLength());
    }
    if(!doc.HasMember("fields") || !doc["fields"].IsArray()){
        JSON_LOG_FUNC("axis_a MISSING FIELD: fields"); 
    }else{
		obj.fields.resize(doc["fields"].Size());
		for(rapidjson::SizeType i = 0; i < doc["fields"].Size(); i++){
				Deserialize(obj.fields[i], doc["fields"][i]);
		}
    }
}

template <>
bool CheckJson(const custom_plots_a& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("plot_name")){
        JSON_LOG_FUNC("custom_plots_a MISSING FIELD: plot_name"); 
        check = false;
    }
    if(!doc.HasMember("axis")){
        JSON_LOG_FUNC("custom_plots_a MISSING FIELD: axis"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const custom_plots_a& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("plot_name", rapidjson::Value().SetString(obj.plot_name.c_str(), obj.plot_name.size(), alloc), alloc);
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.axis.size(); i++){
        	rapidjson::Value new_obj;
        	Serialize(new_obj, obj.axis[i], alloc);
        	v0.PushBack(new_obj, alloc);
    	}
    	out.AddMember("axis", v0, alloc);
    }
}
template<>
void Deserialize(custom_plots_a& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("plot_name") || !doc["plot_name"].IsString()){
        JSON_LOG_FUNC("custom_plots_a MISSING FIELD: plot_name"); 
    }else{
        obj.plot_name = std::string(doc["plot_name"].GetString(), doc["plot_name"].GetStringLength());
    }
    if(!doc.HasMember("axis") || !doc["axis"].IsArray()){
        JSON_LOG_FUNC("custom_plots_a MISSING FIELD: axis"); 
    }else{
		obj.axis.resize(doc["axis"].Size());
		for(rapidjson::SizeType i = 0; i < doc["axis"].Size(); i++){
				Deserialize(obj.axis[i], doc["axis"][i]);
		}
    }
}

template <>
bool CheckJson(const color_o& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("r")){
        JSON_LOG_FUNC("color_o MISSING FIELD: r"); 
        check = false;
    }
    if(!doc.HasMember("g")){
        JSON_LOG_FUNC("color_o MISSING FIELD: g"); 
        check = false;
    }
    if(!doc.HasMember("b")){
        JSON_LOG_FUNC("color_o MISSING FIELD: b"); 
        check = false;
    }
    if(!doc.HasMember("a")){
        JSON_LOG_FUNC("color_o MISSING FIELD: a"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const color_o& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("r", rapidjson::Value().SetDouble(obj.r), alloc);
    out.AddMember("g", rapidjson::Value().SetDouble(obj.g), alloc);
    out.AddMember("b", rapidjson::Value().SetDouble(obj.b), alloc);
    out.AddMember("a", rapidjson::Value().SetDouble(obj.a), alloc);
}
template<>
void Deserialize(color_o& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("r") || !doc["r"].IsDouble()){
        JSON_LOG_FUNC("color_o MISSING FIELD: r"); 
    }else{
        obj.r = doc["r"].GetDouble();
    }
    if(!doc.HasMember("g") || !doc["g"].IsDouble()){
        JSON_LOG_FUNC("color_o MISSING FIELD: g"); 
    }else{
        obj.g = doc["g"].GetDouble();
    }
    if(!doc.HasMember("b") || !doc["b"].IsDouble()){
        JSON_LOG_FUNC("color_o MISSING FIELD: b"); 
    }else{
        obj.b = doc["b"].GetDouble();
    }
    if(!doc.HasMember("a") || !doc["a"].IsDouble()){
        JSON_LOG_FUNC("color_o MISSING FIELD: a"); 
    }else{
        obj.a = doc["a"].GetDouble();
    }
}

template <>
bool CheckJson(const json_trigger_o& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("message")){
        JSON_LOG_FUNC("json_trigger_o MISSING FIELD: message"); 
        check = false;
    }
    if(!doc.HasMember("field")){
        JSON_LOG_FUNC("json_trigger_o MISSING FIELD: field"); 
        check = false;
    }
    if(!doc.HasMember("comparator")){
        JSON_LOG_FUNC("json_trigger_o MISSING FIELD: comparator"); 
        check = false;
    }
    if(!doc.HasMember("value")){
        JSON_LOG_FUNC("json_trigger_o MISSING FIELD: value"); 
        check = false;
    }
    if(!doc.HasMember("color")){
        JSON_LOG_FUNC("json_trigger_o MISSING FIELD: color"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const json_trigger_o& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("message", rapidjson::Value().SetString(obj.message.c_str(), obj.message.size(), alloc), alloc);
    out.AddMember("field", rapidjson::Value().SetString(obj.field.c_str(), obj.field.size(), alloc), alloc);
    out.AddMember("comparator", rapidjson::Value().SetUint64(obj.comparator), alloc);
    out.AddMember("value", rapidjson::Value().SetDouble(obj.value), alloc);
    {
        rapidjson::Value v;
        Serialize(v, obj.color, alloc);
        out.AddMember("color", v, alloc);
    }
}
template<>
void Deserialize(json_trigger_o& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("message") || !doc["message"].IsString()){
        JSON_LOG_FUNC("json_trigger_o MISSING FIELD: message"); 
    }else{
        obj.message = std::string(doc["message"].GetString(), doc["message"].GetStringLength());
    }
    if(!doc.HasMember("field") || !doc["field"].IsString()){
        JSON_LOG_FUNC("json_trigger_o MISSING FIELD: field"); 
    }else{
        obj.field = std::string(doc["field"].GetString(), doc["field"].GetStringLength());
    }
    if(!doc.HasMember("comparator") || !doc["comparator"].IsUint64()){
        JSON_LOG_FUNC("json_trigger_o MISSING FIELD: comparator"); 
    }else{
        obj.comparator = doc["comparator"].GetUint64();
    }
    if(!doc.HasMember("value") || !doc["value"].IsDouble()){
        JSON_LOG_FUNC("json_trigger_o MISSING FIELD: value"); 
    }else{
        obj.value = doc["value"].GetDouble();
    }
    if(!doc.HasMember("color") || !doc["color"].IsObject()){
        JSON_LOG_FUNC("json_trigger_o MISSING FIELD: color"); 
    }else{
        Deserialize(obj.color, doc["color"]);
    }
}

template <>
bool CheckJson(const events_a& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("json_trigger")){
        JSON_LOG_FUNC("events_a MISSING FIELD: json_trigger"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const events_a& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    {
        rapidjson::Value v;
        Serialize(v, obj.json_trigger, alloc);
        out.AddMember("json_trigger", v, alloc);
    }
}
template<>
void Deserialize(events_a& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("json_trigger") || !doc["json_trigger"].IsObject()){
        JSON_LOG_FUNC("events_a MISSING FIELD: json_trigger"); 
    }else{
        Deserialize(obj.json_trigger, doc["json_trigger"]);
    }
}

template <>
bool CheckJson(const post_proc_o& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("last_sample_freq")){
        JSON_LOG_FUNC("post_proc_o MISSING FIELD: last_sample_freq"); 
        check = false;
    }
    if(!doc.HasMember("include_items")){
        JSON_LOG_FUNC("post_proc_o MISSING FIELD: include_items"); 
        check = false;
    }
    if(!doc.HasMember("exclude_items")){
        JSON_LOG_FUNC("post_proc_o MISSING FIELD: exclude_items"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const post_proc_o& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("last_sample_freq", rapidjson::Value().SetUint64(obj.last_sample_freq), alloc);
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.include_items.size(); i++){
        	v0.PushBack(rapidjson::Value().SetString(obj.include_items[i].c_str(), obj.include_items[i].size(), alloc), alloc);
    	}
    	out.AddMember("include_items", v0, alloc);
    }
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.exclude_items.size(); i++){
        	v0.PushBack(rapidjson::Value().SetString(obj.exclude_items[i].c_str(), obj.exclude_items[i].size(), alloc), alloc);
    	}
    	out.AddMember("exclude_items", v0, alloc);
    }
}
template<>
void Deserialize(post_proc_o& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("last_sample_freq") || !doc["last_sample_freq"].IsUint64()){
        JSON_LOG_FUNC("post_proc_o MISSING FIELD: last_sample_freq"); 
    }else{
        obj.last_sample_freq = doc["last_sample_freq"].GetUint64();
    }
    if(!doc.HasMember("include_items") || !doc["include_items"].IsArray()){
        JSON_LOG_FUNC("post_proc_o MISSING FIELD: include_items"); 
    }else{
		obj.include_items.resize(doc["include_items"].Size());
		for(rapidjson::SizeType i = 0; i < doc["include_items"].Size(); i++){
				obj.include_items[i] = doc["include_items"][i].GetString();
		}
    }
    if(!doc.HasMember("exclude_items") || !doc["exclude_items"].IsArray()){
        JSON_LOG_FUNC("post_proc_o MISSING FIELD: exclude_items"); 
    }else{
		obj.exclude_items.resize(doc["exclude_items"].Size());
		for(rapidjson::SizeType i = 0; i < doc["exclude_items"].Size(); i++){
				obj.exclude_items[i] = doc["exclude_items"][i].GetString();
		}
    }
}

template <>
bool CheckJson(const saved_confs_a& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("name")){
        JSON_LOG_FUNC("saved_confs_a MISSING FIELD: name"); 
        check = false;
    }
    if(!doc.HasMember("ip")){
        JSON_LOG_FUNC("saved_confs_a MISSING FIELD: ip"); 
        check = false;
    }
    if(!doc.HasMember("port")){
        JSON_LOG_FUNC("saved_confs_a MISSING FIELD: port"); 
        check = false;
    }
    if(!doc.HasMember("mode")){
        JSON_LOG_FUNC("saved_confs_a MISSING FIELD: mode"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const saved_confs_a& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("name", rapidjson::Value().SetString(obj.name.c_str(), obj.name.size(), alloc), alloc);
    out.AddMember("ip", rapidjson::Value().SetString(obj.ip.c_str(), obj.ip.size(), alloc), alloc);
    out.AddMember("port", rapidjson::Value().SetString(obj.port.c_str(), obj.port.size(), alloc), alloc);
    out.AddMember("mode", rapidjson::Value().SetString(obj.mode.c_str(), obj.mode.size(), alloc), alloc);
}
template<>
void Deserialize(saved_confs_a& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("name") || !doc["name"].IsString()){
        JSON_LOG_FUNC("saved_confs_a MISSING FIELD: name"); 
    }else{
        obj.name = std::string(doc["name"].GetString(), doc["name"].GetStringLength());
    }
    if(!doc.HasMember("ip") || !doc["ip"].IsString()){
        JSON_LOG_FUNC("saved_confs_a MISSING FIELD: ip"); 
    }else{
        obj.ip = std::string(doc["ip"].GetString(), doc["ip"].GetStringLength());
    }
    if(!doc.HasMember("port") || !doc["port"].IsString()){
        JSON_LOG_FUNC("saved_confs_a MISSING FIELD: port"); 
    }else{
        obj.port = std::string(doc["port"].GetString(), doc["port"].GetStringLength());
    }
    if(!doc.HasMember("mode") || !doc["mode"].IsString()){
        JSON_LOG_FUNC("saved_confs_a MISSING FIELD: mode"); 
    }else{
        obj.mode = std::string(doc["mode"].GetString(), doc["mode"].GetStringLength());
    }
}

template <>
bool CheckJson(const app_connection_o& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("ip")){
        JSON_LOG_FUNC("app_connection_o MISSING FIELD: ip"); 
        check = false;
    }
    if(!doc.HasMember("port")){
        JSON_LOG_FUNC("app_connection_o MISSING FIELD: port"); 
        check = false;
    }
    if(!doc.HasMember("mode")){
        JSON_LOG_FUNC("app_connection_o MISSING FIELD: mode"); 
        check = false;
    }
    if(!doc.HasMember("saved_confs")){
        JSON_LOG_FUNC("app_connection_o MISSING FIELD: saved_confs"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const app_connection_o& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("ip", rapidjson::Value().SetString(obj.ip.c_str(), obj.ip.size(), alloc), alloc);
    out.AddMember("port", rapidjson::Value().SetString(obj.port.c_str(), obj.port.size(), alloc), alloc);
    out.AddMember("mode", rapidjson::Value().SetString(obj.mode.c_str(), obj.mode.size(), alloc), alloc);
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.saved_confs.size(); i++){
        	rapidjson::Value new_obj;
        	Serialize(new_obj, obj.saved_confs[i], alloc);
        	v0.PushBack(new_obj, alloc);
    	}
    	out.AddMember("saved_confs", v0, alloc);
    }
}
template<>
void Deserialize(app_connection_o& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("ip") || !doc["ip"].IsString()){
        JSON_LOG_FUNC("app_connection_o MISSING FIELD: ip"); 
    }else{
        obj.ip = std::string(doc["ip"].GetString(), doc["ip"].GetStringLength());
    }
    if(!doc.HasMember("port") || !doc["port"].IsString()){
        JSON_LOG_FUNC("app_connection_o MISSING FIELD: port"); 
    }else{
        obj.port = std::string(doc["port"].GetString(), doc["port"].GetStringLength());
    }
    if(!doc.HasMember("mode") || !doc["mode"].IsString()){
        JSON_LOG_FUNC("app_connection_o MISSING FIELD: mode"); 
    }else{
        obj.mode = std::string(doc["mode"].GetString(), doc["mode"].GetStringLength());
    }
    if(!doc.HasMember("saved_confs") || !doc["saved_confs"].IsArray()){
        JSON_LOG_FUNC("app_connection_o MISSING FIELD: saved_confs"); 
    }else{
		obj.saved_confs.resize(doc["saved_confs"].Size());
		for(rapidjson::SizeType i = 0; i < doc["saved_confs"].Size(); i++){
				Deserialize(obj.saved_confs[i], doc["saved_confs"][i]);
		}
    }
}

template <>
bool CheckJson(const paths_a& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("key")){
        JSON_LOG_FUNC("paths_a MISSING FIELD: key"); 
        check = false;
    }
    if(!doc.HasMember("value")){
        JSON_LOG_FUNC("paths_a MISSING FIELD: value"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const paths_a& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("key", rapidjson::Value().SetString(obj.key.c_str(), obj.key.size(), alloc), alloc);
    out.AddMember("value", rapidjson::Value().SetString(obj.value.c_str(), obj.value.size(), alloc), alloc);
}
template<>
void Deserialize(paths_a& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("key") || !doc["key"].IsString()){
        JSON_LOG_FUNC("paths_a MISSING FIELD: key"); 
    }else{
        obj.key = std::string(doc["key"].GetString(), doc["key"].GetStringLength());
    }
    if(!doc.HasMember("value") || !doc["value"].IsString()){
        JSON_LOG_FUNC("paths_a MISSING FIELD: value"); 
    }else{
        obj.value = std::string(doc["value"].GetString(), doc["value"].GetStringLength());
    }
}

template <>
bool CheckJson(const generic_key_value_a& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("key")){
        JSON_LOG_FUNC("generic_key_value_a MISSING FIELD: key"); 
        check = false;
    }
    if(!doc.HasMember("value")){
        JSON_LOG_FUNC("generic_key_value_a MISSING FIELD: value"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const generic_key_value_a& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("key", rapidjson::Value().SetString(obj.key.c_str(), obj.key.size(), alloc), alloc);
    out.AddMember("value", rapidjson::Value().SetString(obj.value.c_str(), obj.value.size(), alloc), alloc);
}
template<>
void Deserialize(generic_key_value_a& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("key") || !doc["key"].IsString()){
        JSON_LOG_FUNC("generic_key_value_a MISSING FIELD: key"); 
    }else{
        obj.key = std::string(doc["key"].GetString(), doc["key"].GetStringLength());
    }
    if(!doc.HasMember("value") || !doc["value"].IsString()){
        JSON_LOG_FUNC("generic_key_value_a MISSING FIELD: value"); 
    }else{
        obj.value = std::string(doc["value"].GetString(), doc["value"].GetStringLength());
    }
}

template <>
bool CheckJson(const app_config_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("generic_key_value")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: generic_key_value"); 
        check = false;
    }
    if(!doc.HasMember("paths")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: paths"); 
        check = false;
    }
    if(!doc.HasMember("csv_auto_save")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: csv_auto_save"); 
        check = false;
    }
    if(!doc.HasMember("telemetry_auto_get_config")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: telemetry_auto_get_config"); 
        check = false;
    }
    if(!doc.HasMember("last_login_time")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: last_login_time"); 
        check = false;
    }
    if(!doc.HasMember("app_connection")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: app_connection"); 
        check = false;
    }
    if(!doc.HasMember("activeTabs")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: activeTabs"); 
        check = false;
    }
    if(!doc.HasMember("last_connection_ips")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: last_connection_ips"); 
        check = false;
    }
    if(!doc.HasMember("post_proc")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: post_proc"); 
        check = false;
    }
    if(!doc.HasMember("events")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: events"); 
        check = false;
    }
    if(!doc.HasMember("custom_plots")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: custom_plots"); 
        check = false;
    }
    if(!doc.HasMember("theme")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: theme"); 
        check = false;
    }
    if(!doc.HasMember("last_open_mode")){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: last_open_mode"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const app_config_t& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.generic_key_value.size(); i++){
        	rapidjson::Value new_obj;
        	Serialize(new_obj, obj.generic_key_value[i], alloc);
        	v0.PushBack(new_obj, alloc);
    	}
    	out.AddMember("generic_key_value", v0, alloc);
    }
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.paths.size(); i++){
        	rapidjson::Value new_obj;
        	Serialize(new_obj, obj.paths[i], alloc);
        	v0.PushBack(new_obj, alloc);
    	}
    	out.AddMember("paths", v0, alloc);
    }
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
        rapidjson::Value v;
        Serialize(v, obj.post_proc, alloc);
        out.AddMember("post_proc", v, alloc);
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
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.custom_plots.size(); i++){
        	rapidjson::Value new_obj;
        	Serialize(new_obj, obj.custom_plots[i], alloc);
        	v0.PushBack(new_obj, alloc);
    	}
    	out.AddMember("custom_plots", v0, alloc);
    }
    out.AddMember("theme", rapidjson::Value().SetUint64(obj.theme), alloc);
    out.AddMember("last_open_mode", rapidjson::Value().SetUint64(obj.last_open_mode), alloc);
}
template<>
void Deserialize(app_config_t& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("generic_key_value") || !doc["generic_key_value"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: generic_key_value"); 
    }else{
		obj.generic_key_value.resize(doc["generic_key_value"].Size());
		for(rapidjson::SizeType i = 0; i < doc["generic_key_value"].Size(); i++){
				Deserialize(obj.generic_key_value[i], doc["generic_key_value"][i]);
		}
    }
    if(!doc.HasMember("paths") || !doc["paths"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: paths"); 
    }else{
		obj.paths.resize(doc["paths"].Size());
		for(rapidjson::SizeType i = 0; i < doc["paths"].Size(); i++){
				Deserialize(obj.paths[i], doc["paths"][i]);
		}
    }
    if(!doc.HasMember("csv_auto_save") || !doc["csv_auto_save"].IsBool()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: csv_auto_save"); 
    }else{
        obj.csv_auto_save = doc["csv_auto_save"].GetBool();
    }
    if(!doc.HasMember("telemetry_auto_get_config") || !doc["telemetry_auto_get_config"].IsBool()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: telemetry_auto_get_config"); 
    }else{
        obj.telemetry_auto_get_config = doc["telemetry_auto_get_config"].GetBool();
    }
    if(!doc.HasMember("last_login_time") || !doc["last_login_time"].IsDouble()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: last_login_time"); 
    }else{
        obj.last_login_time = doc["last_login_time"].GetDouble();
    }
    if(!doc.HasMember("app_connection") || !doc["app_connection"].IsObject()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: app_connection"); 
    }else{
        Deserialize(obj.app_connection, doc["app_connection"]);
    }
    if(!doc.HasMember("activeTabs") || !doc["activeTabs"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: activeTabs"); 
    }else{
		obj.activeTabs.resize(doc["activeTabs"].Size());
		for(rapidjson::SizeType i = 0; i < doc["activeTabs"].Size(); i++){
				obj.activeTabs[i] = doc["activeTabs"][i].GetString();
		}
    }
    if(!doc.HasMember("last_connection_ips") || !doc["last_connection_ips"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: last_connection_ips"); 
    }else{
		obj.last_connection_ips.resize(doc["last_connection_ips"].Size());
		for(rapidjson::SizeType i = 0; i < doc["last_connection_ips"].Size(); i++){
				obj.last_connection_ips[i] = doc["last_connection_ips"][i].GetString();
		}
    }
    if(!doc.HasMember("post_proc") || !doc["post_proc"].IsObject()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: post_proc"); 
    }else{
        Deserialize(obj.post_proc, doc["post_proc"]);
    }
    if(!doc.HasMember("events") || !doc["events"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: events"); 
    }else{
		obj.events.resize(doc["events"].Size());
		for(rapidjson::SizeType i = 0; i < doc["events"].Size(); i++){
				Deserialize(obj.events[i], doc["events"][i]);
		}
    }
    if(!doc.HasMember("custom_plots") || !doc["custom_plots"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: custom_plots"); 
    }else{
		obj.custom_plots.resize(doc["custom_plots"].Size());
		for(rapidjson::SizeType i = 0; i < doc["custom_plots"].Size(); i++){
				Deserialize(obj.custom_plots[i], doc["custom_plots"][i]);
		}
    }
    if(!doc.HasMember("theme") || !doc["theme"].IsUint64()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: theme"); 
    }else{
        obj.theme = doc["theme"].GetUint64();
    }
    if(!doc.HasMember("last_open_mode") || !doc["last_open_mode"].IsUint64()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: last_open_mode"); 
    }else{
        obj.last_open_mode = doc["last_open_mode"].GetUint64();
    }
}
template<>
void Deserialize(app_config_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("generic_key_value") || !doc["generic_key_value"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: generic_key_value"); 
    }else{
		obj.generic_key_value.resize(doc["generic_key_value"].Size());
		for(rapidjson::SizeType i = 0; i < doc["generic_key_value"].Size(); i++){
				Deserialize(obj.generic_key_value[i], doc["generic_key_value"][i]);
		}
    }
    if(!doc.HasMember("paths") || !doc["paths"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: paths"); 
    }else{
		obj.paths.resize(doc["paths"].Size());
		for(rapidjson::SizeType i = 0; i < doc["paths"].Size(); i++){
				Deserialize(obj.paths[i], doc["paths"][i]);
		}
    }
    if(!doc.HasMember("csv_auto_save") || !doc["csv_auto_save"].IsBool()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: csv_auto_save"); 
    }else{
        obj.csv_auto_save = doc["csv_auto_save"].GetBool();
    }
    if(!doc.HasMember("telemetry_auto_get_config") || !doc["telemetry_auto_get_config"].IsBool()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: telemetry_auto_get_config"); 
    }else{
        obj.telemetry_auto_get_config = doc["telemetry_auto_get_config"].GetBool();
    }
    if(!doc.HasMember("last_login_time") || !doc["last_login_time"].IsDouble()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: last_login_time"); 
    }else{
        obj.last_login_time = doc["last_login_time"].GetDouble();
    }
    if(!doc.HasMember("app_connection") || !doc["app_connection"].IsObject()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: app_connection"); 
    }else{
        Deserialize(obj.app_connection, doc["app_connection"]);
    }
    if(!doc.HasMember("activeTabs") || !doc["activeTabs"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: activeTabs"); 
    }else{
		obj.activeTabs.resize(doc["activeTabs"].Size());
		for(rapidjson::SizeType i = 0; i < doc["activeTabs"].Size(); i++){
				obj.activeTabs[i] = doc["activeTabs"][i].GetString();
		}
    }
    if(!doc.HasMember("last_connection_ips") || !doc["last_connection_ips"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: last_connection_ips"); 
    }else{
		obj.last_connection_ips.resize(doc["last_connection_ips"].Size());
		for(rapidjson::SizeType i = 0; i < doc["last_connection_ips"].Size(); i++){
				obj.last_connection_ips[i] = doc["last_connection_ips"][i].GetString();
		}
    }
    if(!doc.HasMember("post_proc") || !doc["post_proc"].IsObject()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: post_proc"); 
    }else{
        Deserialize(obj.post_proc, doc["post_proc"]);
    }
    if(!doc.HasMember("events") || !doc["events"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: events"); 
    }else{
		obj.events.resize(doc["events"].Size());
		for(rapidjson::SizeType i = 0; i < doc["events"].Size(); i++){
				Deserialize(obj.events[i], doc["events"][i]);
		}
    }
    if(!doc.HasMember("custom_plots") || !doc["custom_plots"].IsArray()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: custom_plots"); 
    }else{
		obj.custom_plots.resize(doc["custom_plots"].Size());
		for(rapidjson::SizeType i = 0; i < doc["custom_plots"].Size(); i++){
				Deserialize(obj.custom_plots[i], doc["custom_plots"][i]);
		}
    }
    if(!doc.HasMember("theme") || !doc["theme"].IsUint64()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: theme"); 
    }else{
        obj.theme = doc["theme"].GetUint64();
    }
    if(!doc.HasMember("last_open_mode") || !doc["last_open_mode"].IsUint64()){
        JSON_LOG_FUNC("app_config_t MISSING FIELD: last_open_mode"); 
    }else{
        obj.last_open_mode = doc["last_open_mode"].GetUint64();
    }
}

template<>
std::string StructToString(const app_config_t& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const app_config_t& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, app_config_t& out)
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
bool LoadStruct(app_config_t& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const app_config_t& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const user_data_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("username")){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: username"); 
        check = false;
    }
    if(!doc.HasMember("token")){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: token"); 
        check = false;
    }
    if(!doc.HasMember("refresh_token")){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: refresh_token"); 
        check = false;
    }
    if(!doc.HasMember("expiry")){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: expiry"); 
        check = false;
    }
    if(!doc.HasMember("role")){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: role"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const user_data_t& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("username", rapidjson::Value().SetString(obj.username.c_str(), obj.username.size(), alloc), alloc);
    out.AddMember("token", rapidjson::Value().SetString(obj.token.c_str(), obj.token.size(), alloc), alloc);
    out.AddMember("refresh_token", rapidjson::Value().SetString(obj.refresh_token.c_str(), obj.refresh_token.size(), alloc), alloc);
    out.AddMember("expiry", rapidjson::Value().SetDouble(obj.expiry), alloc);
    out.AddMember("role", rapidjson::Value().SetUint64(obj.role), alloc);
}
template<>
void Deserialize(user_data_t& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("username") || !doc["username"].IsString()){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: username"); 
    }else{
        obj.username = std::string(doc["username"].GetString(), doc["username"].GetStringLength());
    }
    if(!doc.HasMember("token") || !doc["token"].IsString()){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: token"); 
    }else{
        obj.token = std::string(doc["token"].GetString(), doc["token"].GetStringLength());
    }
    if(!doc.HasMember("refresh_token") || !doc["refresh_token"].IsString()){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: refresh_token"); 
    }else{
        obj.refresh_token = std::string(doc["refresh_token"].GetString(), doc["refresh_token"].GetStringLength());
    }
    if(!doc.HasMember("expiry") || !doc["expiry"].IsDouble()){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: expiry"); 
    }else{
        obj.expiry = doc["expiry"].GetDouble();
    }
    if(!doc.HasMember("role") || !doc["role"].IsUint64()){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: role"); 
    }else{
        obj.role = doc["role"].GetUint64();
    }
}
template<>
void Deserialize(user_data_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("username") || !doc["username"].IsString()){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: username"); 
    }else{
        obj.username = std::string(doc["username"].GetString(), doc["username"].GetStringLength());
    }
    if(!doc.HasMember("token") || !doc["token"].IsString()){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: token"); 
    }else{
        obj.token = std::string(doc["token"].GetString(), doc["token"].GetStringLength());
    }
    if(!doc.HasMember("refresh_token") || !doc["refresh_token"].IsString()){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: refresh_token"); 
    }else{
        obj.refresh_token = std::string(doc["refresh_token"].GetString(), doc["refresh_token"].GetStringLength());
    }
    if(!doc.HasMember("expiry") || !doc["expiry"].IsDouble()){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: expiry"); 
    }else{
        obj.expiry = doc["expiry"].GetDouble();
    }
    if(!doc.HasMember("role") || !doc["role"].IsUint64()){
        JSON_LOG_FUNC("user_data_t MISSING FIELD: role"); 
    }else{
        obj.role = doc["role"].GetUint64();
    }
}

template<>
std::string StructToString(const user_data_t& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const user_data_t& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, user_data_t& out)
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
bool LoadStruct(user_data_t& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const user_data_t& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

#endif //__APP_JSON_IMPLEMENTATION__

#endif // __JSON_LOADER__