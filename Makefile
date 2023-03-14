include config.mk

SRC = jack-follow.c 
OBJ = ${SRC:.c=.o}

all: options jack-follow

options:
	@echo build flags:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

config.h:
	cp config.def.h config.h

jack-follow: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f jack-follow ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f jack-follow ${DESTDIR}${PREFIX}/bin/
	chmod 755 ${DESTDIR}${PREFIX}/bin/jack-follow

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/jack-follow

.PHONY: all options clean install uninstall
