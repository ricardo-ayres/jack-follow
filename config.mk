# jack-follow build configuration

PREFIX = ${HOME}/.local
JACK = -ljack

LIBS = ${JACK} 

CFLAGS = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} ${CPPFLAGS}
LDFLAGS = -s ${LIBS}
CC = cc
