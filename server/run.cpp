#include "server.h"
#include "json/json.h"
using namespace std;

int main()
{
    std::ifstream ifs;
    ifs.open("config.json");
    if (!ifs.is_open())
        throw std::runtime_error("Failed to open the configuration file!");

    // utf8支持
    Json::CharReaderBuilder ReaderBuilder;
    ReaderBuilder["emitUTF8"] = true;

    Json::Value root;
    std::string strerr;
    bool ok = Json::parseFromStream(ReaderBuilder, ifs, &root, &strerr);
    if (!ok)
        throw std::runtime_error("Failed to read the configuration file!");
    
    if (!root.isMember("server-setting"))
        throw std::runtime_error("Profile is missing value 'server-setting'!");
    
    auto check = [&](const std::string& key) {
        if (!root["server-setting"].isMember(key))
            throw std::runtime_error("The server-setting configuration is missing the value '" + key + "'");
    };
    
    for (const std::string& key : {"ip", "port", "connSustainTime"})
        check(key);
    
    string ip = root["server-setting"]["ip"].asString();
    uint port = root["server-setting"]["port"].asUInt();
    uint connSustainTime = root["server-setting"]["connSustainTime"].asUInt();
    run(ip, port, {connSustainTime, 0});
}