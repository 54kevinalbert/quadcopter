#MPU 6050 test program
# check /var/log/kern.log
CC=g++
CFLAGS=-lrt -lm -Iheaders -lbcm2835
SOURCES=src/gyro.cpp
OBJECTS=$(patsubst src/%.cpp, build/%.o, $(SOURCES))

all: runner

runner: src/main.cpp $(OBJECTS)
	$(CC) $(CFLAGS) src/main.cpp $(OBJECTS) -o runner

$(OBJECTS): build/%.o: src/%.cpp build
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f build/* runner
