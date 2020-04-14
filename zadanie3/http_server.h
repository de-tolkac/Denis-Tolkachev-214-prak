#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <fstream>
#include <unistd.h>
#include <vector>
#include <string.h>

#define BUF_LEN 1000

using std::string;
using std::ofstream;
using std::vector;

enum requestType {static_request, CGI_request};

struct getHandler{
    string dist;
    string responseFile;
    requestType type;
};

class Server{
    char request[BUF_LEN];
    int serverSocket;
    int port;
    ofstream log;
    vector<getHandler> getRequests;
public:
    Server(int portNum = 3000, string logFile = "log.txt");
    void Get(string path, string response, requestType type);
    void run();
    ~Server();
};


#endif