/**
 * @author : Dvir Asaf 313531113.
 */
#ifndef SERVER_H_
#define SERVER_H_
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <thread>
#include <cstring>
#include "commands.h"
#include "CLI.h"
using namespace std;
// edit your ClientHandler interface here:
class ClientHandler
{
public:
    virtual void handle(int clientID) = 0;
};

//class socket IO (I-input, O-output) is for read from socket and write into socket.
class socketIO : public DefaultIO
{
    int socketFileDescriptor;
public:
    //constructor
    explicit socketIO(int ID)
    {
        this->socketFileDescriptor = ID;
    }
    string read() override
    {
        string inputFromServer;
        return getServerInput(inputFromServer);
    }
    void read(float *file) override
    {
        int sizeFile = sizeof(*file);
        int localFD = socketFileDescriptor;
        //read N bytes into BUF from socket FD. Returns the number read or -1 for errors.
        recv(localFD, file, sizeFile, 0);
    }
    void write(string string1) override
    {
        int sizeString1 = string1.size();
        //cStr is char pointer
        const char *cStr = string1.c_str();
        int localFD = socketFileDescriptor;
        //send N bytes of BUF to socket FD.  Returns the number sent or -1.
        send(localFD, cStr, sizeString1, 0);
    }
    void write(float file) override
    {
        int sizeFile = sizeof(file);
        char buf[sizeFile + 1];
        int sizeBuf = sizeof(buf);
        int localFD = socketFileDescriptor;
        //write formatted output to buf
        sprintf(buf, "%f", file);
        //send N bytes of BUF to socket FD.  Returns the number sent or -1.
        send(localFD, buf, sizeBuf, 0);
    }
    string &getServerInput(string &inputFromServer) const
    {
        char buf = 0;
        int size = sizeof(char);
        int localFD = socketFileDescriptor;
        //read N bytes into BUF from socket FD. Returns the number read or -1 for errors.
        recv(localFD, &buf, size, 0);
        while (buf != '\n')
        {
            inputFromServer = inputFromServer + buf;
            recv(localFD, &buf, size, 0);
        }
        return inputFromServer;
    }
};

// AnomalyDetectionHandler class - handle Client
class AnomalyDetectionHandler : public ClientHandler
{
public:
    virtual void handle(int clientID)
    {
        socketIO IO(clientID);
        CLI cli(&IO);
        cli.start();
    }
};
// implement on Server.cpp
class Server
{
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