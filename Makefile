VPATH = syntax
SOURCES = src/*.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all:
	cd syntax && bison -d c+-.y && flex c+-.lex && cd .. &&\
	g++ syntax/c+-.tab.c syntax/lex.yy.c src/* -lfl -w -o c+-

c+-: tab.o lex.o $(OBJECTS)
	g++ $^ -lfl -w -o c+-


$(OBJECTS):
	cd src && g++ -c *.cpp

lex.o: lex.yy.c
	g++ -c syntax/$^ -o syntax/lex.o

tab.o: c+-.tab.c
	g++ -c syntax/$^ -lfl -w -o syntax/tab.o

lex.yy.c: c+-.lex
	cd syntax && flex c+-.lex

c+-.tab.c: c+-.y
	cd syntax && bison -d c+-.y

show_scope:
	g++ syntax/c+-.tab.c syntax/lex.yy.c src/* -D SHOW_SCOPE -lfl -w -o c+-

hard:
	cd syntax && bison -d c+-.y && flex c+-.lex && cd .. &&\
	g++ syntax/c+-.tab.c syntax/lex.yy.c src/* -lfl -w -o c+-

debug:
	cd syntax && bison -d c+-.y && flex c+-.lex && cd .. &&\
	g++ syntax/c+-.tab.c syntax/lex.yy.c src/* -D SHOW_SCOPE -g3 -lfl -w -o c+-

clean:
	cd syntax && rm -f c+-.tab.* lex.yy.c *.o ../c+- ../src/*.o
