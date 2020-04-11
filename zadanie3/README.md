# Модельный веб сервер

Я сделал веб-сервер который умеет отдавать статику.
API был навеен популярным фреймворком [expressjs](https://github.com/expressjs/express).
 
### Установка и запуск
Для компиляции и запуска можно использовать makefile.

```sh
$ make
$ make run
```

### Пример использования
Когда мы формируем наш сервер, можем в качестве параметра передать номер порта и имя файла для логов. По умолчанию сервер рабоает на 8080 порте и пишет логи в "log.txt".

```
Server app(8080, "logFile.txt");
app.Get("/", "test1.html");
app.Get("/hello", "test2.html"); //файла не существует, получаем ответ 500
app.run();
```
### Обработка ошибок
![](https://i.ibb.co/kSKtC7R/2020-04-10-0-15-50.png)

> Попытка совершить запрос к необъявленному маршруту. Ответ: 404

![](https://i.ibb.co/MpgjvPH/2020-04-10-0-15-58.png)
> Попытка выполнить POST запрос. Ответ: 501

![](https://i.ibb.co/5sk5bNX/2020-04-11-20-38-52.png)
>Пытаемся открыть /hello, но файл test2.html не существует на сервере. Ответ: 500