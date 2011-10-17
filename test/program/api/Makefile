
BHOOK_TEST_ROOT=../..

include $(BHOOK_TEST_ROOT)/Makefile.env

CXXFLAGS=$(BHOOK_CXXFLAGS)

LDFLAGS=$(BHOOK_LDFLAGS)

all : api

api : api.cpp
	g++ -g -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -rf api
