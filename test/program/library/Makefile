BHOOK_TEST_ROOT=../..

include $(BHOOK_TEST_ROOT)/Makefile.env

MYLIB = lib/output

CXXFLAGS = -I$(MYLIB)/include $(BHOOK_CXXFLAGS)

LDFLAGS = -L$(MYLIB)/lib -lmylib $(BHOOK_LDFLAGS)



all : depend attachlib

depend:
	$(MAKE) -C lib

attachlib : attachlib.cpp
	g++ -g -o $@ $< $(CXXFLAGS) $(LDFLAGS)

#=========================================================================
.PHONY : all clean

clean :
	make clean -C lib
	rm -rf attachlib
