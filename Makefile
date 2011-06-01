# Variables
VER=0.2.0
OBJS=app.o list.o
HEADERS=app.h list.h base.h
MY_CFLAGS=-g -fPIC -I..
SONAME=libapp.so
PREFIX=/usr/local
INCDIR=$(PREFIX)/include/libapp
LIBDIR=$(PREFIX)/lib
PKGCONFIGDIR=$(LIBDIR)/pkgconfig
INSTALL_S = install -s
LN_SF = ln -sf
TESTS=apptest listtest

# Main target
all: $(SONAME) libapp.pc

$(SONAME): $(SONAME).$(VER)

# Dependencies
app.o: base.h app.h app.c
apptest.o: base.h apptest.c app.h list.h
list.o: base.h list.h list.c

$(SONAME).$(VER):	$(OBJS)
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(LDFLAGS) -Wl,-soname=$(SONAME) -shared  -o $@ $(OBJS)

.c.o:
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

libapp.pc: libapp.pc.in
	sed \
		-e 's:@PREFIX@:$(PREFIX):g' \
		-e 's:@LIBDIR@:$(LIBDIR):g' \
		-e 's:@INCLUDEDIR@:$(INCDIR):g' \
		-e 's:@PACKAGE_NAME@:libapp:g' \
		-e 's:@PACKAGE_VERSION@:$(VER):g' \
		$< > $@

clean:
	rm -f $(SONAME).* *.o test/*.o *.pc $(TESTS)

install: $(SONAME) libapp.pc
	install -d '$(LIBDIR)'
	$(INSTALL_S) -t '$(LIBDIR)' $(SONAME).$(VER)
	$(LN_SF) $(SONAME).$(VER) '$(LIBDIR)'/$(SONAME)
	install -d '$(INCDIR)'
	install -m 644 -t '$(INCDIR)' $(HEADERS)
	install -d '$(PKGCONFIGDIR)'
	install -t '$(PKGCONFIGDIR)' libapp.pc

test: $(SONAME) $(TESTS)

apptest: test/apptest.o
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< -lapp

listtest: test/listtest.o
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< -lapp
