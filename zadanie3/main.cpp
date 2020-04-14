#include "http_server.h"

int main(){
    Server app(3000, "logFile.txt");
    app.Get("/", "test1.html", static_request);
    app.Get("/test", "test.cgi", CGI_request); //Вывод через CGI-программу
    app.Get("/hello", "test2.html", static_request); //файла не существует, получаем ответ 500
    app.run();
    return 0;
}