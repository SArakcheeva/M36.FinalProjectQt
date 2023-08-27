#include "database.h"

#include <map>
#include <list>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "networkconfig.h"
#include "user.h"
#include "message.h"
#include "globalparam.h"

#include <QDebug>
#include <QSqlRecord>
#include <QMessageBox>


DataBase::DataBase()
{
    // Connections to DB
    connectionDB();

    // Создать новую базу данных
    createDataBase();

    // Создать таблицу Users
    createUsersTable();

    // Заполнить таблицу Users данными
    User u1("G", "Ger", "123");
    User u2("S", "Sve", "qwe");
    addUser(u1);
    addUser(u2);

    // Создать таблицу Messages
    createMessagesTable();

    allActivUsers();
    loadAllMessages();
}


DataBase::~DataBase()
{
    delete mysql_query;

    // Для удаления соединения с базой данных, надо сначала закрыть базу данных
    db.close();

    //а затем, удалить соединение с помощью статического метода
    QSqlDatabase::removeDatabase("connection");

    system("Pause");
}


void DataBase::connectionDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "connection");//QMYSQL - тип драйвера базы данных, чтобы использовать для соединения
    db.setHostName(QString::fromStdString(hostDB));
    //db.setDatabaseName(QStrin);
    db.setUserName(QString::fromStdString(userDB));
    db.setPassword(QString::fromStdString(passwdDB));
    if(db.open()) {
        qDebug() << "Connection: Success!";
        mysql_query = new QSqlQuery(db);
    }
    else {
        qDebug() << db.lastError().text();
        exit(0);
    }
}


void DataBase::createDataBase()
{
    mysql_query->exec("CREATE DATABASE " + QString::fromStdString(nameDB));
    qDebug() << "CREATE DATABASE " << QString::fromStdString(nameDB) << ". " << mysql_error.text();

    mysql_query->exec("USE " + QString::fromStdString(nameDB));
    qDebug() << "USE " << QString::fromStdString(nameDB) << ". " << mysql_error.text();
}


void DataBase::createUsersTable()
{
    mysql_query->exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS users (id INT AUTO_INCREMENT PRIMARY KEY, login VARCHAR(255) NOT NULL UNIQUE, nickname VARCHAR(255) NOT NULL UNIQUE, password VARCHAR(255) NOT NULL)"));
    qDebug() << "CREATE TABLE users. " << mysql_error.text();
}


bool DataBase::addUser(const User &user)
{
    mysql_query->exec(QString::fromStdString("INSERT INTO users(login, nickname, password) VALUES('" + user.getLogin() + "', '" + user.getName() + "', '" + user.getPassword() + "')"));

    QString err = mysql_error.text();
    if (err.isEmpty()) {
        activUsers.push_back(user);
        return true;
    }
    else
        return false;
}


void DataBase::createMessagesTable()
{
    mysql_query->exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS messages (id INT AUTO_INCREMENT PRIMARY KEY, id_from INT REFERENCES users(id), id_to INT REFERENCES users(id), send_date DATETIME NOT NULL, text VARCHAR(255) NOT NULL)"));
    qDebug() << "CREATE TABLE messages. " << mysql_error.text();
}


int DataBase::findIdByNickname(const QString& nickname)
{
    //connectionDB();
    //mysql_query = new QSqlQuery(db);

    QString query_str = "SELECT id FROM users WHERE nickname = '" + nickname + "'";
    mysql_query->exec(query_str);
    mysql_query->first();

    int id = 0;
    if (mysql_query->size() == 1)
        id = mysql_query->value(0).toInt();
    else
        id = -1;

    return id;
}


std::string DataBase::findNicknameById(int id)
{
    QString query_str = "SELECT nickname FROM users WHERE id = " + QString::number(id);
    mysql_query->exec(query_str);
    mysql_query->first();

    std::string nick;
    try {
        nick = mysql_query->value(0).toString().toStdString();
    }
    catch (...) { }

    return nick;
}


bool DataBase::addMessageForAllUsers(const Message& message_)
{
    std::map<int, QString> userNames = loadUserNames();
    if (userNames.size() == 0) {
        qDebug() << "No information about registered users in the database";
        return false;
    }
    else {
        for (auto user_ : userNames) {
            Message newMess(message_.getNameFrom(), user_.second.toStdString(), message_.getSendTime(), message_.getText());	//Создать сообщение
            addMessageForUser(newMess);
        }
        return true;
    }
}


bool DataBase::addMessageForUser(const Message& message)
{
    // определим id пользователя
    int id_from = findIdByNickname(QString::fromStdString(message.getNameFrom()));
    int id_to = findIdByNickname(QString::fromStdString(message.getNameTo()));
    if (id_from > 0 && id_to > 0) {
        std::string query = "INSERT INTO messages(id_from, id_to, send_date, text) VALUES(" + std::to_string(id_from) + ", " + std::to_string(id_to) + ", '" + message.getSendTime() + "', '" + message.getText() + "')";
        qDebug() << QString::fromStdString(query);
        mysql_query->exec(QString::fromStdString(query));
        QString err = mysql_error.text();

        if (err.isEmpty()) {
            allMessages += QString::fromStdString(message.getNameFrom()) + " to " + QString::fromStdString(message.getNameTo()) + " [" + QString::fromStdString(message.getSendTime()) + "] :   " + QString::fromStdString(message.getText()) + "\n";
            qDebug() << "OK";
            return true;
        }
        else
            qDebug() << mysql_error.text();
    }
    else
        qDebug() << "Error when entering data";

    return false;
}


bool DataBase::isExistLogin(const QString& login)
{
    mysql_query->exec("SELECT * FROM users WHERE login = '" + login + "'");
    mysql_query->first();

    if (mysql_query->size() == 1)  //~ row_countDB
        return true;
    else
        return false;
}


bool DataBase::isCorrectPassword(const QString& login, const QString& password)
{
    mysql_query->exec("SELECT password FROM users WHERE login = '" + login + "'");
    mysql_query->first();

    QString passInDB = mysql_query->value(0).toString();
    qDebug() << mysql_query->value(0).toString();
    if (passInDB == password)
        return true;
    else
        return false;
}


bool DataBase::isExistName(const QString& nickname)
{
    mysql_query->exec("SELECT * FROM users WHERE nickname = '" + nickname + "'");
    mysql_query->first();

    if (mysql_query->size() == 1)
        return true;
    else
        return false;
}


int DataBase::getNumberUser()
{
    mysql_query->exec("SELECT count(*) FROM users");
    mysql_query->first();

    int countUsers = 0;
    try {
        countUsers = mysql_query->value(0).toInt();
    }
    catch (const char* error) {
        qDebug() << error;
    }

    return countUsers;
}


std::map<int, QString> DataBase::loadUserNames()
{
    mysql_query->exec("SELECT id, nickname FROM users");
    mysql_query->first();

    std::map<int, QString> userNames;
    while (mysql_query->next()) {
        if(mysql_query->record().count() >= 2) {//количествo столбцов
            std::pair<int, QString> new_pair = std::make_pair(mysql_query->value(0).toInt(), mysql_query->value(1).toString());
            userNames.insert(new_pair);
        }
    }
    return userNames;
}


std::vector<Message> DataBase::loadMessages(const User& addressee)
{
    std::vector<Message> destination;
    std::map<int, QString> userNames = loadUserNames();
    int userId = findIdByNickname(QString::fromStdString(addressee.getName()));
    if (userId > 0) {
        mysql_query->exec(QString::fromStdString("SELECT * FROM messages WHERE (messages.id_from = " + std::to_string(userId) + " OR messages.id_to = " + std::to_string(userId) + ") ORDER by messages.send_date asc"));
        mysql_query->first();

        // Выводим все что есть в таблице через цикл
        while (mysql_query->next()) {
            if (mysql_query->record().count() == 5) {
                try {
                    std::string str_from = (userNames.at(mysql_query->value(1).toInt())).toStdString();
                    std::string str_to = (userNames.at(mysql_query->value(2).toInt())).toStdString();
                    Message messageInDB(str_from, str_to, mysql_query->value(3).toString().toStdString(), mysql_query->value(4).toString().toStdString());
                    destination.push_back(messageInDB);
                }
                catch(...) {
                        qDebug() << "Exception!!!";
                }
            }
        }
    }
    else
        qDebug() << "No user with nickname <" + QString::fromStdString(addressee.getName()) + "> in the database";

    return destination;
}


void DataBase::removeMessagesToUser(const QString& nickname)
{
    int idDel = findIdByNickname(nickname);

    QString idDel_str = QString::number(idDel);
    mysql_query->exec("DELETE FROM messages WHERE(id_from = " + idDel_str + " OR id_to = " + idDel_str + ")");
}


void DataBase::removeUser(const User &user)
{
    // Удалить сначала все сообщения данного пользователя
    removeMessagesToUser(QString::fromStdString(user.getName()));

    // Удалить пользователя
    mysql_query->exec("DELETE FROM users WHERE nickname = '" + QString::fromStdString(user.getName()) + "'");

    removeUsers.push_back(user);
    activUsers.clear();
    allActivUsers();
}


std::string DataBase::getNameByLogin(const QString& login)
{
    mysql_query->exec("SELECT nickname FROM users WHERE login = '" + login + "'");
    mysql_query->first();

    std::string nickname;
    if(mysql_query->size() > 0)// найдет nickname по указанному логину в БД
        nickname = mysql_query->value(0).toString().toStdString();

    return nickname;
}


void DataBase::loadAllMessages()
{
    std::map<int, QString> userNames = loadUserNames();

    mysql_query->exec("SELECT * FROM messages");
    mysql_query->first();

    // Выводим все что есть в таблице через цикл
    while (mysql_query->next()) {
        if (mysql_query->record().count() == 5) {
            try {
                QString str_from = userNames.at(mysql_query->value(1).toInt());
                QString str_to = userNames.at(mysql_query->value(2).toInt());
                Message messageInDB(str_from.toStdString(), str_to.toStdString(), mysql_query->value(3).toString().toStdString(), mysql_query->value(4).toString().toStdString());
                allMessages += str_from + " to " + str_to + " [" + mysql_query->value(3).toString() + "] :   " + mysql_query->value(4).toString() + "\n";
            }
            catch(...) {
                    qDebug() << "Exception!!!";
            }
        }
    }
}


void DataBase::allActivUsers()
{
    mysql_query->exec("SELECT * FROM users");
    mysql_query->first();

    while (mysql_query->next()) {
        if(mysql_query->record().count() == 4) {//количествo столбцов
            User new_user = User(mysql_query->value(3).toString().toStdString(), mysql_query->value(1).toString().toStdString(), mysql_query->value(2).toString().toStdString());
            activUsers.push_back(new_user);
        }
    }
}
