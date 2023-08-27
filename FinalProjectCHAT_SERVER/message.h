/**
\file Message.h
\brief Класс инкапсулирует данные о сообщении

Содержит поля:
- имя пользователя от кого сообщение
- имя пользователя кому сообщение
- текст сообщения
*/

#pragma once
#include <fstream>
#include <string>

class Message {
    public:
        /**
        Конструктор по-умолчанию
        */
        Message();

        /**
        Параметризованный конструктор
        \param[in] nameUserFrom Ник пользователя от которого сообщение
        \param[in] nameUserTo Ник пользователя кому адресовано сообщение
        \param[in] sendTime Время отправки сообщения
        \param[in] text Текст сообщения
        */
        Message(const std::string& nameUserFrom,
            const std::string& nameUserTo,
            const std::string& sendTime,
            const std::string& text);
        /**
        \return Ник пользователя от которого сообщение
        */
        const std::string& getNameFrom() const;

        /**
        \return Ник пользователя кому сообщение
        */
        const std::string& getNameTo() const;

        /**
        \return Время отправки сообщения
        */
        const std::string& getSendTime() const;

        /**
        \return Текст сообщения
        */
        const std::string& getText() const;

        friend std::fstream& operator >> (std::fstream& is, Message& obj);
        friend std::ostream& operator << (std::ostream& os, const Message& obj);

    private:
        const std::string nameUserFrom_;	///<Имя пользователя от которого сообщение
        const std::string nameUserTo_;		///<Имя пользователя кому адресовано сообщение
        const std::string sendTime_;        ///<Время отправки сообщения
        const std::string text_;			///<Текст сообщения
};
