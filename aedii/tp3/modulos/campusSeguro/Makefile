CC=g++
LDFLAGS=
CFLAGS=-g -std=c++11 -Wall
EXECUTABLE=test
SOURCES= campusSeguro.h \
		campusSeguro.cpp \
		../campus/campus.cpp \
		test.cpp
OBJECTS=$(SOURCES:.cpp=.o)



all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

#http://valgrind.org/docs/manual/quick-start.html
valgrind: $(EXECUTABLE)
	valgrind --leak-check=full -v ./$(EXECUTABLE)

clean:
	rm -rf *.o
	make all
reset:
	reset
	rm -rf *.o
	make all



