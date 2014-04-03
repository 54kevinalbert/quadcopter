
#MPU 6050 test program
CC=cc
CFLAGS=
LDFLAGS=-lrt -lunistd -lncurses
EXE=mpuTest
SOURCES=main.c mpu6050.c rpi.c
OBJECTS=$(SOURCES:.c=.o)

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@