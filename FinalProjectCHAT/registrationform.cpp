#include "registrationform.h"
#include "ui_registrationform.h"
#include <QMessageBox>

RegistrationForm::RegistrationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationForm)
{
    ui->setupUi(this);
}

RegistrationForm::~RegistrationForm()
{
    delete ui;
}

void RegistrationForm::setChat(std::shared_ptr<Chat> chatPtr)
{
    m_chatPtr = chatPtr;
}

void RegistrationForm::on_loginButton_clicked()
{
    // отправка сигнала
    emit loginRequested();
}


void RegistrationForm::on_buttonBox_accepted()
{
    if(!m_chatPtr->isCorrectValue(ui->loginEdit->text().toStdString())) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Incorrect login - invalid characters!"));
        return;
    }
    if(!m_chatPtr->isCorrectValue(ui->nameEdit->text().toStdString())) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Incorrect nickname - invalid characters!"));
        return;
    }
    if(!m_chatPtr->isCorrectValue(ui->passwordEdit->text().toStdString())) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Incorrect password - invalid characters!"));
        return;
    }
    //нужно проверить, что пароли совпадают
    if(ui->passwordEdit->text() != ui->passwordConfirmEdit->text()) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Password not match"));
        return;
    }


    //ЗАПРОС СЕРВЕРУ - ЕСТЬ ЛИ ЛОГИН В БД
    auto existLogin_ = m_chatPtr->isExistLogin(ui->loginEdit->text().toStdString());
    if(existLogin_ == 1) {
        QMessageBox::critical(this,
                          tr("Error"),
                          tr("Login already exists!"));
        return;
    }
    else if(existLogin_ == 2)  {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Connection with the server failed!"));
        return;
    }
    loginChat = ui->loginEdit->text().toStdString();

    //ЗАПРОС СЕРВЕРУ - ЕСТЬ ЛИ ИМЯ ПОЛЬЗОВАТЕЛЯ В БД
    auto existName_ = m_chatPtr->isExistName(ui->nameEdit->text().toStdString());
    if(existName_ == 1) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Nickname already exists!"));
        return;
    }
    if(existName_ == 2)  {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Connection with the server failed!"));
        return;
    }
    nicknameChat = ui->nameEdit->text().toStdString();
    passwordChat = ui->passwordEdit->text().toStdString();

    auto s_answer = m_chatPtr->addUser(loginChat, passwordChat, nicknameChat);
    if (s_answer == 0) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Error writing to the DB!"));
        return;
    }
    else if(s_answer == 2)  {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Connection with the server failed!"));
        return;
    }

    QMessageBox::information(this,
                             tr("Informagtion"),
                             tr("You have successfully registered!"));
    emit accepted();
}


void RegistrationForm::on_buttonBox_rejected()
{
    emit rejected();
}
