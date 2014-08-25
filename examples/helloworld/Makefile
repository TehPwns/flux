CC=g++
CFLAGS=-c -Wall -std=c++11 -I../../include -I../../src
TARGET=helloflux

#--------#

objects = flux.o main.o

all: $(TARGET)

$(TARGET): $(objects)
	$(CC) $(objects) -o $(TARGET)

flux.o: ../../src/flux.cpp
	$(CC) $(CFLAGS) ../../src/flux.cpp
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
