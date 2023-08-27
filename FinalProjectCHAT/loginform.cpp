#include "loginform.h"
#include "ui_loginform.h"
#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::setChat(std::shared_ptr<Chat> chatPtr)
{
    m_chatPtr = chatPtr;
}

void LoginForm::on_buttonBox_accepted()
{
    if(!m_chatPtr->isCorrectValue(ui->loginEdit->text().toStdString())) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Login contains invalid characters!"));
        return;
    }

    if(!m_chatPtr->isCorrectValue(ui->passwordEdit->text().toStdString())) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Password contains invalid characters!"));
        return;
    }

    //ЗАПРОС СЕРВЕРУ - ЕСТЬ ЛИ ЛОГИН В БД
    auto existLogin_ = m_chatPtr->isExistLogin(ui->loginEdit->text().toStdString());
    if(existLogin_ == 0) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Login not found!"));
        return;
    }
    if(existLogin_ == 2)  {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Connection with the server failed!"));
        return;
    }
    loginChat = ui->loginEdit->text().toStdString();

    //ЗАПРОС СЕРВЕРУ - ПАРОЛЬ ПРАВИЛЬНЫЙ
    auto existPassword_ = m_chatPtr->isCorrectPassword(ui->loginEdit->text().toStdString(), ui->passwordEdit->text().toStdString());
    if(existPassword_ == 0) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Password is incorrect!"));
        return;
    }
    else if(existPassword_ == 2)  {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Connection with the server failed!"));
        return;
    }
    passwordChat = ui->passwordEdit->text().toStdString();

    //Загрузить из базы и задать Ник текущего пользователя
    auto name = m_chatPtr->getNameByLogin(ui->loginEdit->text().toStdString());
    if(name.length() == 0) {
         QMessageBox::critical(this,
                               tr("Error"),
                               tr("Incorrect data from server!"));
         return;
    }
    nicknameChat = name;

    emit accepted();
}


void LoginForm::on_buttonBox_rejected()
{
    emit rejected();
}


void LoginForm::on_registrationPushButton_clicked()
{
    // сигнал будет отправляться при нажатии на соответствующую кнопку
    emit registerRequested();
}
