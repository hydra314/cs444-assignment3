#
# R Jesse Chaney
# 

CC = gcc
DEBUG = -g
DEFINES =
DEFINES += -DCHECK

CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
        -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
        -Wdeclaration-after-statement $(DEFINES)
PROG = beavalloc


all: $(PROG)


mystat: mystat.c mystat.h
	$(CC) $(CFLAGS) -c $<

opt: clean
	make DEBUG=-O3

tar: clean
	tar cvfz $(PROG).tar.gz *.[ch] ?akefile

# clean up the compiled files and editor chaff
clean cls:
	rm -f $(PROG) *.o *~ \#*

ci:
	ci -m"auto-checkin" -l *.[ch] ?akefile
