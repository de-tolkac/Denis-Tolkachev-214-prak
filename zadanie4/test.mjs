#! /bin/sh
var i;
var k = 25;
var showText = true;
var header = "Тестовая страница на MJS";
var s = 0;
for(i = 1; i <= k; i++){
    s = s + i;
}

write("<html>");
write("<body>");
write("<h1>" + header + "</h1>");
write("<p>Параметры запроса: " + getenv("LANG")  + "</p>");
write("<p>Сумма чисел от 1 до " + k + ": " + s + "</p>");
if(s > 320){
    write("<p>Больше 320</p>");
}else{
    write("<p>Меньше 320</p>");
}
write("</body>");
write("</html>");