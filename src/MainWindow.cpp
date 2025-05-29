#include "MainWindow.hpp"
#include "ui_MainWindow.h"
//#include "Client.hpp"
//#include "Server.hpp"

#include <QWebSocket>
#include <QWebSocketServer>
#include <QVBoxLayout>
#include <QWidget>
#include <QKeyEvent>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent){
    ui = new Ui::MainWindow;
    socket = new QWebSocket();
    //server = new QWebSocketServer(QStringLiteral("Chat Server"), QWebSocketServer::NonSecureMode, this);

    ui->setupUi(this);
    socket->open(QUrl(QStringLiteral("wss://basicgoserver.onrender.com/ws")));

    connect(socket, &QWebSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QWebSocket::textMessageReceived, this, &MainWindow::readSocket);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);

    //connect(server, &QTcpServer::listen, this, &MainWindow::onConnected); //change onConnected

    //socket->connectToHost("96.35.34.172", 8080);
    //server->listen(QHostAddress::LocalHost,8080);
    //if(socket->waitForConnected()){
        //onConnected();
    //}
}

MainWindow::~MainWindow(){
    
};


void MainWindow::readSocket(const QString& message){
    ui->chatDisplay->append("Anonymous: " + message);
}

void MainWindow::onConnected() {
    ui->statusLabel->setText("Connected!");
}

void MainWindow::sendMessage() {
    QString message = ui->messageInput->text();
    if (message.isEmpty()) return;

    socket->sendTextMessage(message);
    ui->chatDisplay->append("You: " + message);
    ui->messageInput->clear();
}

void MainWindow::hostRoom(){
    ui->statusLabel->setText("Hosting!");
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
        ui->sendButton->click();
    }
}




