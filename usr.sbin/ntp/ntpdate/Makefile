# $FreeBSD$

.include <src.opts.mk>

.PATH: ${SRCTOP}/contrib/ntp/ntpdate

PROG=	ntpdate
MAN=
SRCS=	ntpdate.c version.c

CFLAGS+= -I${SRCTOP}/contrib/ntp/include \
	-I${SRCTOP}/contrib/ntp/lib/isc/include/ \
	-I${SRCTOP}/contrib/ntp/lib/isc/unix/include/ \
	-I${SRCTOP}/contrib/ntp/lib/isc/pthreads/include \
	-I${.CURDIR:H}

LIBADD=	ntp m pthread

.if ${MK_OPENSSL} != "no"
LIBADD+=	crypto
.else
LIBADD+=	md
.endif

CLEANFILES+= .version version.c

version.c:
.if ${MK_REPRODUCIBLE_BUILD} != "no"
	-rm -f .version ;
.endif
	sh -e ${.CURDIR:H}/scripts/mkver ntpdate

.include <bsd.prog.mk>
