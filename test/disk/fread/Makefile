
all : readfile

readfile : readfile.cpp
	g++ -g -o $@ $< $(CXXFLAGS) $(LDFLAGS)

#=========================================================================
.PHONY	: clean

clean :
	rm -f  readfile

