# Variables
MY_CFLAGS=-g -fPIC -I..
TESTS=apptest listtest treetest

# Main target
all: $(TESTS)

# Dependencies
listtest.o: listtest.c
apptest.o: apptest.c
treetest.o: treetest.c

.c.o:
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TESTS)

apptest: apptest.o
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< -L../libapp -lapp

listtest: listtest.o
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< -L../libapp -lapp

treetest: treetest.o
	$(CC) $(MY_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< -L../libapp -lapp
