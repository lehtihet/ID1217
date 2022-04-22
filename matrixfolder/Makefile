# comment section

CC=gcc # compiler
TARGET=matrix # target file
DIMX=3
DIMY=3

all:	
	@$(CC) matrix.c -o $(TARGET) -lpthread

clean:
	rm $(TARGET)

run:
	@./$(TARGET) $(DIMX) $(DIMY)