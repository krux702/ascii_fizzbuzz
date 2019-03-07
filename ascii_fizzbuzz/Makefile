# Makefile

all:
	gcc -DHAVE_CONFIG_H -I. -I. -I. -g -O2 -I/usr/include/ncurses -c ascii_fizzbuzz.c
	/bin/sh /usr/local/src/aalib-1.4.0/libtool --mode=link gcc -g -O2 -I/usr/include/ncurses -o ascii_fizzbuzz ascii_fizzbuzz.o libaa.la -lncurses -lm

clean:
	rm -f ascii_fizzbuzz ascii_fizzbuzz.o .libs/ascii_fizzbuzz


