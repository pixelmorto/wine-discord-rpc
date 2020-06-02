CC = winegcc

CFLAGS = -O2 -Wall

PREFIX := /usr
BINDIR = $(PREFIX)/bin
SHAREDIR = $(PREFIX)/share
INSTALLDIR = $(DESTDIR)$(SHAREDIR)/wine-discord-rpc

default: all
all: wine-discord-rpc.win32.exe.so wine-discord-rpc.win64.exe.so setup-wine-discord-rpc.sh

wine-discord-rpc.win64.exe.so: src/wine-discord-rpc.c
	$(CC) -m64 -o $@ $(CFLAGS) $<

wine-discord-rpc.win32.exe.so: src/wine-discord-rpc.c
	$(CC) -m32 -o $@ $(CFLAGS) $<

clean:
	rm -v wine-discord-rpc.win64.exe.so wine-discord-rpc.win32.exe.so

install: all
	mkdir -v -p $(INSTALLDIR)
	cp -v wine-discord-rpc.win64.exe.so $(INSTALLDIR)/
	cp -v wine-discord-rpc.win32.exe.so $(INSTALLDIR)/
	cp -v setup-wine-discord-rpc.sh $(INSTALLDIR)/
	ln -v -s $(INSTALLDIR)/setup-wine-discord-rpc.sh $(BINDIR)/setup-wine-discord-rpc

uninstall:
	rm -v $(INSTALLDIR)/wine-discord-rpc.win64.exe.so $(INSTALLDIR)/wine-discord-rpc.win32.exe.so $(INSTALLDIR)/setup-wine-discord-rpc.sh $(BINDIR)/setup-wine-discord-rpc
	rmdir -v $(INSTALLDIR)
