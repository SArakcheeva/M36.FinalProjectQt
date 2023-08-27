/**
\file User.h
\brief Класс содержит данные о пользователе
Класс инкапсулирует в себе параметры пользователя:
- Ник (имя) - по нику он будет известен другим пользователям
- Логин - имя по которому он будет заходить в чат
- Пароль
*/

#pragma once
#include <fstream>
#include <string>

class User {
public:
    User();
    User(const std::string& name,
        const std::string& login,
        const std::string& password);
    /**
    Конструктор копирования
    */
    User(const User& u);

    /**
    Перегрузка оператора '==' для поиска пользователя в базе данных
    с использованием алгоритмов STL
    */
    bool operator==(User other) const;

    /**
    \return Ник пользователя
    */
    std::string getName() const;

    /**
    \return Логин пользователя
    */
    std::string getLogin() const;

    /**
    \return Пароль пользователя
    */
    std::string getPassword() const;

    /**
    Задать пользователю Имя
    \param[in] name Имя
    */
    void setName(const std::string& name);

    /**
    Задать пользователю Логин
    \param[in] login Логин
    */
    void setLogin(const std::string& login);

    /**
    Задать пользователю Пароль
    \param[in] password Пароль
    */
    void setPassword(const std::string& password);

    /**
    Присвоить значения полей класса - пустая строка
    */
    void reset();

    friend std::fstream& operator >> (std::fstream& is, User& obj);
    friend std::ostream& operator << (std::ostream& os, const User& obj);

private:
    std::string name_;		///<Ник
    std::string login_;		///<Логин
    std::string password_;	///<Пароль
};
