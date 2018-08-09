# 	@(#)Makefile	8.1 (Berkeley) 7/19/93
# $FreeBSD$

.include <src.opts.mk>
.include <src.tools.mk>

CONFS=	fbtab login.conf login.conf.db motd.template login.access
PROG=	login
SRCS=	login.c login_fbtab.c
CFLAGS+=-DLOGALL
LIBADD=	util pam

CLEANFILES+=	login.conf.db

WARNS?=	5

.if ${MK_AUDIT} != "no"
SRCS+=	login_audit.c
CFLAGS+= -DUSE_BSM_AUDIT
LIBADD+=	bsm
.endif

.if ${MK_SETUID_LOGIN} != "no"
BINOWN=	root
BINMODE=4555
PRECIOUSPROG=
.endif

.include <bsd.endian.mk>

login.conf.db: login.conf
	${CAP_MKDB_CMD} ${CAP_MKDB_ENDIAN} -f ${.TARGET:S/.db//} ${.ALLSRC}

.include <bsd.prog.mk>
