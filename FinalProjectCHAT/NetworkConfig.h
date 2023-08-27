#pragma once

#include <string>

constexpr auto networkConfigFile = "network.config";

class NetworkConfig final {
public:
    NetworkConfig();
    ~NetworkConfig();

    void setIP(const std::string& ip);
    const std::string& getIP() const;

    void setPort(const int port);
    int getPort() const;

private:
    std::string ip_;
    int port_{ 0 };

    void readConfig();
};
