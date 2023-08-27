#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "Chat.h"
#include "Socket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<Chat> chatPtr = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* createClient();
    static int kInstanceCount;

private slots:
    void on_messageLineEdit_returnPressed();
    void on_sendMessageButton_clicked();
    void on_actionDeleteAccount_triggered();
    void on_actionCloseUser_triggered();
    void updateChats();// update all messages by timer
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Chat> m_chatPtr;
};
#endif // MAINWINDOW_H
