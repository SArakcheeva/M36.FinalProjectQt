#ifndef _MY_GLOBAL
#define _MY_GLOBAL
#include <string>

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных

extern int socket_file_descriptor, connection;
extern struct sockaddr_in serveraddress, client;
extern char messageCS[MESSAGE_LENGTH];

extern int PORT;
extern std::string IP;

extern std::string loginChat;
extern std::string passwordChat;
extern std::string nicknameChat;

#endif
