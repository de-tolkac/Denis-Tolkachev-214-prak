#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime> 
#include <vector>
#include <string.h>
#include <signal.h>

#include "http_server.h"

#define BACK_LOG 5
#define BUF_LEN 1000

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"
#define GREEN   "\033[0;32m"
#define WHITE   "\033[1;37m"

#define CLEARSCREEN  "\033[2J\033[1;1H"


using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::ofstream;
using std::find;
using std::vector;
using std::copy;
using std::ifstream;

char* configureResponse(int code, string msg, bool appendMessageToBody){
    string res = "HTTP/1.1 ";
    res += to_string(code);
    res += " ";
    res += msg;
    res += "\r\nStatus: ";
    res += to_string(code);
    res += " ";
    res += msg;
    if(appendMessageToBody){
        res += "\r\nContent-type: text/html; charset=utf-8\r\n\r\n<html><body><p>";
        res += msg;
        res += "</p></body></html>";
    }else{
        res += "\r\nContent-type: text/html; charset=utf-8\r\n\r\n";
    }
    int n = res.length();
    char* ret = (char*)malloc((n + 1)*sizeof(char));
    strcpy(ret, res.c_str());
    return ret;
}

Server::Server(int portNum, string logFile){
        signal(SIGPIPE, SIG_IGN); //Быстрофикс. TODO - надо разобраться почему прилетает SIGPIPE.  
        log.open(logFile, std::ios_base::app); 
        time_t st_time;
        st_time = time(NULL);
        log << string('-',20) << endl <<  asctime(localtime(&st_time)) << endl;
        port = portNum;
        struct sockaddr_in ServerAddress;
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
		    cout << "Ошибка создания сокета\n";
		    exit(1);
        }
        memset(&ServerAddress, 0, sizeof(ServerAddress));
        ServerAddress.sin_family = AF_INET;
        ServerAddress.sin_port = htons(port);
        ServerAddress.sin_addr.s_addr = INADDR_ANY;
        if(bind(serverSocket, (struct sockaddr*) &ServerAddress, sizeof(ServerAddress)) < 0){
            cout << "Ошибка связывания сокета с адресом\n";
            close(serverSocket);
		    exit(1);
        }
        if(listen ( serverSocket, BACK_LOG ) < 0){
            cout << "Ошибка прослушивания\n";
            close(serverSocket);
		    exit(1);
        }
    }

    /* Метод, который регистрирует новый Get запрос */
    void Server::Get(string path, string response){
        bool pathExists = false;
        for(int i = 0; i < (int)getRequests.size(); i++){
            if(getRequests[i].dist == path){
                pathExists = true;
                break;
            }
        }
        if(!pathExists){
            getHandler newPath = {path, response};
            getRequests.push_back(newPath);
        }
    }

    /* Основной цикл сервера */
    void Server::run(){
        cout << CLEARSCREEN;
        cout << "Сервер запущен на " << GREEN << "127.0.0.1:" << port << RESET << endl;
        for (;;){
            struct sockaddr_in ClientAddress;
            size_t ClAddrLen = sizeof(ClientAddress);
            int clientFD = accept(serverSocket, (struct sockaddr *) &ClientAddress, (socklen_t *) &ClAddrLen);
            int req = recv(clientFD, request, BUF_LEN, 0);
            log << "*****\n" << "Received request from: " << inet_ntoa(ClientAddress.sin_addr) << endl << request; 
            if(req < 0){
                cout << "Ошибка принятия запроса\n";
                close(serverSocket);
                exit(1);
            }
            if (clientFD < 0) {
                cout << "Ошибка принятия запроса\n";
                close(serverSocket);
                exit(1);
            }
            if(!strncmp(request, "GET", 3)){
                int k = 5;
                bool pathFound = false;
                char c = request[k];
                while(c != ' '){
                    k++;
                    c = request[k];
                }
                char path[k-3];
                if(k != 5){
                    copy(&request[4], &request[k], &path[0]);
                    path[k-4] = 0;
                }else{
                    path[0] = '/';
                    path[1] = 0;
                }
                int pathId = 0;
                for(int i = 0; i < (int)getRequests.size(); i++){
                    if(getRequests[i].dist == path){
                        pathFound = true;
                        pathId = i;
                        break;
                    }
                }
                cout << "Received request from: " << inet_ntoa(ClientAddress.sin_addr) << " to " << GREEN << path << RESET << "\n";;
                if(pathFound){
                    string line;
                    ifstream responseFile(getRequests[pathId].responseFile);
                    if (responseFile.is_open()){
                        char* res = configureResponse(200, "OK", false);
                        write(clientFD, res, sizeof(char)*strlen(res));
                        free(res);
                        while (getline(responseFile, line)){
                            write(clientFD, line.c_str(), sizeof(char)*strlen(line.c_str()));
                        }
                        responseFile.close();
                    }else{
                        char* res = configureResponse(500, "Internal Server Error", true);
                        send(clientFD, res, strlen(res), 0);
                        free(res);
                    }
                }else{
                    char* res = configureResponse(404, "Not found!", true);
                    send(clientFD, res, strlen(res), 0);
                    free(res);
                }
            }else{
                char* res = configureResponse(501, "Not Implemented", true);
                send(clientFD, res, strlen(res), 0);
                free(res);
            }
            shutdown(clientFD, SHUT_RDWR);
            close(clientFD);
        }
    }

    /* TODO - разобраться почему не прорабавтывает дестркутор при получении сигнала SIGINT */
    Server::~Server(){
        cout << "Сервер на порту " << port << " остановлен\n"; 
        close(serverSocket);
        log.close();
    }