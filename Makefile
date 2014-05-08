
BINS=dummyvncserver mirvncserver # mirscreencast

all: $(BINS)

VNCLIBS=-lvncserver

MIRCFLAGS=-std=c++11 -Wall -fpermissive -I/usr/include/mirclient -I/usr/include/mircommon
MIRLIBS=-lmirclient -lmirprotobuf -lEGL -lboost_program_options -lxcb-glx -lGLESv2 -lmirserver

dummyvncserver: dummyvncserver.c
	$(CC) dummyvncserver.c -o dummyvncserver $(VNCLIBS)

screencast.o: screencast.cpp
	$(CXX) screencast.cpp -c $(MIRCFLAGS)

mirscreencast: screencast.o
	$(CXX) screencast.o -o mirscreencast $(MIRLIBS)

mirvncserver.o: mirvncserver.cpp
	$(CXX) mirvncserver.cpp -c $(MIRCFLAGS)

mirvncserver: mirvncserver.o
	$(CXX) mirvncserver.o -o mirvncserver $(MIRLIBS) $(VNCLIBS)

clean:
	rm $(BINS) *.o
