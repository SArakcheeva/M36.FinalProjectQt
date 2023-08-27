#pragma once
#include <string>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "Socket.h"
#include "NetworkConfig.h"



class Chat
{
public:
    Chat();

    //Проверка Логина/Пароля/Ника на наличие запрещённых символов
    bool isCorrectValue(const std::string& inputValue);

    //Проверить есть ли в базе заданный Логин
    unsigned int isExistLogin(const std::string& login);

    //Проверить соответствует ли Пароль заданному Логину
    unsigned int isCorrectPassword(const std::string& login, const std::string& password);

    //Проверить есть ли в базе заданный Ник
    unsigned int isExistName(const std::string& name);

    //Добавить в базу заданного пользователя
    unsigned int addUser(const std::string& login, const std::string& password, const std::string& name);

    //Удалить аккаунт пользователя
    unsigned int removeAccount(const std::string& login, const std::string& password, const std::string& name);// ответ 1 или 0

    //Разделить строку на подстроки, используя делиметр
    std::vector<std::string> split(const std::string& s, char delim) const;

    //Cписок пользователей
    std::vector<std::string> getUserList() const;

    //Загрузить сообщения текущему пользователю чата
    std::vector<std::string> getChatMessagesToUser(const std::string& login, const std::string& password, const std::string& name) const;

    //Добавить в базу сообщение от одного пользователя другому
    unsigned int sendMessage(const std::string& nameFrom, const std::string& nameAdressee, char time_buffer[], const std::string& textMessage);

    //Загрузить из базы Ник пользователя
     std::string getNameByLogin(const std::string& login) const;

    //Указатель на текущего пользователя чата
    //User* getUser();

private:
    //User* user_;    //<Текущий пользователь чата

};
