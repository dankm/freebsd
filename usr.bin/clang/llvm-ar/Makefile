# $FreeBSD$

PROG_CXX=	llvm-ar

SRCDIR=		tools/llvm-ar
SRCS+=		llvm-ar.cpp

LIBADD+=	z

LINKS=	${BINDIR}/llvm-ar ${BINDIR}/llvm-ranlib
MLINKS=	llvm-ar.1 llvm-ranlib.1

.include "../llvm.prog.mk"
