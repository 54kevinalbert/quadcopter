
#MPU 6050 test program
CC=cc
CFLAGS=-lrt -lm
LDFLAGS=
EXE=mpuTest
SOURCES=main.c mpu6050.c rpi.c
OBJECTS=$(SOURCES:.c=.o)

all: mpu6050.o rpi.o
	$(CC) $(CFLAGS) main.c mpu6050.o rpi.o -o $(EXE)

mpu6050.o: mpu6050.c rpi.o
	$(CC) $(CFLAGS) mpu6050.c rpi.o -o mpu6050.o -c

rpi.o: rpi.c
	$(CC) $(CFLAGS) rpi.c -o rpi.o -c

clean:
	rm -f *.o $(EXE)
