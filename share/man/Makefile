#	@(#)Makefile	8.2 (Berkeley) 4/16/94
# $FreeBSD$

.include <src.opts.mk>

# XXX MISSING:	man3f
SUBDIR=	man1 man3 man4 man5 man6 man7 man8 man9
SUBDIR_PARALLEL=

MAKEWHATIS?=	makewhatis

.if defined(NO_ROOT)
METALOG.add?=	cat -l >> ${METALOG}
.endif

makedb:
.if ${MK_MAN_UTILS} != "no"
	${MAKEWHATIS} ${DESTDIR}${BINDIR}/man
.if ${MK_OPENSSL} != "no"
	${MAKEWHATIS} ${DESTDIR}${BINDIR}/openssl/man
.endif
.if defined(NO_ROOT)
	echo "./${BINDIR}/man/mandoc.db type=file mode=0644 uname=root gname=wheel" | ${METALOG.add}
.if ${MK_OPENSSL} != "no"
	echo "./${BINDIR}/openssl/man/mandoc.db type=file mode=0644 uname=root gname=wheel" | ${METALOG.add}
.endif
.endif
.endif

.include "${.CURDIR}/../Makefile.inc"
.include <bsd.subdir.mk>
