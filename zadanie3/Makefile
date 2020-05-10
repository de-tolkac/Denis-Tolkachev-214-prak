all:
	g++ -Wall -Werror main.cpp http_server.cpp -o server && cd ./cgi-bin && make && cd ..
run:
	./server
clean:
	rm -rf *.o ./server && cd ./cgi-bin && make clean && cd ..