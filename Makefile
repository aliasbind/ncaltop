# This is the Makefile for nCaltop v. 0.01.
# Hopefully, this will be my first (partially) serious project.

CC=gcc
LD=gcc
CFLAGS=`pkg-config --cflags gtk+-2.0`
LIBS=`pkg-config --libs gtk+-2.0`
PROG=nCaltop
OBJS=main.o viewmotion.o storeop.o callbacks.o calcutils.o viewutils.o


all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LIBS) $(OBJS) -o $(PROG)
	./$(PROG)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(PROG) $(OBJS)
