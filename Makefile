#MPU 6050 test program
# check /var/log/kern.log
CC=g++
CFLAGS=-std=c++0x -Iheaders
LDFLAGS=-lrt -lm -lbcm2835
SOURCES=src/main.cpp src/quadcopter.cpp src/motor.cpp src/gyro.cpp
OBJECTS=$(patsubst src/%.cpp, %.o, $(SOURCES))

all: runner

install: runner
	cp runner /usr/local/bin

runner: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJECTS): %.o: src/%.cpp 
	$(CC) $(CFLAGS) -c $< -o $@
	
echo:
	echo $(OBJECTS)

build:
	mkdir build

clean:
	rm -f *.o runner
