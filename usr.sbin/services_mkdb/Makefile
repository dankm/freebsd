# $FreeBSD$

PACKAGE=	runtime

CONFS=	services
PROG=	services_mkdb
MAN=	services_mkdb.8
SRCS=	services_mkdb.c uniq.c extern.h

.include <bsd.endian.mk>
.include <src.opts.mk>
.if !defined(BOOTSTRAPPING) && (${MK_SERVICESDB} != "no")
CONFS+=	services.db
# SERVICES_MKDB_CMD defined here
.include <src.tools.mk>

services.db: services
	${SERVICES_MKDB_CMD} ${CAP_MKDB_ENDIAN} -q -o ${.TARGET} ${.ALLSRC}

CLEANFILES+=	services.db
.endif

.include <bsd.prog.mk>
