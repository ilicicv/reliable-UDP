# Glavni Makefile u src folderu
.PHONY: all client server clean

all: client server

client:
	$(MAKE) -f makefile.client

server:
	$(MAKE) -f makefile.server

clean:
	$(MAKE) -f makefile.client clean
	$(MAKE) -f makefile.server clean
	rm -rf obj client server