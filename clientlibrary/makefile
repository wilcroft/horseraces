#CC = g++
#CFLAGS = --std=c++11
#objectdir = ./
#
clientlibrary.o: hrclientlibrary.cpp hrclientlibrary.h ../horseraces/constants.h ../horseraces/horseraceerrors.h ../horseraces/better.h ../horseraces/hrutils.h
	$(CC) $(CFLAGS) -c -o clientlibrary.o hrclientlibrary.cpp
	cp *.o $(objectdir)/
clean:
	rm -rf *.o
	rm -rf $(objectdir)/clientlibrary.o
