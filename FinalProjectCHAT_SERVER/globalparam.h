#ifndef GLOBALPARAM_H
#define GLOBALPARAM_H

#include <QString>
#include "user.h"

extern int PORT;
extern std::string hostDB;
extern std::string userDB;
extern std::string passwdDB;
extern std::string nameDB;

extern std::vector<User> activUsers;
extern std::vector<User> removeUsers;
extern QString allMessages;

#endif // GLOBALPARAM_H
