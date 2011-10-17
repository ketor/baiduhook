
BHOOK_TEST_ROOT=../..

include $(BHOOK_TEST_ROOT)/Makefile.env


all : user_write hook.so

user_write : user_write.cpp
	g++ -g -o $@ $< $(CXXFLAGS) $(LDFLAGS)

hook.so : hook.cpp
	g++ -shared -rdynamic -o $@ -fPIC $< $(BHOOK_CXXFLAGS)

clean:
	rm -rf user_write hook.so
