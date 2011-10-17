
INCLUDE	= 	-I./ \

CXXFLAGS	=	-Wall -g -D_REENTRANT -D_FILTER -D_DEBUG_ -DNDEBUG 

TARGET = libmylib.a

all		: $(TARGET) output

$(TARGET) : mylib.o
	ar cr $@ $^
	
mylib.o : mylib.cpp mylib.h
	g++ -fPIC -c -o $@ $< $(CXXFLAG) $(INCLUDE)
	
output : $(TARGET)
	if [ ! -d output/lib ]; then mkdir -p output/lib; fi
	mv libmylib.a output/lib
	if [ ! -d output/include ]; then mkdir -p output/include; fi
	cp mylib.h output/include

clean:
	rm -rf output
	rm -rf *.o

