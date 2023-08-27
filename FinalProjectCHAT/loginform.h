#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <memory>
#include "Chat.h"

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

    // создадим функцию, чтобы можно было передать указатель в форму
    // (в сам конструктор передать указатель не можем, т.к. он вызывается не нами)
    void setChat(std::shared_ptr<Chat> chatPtr);

signals:
    void registerRequested();//for open RegistrationForm
    void accepted();
    void rejected();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_registrationPushButton_clicked();

private:
    Ui::LoginForm *ui;
    std::shared_ptr<Chat> m_chatPtr;
};

#endif // LOGINFORM_H
