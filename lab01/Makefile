CC = gcc
CFLAGS = -Wall -std=c11 -c
LDFLAGS = -Wall -std=c11

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS +=-DDEBUG
	LDFLAGS += -DDEBUG
endif

OBJECTS = err.o spawner.o
ALL = W ToONP

all: $(ALL)

%.o : %.c
	$(CC) $(CFLAGS) $<

$(ALL) : % : %.o $(OBJECTS)       
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o  *.gch $(ALL) *~
