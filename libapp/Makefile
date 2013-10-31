# Variables
OBJS=app.o list.o
MY_CFLAGS=-g -fPIC -I..

# Main target
all: $(SONAME)

$(SONAME): $(SONAME).$(VER)
	$(LN_SF) $(SONAME).$(VER) $(SONAME)

# Dependencies
app.o: base.h app.h app.c
list.o: base.h list.h list.c

$(SONAME).$(VER):	$(OBJS)
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(LDFLAGS) -Wl,-soname=$(SONAME) -shared  -o $@ $(OBJS)

.c.o:
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(SONAME) $(SONAME).$(VER) $(OBJS)
