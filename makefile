export CFLAGS = --std=c++11
export CC = g++
projectdir := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
export objectdir = $(projectdir)/objs
export installdir = $(projectdir)/bin
srcdirs = horseraces clientlibrary horseraceserver raceclientgui racedisplay
all: dirs $(srcdirs)

debug: debug-flags all-debug

debug-flags:
CFLAGS += -g

all-debug: dirs $(srcdirs)

.PHONY: subdirs $(srcdirs)

dirs:
	mkdir -p $(objectdir)
	mkdir -p $(installdir)

horseraces:
	$(MAKE) -C horseraces
	
horseraceserver: horseraces 
	$(MAKE) -C horseraceserver

raceclientgui: horseraces clientlibrary
	cd $@ && qmake && cd ..
	$(MAKE) -C $@
	cp $@/$@ $(installdir)/
	cp $@/*.gif $(installdir)/
	
racedisplay: horseraces clientlibrary
	cd $@ && qmake && cd ..
	$(MAKE) -C $@
	cp $@/$@ $(installdir)/
	cp $@/*.gif $(installdir)/

clientlibrary: 
	$(MAKE) -C clientlibrary
#	cd $@ && $(MAKE)
clean:
	$(MAKE) -C horseraces clean
	$(MAKE) -C horseraceserver clean
	$(MAKE) -C raceclientgui clean
	$(MAKE) -C racedisplay clean
	$(MAKE) -C clientlibrary clean
