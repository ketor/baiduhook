

LDFLAGS	= -lpthread 

CPPFILE = $(wildcard *.cpp)
EXE = $(basename $(CPPFILE))

all	: $(EXE)

$(EXE) : % : %.cpp
	g++ $(CXXFLAGS) -o $@  $< $(LDFLAGS)

clean:
	rm -rf $(EXE)
