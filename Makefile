all: a.out.js
.PHONY: all clean clobber


a.out: main.c
	gcc -W -Wall $<

a.out.js: main.c
	emcc -W -Wall $<


clean:
clobber: clean
	rm -rf a.out a.out.js
