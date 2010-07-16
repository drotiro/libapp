# Variables
VER=0.1.0
OBJS=app.o
CFLAGS=-g
SONAME=libapp.so
PREFIX=/usr/local

# Main target
$(SONAME): $(SONAME).$(VER)

# Dependencies
app.o: app.h app.c
apptest.o: apptest.c app.h

$(SONAME).$(VER):	$(OBJS)
	gcc -Wl,-soname=$(SONAME) -shared -fPIC -o $@ $(OBJS)

.c.o:
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SONAME).$(VER) *.o apptest

install: $(SONAME)
	cp $(SONAME).$(VER) $(PREFIX)/lib
	@echo "*** Note: you should probably run 'ldconfig'"

apptest: libapp.so apptest.o
	gcc $(CFLAGS) -o $@ apptest.o  -lapp