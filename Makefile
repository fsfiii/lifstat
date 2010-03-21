# $Id: Makefile,v 1.2 2006-10-31 14:55:47 frank Exp $

SHELL = /bin/sh
.SUFFIXES: .c .o

DESTDIR = /usr/local
BINDIR = bin
MANDIR = man

OBJS = split.o lifstat.o
HEADERS = lifstat.h
CFLAGS = -Wall
TARGET = lifstat
MANPAGE = lifstat.8
CC = gcc

.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

$(TARGET): $(OBJS) $(HDRS)
	$(CC) $(OBJS) -o $(TARGET)

install: $(TARGET)
	mkdir -p $(DESTDIR)/$(BINDIR)
	mkdir -p $(DESTDIR)/$(MANDIR)/man8
	install -m 755 $(TARGET) $(DESTDIR)/$(BINDIR)/
	install -m 755 $(MANPAGE) $(DESTDIR)/$(MANDIR)/man8/

clean:
	rm -f $(TARGET) $(OBJS)
