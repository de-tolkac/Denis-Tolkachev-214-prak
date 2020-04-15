#include "http_server.h"

int main(){
    Server app(3000, "logFile.txt");
    app.Get("/", "test1.html");
    app.Get("/hello", "test2.html"); //файла не существует, получаем ответ 500
    app.use("cgi-bin"); //Дирректория с CGI-программами
    app.run();
    return 0;
}