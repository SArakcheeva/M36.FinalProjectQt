#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include <QWidget>
#include <memory>
#include "Chat.h"

namespace Ui {
class RegistrationForm;
}

class RegistrationForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationForm(QWidget *parent = nullptr);
    ~RegistrationForm();

    // создадим функцию, чтобы можно было передать указатель в форму
    // (в сам конструктор передать указатель не можем, т.к. он вызывается не нами)
    void setChat(std::shared_ptr<Chat> chatPtr);

signals:
    void loginRequested();//for open LoginForm
    void accepted();
    void rejected();

private slots:
    void on_loginButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::RegistrationForm *ui;
    std::shared_ptr<Chat> m_chatPtr;
};

#endif // REGISTRATIONFORM_H
