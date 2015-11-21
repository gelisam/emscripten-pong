all: index.html
.PHONY: all clean clobber


a.out: main.cpp
	gcc -W -Wall $<

index.html: main.cpp
	em++ -std=c++11 -o index.html -g -W -Wall $<
	#em++ -std=c++11 -o index.html -O3 -W -Wall $<


clean:
	rm -rf index.html.map
clobber: clean
	rm -rf index.html index.js
