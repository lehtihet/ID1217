# comment section

CC=gcc # compiler
OTARGET=ompmatrix # target file
ITARGET=ompmatrix.c
SIZE=10000
WORKERS=2

all:	
	@$(CC) $(ITARGET) -o $(OTARGET) -fopenmp

clean:
	rm $(OTARGET)

run:
	@./$(OTARGET) $(SIZE) $(WORKERS)