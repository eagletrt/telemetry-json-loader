#ifndef __TELEMETRY_JSON__
#define __TELEMETRY_JSON__

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

typedef struct connection_t{
    std::string ip;
    std::string port;
    std::string mode;
}connection_t;

typedef struct gps_devices_o{
    std::string addr;
    std::string mode;
    bool enabled;
}gps_devices_o;

typedef struct can_devices_o{
    std::string sock;
    std::string name;
}can_devices_o;

typedef struct stat_json{
    uint64_t Messages;
    uint64_t Average_Frequency_Hz;
    double Duration_seconds;
}stat_json;

typedef struct csv_parser_config{
    std::string subfolder_name;
    bool parse_candump;
    bool parse_gps;
    bool generate_report;
}csv_parser_config;

typedef struct telemetry_config{
    bool camera_enable;
    std::vector<can_devices_o> can_devices;
    bool generate_csv;
    std::vector<gps_devices_o> gps_devices;
    bool connection_downsample;
    uint64_t connection_downsample_mps;
    bool connection_enabled;
    uint64_t connection_send_rate;
    bool connection_send_sensor_data;
    connection_t connection;
}telemetry_config;

typedef struct session_config{
    std::string Circuit;
    std::string Pilot;
    std::string Race;
    std::string Configuration;
    std::string Date;
    std::string Time;
    double Canlib_Version;
}session_config;

#ifdef __TELEMETRY_JSON_IMPLEMENTATION__

template <>
bool CheckJson(const connection_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("ip")){
        JSON_LOG_FUNC("connection_t MISSING FIELD: ip"); 
        check = false;
    }
    if(!doc.HasMember("port")){
        JSON_LOG_FUNC("connection_t MISSING FIELD: port"); 
        check = false;
    }
    if(!doc.HasMember("mode")){
        JSON_LOG_FUNC("connection_t MISSING FIELD: mode"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const connection_t& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("ip", rapidjson::Value().SetString(obj.ip.c_str(), obj.ip.size(), alloc), alloc);
    out.AddMember("port", rapidjson::Value().SetString(obj.port.c_str(), obj.port.size(), alloc), alloc);
    out.AddMember("mode", rapidjson::Value().SetString(obj.mode.c_str(), obj.mode.size(), alloc), alloc);
}
template<>
void Deserialize(connection_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("ip")){
        JSON_LOG_FUNC("connection_t MISSING FIELD: ip"); 
    }else{
    obj.ip = std::string(doc["ip"].GetString(), doc["ip"].GetStringLength());
    }
    if(!doc.HasMember("port")){
        JSON_LOG_FUNC("connection_t MISSING FIELD: port"); 
    }else{
    obj.port = std::string(doc["port"].GetString(), doc["port"].GetStringLength());
    }
    if(!doc.HasMember("mode")){
        JSON_LOG_FUNC("connection_t MISSING FIELD: mode"); 
    }else{
    obj.mode = std::string(doc["mode"].GetString(), doc["mode"].GetStringLength());
    }
}

template <>
bool CheckJson(const gps_devices_o& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("addr")){
        JSON_LOG_FUNC("gps_devices_o MISSING FIELD: addr"); 
        check = false;
    }
    if(!doc.HasMember("mode")){
        JSON_LOG_FUNC("gps_devices_o MISSING FIELD: mode"); 
        check = false;
    }
    if(!doc.HasMember("enabled")){
        JSON_LOG_FUNC("gps_devices_o MISSING FIELD: enabled"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const gps_devices_o& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("addr", rapidjson::Value().SetString(obj.addr.c_str(), obj.addr.size(), alloc), alloc);
    out.AddMember("mode", rapidjson::Value().SetString(obj.mode.c_str(), obj.mode.size(), alloc), alloc);
    out.AddMember("enabled", rapidjson::Value().SetBool(obj.enabled), alloc);
}
template<>
void Deserialize(gps_devices_o& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("addr")){
        JSON_LOG_FUNC("gps_devices_o MISSING FIELD: addr"); 
    }else{
    obj.addr = std::string(doc["addr"].GetString(), doc["addr"].GetStringLength());
    }
    if(!doc.HasMember("mode")){
        JSON_LOG_FUNC("gps_devices_o MISSING FIELD: mode"); 
    }else{
    obj.mode = std::string(doc["mode"].GetString(), doc["mode"].GetStringLength());
    }
    if(!doc.HasMember("enabled")){
        JSON_LOG_FUNC("gps_devices_o MISSING FIELD: enabled"); 
    }else{
        obj.enabled = doc["enabled"].GetBool();
    }
}

template <>
bool CheckJson(const can_devices_o& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("sock")){
        JSON_LOG_FUNC("can_devices_o MISSING FIELD: sock"); 
        check = false;
    }
    if(!doc.HasMember("name")){
        JSON_LOG_FUNC("can_devices_o MISSING FIELD: name"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const can_devices_o& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("sock", rapidjson::Value().SetString(obj.sock.c_str(), obj.sock.size(), alloc), alloc);
    out.AddMember("name", rapidjson::Value().SetString(obj.name.c_str(), obj.name.size(), alloc), alloc);
}
template<>
void Deserialize(can_devices_o& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("sock")){
        JSON_LOG_FUNC("can_devices_o MISSING FIELD: sock"); 
    }else{
    obj.sock = std::string(doc["sock"].GetString(), doc["sock"].GetStringLength());
    }
    if(!doc.HasMember("name")){
        JSON_LOG_FUNC("can_devices_o MISSING FIELD: name"); 
    }else{
    obj.name = std::string(doc["name"].GetString(), doc["name"].GetStringLength());
    }
}

template <>
bool CheckJson(const stat_json& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("Messages")){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Messages"); 
        check = false;
    }
    if(!doc.HasMember("Average_Frequency_Hz")){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Average_Frequency_Hz"); 
        check = false;
    }
    if(!doc.HasMember("Duration_seconds")){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Duration_seconds"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const stat_json& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("Messages", rapidjson::Value().SetUint64(obj.Messages), alloc);
    out.AddMember("Average_Frequency_Hz", rapidjson::Value().SetUint64(obj.Average_Frequency_Hz), alloc);
    out.AddMember("Duration_seconds", rapidjson::Value().SetDouble(obj.Duration_seconds), alloc);
}
template<>
void Deserialize(stat_json& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("Messages")){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Messages"); 
    }else{
        obj.Messages = doc["Messages"].GetUint64();
    }
    if(!doc.HasMember("Average_Frequency_Hz")){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Average_Frequency_Hz"); 
    }else{
        obj.Average_Frequency_Hz = doc["Average_Frequency_Hz"].GetUint64();
    }
    if(!doc.HasMember("Duration_seconds")){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Duration_seconds"); 
    }else{
        obj.Duration_seconds = doc["Duration_seconds"].GetDouble();
    }
}
template<>
void Deserialize(stat_json& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("Messages")){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Messages"); 
    }else{
        obj.Messages = doc["Messages"].GetUint64();
    }
    if(!doc.HasMember("Average_Frequency_Hz")){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Average_Frequency_Hz"); 
    }else{
        obj.Average_Frequency_Hz = doc["Average_Frequency_Hz"].GetUint64();
    }
    if(!doc.HasMember("Duration_seconds")){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Duration_seconds"); 
    }else{
        obj.Duration_seconds = doc["Duration_seconds"].GetDouble();
    }
}

template<>
std::string StructToString(const stat_json& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const stat_json& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, stat_json& out)
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
bool LoadStruct(stat_json& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const stat_json& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const csv_parser_config& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("subfolder_name")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: subfolder_name"); 
        check = false;
    }
    if(!doc.HasMember("parse_candump")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: parse_candump"); 
        check = false;
    }
    if(!doc.HasMember("parse_gps")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: parse_gps"); 
        check = false;
    }
    if(!doc.HasMember("generate_report")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: generate_report"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const csv_parser_config& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("subfolder_name", rapidjson::Value().SetString(obj.subfolder_name.c_str(), obj.subfolder_name.size(), alloc), alloc);
    out.AddMember("parse_candump", rapidjson::Value().SetBool(obj.parse_candump), alloc);
    out.AddMember("parse_gps", rapidjson::Value().SetBool(obj.parse_gps), alloc);
    out.AddMember("generate_report", rapidjson::Value().SetBool(obj.generate_report), alloc);
}
template<>
void Deserialize(csv_parser_config& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("subfolder_name")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: subfolder_name"); 
    }else{
    obj.subfolder_name = std::string(doc["subfolder_name"].GetString(), doc["subfolder_name"].GetStringLength());
    }
    if(!doc.HasMember("parse_candump")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: parse_candump"); 
    }else{
        obj.parse_candump = doc["parse_candump"].GetBool();
    }
    if(!doc.HasMember("parse_gps")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: parse_gps"); 
    }else{
        obj.parse_gps = doc["parse_gps"].GetBool();
    }
    if(!doc.HasMember("generate_report")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: generate_report"); 
    }else{
        obj.generate_report = doc["generate_report"].GetBool();
    }
}
template<>
void Deserialize(csv_parser_config& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("subfolder_name")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: subfolder_name"); 
    }else{
    obj.subfolder_name = std::string(doc["subfolder_name"].GetString(), doc["subfolder_name"].GetStringLength());
    }
    if(!doc.HasMember("parse_candump")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: parse_candump"); 
    }else{
        obj.parse_candump = doc["parse_candump"].GetBool();
    }
    if(!doc.HasMember("parse_gps")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: parse_gps"); 
    }else{
        obj.parse_gps = doc["parse_gps"].GetBool();
    }
    if(!doc.HasMember("generate_report")){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: generate_report"); 
    }else{
        obj.generate_report = doc["generate_report"].GetBool();
    }
}

template<>
std::string StructToString(const csv_parser_config& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const csv_parser_config& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, csv_parser_config& out)
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
bool LoadStruct(csv_parser_config& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const csv_parser_config& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const telemetry_config& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("camera_enable")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: camera_enable"); 
        check = false;
    }
    if(!doc.HasMember("can_devices")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: can_devices"); 
        check = false;
    }
    if(!doc.HasMember("generate_csv")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: generate_csv"); 
        check = false;
    }
    if(!doc.HasMember("gps_devices")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: gps_devices"); 
        check = false;
    }
    if(!doc.HasMember("connection_downsample")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_downsample"); 
        check = false;
    }
    if(!doc.HasMember("connection_downsample_mps")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_downsample_mps"); 
        check = false;
    }
    if(!doc.HasMember("connection_enabled")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_enabled"); 
        check = false;
    }
    if(!doc.HasMember("connection_send_rate")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_send_rate"); 
        check = false;
    }
    if(!doc.HasMember("connection_send_sensor_data")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_send_sensor_data"); 
        check = false;
    }
    if(!doc.HasMember("connection")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const telemetry_config& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("camera_enable", rapidjson::Value().SetBool(obj.camera_enable), alloc);
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.can_devices.size(); i++){
        	rapidjson::Value new_obj;
        	Serialize(new_obj, obj.can_devices[i], alloc);
        	v0.PushBack(new_obj, alloc);
    	}
    	out.AddMember("can_devices", v0, alloc);
    }
    out.AddMember("generate_csv", rapidjson::Value().SetBool(obj.generate_csv), alloc);
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.gps_devices.size(); i++){
        	rapidjson::Value new_obj;
        	Serialize(new_obj, obj.gps_devices[i], alloc);
        	v0.PushBack(new_obj, alloc);
    	}
    	out.AddMember("gps_devices", v0, alloc);
    }
    out.AddMember("connection_downsample", rapidjson::Value().SetBool(obj.connection_downsample), alloc);
    out.AddMember("connection_downsample_mps", rapidjson::Value().SetUint64(obj.connection_downsample_mps), alloc);
    out.AddMember("connection_enabled", rapidjson::Value().SetBool(obj.connection_enabled), alloc);
    out.AddMember("connection_send_rate", rapidjson::Value().SetUint64(obj.connection_send_rate), alloc);
    out.AddMember("connection_send_sensor_data", rapidjson::Value().SetBool(obj.connection_send_sensor_data), alloc);
    {
        rapidjson::Value v;
        Serialize(v, obj.connection, alloc);
        out.AddMember("connection", v, alloc);
    }
}
template<>
void Deserialize(telemetry_config& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("camera_enable")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: camera_enable"); 
    }else{
        obj.camera_enable = doc["camera_enable"].GetBool();
    }
    if(!doc.HasMember("can_devices")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: can_devices"); 
    }else{
	obj.can_devices.resize(doc["can_devices"].Size());
	for(rapidjson::SizeType i = 0; i < doc["can_devices"].Size(); i++){
		Deserialize(obj.can_devices[i], doc["can_devices"][i]);
	}
    }
    if(!doc.HasMember("generate_csv")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: generate_csv"); 
    }else{
        obj.generate_csv = doc["generate_csv"].GetBool();
    }
    if(!doc.HasMember("gps_devices")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: gps_devices"); 
    }else{
	obj.gps_devices.resize(doc["gps_devices"].Size());
	for(rapidjson::SizeType i = 0; i < doc["gps_devices"].Size(); i++){
		Deserialize(obj.gps_devices[i], doc["gps_devices"][i]);
	}
    }
    if(!doc.HasMember("connection_downsample")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_downsample"); 
    }else{
        obj.connection_downsample = doc["connection_downsample"].GetBool();
    }
    if(!doc.HasMember("connection_downsample_mps")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_downsample_mps"); 
    }else{
        obj.connection_downsample_mps = doc["connection_downsample_mps"].GetUint64();
    }
    if(!doc.HasMember("connection_enabled")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_enabled"); 
    }else{
        obj.connection_enabled = doc["connection_enabled"].GetBool();
    }
    if(!doc.HasMember("connection_send_rate")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_send_rate"); 
    }else{
        obj.connection_send_rate = doc["connection_send_rate"].GetUint64();
    }
    if(!doc.HasMember("connection_send_sensor_data")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_send_sensor_data"); 
    }else{
        obj.connection_send_sensor_data = doc["connection_send_sensor_data"].GetBool();
    }
    if(!doc.HasMember("connection")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection"); 
    }else{
        Deserialize(obj.connection, doc["connection"]);
    }
}
template<>
void Deserialize(telemetry_config& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("camera_enable")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: camera_enable"); 
    }else{
        obj.camera_enable = doc["camera_enable"].GetBool();
    }
    if(!doc.HasMember("can_devices")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: can_devices"); 
    }else{
	obj.can_devices.resize(doc["can_devices"].Size());
	for(rapidjson::SizeType i = 0; i < doc["can_devices"].Size(); i++){
		Deserialize(obj.can_devices[i], doc["can_devices"][i]);
	}
    }
    if(!doc.HasMember("generate_csv")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: generate_csv"); 
    }else{
        obj.generate_csv = doc["generate_csv"].GetBool();
    }
    if(!doc.HasMember("gps_devices")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: gps_devices"); 
    }else{
	obj.gps_devices.resize(doc["gps_devices"].Size());
	for(rapidjson::SizeType i = 0; i < doc["gps_devices"].Size(); i++){
		Deserialize(obj.gps_devices[i], doc["gps_devices"][i]);
	}
    }
    if(!doc.HasMember("connection_downsample")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_downsample"); 
    }else{
        obj.connection_downsample = doc["connection_downsample"].GetBool();
    }
    if(!doc.HasMember("connection_downsample_mps")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_downsample_mps"); 
    }else{
        obj.connection_downsample_mps = doc["connection_downsample_mps"].GetUint64();
    }
    if(!doc.HasMember("connection_enabled")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_enabled"); 
    }else{
        obj.connection_enabled = doc["connection_enabled"].GetBool();
    }
    if(!doc.HasMember("connection_send_rate")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_send_rate"); 
    }else{
        obj.connection_send_rate = doc["connection_send_rate"].GetUint64();
    }
    if(!doc.HasMember("connection_send_sensor_data")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_send_sensor_data"); 
    }else{
        obj.connection_send_sensor_data = doc["connection_send_sensor_data"].GetBool();
    }
    if(!doc.HasMember("connection")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection"); 
    }else{
        Deserialize(obj.connection, doc["connection"]);
    }
}

template<>
std::string StructToString(const telemetry_config& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const telemetry_config& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, telemetry_config& out)
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
bool LoadStruct(telemetry_config& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const telemetry_config& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const session_config& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("Circuit")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Circuit"); 
        check = false;
    }
    if(!doc.HasMember("Pilot")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Pilot"); 
        check = false;
    }
    if(!doc.HasMember("Race")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Race"); 
        check = false;
    }
    if(!doc.HasMember("Configuration")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Configuration"); 
        check = false;
    }
    if(!doc.HasMember("Date")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Date"); 
        check = false;
    }
    if(!doc.HasMember("Time")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Time"); 
        check = false;
    }
    if(!doc.HasMember("Canlib_Version")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Canlib_Version"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const session_config& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("Circuit", rapidjson::Value().SetString(obj.Circuit.c_str(), obj.Circuit.size(), alloc), alloc);
    out.AddMember("Pilot", rapidjson::Value().SetString(obj.Pilot.c_str(), obj.Pilot.size(), alloc), alloc);
    out.AddMember("Race", rapidjson::Value().SetString(obj.Race.c_str(), obj.Race.size(), alloc), alloc);
    out.AddMember("Configuration", rapidjson::Value().SetString(obj.Configuration.c_str(), obj.Configuration.size(), alloc), alloc);
    out.AddMember("Date", rapidjson::Value().SetString(obj.Date.c_str(), obj.Date.size(), alloc), alloc);
    out.AddMember("Time", rapidjson::Value().SetString(obj.Time.c_str(), obj.Time.size(), alloc), alloc);
    out.AddMember("Canlib_Version", rapidjson::Value().SetDouble(obj.Canlib_Version), alloc);
}
template<>
void Deserialize(session_config& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("Circuit")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Circuit"); 
    }else{
    obj.Circuit = std::string(doc["Circuit"].GetString(), doc["Circuit"].GetStringLength());
    }
    if(!doc.HasMember("Pilot")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Pilot"); 
    }else{
    obj.Pilot = std::string(doc["Pilot"].GetString(), doc["Pilot"].GetStringLength());
    }
    if(!doc.HasMember("Race")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Race"); 
    }else{
    obj.Race = std::string(doc["Race"].GetString(), doc["Race"].GetStringLength());
    }
    if(!doc.HasMember("Configuration")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Configuration"); 
    }else{
    obj.Configuration = std::string(doc["Configuration"].GetString(), doc["Configuration"].GetStringLength());
    }
    if(!doc.HasMember("Date")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Date"); 
    }else{
    obj.Date = std::string(doc["Date"].GetString(), doc["Date"].GetStringLength());
    }
    if(!doc.HasMember("Time")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Time"); 
    }else{
    obj.Time = std::string(doc["Time"].GetString(), doc["Time"].GetStringLength());
    }
    if(!doc.HasMember("Canlib_Version")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Canlib_Version"); 
    }else{
        obj.Canlib_Version = doc["Canlib_Version"].GetDouble();
    }
}
template<>
void Deserialize(session_config& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("Circuit")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Circuit"); 
    }else{
    obj.Circuit = std::string(doc["Circuit"].GetString(), doc["Circuit"].GetStringLength());
    }
    if(!doc.HasMember("Pilot")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Pilot"); 
    }else{
    obj.Pilot = std::string(doc["Pilot"].GetString(), doc["Pilot"].GetStringLength());
    }
    if(!doc.HasMember("Race")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Race"); 
    }else{
    obj.Race = std::string(doc["Race"].GetString(), doc["Race"].GetStringLength());
    }
    if(!doc.HasMember("Configuration")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Configuration"); 
    }else{
    obj.Configuration = std::string(doc["Configuration"].GetString(), doc["Configuration"].GetStringLength());
    }
    if(!doc.HasMember("Date")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Date"); 
    }else{
    obj.Date = std::string(doc["Date"].GetString(), doc["Date"].GetStringLength());
    }
    if(!doc.HasMember("Time")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Time"); 
    }else{
    obj.Time = std::string(doc["Time"].GetString(), doc["Time"].GetStringLength());
    }
    if(!doc.HasMember("Canlib_Version")){
        JSON_LOG_FUNC("session_config MISSING FIELD: Canlib_Version"); 
    }else{
        obj.Canlib_Version = doc["Canlib_Version"].GetDouble();
    }
}

template<>
std::string StructToString(const session_config& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const session_config& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, session_config& out)
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
bool LoadStruct(session_config& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const session_config& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

#endif //__TELEMETRY_JSON_IMPLEMENTATION__

#endif // __JSON_LOADER__