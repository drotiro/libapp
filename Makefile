# Variables
export VER=0.2.0
export SONAME=libapp.so
export ANAME=libapp.a
PREFIX ?= /usr/local
INCDIR=$(PREFIX)/include
INCAPPDIR=$(PREFIX)/include/libapp
LIBDIR=$(PREFIX)/lib
PKGCONFIGDIR=$(LIBDIR)/pkgconfig
INSTALL_S = install -s
export LN_SF = ln -sf
SUBDIRS = libapp test

# Main target
all: libapp.pc
	@$(foreach i,$(SUBDIRS), make -C $i || exit 1; )

libapp.pc: libapp.pc.in
	sed \
		-e 's:@PREFIX@:$(PREFIX):g' \
		-e 's:@LIBDIR@:$(LIBDIR):g' \
		-e 's:@INCLUDEDIR@:$(INCDIR):g' \
		-e 's:@PACKAGE_NAME@:libapp:g' \
		-e 's:@PACKAGE_VERSION@:$(VER):g' \
		$< > $@

clean:
	rm -f *.pc
	@$(foreach i,$(SUBDIRS), make -C $i clean ;)

install: all libapp.pc
	install -d '$(LIBDIR)'
	$(INSTALL_S) -t '$(LIBDIR)' libapp/$(ANAME)
	$(INSTALL_S) -t '$(LIBDIR)' libapp/$(SONAME).$(VER)
	$(LN_SF) $(SONAME).$(VER) '$(LIBDIR)'/$(SONAME)
	install -d '$(INCAPPDIR)'
	install -m 644 -t '$(INCAPPDIR)' libapp/*.h
	install -d '$(PKGCONFIGDIR)'
	install -t '$(PKGCONFIGDIR)' libapp.pc

.PHONY: clean install all
