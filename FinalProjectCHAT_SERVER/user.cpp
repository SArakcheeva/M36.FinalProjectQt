#include "user.h"
#include <fstream>
#include <assert.h>


//Не требуется инициализировать нулевой строкой объекты типа string
//User::User() : name_(""), login_(""), password_("")
User::User() = default;


User::User(const std::string& name,
    const std::string& login,
    const std::string& password) :
    name_(name), login_(login), password_(password)
{

}


User::User(const User& u)
{
    this->name_ = u.name_;
    this->login_ = u.login_;
    this->password_ = u.password_;
}


bool User::operator==(User other) const
{
    //Объекты равны если совпадает Логин
    if (login_ == other.login_) {
        return true;
    }
    return false;
}


std::string User::getName() const
{
    return name_;
}


std::string User::getLogin() const
{
    return login_;
}


std::string User::getPassword() const
{
    return password_;
}


void User::setName(const std::string& name)
{
    name_ = name;
}


void User::setLogin(const std::string& login)
{
    login_ = login;
}


void User::setPassword(const std::string& password)
{
    password_ = password;
}


void User::reset()
{
    name_.clear();
    login_.clear();
    password_.clear();
}


std::fstream& operator >> (std::fstream& is, User& user)
{
    is >> user.name_;
    is >> user.login_;
    is >> user.password_;
    return is;
}


std::ostream& operator << (std::ostream& os, const User& user)
{
    os << user.name_;
    os << ' ';
    os << user.login_;
    os << ' ';
    os << user.password_;

    return os;
}
