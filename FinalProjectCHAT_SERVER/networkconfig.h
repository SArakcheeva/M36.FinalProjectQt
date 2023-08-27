/**
\file NetworkConfig.h
\brief Класс содержит данные о настройках соединения клиент-сервер и c базой данных
Класс инкапсулирует в себе параметры соединения:
- port - для соединения client -server
- hostDB - для соединения с БД
- userDB_ - имя пользователя, под которым он будет заходить в БД
- passwdDB_ - пароль, по которому он будет заходить в БД
- nameDB_ - название БД
*/
#pragma once

#include <string>
#include "globalparam.h"
#include "networkconfig.h"

constexpr auto networkConfigFile = "network.config";

class NetworkConfig final {
public:
    NetworkConfig();
    ~NetworkConfig();

    void setPort(const int port);
    int getPort() const;

    void setHostDB(const std::string& host);
    const std::string& getHostDB() const;

    void setUserDB(const std::string& user);
    const std::string& getUserDB() const;

    void setPasswdDB(const std::string& passwd);
    const std::string& getPasswdDB() const;

    void setNameDB(const std::string& name);
    const std::string& getNameDB() const;

private:
    int port_{ 0 };			///<port

    std::string hostDB_;	///<host
    std::string userDB_;	///<имя пользователя
    std::string passwdDB_;	///<пароль
    std::string nameDB_;	///<название БД

    void readConfig();
};
