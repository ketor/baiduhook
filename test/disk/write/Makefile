
all : writefile

writefile : writefile.cpp
	g++ -g -o $@ $< $(CXXFLAGS) $(LDFLAGS)

#=========================================================================
.PHONY	: clean

clean :
	rm -f  writefile

