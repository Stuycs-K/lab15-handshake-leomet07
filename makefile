all: client server basic persistant

client: basic_client.o pipe_networking.o
	@gcc -o client basic_client.o pipe_networking.o

server: forking_server.o pipe_networking.o
	@gcc -o server forking_server.o pipe_networking.o

basic: basic_server.o pipe_networking.o
	@gcc -o basic basic_server.o pipe_networking.o

persistant: persistant_server.o pipe_networking.o
	@gcc -o persistant persistant_server.o pipe_networking.o

basic_client.o: basic_client.c pipe_networking.h
	@gcc -c basic_client.c

basic_server.o: basic_server.c pipe_networking.h
	@gcc -c basic_server.c

persistant_server.o: persistant_server.c
	@gcc -c persistant_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	@gcc -c pipe_networking.c

clean:
	rm *.o
