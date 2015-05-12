CC=gcc
file=pools.c standard.c
out=main
all:
	$(CC) -o $(out) test.c command.c $(file)
debug:
	$(CC) -g -o $(out) test.c command.c $(file)
clean:
	rm $(out)