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
    /*
        Instantiates the basic window which has the following functions
            1. Option to connect to a room. This will send binary encrypted data to the server and the server will provide that room for access. 
            This also means the scope of the socket connection may change. 
            2. Option to host a room. This will turn the user into a server, and pass information to the server about the room number. 
            The server then 'matches' any valid hashed room number to any equivalent user, routing them to your 'server'.  
            Ideally, this will eventually all be done automatically without the need for user manual input. 
            This also has the added benefit of cutting the cloud server out of the equation entirely, for additional security. 
    */
    ui = new Ui::MainWindow; 
    ui->setupUi(this);                
    socket = new QWebSocket();

    connect(ui->joinButton, &QPushButton::clicked, this, &MainWindow::joinRoom);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);

    connect(socket, &QWebSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QWebSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(socket, &QWebSocket::textMessageReceived, this, &MainWindow::readSocket);
    connect(socket, &QWebSocket::binaryMessageReceived, this, &MainWindow::serverResponseRecieved);
}

MainWindow::~MainWindow(){
    socket->close();
    delete ui;
};

void MainWindow::joinRoom(){
    /*
        1. Connect -> Function serverResponseRecieved from socket, will run function upon connection to server. 

        2. Connect -> Function server 

        3. Connect -> Function readSocket
            Upon server connection, we read and display text messages. These messages are going to be sent unencrypted, but eventually we will implement
            decryption protocols client side using a key exchange algorithm. 
    */
    socket->open(QUrl(QStringLiteral("wss://basicgoserver.onrender.com/ws")));
    ui->statusLabel->setText("Attempting Connection...");    
}


void MainWindow::readSocket(const QString& message){
    /*
        1. Simply append to display the 'plaintext' message recieved through socket. 
        There may exist data inconsistencies regarding how to handle this display, and beta users have reported bugs in rare instances
        where the length of the message is at an exact value. 
    */
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
    Q_UNUSED(data);
}