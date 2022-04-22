
# comment section

CC=gcc # compiler
TARGET=shuffle # target file
F1=/home/oliver/Desktop/ID1217/HW1/file1.txt
F2=/home/oliver/Desktop/ID1217/HW1/file2.txt

all:	
	@$(CC) shuffle.c -o $(TARGET) -lpthread

clean:
	rm $(TARGET)

run:
	@./$(TARGET) $(F1) $(F2)
