/**
 * @author : Dvir Asaf 313531113.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include "Server.h"

//constructor
Server::Server(int port) throw(const char *) {
    //create a new socket, returns a file descriptor for the new socket, or -1 for errors.
    FD = socket(AF_INET, SOCK_STREAM, 0);
    validateSocket();
    //sin_family = structure describing an Internet socket address.
    server.sin_family = AF_INET;
    //s_addr and sin_addr = Internet address.
    server.sin_addr.s_addr = INADDR_ANY;
    //sin_port = Port number.
    server.sin_port = htons(port);
    //try to bind port to socket.
    bindPort();
}

//check if the socket is valid.
void Server::validateSocket() const {
    if (FD < 0) {
        throw "Error!!! socket is Not valid.";
    }
}

//binding socket to port.
void Server::bindPort() const {
    auto size = sizeof(server);
    //give the socket FD the local address.
    auto link = bind(FD, (struct sockaddr *) &server, size);
    if (link < 0) {
        throw "Error!!! socket in Not bind.";
    }
    //prepare to accept connections on socket FD.
    auto x = listen(FD, 3);
    if (x < 0) {
        throw "Error!!! listen was failed - connection attempt was unsuccessful.";
    }
}

//start the server and wait for client to connect.
void Server::start(ClientHandler &ch) throw(const char *) {
    setConnect(true);
    t = new thread([&ch, this]() {
        while (isConnect) {
            socklen_t clientSize = sizeof(client);
            int clientFD = accept(FD, (struct sockaddr *) &client, &clientSize);
            validateClientFD(clientFD);
            ch.handle(clientFD);
            close(clientFD);
        }
    });
}

//check if the connection between client and server are accepted.
void Server::validateClientFD(int clientFD) {
    if (clientFD < 0) {
        throw "Error!!! connection between client and server was Not accepted.";
    }
}

//set isConnect according to given bool
void Server::setConnect(bool b) {
    isConnect = b; }

//stop the server and wait for thread to finish. close FD.
void Server::stop() {
    setConnect(false);
    t->join(); // do not delete this!
    close(FD);
}
Server::~Server() = default;