#ifndef __MESSAGES_JSON__
#define __MESSAGES_JSON__

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

typedef struct msgs_per_second_o{
    std::string device;
    uint64_t count;
}msgs_per_second_o;

typedef struct file_response_transaction{
    std::string identifier;
    uint64_t transaction_hash;
    std::string transaction_topic;
}file_response_transaction;

typedef struct file_chunk{
    std::string data;
    uint64_t chunk_n;
    uint64_t chunk_total;
    uint64_t transaction_hash;
}file_chunk;

typedef struct file_begin_transaction{
    std::string filename;
    std::string dest_path;
    uint64_t total_chunks;
    uint64_t transaction_hash;
}file_begin_transaction;

typedef struct command_status{
    std::string command;
    std::string data;
}command_status;

typedef struct ping{
    std::string type;
    double time;
}ping;

typedef struct file_ask_transaction{
    std::string identifier;
    uint64_t transaction_hash;
}file_ask_transaction;

typedef struct car_data{
    uint64_t timestamp;
    std::string primary;
    std::string secondary;
    std::string gps;
    std::string inverters;
}car_data;

typedef struct get_telemetry_config{
    std::string type;
    std::string telemetry_config;
    std::string session_config;
}get_telemetry_config;

typedef struct file_end_transaction{
    std::string filename;
    std::string dest_path;
    uint64_t transaction_hash;
}file_end_transaction;

typedef struct telemetry_status{
    std::string type;
    uint64_t timestamp;
    uint64_t zero_timestamp;
    uint64_t data;
    std::vector<msgs_per_second_o> msgs_per_second;
    std::string camera_status;
    std::string camera_error;
    uint64_t cpu_total_load;
    uint64_t cpu_process_load;
    uint64_t mem_process_load;
}telemetry_status;

typedef struct basic_message{
    std::string type;
    std::string data;
}basic_message;

#ifdef __MESSAGES_JSON_IMPLEMENTATION__

template <>
bool CheckJson(const msgs_per_second_o& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("device")){
        JSON_LOG_FUNC("msgs_per_second_o MISSING FIELD: device"); 
        check = false;
    }
    if(!doc.HasMember("count")){
        JSON_LOG_FUNC("msgs_per_second_o MISSING FIELD: count"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Value& out, const msgs_per_second_o& obj, rapidjson::Document::AllocatorType& alloc)
{
    out.SetObject();
    out.AddMember("device", rapidjson::Value().SetString(obj.device.c_str(), obj.device.size(), alloc), alloc);
    out.AddMember("count", rapidjson::Value().SetUint64(obj.count), alloc);
}
template<>
void Deserialize(msgs_per_second_o& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("device")){
        JSON_LOG_FUNC("msgs_per_second_o MISSING FIELD: device"); 
    }else{
    obj.device = std::string(doc["device"].GetString(), doc["device"].GetStringLength());
    }
    if(!doc.HasMember("count")){
        JSON_LOG_FUNC("msgs_per_second_o MISSING FIELD: count"); 
    }else{
        obj.count = doc["count"].GetUint64();
    }
}

template <>
bool CheckJson(const file_response_transaction& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("identifier")){
        JSON_LOG_FUNC("file_response_transaction MISSING FIELD: identifier"); 
        check = false;
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_response_transaction MISSING FIELD: transaction_hash"); 
        check = false;
    }
    if(!doc.HasMember("transaction_topic")){
        JSON_LOG_FUNC("file_response_transaction MISSING FIELD: transaction_topic"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const file_response_transaction& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("identifier", rapidjson::Value().SetString(obj.identifier.c_str(), obj.identifier.size(), alloc), alloc);
    out.AddMember("transaction_hash", rapidjson::Value().SetUint64(obj.transaction_hash), alloc);
    out.AddMember("transaction_topic", rapidjson::Value().SetString(obj.transaction_topic.c_str(), obj.transaction_topic.size(), alloc), alloc);
}
template<>
void Deserialize(file_response_transaction& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("identifier")){
        JSON_LOG_FUNC("file_response_transaction MISSING FIELD: identifier"); 
    }else{
    obj.identifier = std::string(doc["identifier"].GetString(), doc["identifier"].GetStringLength());
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_response_transaction MISSING FIELD: transaction_hash"); 
    }else{
        obj.transaction_hash = doc["transaction_hash"].GetUint64();
    }
    if(!doc.HasMember("transaction_topic")){
        JSON_LOG_FUNC("file_response_transaction MISSING FIELD: transaction_topic"); 
    }else{
    obj.transaction_topic = std::string(doc["transaction_topic"].GetString(), doc["transaction_topic"].GetStringLength());
    }
}
template<>
void Deserialize(file_response_transaction& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("identifier")){
        JSON_LOG_FUNC("file_response_transaction MISSING FIELD: identifier"); 
    }else{
    obj.identifier = std::string(doc["identifier"].GetString(), doc["identifier"].GetStringLength());
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_response_transaction MISSING FIELD: transaction_hash"); 
    }else{
        obj.transaction_hash = doc["transaction_hash"].GetUint64();
    }
    if(!doc.HasMember("transaction_topic")){
        JSON_LOG_FUNC("file_response_transaction MISSING FIELD: transaction_topic"); 
    }else{
    obj.transaction_topic = std::string(doc["transaction_topic"].GetString(), doc["transaction_topic"].GetStringLength());
    }
}

template<>
std::string StructToString(const file_response_transaction& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const file_response_transaction& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, file_response_transaction& out)
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
bool LoadStruct(file_response_transaction& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const file_response_transaction& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const file_chunk& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: data"); 
        check = false;
    }
    if(!doc.HasMember("chunk_n")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: chunk_n"); 
        check = false;
    }
    if(!doc.HasMember("chunk_total")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: chunk_total"); 
        check = false;
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: transaction_hash"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const file_chunk& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("data", rapidjson::Value().SetString(obj.data.c_str(), obj.data.size(), alloc), alloc);
    out.AddMember("chunk_n", rapidjson::Value().SetUint64(obj.chunk_n), alloc);
    out.AddMember("chunk_total", rapidjson::Value().SetUint64(obj.chunk_total), alloc);
    out.AddMember("transaction_hash", rapidjson::Value().SetUint64(obj.transaction_hash), alloc);
}
template<>
void Deserialize(file_chunk& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: data"); 
    }else{
    obj.data = std::string(doc["data"].GetString(), doc["data"].GetStringLength());
    }
    if(!doc.HasMember("chunk_n")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: chunk_n"); 
    }else{
        obj.chunk_n = doc["chunk_n"].GetUint64();
    }
    if(!doc.HasMember("chunk_total")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: chunk_total"); 
    }else{
        obj.chunk_total = doc["chunk_total"].GetUint64();
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: transaction_hash"); 
    }else{
        obj.transaction_hash = doc["transaction_hash"].GetUint64();
    }
}
template<>
void Deserialize(file_chunk& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: data"); 
    }else{
    obj.data = std::string(doc["data"].GetString(), doc["data"].GetStringLength());
    }
    if(!doc.HasMember("chunk_n")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: chunk_n"); 
    }else{
        obj.chunk_n = doc["chunk_n"].GetUint64();
    }
    if(!doc.HasMember("chunk_total")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: chunk_total"); 
    }else{
        obj.chunk_total = doc["chunk_total"].GetUint64();
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_chunk MISSING FIELD: transaction_hash"); 
    }else{
        obj.transaction_hash = doc["transaction_hash"].GetUint64();
    }
}

template<>
std::string StructToString(const file_chunk& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const file_chunk& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, file_chunk& out)
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
bool LoadStruct(file_chunk& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const file_chunk& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const file_begin_transaction& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("filename")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: filename"); 
        check = false;
    }
    if(!doc.HasMember("dest_path")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: dest_path"); 
        check = false;
    }
    if(!doc.HasMember("total_chunks")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: total_chunks"); 
        check = false;
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: transaction_hash"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const file_begin_transaction& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("filename", rapidjson::Value().SetString(obj.filename.c_str(), obj.filename.size(), alloc), alloc);
    out.AddMember("dest_path", rapidjson::Value().SetString(obj.dest_path.c_str(), obj.dest_path.size(), alloc), alloc);
    out.AddMember("total_chunks", rapidjson::Value().SetUint64(obj.total_chunks), alloc);
    out.AddMember("transaction_hash", rapidjson::Value().SetUint64(obj.transaction_hash), alloc);
}
template<>
void Deserialize(file_begin_transaction& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("filename")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: filename"); 
    }else{
    obj.filename = std::string(doc["filename"].GetString(), doc["filename"].GetStringLength());
    }
    if(!doc.HasMember("dest_path")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: dest_path"); 
    }else{
    obj.dest_path = std::string(doc["dest_path"].GetString(), doc["dest_path"].GetStringLength());
    }
    if(!doc.HasMember("total_chunks")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: total_chunks"); 
    }else{
        obj.total_chunks = doc["total_chunks"].GetUint64();
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: transaction_hash"); 
    }else{
        obj.transaction_hash = doc["transaction_hash"].GetUint64();
    }
}
template<>
void Deserialize(file_begin_transaction& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("filename")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: filename"); 
    }else{
    obj.filename = std::string(doc["filename"].GetString(), doc["filename"].GetStringLength());
    }
    if(!doc.HasMember("dest_path")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: dest_path"); 
    }else{
    obj.dest_path = std::string(doc["dest_path"].GetString(), doc["dest_path"].GetStringLength());
    }
    if(!doc.HasMember("total_chunks")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: total_chunks"); 
    }else{
        obj.total_chunks = doc["total_chunks"].GetUint64();
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_begin_transaction MISSING FIELD: transaction_hash"); 
    }else{
        obj.transaction_hash = doc["transaction_hash"].GetUint64();
    }
}

template<>
std::string StructToString(const file_begin_transaction& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const file_begin_transaction& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, file_begin_transaction& out)
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
bool LoadStruct(file_begin_transaction& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const file_begin_transaction& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const command_status& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("command")){
        JSON_LOG_FUNC("command_status MISSING FIELD: command"); 
        check = false;
    }
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("command_status MISSING FIELD: data"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const command_status& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("command", rapidjson::Value().SetString(obj.command.c_str(), obj.command.size(), alloc), alloc);
    out.AddMember("data", rapidjson::Value().SetString(obj.data.c_str(), obj.data.size(), alloc), alloc);
}
template<>
void Deserialize(command_status& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("command")){
        JSON_LOG_FUNC("command_status MISSING FIELD: command"); 
    }else{
    obj.command = std::string(doc["command"].GetString(), doc["command"].GetStringLength());
    }
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("command_status MISSING FIELD: data"); 
    }else{
    obj.data = std::string(doc["data"].GetString(), doc["data"].GetStringLength());
    }
}
template<>
void Deserialize(command_status& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("command")){
        JSON_LOG_FUNC("command_status MISSING FIELD: command"); 
    }else{
    obj.command = std::string(doc["command"].GetString(), doc["command"].GetStringLength());
    }
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("command_status MISSING FIELD: data"); 
    }else{
    obj.data = std::string(doc["data"].GetString(), doc["data"].GetStringLength());
    }
}

template<>
std::string StructToString(const command_status& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const command_status& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, command_status& out)
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
bool LoadStruct(command_status& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const command_status& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const ping& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("ping MISSING FIELD: type"); 
        check = false;
    }
    if(!doc.HasMember("time")){
        JSON_LOG_FUNC("ping MISSING FIELD: time"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const ping& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("type", rapidjson::Value().SetString(obj.type.c_str(), obj.type.size(), alloc), alloc);
    out.AddMember("time", rapidjson::Value().SetDouble(obj.time), alloc);
}
template<>
void Deserialize(ping& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("ping MISSING FIELD: type"); 
    }else{
    obj.type = std::string(doc["type"].GetString(), doc["type"].GetStringLength());
    }
    if(!doc.HasMember("time")){
        JSON_LOG_FUNC("ping MISSING FIELD: time"); 
    }else{
        obj.time = doc["time"].GetDouble();
    }
}
template<>
void Deserialize(ping& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("ping MISSING FIELD: type"); 
    }else{
    obj.type = std::string(doc["type"].GetString(), doc["type"].GetStringLength());
    }
    if(!doc.HasMember("time")){
        JSON_LOG_FUNC("ping MISSING FIELD: time"); 
    }else{
        obj.time = doc["time"].GetDouble();
    }
}

template<>
std::string StructToString(const ping& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const ping& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, ping& out)
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
bool LoadStruct(ping& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const ping& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const file_ask_transaction& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("identifier")){
        JSON_LOG_FUNC("file_ask_transaction MISSING FIELD: identifier"); 
        check = false;
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_ask_transaction MISSING FIELD: transaction_hash"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const file_ask_transaction& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("identifier", rapidjson::Value().SetString(obj.identifier.c_str(), obj.identifier.size(), alloc), alloc);
    out.AddMember("transaction_hash", rapidjson::Value().SetUint64(obj.transaction_hash), alloc);
}
template<>
void Deserialize(file_ask_transaction& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("identifier")){
        JSON_LOG_FUNC("file_ask_transaction MISSING FIELD: identifier"); 
    }else{
    obj.identifier = std::string(doc["identifier"].GetString(), doc["identifier"].GetStringLength());
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_ask_transaction MISSING FIELD: transaction_hash"); 
    }else{
        obj.transaction_hash = doc["transaction_hash"].GetUint64();
    }
}
template<>
void Deserialize(file_ask_transaction& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("identifier")){
        JSON_LOG_FUNC("file_ask_transaction MISSING FIELD: identifier"); 
    }else{
    obj.identifier = std::string(doc["identifier"].GetString(), doc["identifier"].GetStringLength());
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_ask_transaction MISSING FIELD: transaction_hash"); 
    }else{
        obj.transaction_hash = doc["transaction_hash"].GetUint64();
    }
}

template<>
std::string StructToString(const file_ask_transaction& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const file_ask_transaction& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, file_ask_transaction& out)
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
bool LoadStruct(file_ask_transaction& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const file_ask_transaction& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const car_data& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("timestamp")){
        JSON_LOG_FUNC("car_data MISSING FIELD: timestamp"); 
        check = false;
    }
    if(!doc.HasMember("primary")){
        JSON_LOG_FUNC("car_data MISSING FIELD: primary"); 
        check = false;
    }
    if(!doc.HasMember("secondary")){
        JSON_LOG_FUNC("car_data MISSING FIELD: secondary"); 
        check = false;
    }
    if(!doc.HasMember("gps")){
        JSON_LOG_FUNC("car_data MISSING FIELD: gps"); 
        check = false;
    }
    if(!doc.HasMember("inverters")){
        JSON_LOG_FUNC("car_data MISSING FIELD: inverters"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const car_data& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("timestamp", rapidjson::Value().SetUint64(obj.timestamp), alloc);
    out.AddMember("primary", rapidjson::Value().SetString(obj.primary.c_str(), obj.primary.size(), alloc), alloc);
    out.AddMember("secondary", rapidjson::Value().SetString(obj.secondary.c_str(), obj.secondary.size(), alloc), alloc);
    out.AddMember("gps", rapidjson::Value().SetString(obj.gps.c_str(), obj.gps.size(), alloc), alloc);
    out.AddMember("inverters", rapidjson::Value().SetString(obj.inverters.c_str(), obj.inverters.size(), alloc), alloc);
}
template<>
void Deserialize(car_data& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("timestamp")){
        JSON_LOG_FUNC("car_data MISSING FIELD: timestamp"); 
    }else{
        obj.timestamp = doc["timestamp"].GetUint64();
    }
    if(!doc.HasMember("primary")){
        JSON_LOG_FUNC("car_data MISSING FIELD: primary"); 
    }else{
    obj.primary = std::string(doc["primary"].GetString(), doc["primary"].GetStringLength());
    }
    if(!doc.HasMember("secondary")){
        JSON_LOG_FUNC("car_data MISSING FIELD: secondary"); 
    }else{
    obj.secondary = std::string(doc["secondary"].GetString(), doc["secondary"].GetStringLength());
    }
    if(!doc.HasMember("gps")){
        JSON_LOG_FUNC("car_data MISSING FIELD: gps"); 
    }else{
    obj.gps = std::string(doc["gps"].GetString(), doc["gps"].GetStringLength());
    }
    if(!doc.HasMember("inverters")){
        JSON_LOG_FUNC("car_data MISSING FIELD: inverters"); 
    }else{
    obj.inverters = std::string(doc["inverters"].GetString(), doc["inverters"].GetStringLength());
    }
}
template<>
void Deserialize(car_data& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("timestamp")){
        JSON_LOG_FUNC("car_data MISSING FIELD: timestamp"); 
    }else{
        obj.timestamp = doc["timestamp"].GetUint64();
    }
    if(!doc.HasMember("primary")){
        JSON_LOG_FUNC("car_data MISSING FIELD: primary"); 
    }else{
    obj.primary = std::string(doc["primary"].GetString(), doc["primary"].GetStringLength());
    }
    if(!doc.HasMember("secondary")){
        JSON_LOG_FUNC("car_data MISSING FIELD: secondary"); 
    }else{
    obj.secondary = std::string(doc["secondary"].GetString(), doc["secondary"].GetStringLength());
    }
    if(!doc.HasMember("gps")){
        JSON_LOG_FUNC("car_data MISSING FIELD: gps"); 
    }else{
    obj.gps = std::string(doc["gps"].GetString(), doc["gps"].GetStringLength());
    }
    if(!doc.HasMember("inverters")){
        JSON_LOG_FUNC("car_data MISSING FIELD: inverters"); 
    }else{
    obj.inverters = std::string(doc["inverters"].GetString(), doc["inverters"].GetStringLength());
    }
}

template<>
std::string StructToString(const car_data& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const car_data& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, car_data& out)
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
bool LoadStruct(car_data& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const car_data& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const get_telemetry_config& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("get_telemetry_config MISSING FIELD: type"); 
        check = false;
    }
    if(!doc.HasMember("telemetry_config")){
        JSON_LOG_FUNC("get_telemetry_config MISSING FIELD: telemetry_config"); 
        check = false;
    }
    if(!doc.HasMember("session_config")){
        JSON_LOG_FUNC("get_telemetry_config MISSING FIELD: session_config"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const get_telemetry_config& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("type", rapidjson::Value().SetString(obj.type.c_str(), obj.type.size(), alloc), alloc);
    out.AddMember("telemetry_config", rapidjson::Value().SetString(obj.telemetry_config.c_str(), obj.telemetry_config.size(), alloc), alloc);
    out.AddMember("session_config", rapidjson::Value().SetString(obj.session_config.c_str(), obj.session_config.size(), alloc), alloc);
}
template<>
void Deserialize(get_telemetry_config& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("get_telemetry_config MISSING FIELD: type"); 
    }else{
    obj.type = std::string(doc["type"].GetString(), doc["type"].GetStringLength());
    }
    if(!doc.HasMember("telemetry_config")){
        JSON_LOG_FUNC("get_telemetry_config MISSING FIELD: telemetry_config"); 
    }else{
    obj.telemetry_config = std::string(doc["telemetry_config"].GetString(), doc["telemetry_config"].GetStringLength());
    }
    if(!doc.HasMember("session_config")){
        JSON_LOG_FUNC("get_telemetry_config MISSING FIELD: session_config"); 
    }else{
    obj.session_config = std::string(doc["session_config"].GetString(), doc["session_config"].GetStringLength());
    }
}
template<>
void Deserialize(get_telemetry_config& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("get_telemetry_config MISSING FIELD: type"); 
    }else{
    obj.type = std::string(doc["type"].GetString(), doc["type"].GetStringLength());
    }
    if(!doc.HasMember("telemetry_config")){
        JSON_LOG_FUNC("get_telemetry_config MISSING FIELD: telemetry_config"); 
    }else{
    obj.telemetry_config = std::string(doc["telemetry_config"].GetString(), doc["telemetry_config"].GetStringLength());
    }
    if(!doc.HasMember("session_config")){
        JSON_LOG_FUNC("get_telemetry_config MISSING FIELD: session_config"); 
    }else{
    obj.session_config = std::string(doc["session_config"].GetString(), doc["session_config"].GetStringLength());
    }
}

template<>
std::string StructToString(const get_telemetry_config& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const get_telemetry_config& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, get_telemetry_config& out)
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
bool LoadStruct(get_telemetry_config& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const get_telemetry_config& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const file_end_transaction& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("filename")){
        JSON_LOG_FUNC("file_end_transaction MISSING FIELD: filename"); 
        check = false;
    }
    if(!doc.HasMember("dest_path")){
        JSON_LOG_FUNC("file_end_transaction MISSING FIELD: dest_path"); 
        check = false;
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_end_transaction MISSING FIELD: transaction_hash"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const file_end_transaction& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("filename", rapidjson::Value().SetString(obj.filename.c_str(), obj.filename.size(), alloc), alloc);
    out.AddMember("dest_path", rapidjson::Value().SetString(obj.dest_path.c_str(), obj.dest_path.size(), alloc), alloc);
    out.AddMember("transaction_hash", rapidjson::Value().SetUint64(obj.transaction_hash), alloc);
}
template<>
void Deserialize(file_end_transaction& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("filename")){
        JSON_LOG_FUNC("file_end_transaction MISSING FIELD: filename"); 
    }else{
    obj.filename = std::string(doc["filename"].GetString(), doc["filename"].GetStringLength());
    }
    if(!doc.HasMember("dest_path")){
        JSON_LOG_FUNC("file_end_transaction MISSING FIELD: dest_path"); 
    }else{
    obj.dest_path = std::string(doc["dest_path"].GetString(), doc["dest_path"].GetStringLength());
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_end_transaction MISSING FIELD: transaction_hash"); 
    }else{
        obj.transaction_hash = doc["transaction_hash"].GetUint64();
    }
}
template<>
void Deserialize(file_end_transaction& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("filename")){
        JSON_LOG_FUNC("file_end_transaction MISSING FIELD: filename"); 
    }else{
    obj.filename = std::string(doc["filename"].GetString(), doc["filename"].GetStringLength());
    }
    if(!doc.HasMember("dest_path")){
        JSON_LOG_FUNC("file_end_transaction MISSING FIELD: dest_path"); 
    }else{
    obj.dest_path = std::string(doc["dest_path"].GetString(), doc["dest_path"].GetStringLength());
    }
    if(!doc.HasMember("transaction_hash")){
        JSON_LOG_FUNC("file_end_transaction MISSING FIELD: transaction_hash"); 
    }else{
        obj.transaction_hash = doc["transaction_hash"].GetUint64();
    }
}

template<>
std::string StructToString(const file_end_transaction& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const file_end_transaction& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, file_end_transaction& out)
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
bool LoadStruct(file_end_transaction& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const file_end_transaction& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const telemetry_status& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: type"); 
        check = false;
    }
    if(!doc.HasMember("timestamp")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: timestamp"); 
        check = false;
    }
    if(!doc.HasMember("zero_timestamp")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: zero_timestamp"); 
        check = false;
    }
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: data"); 
        check = false;
    }
    if(!doc.HasMember("msgs_per_second")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: msgs_per_second"); 
        check = false;
    }
    if(!doc.HasMember("camera_status")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: camera_status"); 
        check = false;
    }
    if(!doc.HasMember("camera_error")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: camera_error"); 
        check = false;
    }
    if(!doc.HasMember("cpu_total_load")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: cpu_total_load"); 
        check = false;
    }
    if(!doc.HasMember("cpu_process_load")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: cpu_process_load"); 
        check = false;
    }
    if(!doc.HasMember("mem_process_load")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: mem_process_load"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const telemetry_status& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("type", rapidjson::Value().SetString(obj.type.c_str(), obj.type.size(), alloc), alloc);
    out.AddMember("timestamp", rapidjson::Value().SetUint64(obj.timestamp), alloc);
    out.AddMember("zero_timestamp", rapidjson::Value().SetUint64(obj.zero_timestamp), alloc);
    out.AddMember("data", rapidjson::Value().SetUint64(obj.data), alloc);
    {
        rapidjson::Value v0;
        v0.SetArray();
        for(size_t i = 0; i < obj.msgs_per_second.size(); i++){
        	rapidjson::Value new_obj;
        	Serialize(new_obj, obj.msgs_per_second[i], alloc);
        	v0.PushBack(new_obj, alloc);
    	}
    	out.AddMember("msgs_per_second", v0, alloc);
    }
    out.AddMember("camera_status", rapidjson::Value().SetString(obj.camera_status.c_str(), obj.camera_status.size(), alloc), alloc);
    out.AddMember("camera_error", rapidjson::Value().SetString(obj.camera_error.c_str(), obj.camera_error.size(), alloc), alloc);
    out.AddMember("cpu_total_load", rapidjson::Value().SetUint64(obj.cpu_total_load), alloc);
    out.AddMember("cpu_process_load", rapidjson::Value().SetUint64(obj.cpu_process_load), alloc);
    out.AddMember("mem_process_load", rapidjson::Value().SetUint64(obj.mem_process_load), alloc);
}
template<>
void Deserialize(telemetry_status& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: type"); 
    }else{
    obj.type = std::string(doc["type"].GetString(), doc["type"].GetStringLength());
    }
    if(!doc.HasMember("timestamp")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: timestamp"); 
    }else{
        obj.timestamp = doc["timestamp"].GetUint64();
    }
    if(!doc.HasMember("zero_timestamp")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: zero_timestamp"); 
    }else{
        obj.zero_timestamp = doc["zero_timestamp"].GetUint64();
    }
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: data"); 
    }else{
        obj.data = doc["data"].GetUint64();
    }
    if(!doc.HasMember("msgs_per_second")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: msgs_per_second"); 
    }else{
	obj.msgs_per_second.resize(doc["msgs_per_second"].Size());
	for(rapidjson::SizeType i = 0; i < doc["msgs_per_second"].Size(); i++){
		Deserialize(obj.msgs_per_second[i], doc["msgs_per_second"][i]);
	}
    }
    if(!doc.HasMember("camera_status")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: camera_status"); 
    }else{
    obj.camera_status = std::string(doc["camera_status"].GetString(), doc["camera_status"].GetStringLength());
    }
    if(!doc.HasMember("camera_error")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: camera_error"); 
    }else{
    obj.camera_error = std::string(doc["camera_error"].GetString(), doc["camera_error"].GetStringLength());
    }
    if(!doc.HasMember("cpu_total_load")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: cpu_total_load"); 
    }else{
        obj.cpu_total_load = doc["cpu_total_load"].GetUint64();
    }
    if(!doc.HasMember("cpu_process_load")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: cpu_process_load"); 
    }else{
        obj.cpu_process_load = doc["cpu_process_load"].GetUint64();
    }
    if(!doc.HasMember("mem_process_load")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: mem_process_load"); 
    }else{
        obj.mem_process_load = doc["mem_process_load"].GetUint64();
    }
}
template<>
void Deserialize(telemetry_status& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: type"); 
    }else{
    obj.type = std::string(doc["type"].GetString(), doc["type"].GetStringLength());
    }
    if(!doc.HasMember("timestamp")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: timestamp"); 
    }else{
        obj.timestamp = doc["timestamp"].GetUint64();
    }
    if(!doc.HasMember("zero_timestamp")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: zero_timestamp"); 
    }else{
        obj.zero_timestamp = doc["zero_timestamp"].GetUint64();
    }
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: data"); 
    }else{
        obj.data = doc["data"].GetUint64();
    }
    if(!doc.HasMember("msgs_per_second")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: msgs_per_second"); 
    }else{
	obj.msgs_per_second.resize(doc["msgs_per_second"].Size());
	for(rapidjson::SizeType i = 0; i < doc["msgs_per_second"].Size(); i++){
		Deserialize(obj.msgs_per_second[i], doc["msgs_per_second"][i]);
	}
    }
    if(!doc.HasMember("camera_status")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: camera_status"); 
    }else{
    obj.camera_status = std::string(doc["camera_status"].GetString(), doc["camera_status"].GetStringLength());
    }
    if(!doc.HasMember("camera_error")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: camera_error"); 
    }else{
    obj.camera_error = std::string(doc["camera_error"].GetString(), doc["camera_error"].GetStringLength());
    }
    if(!doc.HasMember("cpu_total_load")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: cpu_total_load"); 
    }else{
        obj.cpu_total_load = doc["cpu_total_load"].GetUint64();
    }
    if(!doc.HasMember("cpu_process_load")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: cpu_process_load"); 
    }else{
        obj.cpu_process_load = doc["cpu_process_load"].GetUint64();
    }
    if(!doc.HasMember("mem_process_load")){
        JSON_LOG_FUNC("telemetry_status MISSING FIELD: mem_process_load"); 
    }else{
        obj.mem_process_load = doc["mem_process_load"].GetUint64();
    }
}

template<>
std::string StructToString(const telemetry_status& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const telemetry_status& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, telemetry_status& out)
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
bool LoadStruct(telemetry_status& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const telemetry_status& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

template <>
bool CheckJson(const basic_message& obj, const rapidjson::Document& doc)
{
    bool check = true;
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("basic_message MISSING FIELD: type"); 
        check = false;
    }
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("basic_message MISSING FIELD: data"); 
        check = false;
    }
    return check;
}

template<>
void Serialize(rapidjson::Document& out, const basic_message& obj)
{
    out.SetObject();
    rapidjson::Document::AllocatorType& alloc = out.GetAllocator();
    out.AddMember("type", rapidjson::Value().SetString(obj.type.c_str(), obj.type.size(), alloc), alloc);
    out.AddMember("data", rapidjson::Value().SetString(obj.data.c_str(), obj.data.size(), alloc), alloc);
}
template<>
void Deserialize(basic_message& obj, rapidjson::Document& doc)
{
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("basic_message MISSING FIELD: type"); 
    }else{
    obj.type = std::string(doc["type"].GetString(), doc["type"].GetStringLength());
    }
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("basic_message MISSING FIELD: data"); 
    }else{
    obj.data = std::string(doc["data"].GetString(), doc["data"].GetStringLength());
    }
}
template<>
void Deserialize(basic_message& obj, rapidjson::Value& doc)
{
    if(!doc.HasMember("type")){
        JSON_LOG_FUNC("basic_message MISSING FIELD: type"); 
    }else{
    obj.type = std::string(doc["type"].GetString(), doc["type"].GetStringLength());
    }
    if(!doc.HasMember("data")){
        JSON_LOG_FUNC("basic_message MISSING FIELD: data"); 
    }else{
    obj.data = std::string(doc["data"].GetString(), doc["data"].GetStringLength());
    }
}

template<>
std::string StructToString(const basic_message& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
std::string StructToStringPretty(const basic_message& obj)
{
    rapidjson::Document doc;
    rapidjson::StringBuffer sb;
    Serialize(doc, obj);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);
    return sb.GetString();;
}

template<>
bool StringToStruct(const std::string& obj_str, basic_message& out)
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
bool LoadStruct(basic_message& out, const std::string& path)
{
    rapidjson::Document doc;
    LoadJSON(doc, path);
    bool check_passed = CheckJson(out, doc);
    Deserialize(out, doc);
    return check_passed;
}
template<>
void SaveStruct(const basic_message& obj, const std::string& path)
{
    rapidjson::Document doc;
    Serialize(doc, obj);
    SaveJSON(doc, path);
}

#endif //__MESSAGES_JSON_IMPLEMENTATION__

#endif // __JSON_LOADER__