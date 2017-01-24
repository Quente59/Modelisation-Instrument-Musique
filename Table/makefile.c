# makefile for simulation of the table
#
# Type "make clean" to delete all object and executable files.

# SRC is the directory in which the C source files are, and BIN is the
# directory where you want to put the executable programs. OBJ is the 
# directory where you want to put the object files.

SRC = ./
OBJ = obj/
BIN = ./

# CC should be set to the name of your favorite C compiler.

CC = cc

# An example CSWITCHES line is:
#   CSWITCHES = -O -DNO_TIMER -DLINUX -I/usr/X11R6/include -L/usr/X11R6/lib -ansi
#CSWITCHES = -O -DLINUX -I/usr/X11R6/include -L/usr/X11R6/lib
CSWITCHES = -O -W -Wall -g
# RM should be set to the name of your favorite rm (file deletion program).
RM = /bin/rm


all: $(BIN)simuTable

$(BIN)simuTable: $(OBJ)simuTable.o $(OBJ)mesh.o
	$(CC)  -g  -o $(BIN)simuTable $(OBJ)simuTable.o $(OBJ)mesh.o
 
$(OBJ)simuTable.o: $(SRC)main.c $(SRC)mesh.h 
	$(CC) $(CSWITCHES) -c -o $(OBJ)simuTable.o \
		$(SRC)main.c

$(OBJ)mesh.o: $(SRC)mesh.c $(SRC)mesh.h
	$(CC) $(CSWITCHES) $(TRILIBDEFS) -c -o $(OBJ)mesh.o \
		$(SRC)mesh.c

clean:
$(RM) $(BIN)simuTable $(OBJ)*
