CC=g++
CFLAGS=-c -Wall -std=c++11 -I../../
LFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
TARGET=900sq

#--------#

objects = flux.o main.o

all: $(TARGET)

$(TARGET): $(objects)
	$(CC) $(objects) -o $(TARGET) $(LFLAGS)

flux.o: ../../flux.cpp
	$(CC) $(CFLAGS) ../../flux.cpp
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
