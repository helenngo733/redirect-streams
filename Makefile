main: main.c redir.c
	gcc -o main main.c redir.c

clean:
	rm main
