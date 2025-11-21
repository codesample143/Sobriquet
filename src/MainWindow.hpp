#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QString>
#include <QWebSocket>
#include <QWebSocketServer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
    signals:
        void newMessage(QString);
    private slots:
        void readSocket(const QString& message);
        void onConnected();
        void onDisconnected();
        void sendMessage();
        void hostRoom();
        void joinRoom();

        void serverResponseRecieved(const QByteArray& data);
        //void disconnectSocket();
        //void displayMessage(const QString& str);

    private:
        QWebSocket* socket;
        QWebSocketServer* server;
        Ui::MainWindow *ui;
    protected:
        void keyPressEvent(QKeyEvent* event) override;

        //add server here later
};


#endif