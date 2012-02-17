CPPFLAGS ?= -Wall
CFLAGS   ?= -pipe -O2
PREFIX   ?= usr/local

squirrelclock: squirrelclock.c

%: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $^

.c:
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $>

bin    := $(DESTDIR)/$(PREFIX)/bin

install: $(bin)/squirrelclock

$(bin)/squirrelclock: squirrelclock $(bin)
	cp -p $(@F) $(@D)

$(bin):
	mkdir -p $@

.PHONY: install
