# $FreeBSD$

SHLIB_NAME=	dynthr_mod.so
SHLIBDIR=	${TESTSDIR}
SRCS=		dynthr_mod.c
LIBADD=		pthread

TESTSDIR:=	${TESTSBASE}/${RELDIR:C/libc\/tests/libc/:H}

DISTRIBUTION:=	tests

.include <bsd.lib.mk>
