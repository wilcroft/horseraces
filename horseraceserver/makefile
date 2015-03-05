server: servermain.cpp hrserver.h
	$(CC) servermain.cpp  $(objectdir)/horse.o $(objectdir)/better.o $(objectdir)/horserace.o $(objectdir)/hrutils.o $(objectdir)/race.o -o server $(CFLAGS) -ltinfo -pthread
	cp server $(installdir)/server
	cp *.txt $(installdir)/
	
clean:
	rm -rf server
	rm -rf $(installdir)/server
	rm -rf $(installdir)/*.txt
