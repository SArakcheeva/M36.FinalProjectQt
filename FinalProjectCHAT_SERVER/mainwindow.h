#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <database.h>
#include <globalparam.h>
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectPushButton_clicked();
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();
    void updateChat();
    void on_userTableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    QTcpServer* mTcpServer;
    QTcpSocket* mTcpSocket;
    DataBase* m_dbPtr;
};
#endif // MAINWINDOW_H
