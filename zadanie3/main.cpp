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
using std::ofstream;
using std::find;
using std::vector;
using std::copy;
using std::ifstream;

struct getHandler{
    string dist;
    string responseFile;
};

class Server{
    char request[BUF_LEN];
    int serverSocket;
    int port;
    ofstream log;
    vector<getHandler> getRequests;
public:
    Server(int portNum = 3000, string logFile = "log.txt"){
        signal(SIGPIPE, SIG_IGN);
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
    void Get(string path, string response){
        bool pathExists = false;
        for(int i = 0; i < getRequests.size(); i++){
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
    void run(){
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
                for(int i = 0; i < getRequests.size(); i++){
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
                        char res[] = "HTTP/1.1 200 OK\nVersion: HTTP/1.1\nContent-type: text/html;\n\n";
                        write(clientFD, res, sizeof(char)*strlen(res));
                        while (getline(responseFile, line)){
                            write(clientFD, line.c_str(), sizeof(char)*strlen(line.c_str()));
                        }
                        responseFile.close();
                    }else{
                        char res[] = "HTTP/1.1 500 Internal Server Error\nVersion: HTTP/1.1\nContent-type: text/html; charset=utf-8\n\n<html><body><p>Внутренняя ошибка сервера</p></body></html>";
                        send(clientFD, res, strlen(res), 0);
                    }
                }else{
                    char res[] = "HTTP/1.1 404 OK\nVersion: HTTP/1.1\nContent-type: text/html; charset=utf-8\n\n<html><body><p>Страница, которую вы запрашиваете, не существует!</p></body></html>";
                    send(clientFD, res, strlen(res), 0);
                }
            }else{
                char res[] = "HTTP/1.1 501 Not Implemented\nVersion: HTTP/1.1\nContent-type: text/html; charset=utf-8\n\n<html><body><p>Method is not Implemented yet!</p></body></html>";
                send(clientFD, res, strlen(res), 0);
            }
            shutdown(clientFD, SHUT_RDWR);
            close(clientFD);
        }
    }
    ~Server(){
        cout << "Сервер на порту " << port << " остановлен\n"; 
        close(serverSocket);
        log.close();
    }
};
int main(){
    Server app(8081, "logFile.txt");
    app.Get("/", "test1.html");
    app.Get("/hello", "test2.html"); //файла не существует, получаем ответ 500
    app.run();
    return 0;
}