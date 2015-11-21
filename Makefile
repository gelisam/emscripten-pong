all: index.html
.PHONY: all clean clobber


a.out: main.cpp
	gcc -W -Wall $<

index.html: main.cpp
	em++ -o index.html -W -Wall $<


clean:
clobber: clean
	rm -rf a.out a.out.js index.html
