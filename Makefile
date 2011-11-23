VPATH = syntax
SOURCES = src/*.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all:
	cd syntax && bison -d c+-.y && flex c+-.lex && cd .. &&\
	g++ syntax/c+-.tab.c syntax/lex.yy.c src/* -lfl -w -o c+-

debug:
	cd syntax && bison -d c+-.y && flex c+-.lex && cd .. &&\
	g++ syntax/c+-.tab.c syntax/lex.yy.c src/* -D SHOW_SCOPE -g3 -lfl -w -o c+-

clean:
	cd syntax && rm -f c+-.tab.* lex.yy.c *.o ../c+- ../src/*.o ../out.j out.class
