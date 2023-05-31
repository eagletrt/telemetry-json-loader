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
    bool tls;
    std::string cafile;
    std::string capath;
    std::string certfile;
    std::string keyfile;
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

typedef struct track_conditions_t{
    double temperature;
    double humidity;
    std::string other;
}track_conditions_t;

typedef struct pilot_t{
    std::string name;
    double weight;
}pilot_t;

typedef struct wheel_rear_t{
    double camber;
    double toe;
    double bound;
    double rebound;
    double preload;
}wheel_rear_t;

typedef struct wheel_front_t{
    double camber;
    double toe;
    double bound;
    double rebound;
    double preload;
}wheel_front_t;

typedef struct aero_t{
    double angle_of_incidence_front;
    double angle_of_incidence_rear;
}aero_t;

typedef struct csv_parser_config{
    std::string subfolder_name;
    bool parse_candump;
    bool parse_gps;
    bool generate_report;
}csv_parser_config;

typedef struct stat_json{
    uint64_t Messages;
    uint64_t Average_Frequency_Hz;
    double Duration_seconds;
}stat_json;

typedef struct car_setup{
    aero_t aero;
    wheel_front_t wheel_front;
    wheel_rear_t wheel_rear;
    pilot_t pilot;
    track_conditions_t track_conditions;
    std::string wheel_compound;
    double ride_height;
    std::string balancing;
    std::string other;
}car_setup;

typedef struct telemetry_config{
    std::string device_id;
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

typedef struct telemetry_login_data{
    std::string username;
    std::string password;
}telemetry_login_data;

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
    if(!doc.HasMember("tls")){
        JSON_LOG_FUNC("connection_t MISSING FIELD: tls"); 
        check = false;
    }
    if(!doc.HasMember("cafile")){
        JSON_LOG_FUNC("connection_t MISSING FIELD: cafile"); 
        check = false;
    }
    if(!doc.HasMember("capath")){
        JSON_LOG_FUNC("connection_t MISSING FIELD: capath"); 
        check = false;
    }
    if(!doc.HasMember("certfile")){
        JSON_LOG_FUNC("connection_t MISSING FIELD: certfile"); 
        check = false;
    }
    if(!doc.HasMember("keyfile")){
        JSON_LOG_FUNC("connection_t MISSING FIELD: keyfile"); 
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
    out.AddMember("tls", rapidjson::Value().SetBool(obj.tls), alloc);
    out.AddMember("cafile", rapidjson::Value().SetString(obj.cafile.c_str(), obj.cafile.size(), alloc), alloc);
    out.AddMember("capath", rapidjson::Value().SetString(obj.capath.c_str(), obj.capath.size(), alloc), alloc);
    out.AddMember("certfile", rapidjson::Value().SetString(obj.certfile.c_str(), obj.certfile.size(), alloc), alloc);
    out.AddMember("keyfile", rapidjson::Value().SetString(obj.keyfile.c_str(), obj.keyfile.size(), alloc), alloc);
}
template<>
void Deserialize(connection_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("ip") && doc["ip"].IsString()){
        JSON_LOG_FUNC("connection_t MISSING FIELD: ip"); 
    }else{
        obj.ip = std::string(doc["ip"].GetString(), doc["ip"].GetStringLength());
    }
    if(!doc.HasMember("port") && doc["port"].IsString()){
        JSON_LOG_FUNC("connection_t MISSING FIELD: port"); 
    }else{
        obj.port = std::string(doc["port"].GetString(), doc["port"].GetStringLength());
    }
    if(!doc.HasMember("mode") && doc["mode"].IsString()){
        JSON_LOG_FUNC("connection_t MISSING FIELD: mode"); 
    }else{
        obj.mode = std::string(doc["mode"].GetString(), doc["mode"].GetStringLength());
    }
    if(!doc.HasMember("tls") && doc["tls"].IsBool()){
        JSON_LOG_FUNC("connection_t MISSING FIELD: tls"); 
    }else{
        obj.tls = doc["tls"].GetBool();
    }
    if(!doc.HasMember("cafile") && doc["cafile"].IsString()){
        JSON_LOG_FUNC("connection_t MISSING FIELD: cafile"); 
    }else{
        obj.cafile = std::string(doc["cafile"].GetString(), doc["cafile"].GetStringLength());
    }
    if(!doc.HasMember("capath") && doc["capath"].IsString()){
        JSON_LOG_FUNC("connection_t MISSING FIELD: capath"); 
    }else{
        obj.capath = std::string(doc["capath"].GetString(), doc["capath"].GetStringLength());
    }
    if(!doc.HasMember("certfile") && doc["certfile"].IsString()){
        JSON_LOG_FUNC("connection_t MISSING FIELD: certfile"); 
    }else{
        obj.certfile = std::string(doc["certfile"].GetString(), doc["certfile"].GetStringLength());
    }
    if(!doc.HasMember("keyfile") && doc["keyfile"].IsString()){
        JSON_LOG_FUNC("connection_t MISSING FIELD: keyfile"); 
    }else{
        obj.keyfile = std::string(doc["keyfile"].GetString(), doc["keyfile"].GetStringLength());
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
    if(!doc.HasMember("addr") && doc["addr"].IsString()){
        JSON_LOG_FUNC("gps_devices_o MISSING FIELD: addr"); 
    }else{
        obj.addr = std::string(doc["addr"].GetString(), doc["addr"].GetStringLength());
    }
    if(!doc.HasMember("mode") && doc["mode"].IsString()){
        JSON_LOG_FUNC("gps_devices_o MISSING FIELD: mode"); 
    }else{
        obj.mode = std::string(doc["mode"].GetString(), doc["mode"].GetStringLength());
    }
    if(!doc.HasMember("enabled") && doc["enabled"].IsBool()){
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
    if(!doc.HasMember("sock") && doc["sock"].IsString()){
        JSON_LOG_FUNC("can_devices_o MISSING FIELD: sock"); 
    }else{
        obj.sock = std::string(doc["sock"].GetString(), doc["sock"].GetStringLength());
    }
    if(!doc.HasMember("name") && doc["name"].IsString()){
        JSON_LOG_FUNC("can_devices_o MISSING FIELD: name"); 
    }else{
        obj.name = std::string(doc["name"].GetString(), doc["name"].GetStringLength());
    }
}

template <>
bool CheckJson(const track_conditions_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("temperature")){
        JSON_LOG_FUNC("track_conditions_t MISSING FIELD: temperature"); 
        check = false;
    }
    if(!doc.HasMember("humidity")){
        JSON_LOG_FUNC("track_conditions_t MISSING FIELD: humidity"); 
        check = false;
    }
    if(!doc.HasMember("other")){
        JSON_LOG_FUNC("track_conditions_t MISSING FIELD: other"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const track_conditions_t& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("temperature", rapidjson::Value().SetDouble(obj.temperature), alloc);
    out.AddMember("humidity", rapidjson::Value().SetDouble(obj.humidity), alloc);
    out.AddMember("other", rapidjson::Value().SetString(obj.other.c_str(), obj.other.size(), alloc), alloc);
}
template<>
void Deserialize(track_conditions_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("temperature") && doc["temperature"].IsDouble()){
        JSON_LOG_FUNC("track_conditions_t MISSING FIELD: temperature"); 
    }else{
        obj.temperature = doc["temperature"].GetDouble();
    }
    if(!doc.HasMember("humidity") && doc["humidity"].IsDouble()){
        JSON_LOG_FUNC("track_conditions_t MISSING FIELD: humidity"); 
    }else{
        obj.humidity = doc["humidity"].GetDouble();
    }
    if(!doc.HasMember("other") && doc["other"].IsString()){
        JSON_LOG_FUNC("track_conditions_t MISSING FIELD: other"); 
    }else{
        obj.other = std::string(doc["other"].GetString(), doc["other"].GetStringLength());
    }
}

template <>
bool CheckJson(const pilot_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("name")){
        JSON_LOG_FUNC("pilot_t MISSING FIELD: name"); 
        check = false;
    }
    if(!doc.HasMember("weight")){
        JSON_LOG_FUNC("pilot_t MISSING FIELD: weight"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const pilot_t& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("name", rapidjson::Value().SetString(obj.name.c_str(), obj.name.size(), alloc), alloc);
    out.AddMember("weight", rapidjson::Value().SetDouble(obj.weight), alloc);
}
template<>
void Deserialize(pilot_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("name") && doc["name"].IsString()){
        JSON_LOG_FUNC("pilot_t MISSING FIELD: name"); 
    }else{
        obj.name = std::string(doc["name"].GetString(), doc["name"].GetStringLength());
    }
    if(!doc.HasMember("weight") && doc["weight"].IsDouble()){
        JSON_LOG_FUNC("pilot_t MISSING FIELD: weight"); 
    }else{
        obj.weight = doc["weight"].GetDouble();
    }
}

template <>
bool CheckJson(const wheel_rear_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("camber")){
        JSON_LOG_FUNC("wheel_rear_t MISSING FIELD: camber"); 
        check = false;
    }
    if(!doc.HasMember("toe")){
        JSON_LOG_FUNC("wheel_rear_t MISSING FIELD: toe"); 
        check = false;
    }
    if(!doc.HasMember("bound")){
        JSON_LOG_FUNC("wheel_rear_t MISSING FIELD: bound"); 
        check = false;
    }
    if(!doc.HasMember("rebound")){
        JSON_LOG_FUNC("wheel_rear_t MISSING FIELD: rebound"); 
        check = false;
    }
    if(!doc.HasMember("preload")){
        JSON_LOG_FUNC("wheel_rear_t MISSING FIELD: preload"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const wheel_rear_t& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("camber", rapidjson::Value().SetDouble(obj.camber), alloc);
    out.AddMember("toe", rapidjson::Value().SetDouble(obj.toe), alloc);
    out.AddMember("bound", rapidjson::Value().SetDouble(obj.bound), alloc);
    out.AddMember("rebound", rapidjson::Value().SetDouble(obj.rebound), alloc);
    out.AddMember("preload", rapidjson::Value().SetDouble(obj.preload), alloc);
}
template<>
void Deserialize(wheel_rear_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("camber") && doc["camber"].IsDouble()){
        JSON_LOG_FUNC("wheel_rear_t MISSING FIELD: camber"); 
    }else{
        obj.camber = doc["camber"].GetDouble();
    }
    if(!doc.HasMember("toe") && doc["toe"].IsDouble()){
        JSON_LOG_FUNC("wheel_rear_t MISSING FIELD: toe"); 
    }else{
        obj.toe = doc["toe"].GetDouble();
    }
    if(!doc.HasMember("bound") && doc["bound"].IsDouble()){
        JSON_LOG_FUNC("wheel_rear_t MISSING FIELD: bound"); 
    }else{
        obj.bound = doc["bound"].GetDouble();
    }
    if(!doc.HasMember("rebound") && doc["rebound"].IsDouble()){
        JSON_LOG_FUNC("wheel_rear_t MISSING FIELD: rebound"); 
    }else{
        obj.rebound = doc["rebound"].GetDouble();
    }
    if(!doc.HasMember("preload") && doc["preload"].IsDouble()){
        JSON_LOG_FUNC("wheel_rear_t MISSING FIELD: preload"); 
    }else{
        obj.preload = doc["preload"].GetDouble();
    }
}

template <>
bool CheckJson(const wheel_front_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("camber")){
        JSON_LOG_FUNC("wheel_front_t MISSING FIELD: camber"); 
        check = false;
    }
    if(!doc.HasMember("toe")){
        JSON_LOG_FUNC("wheel_front_t MISSING FIELD: toe"); 
        check = false;
    }
    if(!doc.HasMember("bound")){
        JSON_LOG_FUNC("wheel_front_t MISSING FIELD: bound"); 
        check = false;
    }
    if(!doc.HasMember("rebound")){
        JSON_LOG_FUNC("wheel_front_t MISSING FIELD: rebound"); 
        check = false;
    }
    if(!doc.HasMember("preload")){
        JSON_LOG_FUNC("wheel_front_t MISSING FIELD: preload"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const wheel_front_t& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("camber", rapidjson::Value().SetDouble(obj.camber), alloc);
    out.AddMember("toe", rapidjson::Value().SetDouble(obj.toe), alloc);
    out.AddMember("bound", rapidjson::Value().SetDouble(obj.bound), alloc);
    out.AddMember("rebound", rapidjson::Value().SetDouble(obj.rebound), alloc);
    out.AddMember("preload", rapidjson::Value().SetDouble(obj.preload), alloc);
}
template<>
void Deserialize(wheel_front_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("camber") && doc["camber"].IsDouble()){
        JSON_LOG_FUNC("wheel_front_t MISSING FIELD: camber"); 
    }else{
        obj.camber = doc["camber"].GetDouble();
    }
    if(!doc.HasMember("toe") && doc["toe"].IsDouble()){
        JSON_LOG_FUNC("wheel_front_t MISSING FIELD: toe"); 
    }else{
        obj.toe = doc["toe"].GetDouble();
    }
    if(!doc.HasMember("bound") && doc["bound"].IsDouble()){
        JSON_LOG_FUNC("wheel_front_t MISSING FIELD: bound"); 
    }else{
        obj.bound = doc["bound"].GetDouble();
    }
    if(!doc.HasMember("rebound") && doc["rebound"].IsDouble()){
        JSON_LOG_FUNC("wheel_front_t MISSING FIELD: rebound"); 
    }else{
        obj.rebound = doc["rebound"].GetDouble();
    }
    if(!doc.HasMember("preload") && doc["preload"].IsDouble()){
        JSON_LOG_FUNC("wheel_front_t MISSING FIELD: preload"); 
    }else{
        obj.preload = doc["preload"].GetDouble();
    }
}

template <>
bool CheckJson(const aero_t& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("angle_of_incidence_front")){
        JSON_LOG_FUNC("aero_t MISSING FIELD: angle_of_incidence_front"); 
        check = false;
    }
    if(!doc.HasMember("angle_of_incidence_rear")){
        JSON_LOG_FUNC("aero_t MISSING FIELD: angle_of_incidence_rear"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const aero_t& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("angle_of_incidence_front", rapidjson::Value().SetDouble(obj.angle_of_incidence_front), alloc);
    out.AddMember("angle_of_incidence_rear", rapidjson::Value().SetDouble(obj.angle_of_incidence_rear), alloc);
}
template<>
void Deserialize(aero_t& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("angle_of_incidence_front") && doc["angle_of_incidence_front"].IsDouble()){
        JSON_LOG_FUNC("aero_t MISSING FIELD: angle_of_incidence_front"); 
    }else{
        obj.angle_of_incidence_front = doc["angle_of_incidence_front"].GetDouble();
    }
    if(!doc.HasMember("angle_of_incidence_rear") && doc["angle_of_incidence_rear"].IsDouble()){
        JSON_LOG_FUNC("aero_t MISSING FIELD: angle_of_incidence_rear"); 
    }else{
        obj.angle_of_incidence_rear = doc["angle_of_incidence_rear"].GetDouble();
    }
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
    if(!doc.HasMember("subfolder_name") && doc["subfolder_name"].IsString()){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: subfolder_name"); 
    }else{
        obj.subfolder_name = std::string(doc["subfolder_name"].GetString(), doc["subfolder_name"].GetStringLength());
    }
    if(!doc.HasMember("parse_candump") && doc["parse_candump"].IsBool()){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: parse_candump"); 
    }else{
        obj.parse_candump = doc["parse_candump"].GetBool();
    }
    if(!doc.HasMember("parse_gps") && doc["parse_gps"].IsBool()){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: parse_gps"); 
    }else{
        obj.parse_gps = doc["parse_gps"].GetBool();
    }
    if(!doc.HasMember("generate_report") && doc["generate_report"].IsBool()){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: generate_report"); 
    }else{
        obj.generate_report = doc["generate_report"].GetBool();
    }
}
template<>
void Deserialize(csv_parser_config& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("subfolder_name") && doc["subfolder_name"].IsString()){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: subfolder_name"); 
    }else{
        obj.subfolder_name = std::string(doc["subfolder_name"].GetString(), doc["subfolder_name"].GetStringLength());
    }
    if(!doc.HasMember("parse_candump") && doc["parse_candump"].IsBool()){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: parse_candump"); 
    }else{
        obj.parse_candump = doc["parse_candump"].GetBool();
    }
    if(!doc.HasMember("parse_gps") && doc["parse_gps"].IsBool()){
        JSON_LOG_FUNC("csv_parser_config MISSING FIELD: parse_gps"); 
    }else{
        obj.parse_gps = doc["parse_gps"].GetBool();
    }
    if(!doc.HasMember("generate_report") && doc["generate_report"].IsBool()){
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
    if(!doc.HasMember("Messages") && doc["Messages"].IsUint64()){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Messages"); 
    }else{
        obj.Messages = doc["Messages"].GetUint64();
    }
    if(!doc.HasMember("Average_Frequency_Hz") && doc["Average_Frequency_Hz"].IsUint64()){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Average_Frequency_Hz"); 
    }else{
        obj.Average_Frequency_Hz = doc["Average_Frequency_Hz"].GetUint64();
    }
    if(!doc.HasMember("Duration_seconds") && doc["Duration_seconds"].IsDouble()){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Duration_seconds"); 
    }else{
        obj.Duration_seconds = doc["Duration_seconds"].GetDouble();
    }
}
template<>
void Deserialize(stat_json& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("Messages") && doc["Messages"].IsUint64()){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Messages"); 
    }else{
        obj.Messages = doc["Messages"].GetUint64();
    }
    if(!doc.HasMember("Average_Frequency_Hz") && doc["Average_Frequency_Hz"].IsUint64()){
        JSON_LOG_FUNC("stat_json MISSING FIELD: Average_Frequency_Hz"); 
    }else{
        obj.Average_Frequency_Hz = doc["Average_Frequency_Hz"].GetUint64();
    }
    if(!doc.HasMember("Duration_seconds") && doc["Duration_seconds"].IsDouble()){
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
bool CheckJson(const car_setup& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("aero")){
        JSON_LOG_FUNC("car_setup MISSING FIELD: aero"); 
        check = false;
    }
    if(!doc.HasMember("wheel_front")){
        JSON_LOG_FUNC("car_setup MISSING FIELD: wheel_front"); 
        check = false;
    }
    if(!doc.HasMember("wheel_rear")){
        JSON_LOG_FUNC("car_setup MISSING FIELD: wheel_rear"); 
        check = false;
    }
    if(!doc.HasMember("pilot")){
        JSON_LOG_FUNC("car_setup MISSING FIELD: pilot"); 
        check = false;
    }
    if(!doc.HasMember("track_conditions")){
        JSON_LOG_FUNC("car_setup MISSING FIELD: track_conditions"); 
        check = false;
    }
    if(!doc.HasMember("wheel_compound")){
        JSON_LOG_FUNC("car_setup MISSING FIELD: wheel_compound"); 
        check = false;
    }
    if(!doc.HasMember("ride_height")){
        JSON_LOG_FUNC("car_setup MISSING FIELD: ride_height"); 
        check = false;
    }
    if(!doc.HasMember("balancing")){
        JSON_LOG_FUNC("car_setup MISSING FIELD: balancing"); 
        check = false;
    }
    if(!doc.HasMember("other")){
        JSON_LOG_FUNC("car_setup MISSING FIELD: other"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const car_setup& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    {
        rapidjson::Value v;
        Serialize(v, obj.aero, alloc);
        out.AddMember("aero", v, alloc);
    }
    {
        rapidjson::Value v;
        Serialize(v, obj.wheel_front, alloc);
        out.AddMember("wheel_front", v, alloc);
    }
    {
        rapidjson::Value v;
        Serialize(v, obj.wheel_rear, alloc);
        out.AddMember("wheel_rear", v, alloc);
    }
    {
        rapidjson::Value v;
        Serialize(v, obj.pilot, alloc);
        out.AddMember("pilot", v, alloc);
    }
    {
        rapidjson::Value v;
        Serialize(v, obj.track_conditions, alloc);
        out.AddMember("track_conditions", v, alloc);
    }
    out.AddMember("wheel_compound", rapidjson::Value().SetString(obj.wheel_compound.c_str(), obj.wheel_compound.size(), alloc), alloc);
    out.AddMember("ride_height", rapidjson::Value().SetDouble(obj.ride_height), alloc);
    out.AddMember("balancing", rapidjson::Value().SetString(obj.balancing.c_str(), obj.balancing.size(), alloc), alloc);
    out.AddMember("other", rapidjson::Value().SetString(obj.other.c_str(), obj.other.size(), alloc), alloc);
}
template<>
void Deserialize(car_setup& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("aero") && doc["aero"].IsObject()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: aero"); 
    }else{
        Deserialize(obj.aero, doc["aero"]);
    }
    if(!doc.HasMember("wheel_front") && doc["wheel_front"].IsObject()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: wheel_front"); 
    }else{
        Deserialize(obj.wheel_front, doc["wheel_front"]);
    }
    if(!doc.HasMember("wheel_rear") && doc["wheel_rear"].IsObject()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: wheel_rear"); 
    }else{
        Deserialize(obj.wheel_rear, doc["wheel_rear"]);
    }
    if(!doc.HasMember("pilot") && doc["pilot"].IsObject()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: pilot"); 
    }else{
        Deserialize(obj.pilot, doc["pilot"]);
    }
    if(!doc.HasMember("track_conditions") && doc["track_conditions"].IsObject()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: track_conditions"); 
    }else{
        Deserialize(obj.track_conditions, doc["track_conditions"]);
    }
    if(!doc.HasMember("wheel_compound") && doc["wheel_compound"].IsString()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: wheel_compound"); 
    }else{
        obj.wheel_compound = std::string(doc["wheel_compound"].GetString(), doc["wheel_compound"].GetStringLength());
    }
    if(!doc.HasMember("ride_height") && doc["ride_height"].IsDouble()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: ride_height"); 
    }else{
        obj.ride_height = doc["ride_height"].GetDouble();
    }
    if(!doc.HasMember("balancing") && doc["balancing"].IsString()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: balancing"); 
    }else{
        obj.balancing = std::string(doc["balancing"].GetString(), doc["balancing"].GetStringLength());
    }
    if(!doc.HasMember("other") && doc["other"].IsString()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: other"); 
    }else{
        obj.other = std::string(doc["other"].GetString(), doc["other"].GetStringLength());
    }
}
template<>
void Deserialize(car_setup& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("aero") && doc["aero"].IsObject()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: aero"); 
    }else{
        Deserialize(obj.aero, doc["aero"]);
    }
    if(!doc.HasMember("wheel_front") && doc["wheel_front"].IsObject()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: wheel_front"); 
    }else{
        Deserialize(obj.wheel_front, doc["wheel_front"]);
    }
    if(!doc.HasMember("wheel_rear") && doc["wheel_rear"].IsObject()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: wheel_rear"); 
    }else{
        Deserialize(obj.wheel_rear, doc["wheel_rear"]);
    }
    if(!doc.HasMember("pilot") && doc["pilot"].IsObject()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: pilot"); 
    }else{
        Deserialize(obj.pilot, doc["pilot"]);
    }
    if(!doc.HasMember("track_conditions") && doc["track_conditions"].IsObject()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: track_conditions"); 
    }else{
        Deserialize(obj.track_conditions, doc["track_conditions"]);
    }
    if(!doc.HasMember("wheel_compound") && doc["wheel_compound"].IsString()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: wheel_compound"); 
    }else{
        obj.wheel_compound = std::string(doc["wheel_compound"].GetString(), doc["wheel_compound"].GetStringLength());
    }
    if(!doc.HasMember("ride_height") && doc["ride_height"].IsDouble()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: ride_height"); 
    }else{
        obj.ride_height = doc["ride_height"].GetDouble();
    }
    if(!doc.HasMember("balancing") && doc["balancing"].IsString()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: balancing"); 
    }else{
        obj.balancing = std::string(doc["balancing"].GetString(), doc["balancing"].GetStringLength());
    }
    if(!doc.HasMember("other") && doc["other"].IsString()){
        JSON_LOG_FUNC("car_setup MISSING FIELD: other"); 
    }else{
        obj.other = std::string(doc["other"].GetString(), doc["other"].GetStringLength());
    }
}

template<>
std::string StructToString(const car_setup& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const car_setup& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, car_setup& out)
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
bool LoadStruct(car_setup& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const car_setup& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const telemetry_config& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("device_id")){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: device_id"); 
        check = false;
    }
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
    out.AddMember("device_id", rapidjson::Value().SetString(obj.device_id.c_str(), obj.device_id.size(), alloc), alloc);
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
    if(!doc.HasMember("device_id") && doc["device_id"].IsString()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: device_id"); 
    }else{
        obj.device_id = std::string(doc["device_id"].GetString(), doc["device_id"].GetStringLength());
    }
    if(!doc.HasMember("camera_enable") && doc["camera_enable"].IsBool()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: camera_enable"); 
    }else{
        obj.camera_enable = doc["camera_enable"].GetBool();
    }
    if(!doc.HasMember("can_devices") && doc["can_devices"].IsObject()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: can_devices"); 
    }else{
		obj.can_devices.resize(doc["can_devices"].Size());
		for(rapidjson::SizeType i = 0; i < doc["can_devices"].Size(); i++){
				Deserialize(obj.can_devices[i], doc["can_devices"][i]);
		}
    }
    if(!doc.HasMember("generate_csv") && doc["generate_csv"].IsBool()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: generate_csv"); 
    }else{
        obj.generate_csv = doc["generate_csv"].GetBool();
    }
    if(!doc.HasMember("gps_devices") && doc["gps_devices"].IsObject()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: gps_devices"); 
    }else{
		obj.gps_devices.resize(doc["gps_devices"].Size());
		for(rapidjson::SizeType i = 0; i < doc["gps_devices"].Size(); i++){
				Deserialize(obj.gps_devices[i], doc["gps_devices"][i]);
		}
    }
    if(!doc.HasMember("connection_downsample") && doc["connection_downsample"].IsBool()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_downsample"); 
    }else{
        obj.connection_downsample = doc["connection_downsample"].GetBool();
    }
    if(!doc.HasMember("connection_downsample_mps") && doc["connection_downsample_mps"].IsUint64()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_downsample_mps"); 
    }else{
        obj.connection_downsample_mps = doc["connection_downsample_mps"].GetUint64();
    }
    if(!doc.HasMember("connection_enabled") && doc["connection_enabled"].IsBool()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_enabled"); 
    }else{
        obj.connection_enabled = doc["connection_enabled"].GetBool();
    }
    if(!doc.HasMember("connection_send_rate") && doc["connection_send_rate"].IsUint64()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_send_rate"); 
    }else{
        obj.connection_send_rate = doc["connection_send_rate"].GetUint64();
    }
    if(!doc.HasMember("connection_send_sensor_data") && doc["connection_send_sensor_data"].IsBool()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_send_sensor_data"); 
    }else{
        obj.connection_send_sensor_data = doc["connection_send_sensor_data"].GetBool();
    }
    if(!doc.HasMember("connection") && doc["connection"].IsObject()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection"); 
    }else{
        Deserialize(obj.connection, doc["connection"]);
    }
}
template<>
void Deserialize(telemetry_config& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("device_id") && doc["device_id"].IsString()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: device_id"); 
    }else{
        obj.device_id = std::string(doc["device_id"].GetString(), doc["device_id"].GetStringLength());
    }
    if(!doc.HasMember("camera_enable") && doc["camera_enable"].IsBool()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: camera_enable"); 
    }else{
        obj.camera_enable = doc["camera_enable"].GetBool();
    }
    if(!doc.HasMember("can_devices") && doc["can_devices"].IsObject()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: can_devices"); 
    }else{
		obj.can_devices.resize(doc["can_devices"].Size());
		for(rapidjson::SizeType i = 0; i < doc["can_devices"].Size(); i++){
				Deserialize(obj.can_devices[i], doc["can_devices"][i]);
		}
    }
    if(!doc.HasMember("generate_csv") && doc["generate_csv"].IsBool()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: generate_csv"); 
    }else{
        obj.generate_csv = doc["generate_csv"].GetBool();
    }
    if(!doc.HasMember("gps_devices") && doc["gps_devices"].IsObject()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: gps_devices"); 
    }else{
		obj.gps_devices.resize(doc["gps_devices"].Size());
		for(rapidjson::SizeType i = 0; i < doc["gps_devices"].Size(); i++){
				Deserialize(obj.gps_devices[i], doc["gps_devices"][i]);
		}
    }
    if(!doc.HasMember("connection_downsample") && doc["connection_downsample"].IsBool()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_downsample"); 
    }else{
        obj.connection_downsample = doc["connection_downsample"].GetBool();
    }
    if(!doc.HasMember("connection_downsample_mps") && doc["connection_downsample_mps"].IsUint64()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_downsample_mps"); 
    }else{
        obj.connection_downsample_mps = doc["connection_downsample_mps"].GetUint64();
    }
    if(!doc.HasMember("connection_enabled") && doc["connection_enabled"].IsBool()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_enabled"); 
    }else{
        obj.connection_enabled = doc["connection_enabled"].GetBool();
    }
    if(!doc.HasMember("connection_send_rate") && doc["connection_send_rate"].IsUint64()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_send_rate"); 
    }else{
        obj.connection_send_rate = doc["connection_send_rate"].GetUint64();
    }
    if(!doc.HasMember("connection_send_sensor_data") && doc["connection_send_sensor_data"].IsBool()){
        JSON_LOG_FUNC("telemetry_config MISSING FIELD: connection_send_sensor_data"); 
    }else{
        obj.connection_send_sensor_data = doc["connection_send_sensor_data"].GetBool();
    }
    if(!doc.HasMember("connection") && doc["connection"].IsObject()){
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
bool CheckJson(const telemetry_login_data& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("username")){
        JSON_LOG_FUNC("telemetry_login_data MISSING FIELD: username"); 
        check = false;
    }
    if(!doc.HasMember("password")){
        JSON_LOG_FUNC("telemetry_login_data MISSING FIELD: password"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const telemetry_login_data& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("username", rapidjson::Value().SetString(obj.username.c_str(), obj.username.size(), alloc), alloc);
    out.AddMember("password", rapidjson::Value().SetString(obj.password.c_str(), obj.password.size(), alloc), alloc);
}
template<>
void Deserialize(telemetry_login_data& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("username") && doc["username"].IsString()){
        JSON_LOG_FUNC("telemetry_login_data MISSING FIELD: username"); 
    }else{
        obj.username = std::string(doc["username"].GetString(), doc["username"].GetStringLength());
    }
    if(!doc.HasMember("password") && doc["password"].IsString()){
        JSON_LOG_FUNC("telemetry_login_data MISSING FIELD: password"); 
    }else{
        obj.password = std::string(doc["password"].GetString(), doc["password"].GetStringLength());
    }
}
template<>
void Deserialize(telemetry_login_data& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("username") && doc["username"].IsString()){
        JSON_LOG_FUNC("telemetry_login_data MISSING FIELD: username"); 
    }else{
        obj.username = std::string(doc["username"].GetString(), doc["username"].GetStringLength());
    }
    if(!doc.HasMember("password") && doc["password"].IsString()){
        JSON_LOG_FUNC("telemetry_login_data MISSING FIELD: password"); 
    }else{
        obj.password = std::string(doc["password"].GetString(), doc["password"].GetStringLength());
    }
}

template<>
std::string StructToString(const telemetry_login_data& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const telemetry_login_data& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, telemetry_login_data& out)
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
bool LoadStruct(telemetry_login_data& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const telemetry_login_data& obj, const std::string& path)
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
    if(!doc.HasMember("Circuit") && doc["Circuit"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Circuit"); 
    }else{
        obj.Circuit = std::string(doc["Circuit"].GetString(), doc["Circuit"].GetStringLength());
    }
    if(!doc.HasMember("Pilot") && doc["Pilot"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Pilot"); 
    }else{
        obj.Pilot = std::string(doc["Pilot"].GetString(), doc["Pilot"].GetStringLength());
    }
    if(!doc.HasMember("Race") && doc["Race"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Race"); 
    }else{
        obj.Race = std::string(doc["Race"].GetString(), doc["Race"].GetStringLength());
    }
    if(!doc.HasMember("Configuration") && doc["Configuration"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Configuration"); 
    }else{
        obj.Configuration = std::string(doc["Configuration"].GetString(), doc["Configuration"].GetStringLength());
    }
    if(!doc.HasMember("Date") && doc["Date"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Date"); 
    }else{
        obj.Date = std::string(doc["Date"].GetString(), doc["Date"].GetStringLength());
    }
    if(!doc.HasMember("Time") && doc["Time"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Time"); 
    }else{
        obj.Time = std::string(doc["Time"].GetString(), doc["Time"].GetStringLength());
    }
    if(!doc.HasMember("Canlib_Version") && doc["Canlib_Version"].IsDouble()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Canlib_Version"); 
    }else{
        obj.Canlib_Version = doc["Canlib_Version"].GetDouble();
    }
}
template<>
void Deserialize(session_config& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("Circuit") && doc["Circuit"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Circuit"); 
    }else{
        obj.Circuit = std::string(doc["Circuit"].GetString(), doc["Circuit"].GetStringLength());
    }
    if(!doc.HasMember("Pilot") && doc["Pilot"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Pilot"); 
    }else{
        obj.Pilot = std::string(doc["Pilot"].GetString(), doc["Pilot"].GetStringLength());
    }
    if(!doc.HasMember("Race") && doc["Race"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Race"); 
    }else{
        obj.Race = std::string(doc["Race"].GetString(), doc["Race"].GetStringLength());
    }
    if(!doc.HasMember("Configuration") && doc["Configuration"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Configuration"); 
    }else{
        obj.Configuration = std::string(doc["Configuration"].GetString(), doc["Configuration"].GetStringLength());
    }
    if(!doc.HasMember("Date") && doc["Date"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Date"); 
    }else{
        obj.Date = std::string(doc["Date"].GetString(), doc["Date"].GetStringLength());
    }
    if(!doc.HasMember("Time") && doc["Time"].IsString()){
        JSON_LOG_FUNC("session_config MISSING FIELD: Time"); 
    }else{
        obj.Time = std::string(doc["Time"].GetString(), doc["Time"].GetStringLength());
    }
    if(!doc.HasMember("Canlib_Version") && doc["Canlib_Version"].IsDouble()){
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