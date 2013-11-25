# Variables
OBJS=app.o list.o tree.o
MY_CFLAGS=-fPIC -I..
AR ?= ar
CC ?= gcc


# Main target
all: $(SONAME) $(ANAME) check

$(SONAME): $(SONAME).$(VER)
	$(LN_SF) $(SONAME).$(VER) $(SONAME)

# Dependencies
app.o: base.h app.h app.c
list.o: base.h list.h list.c
tree.o: list.h tree.h tree.c

$(SONAME).$(VER): $(OBJS)
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(LDFLAGS) -Wl,-soname=$(SONAME) -shared  -o $@ $(OBJS)

$(ANAME): $(OBJS)
	$(AR) rc $@ $+

.c.o:
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean: check
	rm -f $(SONAME) $(SONAME).$(VER) $(ANAME) $(OBJS)

check:
ifndef SONAME
	@echo "Please use the top-level Makefile"
	@exit 1
endif
