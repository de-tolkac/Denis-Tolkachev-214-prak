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
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

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

struct requestProcess{
    int fd;
    int pid;
};

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

void printError(string err, int clientResponseCode){
    cout << "Произошла " << RED << "ошибка: " << RESET << err << " [Ответ: " << YELLOW << clientResponseCode << RESET << "]\n"; 
}

Server::Server(int portNum, string logFile){
        signal(SIGPIPE, SIG_IGN); //Быстрофикс. TODO - надо разобраться почему прилетает SIGPIPE.  
        log.open(logFile, std::ios_base::out | std::ios_base::app); 
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

    /* Регистрируем дирректорию с CGI-программами */
    void Server::use(string dirName){
        IS_CGI_SET = true;
        cgiDir = dirName;
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
            if(clientFD < 0) {
                cout << "Ошибка принятия запроса\n";
                close(serverSocket);
                exit(1);
            }
            int pid = fork();
            if(pid == 0){
                /* Потомок, занимающийся конкретным запросом */
                if(!strncmp(request, "GET", 3)){
                    int k = 5;
                    bool pathFound = false;
                    bool reqWithParams = false;
                    char* params;
                    char c = request[k];
                    while((c != ' ') && (c != '?')){
                        k++;
                        c = request[k];
                    }
                    if(c == '?'){
                        reqWithParams = true;
                        int n = k;
                        while(c != ' '){
                            n++;
                            c = request[n];
                        }
                        params = (char*)malloc(sizeof(char) * (n - k + 2));
                        copy(&request[k + 1], &request[n], &params[0]);
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
                        /* Обрабатываем обычный запрос статической страницы  */
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
                            string err = "Ошибка открытия файла " + getRequests[pathId].responseFile;
                            printError(err, 500);
                            char* res = configureResponse(500, "Internal Server Error 1", true);
                            send(clientFD, res, strlen(res), 0);
                            free(res);
                        }
                    }else{
                        chdir("./cgi-bin");
                        string f = path;
                        f = f.substr(1);
                        ifstream cgi_file(f);
                        if(cgi_file.good()){
                            int status;
                            string name = to_string(getpid()) + ".txt";
                            int fd = open(name.c_str(), O_WRONLY|O_CREAT|O_TRUNC, 0644);
                            dup2(fd, 1);
                            if(!fork()){
                                chdir("./cgi-bin");
                                string processName = path;
                                processName.erase(0, 1);
                                char *argv[] = { (char*)processName.c_str(), NULL };
                                string QUERY_STRING = "QUERY_STRING=";
                                if(reqWithParams){
                                    QUERY_STRING +=  params;
                                }
                                char *env[] = {(char*)QUERY_STRING.c_str(), NULL};
                                execve(processName.c_str(), argv, env);
                                perror("execve() ");
                                exit(1);
                            }
                            close(fd);
                            wait(&status);
                            if(WIFEXITED(status)){
                                if(WEXITSTATUS(status) == 0){
                                    string line;
                                    ifstream responseFile(name);
                                    if (responseFile.is_open()){
                                        char* res = configureResponse(200, "OK", false);
                                        write(clientFD, res, sizeof(char)*strlen(res));
                                        free(res);
                                        while (getline(responseFile, line)){
                                            write(clientFD, line.c_str(), sizeof(char)*strlen(line.c_str()));
                                        }
                                        responseFile.close();
                                    }else{
                                        string err = "Ошибка открытия файла " + name;
                                        printError(err, 500);
                                        char* res = configureResponse(500, "Internal Server Error 2", true);
                                        send(clientFD, res, strlen(res), 0);
                                        free(res);
                                    }
                                }else{
                                    string err = "CGI процесс завершился со статусом " + to_string(WEXITSTATUS(status));
                                    printError(err, 500);
                                    char* res = configureResponse(500, "Internal Server Error 3", true);
                                    send(clientFD, res, strlen(res), 0);
                                }
                            }else if(WIFSIGNALED(status)){
                                string err = "CGI процесс прислал сигнал " + to_string(WIFSIGNALED(status));
                                printError(err, 500);
                                char* res = configureResponse(500, "Internal Server Error 4", true);
                                send(clientFD, res, strlen(res), 0);
                            }
                            remove(name.c_str());
                        }else{
                            char* res = configureResponse(404, "Not found!", true);
                            send(clientFD, res, strlen(res), 0);
                            free(res);
                        }
                        
                    }
                    if(reqWithParams){
                        free(params);
                    }
                }else{
                    char* res = configureResponse(501, "Not Implemented", true);
                    send(clientFD, res, strlen(res), 0);
                    free(res);
                }
                shutdown(clientFD, SHUT_RDWR);
                close(clientFD);
                close(serverSocket);
                log.close();
                exit(0);
            }else if(pid > 0){
                close(clientFD);
                int endPID, status;
                while((endPID = waitpid(0, &status, WNOHANG)) > 0);
            }else{
                /* Паника на борту! */
                cout << "Произошла критическая " << RED << "ошибка" << RESET << ". Выходим...\n";
                exit(1);
            }
        }
    }

    /* TODO - разобраться почему не прорабатывает дестркутор при получении сигнала SIGINT */
    Server::~Server(){
        cout << "Сервер на порту " << port << " остановлен\n"; 
        close(serverSocket);
        log.close();
    }