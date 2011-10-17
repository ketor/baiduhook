
BHOOK_TEST_ROOT=../..

include $(BHOOK_TEST_ROOT)/Makefile.env


all : main my_printf.so

main : main.cpp
	g++ -g -o $@ $< $(CXXFLAGS) $(LDFLAGS)

my_printf.so : my_printf.cpp
	g++ -shared -rdynamic -o $@ -fPIC $< $(BHOOK_CXXFLAGS)

clean:
	rm -rf main my_printf.so
