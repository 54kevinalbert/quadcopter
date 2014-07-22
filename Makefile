#MPU 6050 test program
# check /var/log/kern.log
CC=g++
CFLAGS=-lrt -lm -lbcm2835 -Iheaders
LDFLAGS=
SOURCES=src/main.cpp src/gyro.cpp
OBJECTS=$(patsubst src/%.cpp, build/%.o, $(SOURCES))

all: runner

runner: build/main.o
	$(CC) $(LDFLAGS) $< -o runner $(CFLAGS)

$(OBJECTS): build/%.o: src/%.cpp build
	$(CC) -c $< -o $@ $(CFLAGS) 

clean:
	rm -f build/* runner
