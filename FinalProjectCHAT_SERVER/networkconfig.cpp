#include "networkconfig.h"

#include <filesystem>
#include <fstream>


NetworkConfig::NetworkConfig() {
    readConfig();
}


NetworkConfig::~NetworkConfig() {
}


void NetworkConfig::setPort(const int port) {
    port_ = port;
}


int NetworkConfig::getPort() const {
    return port_;
}


void NetworkConfig::setHostDB(const std::string& host) {
    hostDB_ = host;
}


const std::string& NetworkConfig::getHostDB() const {
    return hostDB_;
}


void NetworkConfig::setUserDB(const std::string& user) {
    userDB_ = user;
}


const std::string& NetworkConfig::getUserDB() const {
    return userDB_;
}


void NetworkConfig::setPasswdDB(const std::string& passwd) {
    passwdDB_ = passwd;
}


const std::string& NetworkConfig::getPasswdDB() const {
    return passwdDB_;
}


void NetworkConfig::setNameDB(const std::string& name) {
    nameDB_ = name;
}


const std::string& NetworkConfig::getNameDB() const {
    return nameDB_;
}


void NetworkConfig::readConfig() {
    std::fstream file(networkConfigFile, std::ios::in);

    // perms - Этот тип представляет права доступа к файлам
    auto permissions = std::filesystem::perms::group_all | std::filesystem::perms::others_all;

    std::filesystem::permissions(networkConfigFile, permissions, std::filesystem::perm_options::remove);

    const std::string delimiter = ":";
    if(file.is_open()) {
        std::string line;
        std::string name;
        std::string value;

        while (std::getline(file, line)) {
            size_t delimiterPosition = line.find(delimiter);

            if(delimiterPosition > 0) {
                name = line.substr(0, delimiterPosition);
                value = line.substr(delimiterPosition + 1);

                if(name == "port") {
                    port_ = std::stoi(value);
                }
                else if(name == "DBhost") {
                    hostDB_ = value;
                }
                else if(name == "DBuser") {
                    userDB_ = value;
                }
                else if(name == "DBpasswd") {
                    passwdDB_ = value;
                }
                else if(name == "DBname") {
                    nameDB_ = value;
                }
            }
        }
    }

    file.close();
}
