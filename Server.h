/**
 * @author : Dvir Asaf 313531113.
 */
#ifndef SERVER_H_
#define SERVER_H_
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include "commands.h"
#include "CLI.h"
using namespace std;
// edit your ClientHandler interface here:
class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};

//class socket IO (I-input, O-output) is for read from socket and write into socket.
class socketIO : public DefaultIO {
    int socketFD;
public:
    //constructor
    explicit socketIO(int ID) {
        this->socketFD = ID;
    }
    string read() override {
        string serverInput;
        return getServerInput(serverInput);
    }
    void read(float *f) override {
        //read N bytes into BUF from socket FD. Returns the number read or -1 for errors.
        recv(socketFD, f, sizeof(*f), 0);
    }
    void write(string s) override {
        //cStr = char pointer
        const char *cStr = s.c_str();
        //send N bytes of BUF to socket FD.  Returns the number sent or -1.
        send(socketFD, cStr, s.size(), 0);
    }
    void write(float f) override {
        char buf[sizeof(f) + 1];
        //Write formatted output to socket
        sprintf(buf, "%f", f);
        //send N bytes of BUF to socket FD.  Returns the number sent or -1.
        send(socketFD, buf, sizeof(buf), 0);
    }
    string &getServerInput(string &serverInput) const {
        char buf = 0;
        //read N bytes into BUF from socket FD. Returns the number read or -1 for errors.
        recv(socketFD, &buf, sizeof(char), 0);
        while (buf != '\n') {
            serverInput += buf;
            recv(socketFD, &buf, sizeof(char), 0);
        }
        return serverInput;
    }
};

// AnomalyDetectionHandler class - handle Client
class AnomalyDetectionHandler : public ClientHandler {
public:
    virtual void handle(int clientID) {
        socketIO IO(clientID);
        CLI cli(&IO);
        cli.start();
    }
};
// implement on Server.cpp
class Server {
    thread *t{}; // the thread to run the start() method in
public:
    //isConnect = bool for know if their connection between client and server
    bool isConnect{};
    int FD;
    //side server
    struct sockaddr_in server{};
    //side client
    struct sockaddr_in client{};
    Server(int port) throw(const char *);
    virtual ~Server();
    void start(ClientHandler &ch) throw(const char *);
    void stop();
    void bindPort() const;
    void setConnect(bool b);
    void validateSocket() const;
    static void validateClientFD(int clientFD) ;
};
#endif /* SERVER_H_ */