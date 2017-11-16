CC = g++
CFLAGS = -std=c++11

mysh: mysh.cc
	$(CC) $(CFLAGS) mysh.cc -o mysh
clean:
	rm -f *.o mysh
