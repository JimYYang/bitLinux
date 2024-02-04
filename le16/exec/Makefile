.PHONY:all
all:myexec mybin
myexec:myexec.c
	gcc -o $@ $^ -std=c99
mybin:mybin.c
	gcc -o $@ $^ -std=c99
.PHONY:clean
clean:
	rm -f myexec mybin
