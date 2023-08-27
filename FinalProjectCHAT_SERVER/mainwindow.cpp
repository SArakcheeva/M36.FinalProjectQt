#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globalparam.h"
#include "networkconfig.h"
#include <sstream>
#include <QDialog>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QTimer>

QTimer* timer;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // PORT number and Data to DB connection from network.config file
    NetworkConfig config;
    PORT = config.getPort();
    ui->port_lineEdit->setText(QString::number(PORT));

    hostDB = config.getHostDB();
    hostDB.erase(hostDB.find(' '), 1);
    ui->host_lineEdit->setText(QString::fromStdString(hostDB));

    userDB = config.getUserDB();
    userDB.erase(userDB.find(' '), 1);
    ui->user_lineEdit->setText(QString::fromStdString(userDB));

    passwdDB = config.getPasswdDB();
    passwdDB.erase(passwdDB.find(' '), 1);
    ui->password_lineEdit->setText(QString::fromStdString(passwdDB));

    nameDB = config.getNameDB();
    nameDB.erase(nameDB.find(' '), 1);
    ui->name_lineEdit->setText(QString::fromStdString(nameDB));
}


MainWindow::~MainWindow()
{
    slotClientDisconnected();
    delete ui;
}


void MainWindow::on_connectPushButton_clicked()
{
    PORT = ui->port_lineEdit->text().toInt();
    hostDB = ui->host_lineEdit->text().toStdString();
    userDB = ui->user_lineEdit->text().toStdString();
    passwdDB = ui->password_lineEdit->text().toStdString();
    nameDB = ui->name_lineEdit->text().toStdString();

    ui->connectionBrowser->append("DATABASE: host [" + QString::fromStdString(hostDB) + "] ; user [" + QString::fromStdString(userDB) + "]; pwd [" + QString::fromStdString(passwdDB) + "]; name [" + QString::fromStdString(nameDB) + "]");

    m_dbPtr = new DataBase();

    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MainWindow::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, PORT)){
        ui->connectionBrowser->append("Server is not started. Try again");
    }
    else {
        ui->connectionBrowser->append("Server is started");

        // таймер, который будет раз  вопределенный период времени обновлять поля сообщений
        timer = new QTimer(this); // укажем пэрента (this), чтобы при удалении его удалили mainWindow
        connect(timer, &QTimer::timeout, this, &MainWindow::updateChat);
        timer->start(10000);// запустим таймер 10 sec
    }
}


void MainWindow::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection();

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MainWindow::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MainWindow::slotClientDisconnected);
}


std::vector<std::string> split(std::string & s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}


void MainWindow::slotServerRead()
{
    while(mTcpSocket->bytesAvailable() > 0)
    {
        QByteArray array = mTcpSocket->readAll();
        std::string messageCS = QString(array).toStdString();

        if (messageCS == "end") {
            ui->connectionBrowser->append("Client Exited\n");
            ui->connectionBrowser->append("Server is Exiting\n");
            break;
        }
        ui->connectionBrowser->append("Data received from client: " + QString(array));

        try {
            std::vector<std::string> vector_messageCS = split(messageCS, '|');
            array.clear();

            if(vector_messageCS.size() < 1) {
                ui->connectionBrowser->append("Incorrect request from Client");
            }
            else {
                int id_cmd = atoi( vector_messageCS[0].c_str() );
                switch(id_cmd)
                {
                    case 0: {/*Проверить есть ли в базе заданный Логин*/
                        bool _isExistLogin = m_dbPtr->isExistLogin(QString::fromStdString(vector_messageCS[1]));//true->ok
                        messageCS = std::to_string(_isExistLogin).c_str();
                        break;
                    }
                    case 1: {/*Проверить соответствует ли Пароль заданному Логину*/
                        bool _isCorrectPassword = m_dbPtr->isCorrectPassword(QString::fromStdString(vector_messageCS[1]), QString::fromStdString(vector_messageCS[2]));
                        qDebug() << QString::fromStdString(messageCS);
                        messageCS = std::to_string(_isCorrectPassword).c_str();
                        break;
                    }
                    case 2: {/*Проверить есть ли в базе заданный Ник*/
                        bool _isExistName = m_dbPtr->isExistName(QString::fromStdString(vector_messageCS[1]));
                        messageCS = std::to_string(_isExistName).c_str();
                        break;
                    }
                    case 3: {/*Добавить в базу заданного пользователя*/
                        User new_user(vector_messageCS[3], vector_messageCS[1], vector_messageCS[2]);
                        bool res3 = m_dbPtr->addUser(new_user);
                        messageCS = std::to_string(res3).c_str();
                        break;
                    }
                    case 4: {/*Удалить заданного пользователя из базы*/
                        User del_user(vector_messageCS[3], vector_messageCS[1], vector_messageCS[2]);
                        m_dbPtr->removeUser(del_user);
                        messageCS = "1";
                        break;
                    }
                    case 5: {/*Количество зарегистрированных пользователей*/
                        int _numberUser = 0;
                        _numberUser = m_dbPtr->getNumberUser();
                        messageCS = std::to_string(_numberUser).c_str();
                        break;
                    }
                    case 6: {/*Загрузить имена зарегистрированных пользователей*/
                        std::map<int, QString> userNames = m_dbPtr->loadUserNames();
                        std::string allUsers;
                        for (auto name : userNames) {
                            allUsers = allUsers + (name.second).toStdString() + "|";
                        }
                        messageCS = allUsers.c_str();
                        break;
                    }
                    case 7: {/*Загрузить сообщения, адресованные заданному пользователю*/
                        User mess_user(vector_messageCS[3], vector_messageCS[1], vector_messageCS[2]);
                        std::vector<Message> messagesToUser = m_dbPtr->loadMessages(mess_user);              //Заполнить вектор - сообщениями пользователю
                        std::string user_messages;
                        if (messagesToUser.empty()) {
                            user_messages = "No messages for you";
                        }
                        else {
                            for (auto message : messagesToUser) {
                                user_messages += message.getNameFrom() + " to " + message.getNameTo() + " [" + message.getSendTime() + "] :  " + message.getText() + "|";
                            }
                        }
                        messageCS = user_messages.c_str();
                        break;
                    }
                    case 8: {/*Добавить в базу сообщение от одного пользователя другому*/
                        Message message(vector_messageCS[1], vector_messageCS[2], vector_messageCS[3], vector_messageCS[4]);	//Создать сообщение
                        bool res8 = false;
                        if(message.getNameTo() == "all")				//Поместить в базу сообщений
                            res8 = m_dbPtr->addMessageForAllUsers(message);
                        else
                            res8 = m_dbPtr->addMessageForUser(message);
                        std::string res8_str = res8 ? "1" : "0";
                        messageCS = res8_str.c_str();
                        break;
                    }
                    case 9: {/*Загрузить из базы Ник пользователя*/
                        const std::string name = m_dbPtr->getNameByLogin(QString::fromStdString(vector_messageCS[1]));
                        messageCS = name.c_str();
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        catch (...) {
            ui->connectionBrowser->append("Exception: incorrect data!");
            messageCS = "0";
        }


        array.append(QString::fromStdString(messageCS).toUtf8());
        // Если передали >= 0  байт, значит пересылка прошла успешно
        if (array.size() >= 0) {
            ui->connectionBrowser->append(QString::fromStdString(messageCS) + " Data successfully sent to the client");
        }
        mTcpSocket->write(array);
    }
}


void MainWindow::slotClientDisconnected()
{
    delete mTcpServer;
}


void MainWindow::updateChat()
{
    ui->userTableWidget->clearContents();    
    ui->userTableWidget->setRowCount(activUsers.size());
    for(size_t i = 0; i < activUsers.size(); i++) {
        ui->userTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(activUsers[i].getName())));
        ui->userTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(activUsers[i].getLogin())));
        ui->userTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(activUsers[i].getPassword())));
    }

    ui->removeUsersTableWidget->clearContents();
    ui->removeUsersTableWidget->setRowCount(removeUsers.size());
    for(size_t i = 0; i < removeUsers.size(); i++) {
        ui->removeUsersTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(removeUsers[i].getName())));
        ui->removeUsersTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(removeUsers[i].getLogin())));
        ui->removeUsersTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(removeUsers[i].getPassword())));
    }

    if(ui->chatBrowser->toPlainText() != allMessages)
        ui->chatBrowser->setText(allMessages);
}


// delete user
void MainWindow::on_userTableWidget_cellDoubleClicked(int row, int column)
{
    std::string nickname = ui->userTableWidget->item(row, 0)->text().toStdString();
    std::string login = ui->userTableWidget->item(row, 1)->text().toStdString();
    std::string password = ui->userTableWidget->item(row, 2)->text().toStdString();
    if(nickname.length() == 0 || login.length() == 0 || password.length() == 0)
        return;


    QMessageBox msgBox;
    msgBox.setText("Delete user");
    msgBox.setInformativeText("User < " + ui->userTableWidget->item(row, 0)->text() + " > will be deleted.\nAre you sure?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if (msgBox.exec() == QMessageBox::Cancel)
        return;

    User deleteUser;
    deleteUser.setName(nickname);
    deleteUser.setLogin(login);
    deleteUser.setPassword(password);

    m_dbPtr->removeUser(deleteUser);
    QMessageBox::information(this,
                             tr("Attention"),
                             tr("Your account has been deleted"));
}
