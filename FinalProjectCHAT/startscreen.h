#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QDialog>
#include <memory>
#include "Chat.h"

namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartScreen(std::shared_ptr<Chat> chatPtr = nullptr, QWidget *parent = nullptr);
    ~StartScreen();
    void setLoginForm();
    void setRegistrationForm();

    std::shared_ptr<Chat> getChat() const;

public slots:
    void onLoggedIn();
    void onRejectRequested();

private:
    Ui::StartScreen *ui;
    std::shared_ptr<Chat> m_chatPtr;
};

#endif // STARTSCREEN_H
