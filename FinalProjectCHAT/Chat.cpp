#include "Chat.h"
#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <memory>
#include <QMessageBox>
#include "Socket.h"

bool Chat::isCorrectValue(const std::string &inputValue)
{
    //Можно вводить символы латинского алфавита и арабские цифры
    const std::string permissionedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t pos = inputValue.find_first_not_of(permissionedChars);
    if (pos != std::string::npos) {
        return false;
    }
    return true;
}


unsigned int Chat::isExistLogin(const std::string& login)
{
    // Connection with the server failed
    if (connection == -1)
        return 2;

    // ЗАПРОС СЕРВЕРУ - ЕСТЬ ЛИ ЛОГИН В БД
    std::string request = "0|" + login;
    bzero(messageCS, sizeof(messageCS));
    strcpy(messageCS, request.c_str());
    write(socket_file_descriptor, messageCS, sizeof(messageCS));

    // Ждем ответа от сервера
    bzero(messageCS, sizeof(messageCS));
    read(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Convert answer to int
    unsigned existLogin_ = 0;
    sscanf(messageCS, "%d", &existLogin_);

    return existLogin_;
}


unsigned int Chat::isCorrectPassword(const std::string& login, const std::string& password)
{
    // Connection with the server failed
    if (connection == -1)
        return 2;

    //ЗАПРОС СЕРВЕРУ - ПАРОЛЬ ПРАВИЛЬНЫЙ
    std::string request = "1|" + login + "|" + password;
    bzero(messageCS, sizeof(messageCS));
    strcpy(messageCS, request.c_str());
    write(socket_file_descriptor, messageCS, sizeof(messageCS));

    // Ждем ответа от сервера
    bzero(messageCS, sizeof(messageCS));
    read(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Convert answer to int
    unsigned pass_isCorrect = 0;
    sscanf(messageCS, "%d", &pass_isCorrect);

    return pass_isCorrect;
}


unsigned int Chat::isExistName(const std::string& name)
{
    // Connection with the server failed
    if (connection == -1)
        return 2;

    //ЗАПРОС СЕРВЕРУ - ЕСТЬ ЛИ ИМЯ ПОЛЬЗОВАТЕЛЯ В БД
    const std::string request = "2|" + name;
    bzero(messageCS, sizeof(messageCS));
    strcpy(messageCS, request.c_str());
    write(socket_file_descriptor, messageCS, sizeof(messageCS));

    // Ждем ответа от сервера
    bzero(messageCS, sizeof(messageCS));
    read(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Convert answer to int
    unsigned name_isCorrect = 0;
    sscanf(messageCS, "%d", &name_isCorrect);

    return name_isCorrect;
}


unsigned int Chat::addUser(const std::string& login, const std::string& password, const std::string& name)
{
    // Connection with the server failed
    if (connection == -1)
        return 2;

    //ЗАПРОС СЕРВЕРУ - ДОБАВИТЬ ПОЛЬЗОВАТЕЛЯ В БД
    const std::string request = "3|" + login + "|" + password + "|" + name;
    bzero(messageCS, sizeof(messageCS));
    strcpy(messageCS, request.c_str());
    write(socket_file_descriptor, messageCS, sizeof(messageCS));

    // Ждем ответа от сервера
    bzero(messageCS, sizeof(messageCS));
    read(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Convert answer to int
    unsigned s_answer = 0;
    sscanf(messageCS, "%d", &s_answer);

    return s_answer;    
}


unsigned int Chat::removeAccount(const std::string& login, const std::string& password, const std::string& name)
{
    // Connection with the server failed
    if (connection == -1)
        return 2;

    //ЗАПРОС СЕРВЕРУ - УДАЛИТЬ ПОЛЬЗОВАТЕЛЯ
    const std::string request = "4|" + login + "|" + password + "|" + name;
    bzero(messageCS, sizeof(messageCS));
    strcpy(messageCS, request.c_str());
    write(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Ждем ответа от сервера
    bzero(messageCS, sizeof(messageCS));
    read(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Convert answer to int (ответ 1 или 0)
    unsigned int isRemove = 0;
    sscanf(messageCS, "%d", &isRemove);

    return isRemove;
}


std::vector<std::string> Chat::split(const std::string& s, char delim) const
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}


std::vector<std::string> Chat::getUserList() const
{
    std::vector<std::string> userNames;

    if (connection != -1) {
        //ЗАПРОС СЕРВЕРУ - СПИСОК ВСЕХ ПОЛЬЗОВАТЕЛЕЙ
        bzero(messageCS, sizeof(messageCS));
        strcpy(messageCS, "6");
        write(socket_file_descriptor, messageCS, sizeof(messageCS));

        // Ждем ответа от сервера
        bzero(messageCS, sizeof(messageCS));
        read(socket_file_descriptor, messageCS, sizeof(messageCS));

        //ответ СТРОКА ИМЕН
        userNames = split(messageCS, '|');
    }
    return userNames;
}


std::vector<std::string> Chat::getChatMessagesToUser(const std::string& login, const std::string& password, const std::string& name) const
{
    std::vector<std::string> userMess;

    if (connection != -1) {
        //ЗАПРОС СЕРВЕРУ
        const std::string request = "7|" + login + "|" + password + "|" + name;
        bzero(messageCS, sizeof(messageCS));
        strcpy(messageCS, request.c_str());
        write(socket_file_descriptor, messageCS, sizeof(messageCS));

        //Ждем ответа от сервера
        bzero(messageCS, sizeof(messageCS));
        read(socket_file_descriptor, messageCS, sizeof(messageCS));

        //Загрузить сообщения и вывести на экран
        userMess = split(messageCS, '|');
    }
    return userMess;
}


unsigned int Chat::sendMessage(const std::string &nameFrom, const std::string &nameAdressee, char time_buffer[], const std::string &textMessage)
{
    // Connection with the server failed
    if (connection == -1)
        return 2;

    //ЗАПРОС СЕРВЕРУ - ОТПРАВИТЬ СООБЩЕНИЕ
    const std::string request = "8|" + nameFrom + "|" + nameAdressee + "|" + time_buffer + "|" + textMessage;
    bzero(messageCS, sizeof(messageCS));
    strcpy(messageCS, request.c_str());
    write(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Ждем ответа от сервера
    bzero(messageCS, sizeof(messageCS));
    read(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Convert answer to int ( 1 - ОК )
    unsigned server_answer = 0;
    sscanf(messageCS, "%d", &server_answer);

    return server_answer;
}


std::string Chat::getNameByLogin(const std::string& login) const
{
    std::string name;

    if (connection != -1) {
        const std::string request = "9|" + login;
        bzero(messageCS, sizeof(messageCS));
        strcpy(messageCS, request.c_str());
        write(socket_file_descriptor, messageCS, sizeof(messageCS));

        //Ждем ответа от сервера
        bzero(messageCS, sizeof(messageCS));
        read(socket_file_descriptor, messageCS, sizeof(messageCS));

        name = std::string(messageCS);
    }
    return name;
}
