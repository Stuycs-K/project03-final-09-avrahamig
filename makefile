all: player server
player: client.o pipe_networking.o
	@gcc -o client client.o pipe_networking.o

setup: server.o pipe_networking.o
	@gcc -o setup server.o pipe_networking.o

client.o: client.c pipe_networking.h
	@gcc -c client.c -Wall

server.o: server.c pipe_networking.h
	@gcc -c server.c -Wall

pipe_networking.o: pipe_networking.c pipe_networking.h
	@gcc -c pipe_networking.c -Wall

clean:
	rm *.o
	rm *~
