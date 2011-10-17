
BHOOK_TEST_ROOT=../..

include $(BHOOK_TEST_ROOT)/Makefile.env

CXXFLAGS=$(BHOOK_CXXFLAGS)

LDFLAGS=$(BHOOK_LDFLAGS)

all : simple hook.so

simple : simple.cpp
	g++ -g -o $@ $< $(CXXFLAGS) $(LDFLAGS)

hook.so : hook.cpp
	g++ -shared -rdynamic -o $@ -fPIC $< $(BHOOK_CXXFLAGS)

clean:
	rm -rf simple hook.so
