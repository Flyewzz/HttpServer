httpd: src/main.c src/serverconfig.c src/evserver.c src/common.c src/httphandler.c dir
	gcc src/main.c src/serverconfig.c src/evserver.c src/common.c src/httphandler.c -o target/httpd -lev -I.

client: src/client.c src/common.c dir
	gcc src/client.c src/common.c -o target/client -I.

all: httpd client

dir:
	mkdir -p target
