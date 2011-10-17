
BHOOK_TEST_ROOT=../..

include $(BHOOK_TEST_ROOT)/Makefile.env

MYLIB = lib/output

CXXFLAGS = -I$(MYLIB)/include $(BHOOK_CXXFLAGS) $(GTEST_CXXFLAGS)

LDFLAGS = -L$(MYLIB)/lib -lmylib $(BHOOK_LDFLAGS) $(GTEST_LDFLAGS)

CC = g++

CPPFILE=$(wildcard *.cpp)
EXE=$(basename $(CPPFILE))

all : depend $(EXE)

depend:
	$(MAKE) -C lib

$(EXE) : % : %.cpp
	$(CC) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

#=========================================================================
.PHONY : all clean

clean :
	make clean -C lib
	rm -rf $(EXE)
