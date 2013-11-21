CC := g++
CFLAGS := -O2 -std=c++0x -march=native -mtune=native -funroll-loops -Wall
SRC := main.cpp setting.h dataset.cpp dataset.h model.cpp model.h utils.cpp utils.h

all: jbasmf

jbasmf: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o jbasmf -larmadillo 

clean:
	rm -f jbasmf
