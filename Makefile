.POSIX:

include config.mk

SRC = src/main.c
OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -pedantic -Iinclude
LDFLAGS =

.PHONY: all
all: edxp

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

edxp: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

.PHONY: clean 
clean:
	rm -f $(OBJ) edxp

.PHONY: install
install: edxp
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f edxp $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/edxp

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/edxp

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@
