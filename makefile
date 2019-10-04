
SRCS= opt_test.c server_test.c
PROG= server_test
CC=gcc
CFLAGS=-g
OBJS=$(SRCS:.c=.o)
$(PROG):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
$(OBJS):opt_test.h
clean:
	rm -rf $(OBJS)
