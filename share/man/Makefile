#	@(#)Makefile	8.2 (Berkeley) 4/16/94
# $FreeBSD$

.include <src.opts.mk>

# XXX MISSING:	man3f
SUBDIR=	man1 man3 man3lua man4 man5 man6 man7 man8 man9
SUBDIR_PARALLEL=

MAKEWHATIS?=	makewhatis

.if defined(NO_ROOT)
METALOG.add?=	cat -l >> ${METALOG}
.if ${INSTALL:M-D} && !defined(DISTBASE)
DISTBASE=${DESTDIR:S,${INSTALL:C/^.*-D ([^[:space:]]+).*/\1/W},,}
.endif
.endif

makedb:
.if ${MK_MAN_UTILS} != "no"
	${MAKEWHATIS} ${DESTDIR}${BINDIR}/man
.if ${MK_OPENSSL} != "no"
	${MAKEWHATIS} ${DESTDIR}${BINDIR}/openssl/man
.endif
.if defined(NO_ROOT)
	if [ -e ${DESTDIR}${BINDIR}/man/mandoc.db ]; then \
		echo ".${DISTBASE}${BINDIR}/man/mandoc.db type=file uname=${BINOWN} gname=${BINGRP} mode=${CONFMODE}" | ${METALOG.add}
	fi
	if [ -e ${DESTDIR}${BINDIR}/openssl/man/mandoc.db ]; then \
		echo ".${DISTBASE}${BINDIR}/openssl/man/mandoc.db type=file uname=${BINOWN} gname=${BINGRP} mode=${CONFMODE}" | ${METALOG.add}
	fi
.endif
.endif

.include "${.CURDIR}/../Makefile.inc"
.include <bsd.subdir.mk>
