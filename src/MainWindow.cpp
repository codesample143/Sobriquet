#include "MainWindow.hpp"
#include "ui_MainWindow.h"
//#include "Client.hpp"
//#include "Server.hpp"

#include <QWebSocket>
#include <QWebSocketServer>
#include <QVBoxLayout>
#include <QWidget>
#include <QKeyEvent>
#include <QJsonObject>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent){
    ui = new Ui::MainWindow;
    socket = new QWebSocket();
    //server = new QWebSocketServer(QStringLiteral("Chat Server"), QWebSocketServer::NonSecureMode, this);

    ui->setupUi(this);                        

    connect(ui->joinButton, &QPushButton::clicked, this, &MainWindow::joinRoom);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
    connect(socket, &QWebSocket::disconnected, this, &MainWindow::onDisconnected);

    //connect(server, &QTcpServer::listen, this, &MainWindow::onConnected); //change onConnected

    //socket->connectToHost("96.35.34.172", 8080);
    //server->listen(QHostAddress::LocalHost,8080);
    //if(socket->waitForConnected()){
        //onConnected();
    //}

}

MainWindow::~MainWindow(){
    socket->close();
};

void MainWindow::joinRoom(){
    connect(socket, &QWebSocket::connected, this, &MainWindow::serverResponseRecieved);
    connect(socket, &QWebSocket::binaryMessageReceived, this, &MainWindow::serverResponseRecieved); //not gonna work right now because server is not equipped to handle responding with somethign
    connect(socket, &QWebSocket::textMessageReceived, this, &MainWindow::readSocket);

    socket->open(QUrl(QStringLiteral("wss://basicgoserver.onrender.com/ws")));
    ui->statusLabel->setText("Attempting Connection...");    
    //socket->sendBinaryMessage(a); incomplete logic, it's gonna send a certain response to the server which will parse the bytes, then translate into creation of a distributed node systm
}


void MainWindow::readSocket(const QString& message){
    ui->chatDisplay->append("Anonymous: " + message);
}

void MainWindow::onConnected() {
    ui->statusLabel->setText("Connected!");
    QByteArray qb; 
    qb.append(ui->roomInput->text().toUtf8()); //payload of roomnumber input, more to come in the future, add salt logic later
    socket->sendBinaryMessage(qb); //error check later on the numebr of bytes
}

void MainWindow::onDisconnected(){
    ui->statusLabel->setText("Disconnected");
}

void MainWindow::sendMessage() {
    QString message = ui->messageInput->text();
    if (message.isEmpty()) return;

    socket->sendTextMessage(message);
    ui->chatDisplay->append("You: " + message);
    ui->messageInput->clear();
}

void MainWindow::hostRoom(){ //for the future
    ui->statusLabel->setText("Hosting!");
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
        ui->sendButton->click();
    }
}


void MainWindow::serverResponseRecieved(const QByteArray& data){

}