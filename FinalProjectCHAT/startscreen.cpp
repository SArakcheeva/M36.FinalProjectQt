#include "startscreen.h"
#include "ui_startscreen.h"
#include "Socket.h"
#include <memory>

StartScreen::StartScreen(std::shared_ptr<Chat> chatPtr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartScreen)
{
    ui->setupUi(this);

    // если нам на вход была подана БД - используем ее
    if(chatPtr) {
        m_chatPtr = chatPtr;
    }
    /*/ если не подавалась - мы создаем новую БД
    else {
        m_chatPtr = std::make_shared<Chat>();
    }*/

    // после создания интерфейса - установили БД
    ui->loginWidget->setChat(m_chatPtr);
    ui->registerWidget->setChat(m_chatPtr);

    connect(ui->loginWidget, &LoginForm::registerRequested, this, &StartScreen::setRegistrationForm);
    connect(ui->loginWidget, &LoginForm::accepted, this, &StartScreen::onLoggedIn);
    connect(ui->loginWidget, &LoginForm::rejected, this, &StartScreen::onRejectRequested);
    connect(ui->registerWidget, &RegistrationForm::loginRequested, this, &StartScreen::setLoginForm);
    connect(ui->registerWidget, &RegistrationForm::accepted, this, &StartScreen::onLoggedIn);
    connect(ui->registerWidget, &RegistrationForm::rejected, this, &StartScreen::onRejectRequested);
}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::setLoginForm()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StartScreen::setRegistrationForm()
{
    ui->stackedWidget->setCurrentIndex(1);
}

std::shared_ptr<Chat> StartScreen::getChat() const
{
    return m_chatPtr;
}

void StartScreen::onLoggedIn()
{
    accept();
}

void StartScreen::onRejectRequested()
{
    // закрываем сокет, завершаем соединение
    std::string request = "end";
    bzero(messageCS, sizeof(messageCS));
    strcpy(messageCS, request.c_str());
    write(socket_file_descriptor, messageCS, sizeof(messageCS));
    close();//socket_file_descriptor);

    reject();
}
