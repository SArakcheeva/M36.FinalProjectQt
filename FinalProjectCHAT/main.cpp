#include "mainwindow.h"
#include "Chat.h"
#include "Socket.h"
#include <QApplication>
#include <QDebug>


int socket_file_descriptor, connection;
struct sockaddr_in serveraddress, client;
char messageCS[MESSAGE_LENGTH];
int PORT;
std::string IP;
std::string loginChat;
std::string passwordChat;
std::string nicknameChat;


int main(int argc, char *argv[])
{
    try {
        // PORT and IP from file network.config
        NetworkConfig config;
        PORT = config.getPort();
        IP = config.getIP();
        IP.erase(IP.find(' '), 1);
        config.setPort(PORT);
        config.setIP(IP);

        // Создадим сокет
        socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_file_descriptor == -1) {
            qDebug() << "Creation of Socket failed!";
            exit(1);
        }

        // Установим адрес сервера
        serveraddress.sin_addr.s_addr = inet_addr(IP.c_str());
        // Зададим номер порта
        serveraddress.sin_port = htons(PORT);
        // Используем IPv4
        serveraddress.sin_family = AF_INET;

        // Установим соединение с сервером/
        connection = connect(socket_file_descriptor, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
        if (connection == -1) {
            qDebug() << "Connection with the server failed!";
            exit(1);
        }
    }
    catch (std::bad_alloc& error) {
        qDebug() << "Ошибка выделения памяти: " << error.what();
    }
    catch (std::exception& error) {
        qDebug() << error.what();
    }
    catch (...) {
        qDebug() << "Неопределённое исключение";
    }

    //------------------------------------------------------------------------------------
    QApplication a(argc, argv);

    // при запуске программы сразу запрашивается логин/пароль
    auto w = MainWindow::createClient();
    if(w)// чтобы не было исключений, т.к. может вернуть нулевой указатель
        w->show();
    else
        return 0;// т.е. если не создали ни одной инстанции, то просто выходим из программы

    return a.exec();
}
