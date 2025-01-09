compile: compile_client compile_server compile_basic compile_persistant

compile_client client.out: basic_client.o pipe_networking.o
	@gcc -o client.out basic_client.o pipe_networking.o

compile_server server.out: forking_server.o pipe_networking.o
	@gcc -o server.out forking_server.o pipe_networking.o

compile_basic basic.out: basic_server.o pipe_networking.o
	@gcc -o basic.out basic_server.o pipe_networking.o

compile_persistant persistant.out: persistant_server.o pipe_networking.o
	@gcc -o persistant.out persistant_server.o pipe_networking.o

basic_client.o: basic_client.c pipe_networking.h
	@gcc -c basic_client.c

basic_server.o: basic_server.c pipe_networking.h
	@gcc -c basic_server.c

persistant_server.o: persistant_server.c
	@gcc -c persistant_server.c

forking_server.o: forking_server.c
	@gcc -c forking_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	@gcc -c pipe_networking.c

client: client.out
	@./client.out

server: server.out
	@./server.out

basic: basic.out
	@./basic.out

persistant: persistant.out
	@./persistant.out

clean:
	rm *.o *.out basic client server persistant
