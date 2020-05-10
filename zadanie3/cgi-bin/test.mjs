#! /Users/DEN/dev/c++/git/Denis-Tolkachev-214-prak/zadanie3/cgi-bin/lex
var i;
var header = "Тестовая страница на MJS";
var s = 0;
for(i = 1; i <= 25; i = i + 1){
    s = s + i;
}
write("<html>");
write("<body>");
write("<h1>" + header + "</h1>");
write("<p>Параметры запроса: " + getenv("QUERY_STRING")  + "</p>");
write("<p>Сумма чисел от 1 до 25: " + s + "</p>");
write("</body>");
write("</html>");