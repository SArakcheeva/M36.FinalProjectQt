#include "message.h"
#include <fstream>
#include <assert.h>


Message::Message() = default;

Message::Message(const std::string& nameUserFrom,
    const std::string& nameUserTo,
    const std::string& sendTime,
    const std::string& text) :
    nameUserFrom_(nameUserFrom),
    nameUserTo_(nameUserTo),
    sendTime_(sendTime),
    text_(text)
{

}


const std::string& Message::getNameFrom() const
{
    return nameUserFrom_;
}


const std::string& Message::getNameTo() const
{
    return nameUserTo_;
}


const std::string& Message::getSendTime() const
{
    return sendTime_;
}


const std::string& Message::getText() const
{
    return text_;
}


std::fstream& operator >> (std::fstream& is, Message& mess)
{
    /*is >> mess.text_;
    is >> mess.nameUserFrom_;
    is >> mess.nameUserTo_;*/
    return is;
}


std::ostream& operator << (std::ostream& os, const Message& mess)
{
    os << mess.text_;
    os << ' ';
    os << mess.nameUserFrom_;
    os << ' ';
    os << mess.nameUserTo_;

    return os;
}
