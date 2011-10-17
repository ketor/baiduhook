
CXXFLAGS	=	-Wall -g -D_REENTRANT -D_FILTER -D_DEBUG_ -DNDEBUG 

CPP		= g++

TARGET = libmylib.so

all		: $(TARGET) output

$(TARGET) : mylib.o
	g++ -shared -rdynamic -o $@ -fPIC $< 
	
mylib.o : mylib.cpp mylib.h
	g++ -fPIC -c -o $@ $< $(CXXFLAGS)
	
output : $(TARGET)
	if [ ! -d output/lib ]; then mkdir -p output/lib; fi
	mv libmylib.so output/lib
	if [ ! -d output/include ]; then mkdir -p output/include; fi
	cp mylib.h output/include

clean:
	rm -rf output
	rm -rf *.o

