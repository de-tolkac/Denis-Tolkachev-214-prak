#! /bin/sh
var i;
var showText = true;
var header = "Тестовая страница на MJS";
var s = 0;
for(i = 1; i <= 25; i = i + 1){
    s = s + i;
}
write("<html>");
write("<body>");
write("<h1>" + header + "</h1>");
write("<p>Параметры запроса: " + getenv("LANG")  + "</p>");
write("<p>Сумма чисел от 1 до 25: " + s + "</p>");
if(showText){
    write("<p>Текст показываем</p>");
}else{
    write("<p>Текст не показываем</p>");
}
write("</body>");
write("</html>");