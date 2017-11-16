CC = g++
CFLAGS = -std=c++11

mysh: mysh.cc Program_Control.cc
	$(CC) $(CFLAGS) mysh.cc Program_Control.cc -o mysh
clean:
	rm -f *.o mysh
