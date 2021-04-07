CC = gcc
CPPFLAGS = -D_XOPEN_SOURCE -D_FORTIFY_SOURCE=2
CFLAGS = -std=c11 -Wpedantic -Wall -Wextra  -fstack-protector-all -fpie -pthread -O -O2
LDFLAGS = -pthread -lrt

sources= server.c
objects= $(sources:.c=.o) ./libanalyse.so ./libfifo.so

executable = server

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)
	
%.o: %.c
	$(CC) $(LDFLAGS) -o $@ -c $<


