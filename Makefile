CC=gcc
file=command.c pools.c standard.c
out=main
all:
	$(CC) -o $(out) test.c $(file)
debug:
	$(CC) -g -o $(out) test.c $(file)
clean:
	rm $(out)