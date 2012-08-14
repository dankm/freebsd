# $FreeBSD$

.PATH: ${.CURDIR}/../common

SRCS=		crt1.c crti.S crtn.S crtbegin.S crtend.S
OBJS=		${SRCS:N*.h:R:S/$/.o/g}
OBJS+=		Scrt1.o gcrt1.o crtbeginS.o
CFLAGS+=	-I${.CURDIR}/../common \
		-I${.CURDIR}/../../libc/include
CFLAGS+=	-fno-omit-frame-pointer

all: ${OBJS}

CLEANFILES=	${OBJS}
CLEANFILES+=	crt1.s gcrt1.s Scrt1.s

# See the comment in lib/csu/common/crtbrand.c for the reason crt1.c is not
# directly compiled to .o files.

crt1.s: crt1.c
	${CC} ${CFLAGS} -S -o ${.TARGET} ${.CURDIR}/crt1.c
	sed ${SED_FIX_NOTE} ${.TARGET}

crt1.o: crt1.s
	${CC} ${ACFLAGS} -c -o ${.TARGET} crt1.s

gcrt1.s: crt1.c
	${CC} ${CFLAGS} -DGCRT -S -o ${.TARGET} ${.CURDIR}/crt1.c
	sed ${SED_FIX_NOTE} ${.TARGET}

gcrt1.o: gcrt1.s
	${CC} ${ACFLAGS} -c -o ${.TARGET} gcrt1.s

Scrt1.s: crt1.c
	${CC} ${CFLAGS} -fPIC -DPIC -S -o ${.TARGET} ${.CURDIR}/crt1.c
	sed ${SED_FIX_NOTE} ${.TARGET}

Scrt1.o: Scrt1.s
	${CC} ${ACFLAGS} -c -o ${.TARGET} Scrt1.s

crtbeginS.o: crtbegin.S
	${CC} ${CFLAGS} -fPIC -DPIC -DSHARED -c -o ${.TARGET} ${.CURDIR}/crtbegin.S

realinstall:
	${INSTALL} -o ${LIBOWN} -g ${LIBGRP} -m ${LIBMODE} \
	    ${OBJS} ${DESTDIR}${LIBDIR}

SYMLINKS+=      crtbegin.o ${LIBDIR}/crtbeginT.o
SYMLINKS+=      crtend.o ${LIBDIR}/crtendS.o

.include <bsd.lib.mk>
