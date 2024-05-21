CC=gcc
DFLAGS=-ggdb -Wall -Wextra --std=c99 --pedantic
RFLAGS=-O2
LIBS=`pkg-config --libs raylib`
SRCDIR=src
BUILDIR=build
BINPATH=$(BUILDIR)/langant
DOCSPATH=docs
HEADERS=$(wildcard $(SRCDIR)/*.h)
CFILES=$(HEADERS:.h=.c)

.PHONY: debug release clean

$(BINPATH)-debug: $(SRCDIR)/main.c $(HEADERS) $(CFILES)
	mkdir -p $(BUILDIR)
	$(CC) $(DFLAGS) -o $(BINPATH)-debug $< $(LIBS)


$(BINPATH)-release: $(SRCDIR)/main.c $(HEADERS) $(CFILES)
	mkdir -p $(BUILDIR)
	$(CC) $(RFLAGS) -o $(BINPATH)-release $< $(LIBS)

debug: $(BINPATH)-debug
	@gdb -q ./$<

release: $(BINPATH)-release $(HEADERS) $(CFILES)
	@./$<

clean:
	rm -rf $(BUILDIR)
