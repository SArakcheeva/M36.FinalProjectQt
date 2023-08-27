#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startscreen.h"
#include <QDialog>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include <QCloseEvent>

QTimer* timer;
int MainWindow::kInstanceCount = 0;

MainWindow::MainWindow(std::shared_ptr<Chat> chatPtr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kInstanceCount++;

    m_chatPtr = chatPtr;
    this->setWindowTitle(QString::fromStdString(nicknameChat + ", welcome to CHAT!"));

/*
    QPixmap pixmap(":/image/message_icon.qrc");
    QIcon ButtonIcon(pixmap);
    ui->sendMessageButton->setIcon(ButtonIcon);
    ui->sendMessageButton->setIconSize(pixmap.rect().size());
    ui->sendMessageButton->setStyleSheet("qproperty-icon: url(:/image/message_icon.qrc);");
*/

    ui->sendMessageButton->setToolTip("Send message");

    updateChats();

    // таймер, который будет раз  вопределенный период времени обновлять поля сообщений
    timer = new QTimer(this); // укажем пэрента (this), чтобы при удалении его удалили mainWindow
    connect(timer, &QTimer::timeout, this, &MainWindow::updateChats);
    timer->start(30000);// запустим таймер 10 msec
}


MainWindow::~MainWindow()
{
    delete ui;
    kInstanceCount--;
    if(kInstanceCount <= 0) {
        qApp->exit(0);
    }
}


MainWindow *MainWindow::createClient()
{
    StartScreen s;
    auto result = s.exec(); // exec возвращает int
    if (result == QDialog::Rejected) {
        return nullptr;
    }

    auto w = new MainWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);// удалит наш виджет, если мы его закроем
    return w;
}


void MainWindow::on_messageLineEdit_returnPressed()
{
    on_sendMessageButton_clicked();
}


void MainWindow::on_sendMessageButton_clicked()
{
    // создадим диалог(форму), где открывается список доступных адресатов
    // диалог удалится при выходе их области видимости
    QDialog dial(this);
    dial.setModal(true);// это значит, что наш диалог прервет все выполнение программы и не даст выполнять никакие действия, пока данный диалог не будет закрыт

    auto l = new QVBoxLayout();
    dial.setLayout(l);
    auto userListWgt = new QListWidget(&dial);
    l->addWidget(userListWgt);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dial);
    l->addWidget(buttonBox);

    // создадим коннекты
    connect(buttonBox, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dial, &QDialog::reject);

    // заполнили список имеющимися юзерами
    userListWgt->addItem(QString::fromStdString("all"));
    auto userList = m_chatPtr->getUserList();
    for(auto user : userList)
        userListWgt->addItem(QString::fromStdString(user));

    // условие - в чате есть зарагистрированные пользователи, поэтому можно написать
    userListWgt->setCurrentRow(0);// по идее, у нас всегда будет выбран один элемент

    auto result = dial.exec();
    if(result == QDialog::Accepted && userListWgt->currentItem())
    {
        //Текст введён
        if (!ui->messageLineEdit->text().toStdString().empty())
        {
            std::time_t t = std::time(nullptr);
            std::tm* now = std::localtime(&t);
            char time_buffer[128];
            strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", now);
            bool server_answer = m_chatPtr->sendMessage(nicknameChat,
                                                        userListWgt->currentItem()->text().toStdString(),
                                                        time_buffer,
                                                        ui->messageLineEdit->text().toStdString());
            if (server_answer == 1)
                ui->statusbar->showMessage("The message has been sent!", 10000);

            updateChats();
            ui->messageLineEdit->clear();
        }

        else //Текст не введён
            ui->statusbar->showMessage("The message has not been sent (text is missing)", 10000);
    }
}

void MainWindow::on_actionDeleteAccount_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Warning");
    msgBox.setInformativeText("Your account will be deleted!\nAre you sure you want to do this?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int res = msgBox.exec();
    if (res == QMessageBox::Cancel)
        return;

    timer->stop();
    auto isRemove = m_chatPtr->removeAccount(loginChat, passwordChat, nicknameChat);
    if(isRemove == 2)  {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Connection with the server failed!"));
        return;
    }
    else if (isRemove == 1) {
        loginChat = "";
        passwordChat = "";
        nicknameChat = "";

        QMessageBox::information(this,
                              tr("Attention"),
                              tr("Your account has been deleted"));

        on_actionCloseUser_triggered();
     }
     else {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Error deleting from the DB. Your account hasn't been deleted"));
        timer->start();
     }
}


void MainWindow::on_actionCloseUser_triggered()
{
    loginChat = "";
    passwordChat = "";
    nicknameChat = "";

    timer->stop();
    this->close();

    auto w = createClient();
    if(w) // чтобы не было исключений, т.к. может вернуть нулевой указатель
        w->show();
}


void MainWindow::updateChats()
{
    // сначала получим все сообщения
    auto chatMessages = m_chatPtr->getChatMessagesToUser(loginChat, passwordChat, nicknameChat);
    if (chatMessages.empty())
        ui->statusbar->showMessage("No messages for you", 10000);
    else
    {
        // "засунем" в эту переменную все сообщения с правильным форматированием
        // (добавим перенос строки после каждого сообщения - "\n")
        QString chat;
        for (const auto &msg : chatMessages) {
            if(msg.length() > 0)
                chat.append(QString::fromStdString(msg) + "\n");
        }

        if(ui->chatBrowser->toPlainText() != chat)
            ui->chatBrowser->setText(chat);
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
//    if(continueChat != true) {
//        //закрываем сокет, завершаем соединение
//        const std::string request = "end";
//        bzero(messageCS, sizeof(messageCS));
//        strcpy(messageCS, request.c_str());
//        write(socket_file_descriptor, messageCS, sizeof(messageCS));
//        close();//socket_file_descriptor);
//    }
}
