

CC=gcc # compiler
OSEQJAC=seqJac # target file
ISEQJAC=seqJac.c # input file
OPARJAC=parJac 
IPARJAC=parJac.c
OSEQMULTI=seqMulti
ISEQMULTI=seqMulti.c
OPARMULTI=parMulti
IPARMULTI=parMulti.c

GRIDSIZE=100
NUMITERS=100000
NUMWORKERS=4

all: seqJac parJac seqMulti parMulti

seqJac:
	@echo "Compiling sequential jacobian..."
	@$(CC) $(ISEQJAC) -o $(OSEQJAC)

parJac:
	@echo "Compiling parallel jacobian..."
	@$(CC) $(IPARJAC) -o $(OPARJAC) -fopenmp

seqMulti:
	@echo "Compiling sequential multi..."
	@$(CC) $(ISEQMULTI) -o $(OSEQMULTI)

parMulti:
	@echo "Compiling parallel multi..."
	@$(CC) $(IPARMULTI) -o $(OPARMULTI) -fopenmp


runSeqJac:
	@./$(OSEQJAC) $(GRIDSIZE) $(NUMITERS)

runParJac:
	@./$(OPARJAC) $(GRIDSIZE) $(NUMITERS) $(NUMWORKERS)

runSeqMulti:
	@./$(OSEQMULTI) $(GRIDSIZE) $(NUMITERS)

runParMulti:
	@./$(OPARMULTI) $(GRIDSIZE) $(NUMITERS) $(NUMWORKERS)
