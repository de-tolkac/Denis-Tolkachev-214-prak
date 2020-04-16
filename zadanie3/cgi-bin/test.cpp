#include <iostream>
using namespace std;
int main (){
    cout << "<html><body><h1>Тестовая страница для моего веб-сервера</h1><br><p>Параметры запроса: " << getenv("QUERY_STRING") << "</p></body></html>";
    return 0; 
}