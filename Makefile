# Variables
VER=0.1.0
OBJS=app.o
CFLAGS=-g -fPIC
SONAME=libapp.so
PREFIX=/usr/local
INCDIR=$(PREFIX)/include/libapp
LIBDIR=$(PREFIX)/lib

# Main target
$(SONAME): $(SONAME).$(VER)

# Dependencies
app.o: app.h app.c
apptest.o: apptest.c app.h

$(SONAME).$(VER):	$(OBJS)
	gcc $(CFLAGS) -Wl,-soname=$(SONAME) -shared  -o $@ $(OBJS)

.c.o:
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SONAME).$(VER) *.o apptest

install: $(SONAME)
	install -s $(SONAME).$(VER) $(LIBDIR)
	install -d $(INCDIR)
	install -m 644 -t $(INCDIR) app.h
	
	@echo "*** Note: you should probably run 'ldconfig'"

install_debug: $(SONAME)
	install $(SONAME).$(VER) $(PREFIX)/lib

apptest: apptest.o
	gcc $(CFLAGS) -o $@ apptest.o  -lapp
