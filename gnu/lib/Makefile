# $FreeBSD$

.include <bsd.own.mk>

# amd64 and i386 have BSD licensed crtbegin.o and crtend.o
.if ${MACHINE_CPUARCH} != "amd64" && ${MACHINE_CPUARCH} != "i386"
SUBDIR= csu
.else
SUBDIR=
.endif

SUBDIR+= libgcc libgcov libdialog libgomp libregex libreadline

.if ${MK_SSP} != "no"
SUBDIR+= libssp
.endif

# libsupc++ uses libstdc++ headers, although 'make includes' should
# have taken care of that already.
.if ${MK_CXX} != "no"
SUBDIR+= libstdc++ libsupc++
.endif

.include <bsd.subdir.mk>
