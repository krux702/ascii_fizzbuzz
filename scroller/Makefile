# Makefile

all:
	gcc -DHAVE_CONFIG_H -I. -I. -I. -g -O2 -I/usr/include/ncurses -c aatest.c
	/bin/sh /usr/local/src/aalib-1.4.0/libtool --mode=link gcc -g -O2 -I/usr/include/ncurses -o aatest aatest.o libaa.la -lncurses -lm

clean:
	rm -f aatest aatest.o .libs/aatest


