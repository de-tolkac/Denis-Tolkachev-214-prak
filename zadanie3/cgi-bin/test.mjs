#! /Users/DEN/dev/c++/git/Denis-Tolkachev-214-prak/zadanie3/cgi-bin/lex
var i;
var k = 15;
var showText = true;
var header = "Тестовая страница на MJS";
var s = 0;
for(i = 1; i <= k; i++){
    s = s + i;
}
write("<html>");
write("<body>");
write("<h1>" + header + "</h1>");
write("<p>Параметры запроса: " + getenv("QUERY_STRING")  + "</p>");
write("<p>Сумма чисел от 1 до " + k + ": " + s + "</p>");
if(s > 320){
    write("<p>Больше 320</p>");
}else{
    write("<p>Меньше 320</p>");
}
write("</body>");
write("</html>");