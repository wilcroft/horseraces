objs = horse.o better.o horserace.o hrutils.o race.o
all: $(objs) test

better.o: better.h better.cpp constants.h horseraceerrors.h
	$(CC) $(CFLAGS) -c -o better.o better.cpp
	cp better.o $(objectdir)/
	
horse.o: horse.cpp horse.h constants.h horseraceerrors.h
	$(CC) $(CFLAGS) -c -o horse.o horse.cpp
	cp horse.o $(objectdir)/	
horserace.o: horserace.cpp horserace.h constants.h horseraceerrors.h race.h better.h
	$(CC) $(CFLAGS) -c -o horserace.o horserace.cpp
	cp horserace.o $(objectdir)/	
race.o: race.cpp race.h constants.h horseraceerrors.h better.h horse.h
	$(CC) $(CFLAGS) -c -lm -o race.o race.cpp
	cp race.o $(objectdir)/
hrutils.o: hrutils.cpp hrutils.h constants.h
	$(CC) $(CFLAGS) -c -lcurses -ltinfo -o hrutils.o hrutils.cpp
	cp hrutils.o $(objectdir)/
test: main.cpp $(objs)
	$(CC) $(CFLAGS) main.cpp $(objs) -o test -ltinfo
clean:
	rm -rf $(objs)
	rm -rf test
	cd $(objectdir)
	rm -rf $(objs)
	cd $(installdir)
	rm -rf test
