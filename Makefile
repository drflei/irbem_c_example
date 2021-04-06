# CPP           C Compiler for standard rules
# FLAGS         C compiler flags for standard rules 

CPP=g++
FLAGS= -Wall -fPIC -m64 -O2
q=bin
SRC=irbem_ex.cpp
OBJ=$(SRC:.cpp=.o)
LIBS=-lstdc++
INCS=-I.
EXE=irbem_ex

all: $(OBJ) $(EXE)

$(EXE): $(OBJ)
	@if [ ! -f ${q} ]; then \
	mkdir bin; \
	fi; \
	g++  $(FLAGS) $(OBJ) $(LIBS) -L$(IRBEM) -lirbem -lgfortran -o bin/$(EXE)

.cpp.o: 
	@echo -n "Compiling file $< \n"
	$(CPP) $(INCS) $(FLAGS) -c $<


.PHONY : clean
clean:
	rm -f   *.o   *~   */*~   bin/$(EXE)
