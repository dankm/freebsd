/*-
 * Copyright (c) 2003 Peter Wemm.
 * Copyright (c) 1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

/*
 * Functions to provide access to special i386/amd64 instructions.
 * This in included in sys/systm.h, and that file should be
 * used in preference to this.
 */

#ifndef _MACHINE_CPUFUNC_H_
#define	_MACHINE_CPUFUNC_H_

#ifndef _SYS_CDEFS_H_
#error this file needs sys/cdefs.h as a prerequisite
#endif

#if defined(XEN) && defined(__i386__)
extern void xen_cli(void);
extern void xen_sti(void);
extern u_int xen_rcr2(void);
extern void xen_load_cr3(u_int data);
extern void xen_tlb_flush(void);
extern void xen_invlpg(u_int addr);
extern void write_eflags(register_t eflags);
extern register_t read_eflags(void);
#endif

struct region_descriptor;

#define readb(va)	(*(volatile uint8_t *) (va))
#define readw(va)	(*(volatile uint16_t *) (va))
#define readl(va)	(*(volatile uint32_t *) (va))
#define readq(va)	(*(volatile uint64_t *) (va))

#define writeb(va, d)	(*(volatile uint8_t *) (va) = (d))
#define writew(va, d)	(*(volatile uint16_t *) (va) = (d))
#define writel(va, d)	(*(volatile uint32_t *) (va) = (d))
#define writeq(va, d)	(*(volatile uint64_t *) (va) = (d))

#if defined(__GNUCLIKE_ASM) && defined(__CC_SUPPORTS___INLINE)

static __inline void
breakpoint(void)
{
	__asm __volatile("int $3");
}

static __inline u_int
bsfl(u_int mask)
{
	u_int	result;

	__asm("bsfl %1,%0" : "=r" (result) : "rm" (mask) : "cc");
	return (result);
}

static __inline u_int
bsrl(u_int mask)
{
	u_int	result;

	__asm("bsrl %1,%0" : "=r" (result) : "rm" (mask) : "cc");
	return (result);
}

#ifdef __amd64__
static __inline u_long
bsrq(u_long mask)
{
	u_long	result;

	__asm("bsrq %1,%0" : "=r" (result) : "rm" (mask) : "cc");
	return (result);
}

static __inline u_long
bsfq(u_long mask)
{
	u_long	result;

	__asm("bsfq %1,%0" : "=r" (result) : "rm" (mask) : "cc");
	return (result);
}
#endif

static __inline void
clflush(u_long addr)
{

	__asm __volatile("clflush %0" : : "m" (*(char *)addr));
}

static __inline void
clts(void)
{

	__asm __volatile("clts");
}

static __inline void
disable_intr(void)
{
#if defined(XEN) && defined(__i386__)
	xen_cli();
#else	
	__asm __volatile("cli" : : : "memory");
#endif
}

static __inline void
do_cpuid(u_int ax, u_int *p)
{
	__asm __volatile("cpuid"
			 : "=a" (p[0]), "=b" (p[1]), "=c" (p[2]), "=d" (p[3])
			 :  "0" (ax));
}

static __inline void
cpuid_count(u_int ax, u_int cx, u_int *p)
{
	__asm __volatile("cpuid"
			 : "=a" (p[0]), "=b" (p[1]), "=c" (p[2]), "=d" (p[3])
			 :  "0" (ax), "c" (cx));
}

static __inline void
enable_intr(void)
{
#if defined(XEN) && defined(__i386__)
	xen_sti();
#else	
	__asm __volatile("sti");
#endif
}

#ifdef _KERNEL

#define	HAVE_INLINE_FFS
#ifdef __amd64__
#define        ffs(x)  __builtin_ffs(x)

#define	HAVE_INLINE_FLSL

static __inline int
flsl(long mask)
{
	return (mask == 0 ? mask : (int)bsrq((u_long)mask) + 1);
}

#define	HAVE_INLINE_FFSL

static __inline int
ffsl(long mask)
{
	return (mask == 0 ? mask : (int)bsfq((u_long)mask) + 1);
}

#else

static __inline int
ffs(int mask)
{
	/*
	 * Note that gcc-2's builtin ffs would be used if we didn't declare
	 * this inline or turn off the builtin.  The builtin is faster but
	 * broken in gcc-2.4.5 and slower but working in gcc-2.5 and later
	 * versions.
	 */
	 return (mask == 0 ? mask : (int)bsfl((u_int)mask) + 1);
}

#endif /* __amd64__ */

#define	HAVE_INLINE_FLS

static __inline int
fls(int mask)
{
	return (mask == 0 ? mask : (int)bsrl((u_int)mask) + 1);
}

#endif /* _KERNEL */

static __inline void
halt(void)
{
	__asm __volatile("hlt");
}

static __inline u_char
inb(u_int port)
{
	u_char	data;

	__asm __volatile("inb %w1, %0" : "=a" (data) : "Nd" (port));
	return (data);
}

static __inline u_int
inl(u_int port)
{
	u_int	data;

	__asm __volatile("inl %w1, %0" : "=a" (data) : "Nd" (port));
	return (data);
}

static __inline void
insb(u_int port, void *addr, size_t count)
{
	__asm __volatile("cld; rep; insb"
			 : "+D" (addr), "+c" (count)
			 : "d" (port)
			 : "memory");
}

static __inline void
insw(u_int port, void *addr, size_t count)
{
	__asm __volatile("cld; rep; insw"
			 : "+D" (addr), "+c" (count)
			 : "d" (port)
			 : "memory");
}

static __inline void
insl(u_int port, void *addr, size_t count)
{
	__asm __volatile("cld; rep; insl"
			 : "+D" (addr), "+c" (count)
			 : "d" (port)
			 : "memory");
}

static __inline void
invd(void)
{
	__asm __volatile("invd");
}

static __inline u_short
inw(u_int port)
{
	u_short	data;

	__asm __volatile("inw %w1, %0" : "=a" (data) : "Nd" (port));
	return (data);
}

static __inline void
outb(u_int port, u_char data)
{
	__asm __volatile("outb %0, %w1" : : "a" (data), "Nd" (port));
}

static __inline void
outl(u_int port, u_int data)
{
	__asm __volatile("outl %0, %w1" : : "a" (data), "Nd" (port));
}

static __inline void
outsb(u_int port, const void *addr, size_t count)
{
	__asm __volatile("cld; rep; outsb"
			 : "+S" (addr), "+c" (count)
			 : "d" (port));
}

static __inline void
outsw(u_int port, const void *addr, size_t count)
{
	__asm __volatile("cld; rep; outsw"
			 : "+S" (addr), "+c" (count)
			 : "d" (port));
}

static __inline void
outsl(u_int port, const void *addr, size_t count)
{
	__asm __volatile("cld; rep; outsl"
			 : "+S" (addr), "+c" (count)
			 : "d" (port));
}

static __inline void
outw(u_int port, u_short data)
{
	__asm __volatile("outw %0, %w1" : : "a" (data), "Nd" (port));
}

static __inline u_long
popcntq(u_long mask)
{
	u_long result;

	__asm __volatile("popcntq %1,%0" : "=r" (result) : "rm" (mask));
	return (result);
}

static __inline void
lfence(void)
{

	__asm __volatile("lfence" : : : "memory");
}

static __inline void
mfence(void)
{

	__asm __volatile("mfence" : : : "memory");
}

static __inline void
ia32_pause(void)
{
	__asm __volatile("pause");
}

#ifdef __amd64__
static __inline register_t
read_flags(void)
{
	register_t	rf;

	__asm __volatile("pushfq; popq %0" : "=r" (rf));
	return (rf);
}

#else
static __inline register_t
#ifdef XEN
_read_flags(void)
#else	
read_flags(void)
#endif
{
	register_t	ef;

	__asm __volatile("pushfl; popl %0" : "=r" (ef));
	return (ef);
}
#endif

static __inline uint64_t
rdmsr(u_int msr)
{
	union {
		uint32_t rv32[2];
		uint64_t rv;
	} u;
#ifdef __amd64__
	__asm __volatile("rdmsr" : "=a" (u.rv32[0]), "=d" (u.rv32[1]) : "c" (msr));
#else
	__asm __volatile("rdmsr" : "=A" (u.rv) : "c" (msr));
#endif
	return (u.rv);
}

static __inline uint64_t
rdpmc(u_int pmc)
{
	union {
		uint32_t rv32[2];
		uint64_t rv;
	} u;
#ifdef __amd64__
	__asm __volatile("rdpmc" : "=a" (u.rv32[0]), "=d" (u.rv32[1]) : "c" (pmc));
#else
	__asm __volatile("rdpmc" : "=A" (u.rv) : "c" (pmc));
#endif
	return (u.rv);
}

static __inline uint64_t
rdtsc(void)
{
	union {
		uint32_t rv32[2];
		uint64_t rv;
	} u;
#ifdef __amd64__
	__asm __volatile("rdtsc" : "=a" (u.rv32[0]), "=d" (u.rv32[1]));
#else
	__asm __volatile("rdtsc" : "=A" (u.rv));
#endif
	return (u.rv);
}

static __inline uint32_t
rdtsc32(void)
{
	uint32_t rv;

	__asm __volatile("rdtsc" : "=a" (rv) : : "edx");
	return (rv);
}

static __inline void
wbinvd(void)
{
	__asm __volatile("wbinvd");
}

static __inline void
#if defined(XEN) && defined(__i386)
_write_flags(register_t flags)
#else
write_flags(register_t flags)
#endif
{
#ifdef __amd64__
	__asm __volatile("pushq %0;  popfq" : : "r" (flags));
#else
	__asm __volatile("pushl %0; popfl" : : "r" (flags));
#endif
}

static __inline void
wrmsr(u_int msr, uint64_t newval)
{
	union {
		uint32_t newval32[2];
		uint64_t newval;
	} u;

	u.newval = newval;
	__asm __volatile("wrmsr" : : "a" (u.newval32[0]), "d" (u.newval32[1]), "c" (msr));
}

#ifdef __amd64__
#define CR_MOV "movq"
#else
#define CR_MOV "movl"
#endif
static __inline void
load_cr0(register_t data)
{
	__asm __volatile(CR_MOV " %0,%%cr0" : : "r" (data));
}

static __inline register_t
rcr0(void)
{
	register_t	data;

	__asm __volatile(CR_MOV " %%cr0,%0" : "=r" (data));
	return (data);
}

static __inline register_t
rcr2(void)
{
	register_t	data;

#if defined(XEN) && defined(__i386__)
	return (xen_rcr2());
#endif
	__asm __volatile(CR_MOV " %%cr2,%0" : "=r" (data));
	return (data);
}

static __inline void
load_cr3(register_t data)
{
#if defined(XEN) && defined(__i386__)
	xen_load_cr3(data);
#else
	__asm __volatile(CR_MOV " %0,%%cr3" : : "r" (data) : "memory");
#endif
}

static __inline register_t
rcr3(void)
{
	register_t	data;

	__asm __volatile(CR_MOV " %%cr3,%0" : "=r" (data));
	return (data);
}

static __inline void
load_cr4(register_t data)
{
	__asm __volatile(CR_MOV " %0,%%cr4" : : "r" (data));
}

static __inline register_t
rcr4(void)
{
	register_t	data;

	__asm __volatile(CR_MOV " %%cr4,%0" : "=r" (data));
	return (data);
}

static __inline register_t
rxcr(u_int reg)
{
	u_int low, high;

	__asm __volatile("xgetbv" : "=a" (low), "=d" (high) : "c" (reg));
	return (low | ((uint64_t)high << 32));
}

static __inline void
load_xcr(u_int reg, u_long val)
{
	union {
		u_int val32[2];
		u_long val;
	} u;

	u.val = val;

	__asm __volatile("xsetbv" : : "c" (reg), "a" (u.val32[0]), "d" (u.val32[1]));
}

/*
 * Global TLB flush (except for thise for pages marked PG_G)
 */
static __inline void
invltlb(void)
{
#if defined(XEN) && defined(__i386__)
	xen_tlb_flush();
#else	
	load_cr3(rcr3());
#endif
}

/*
 * TLB flush for an individual page (even if it has PG_G).
 * Only works on 486+ CPUs (i386 does not have PG_G).
 */
static __inline void
#ifdef __amd64__
invlpg(u_long addr)
#else
invlpg(u_int addr)
#endif
{
#if defined(XEN) && defined(__i386__)
	xen_invlpg(addr);
#else
	__asm __volatile("invlpg %0" : : "m" (*(char *)addr) : "memory");
#endif
}

static __inline u_short
rfs(void)
{
	u_short sel;
	__asm __volatile("movw %%fs,%0" : "=rm" (sel));
	return (sel);
}

static __inline u_short
rgs(void)
{
	u_short sel;
	__asm __volatile("movw %%gs,%0" : "=rm" (sel));
	return (sel);
}

static __inline u_short
rss(void)
{
	u_short sel;
	__asm __volatile("movw %%ss,%0" : "=rm" (sel));
	return (sel);
}

static __inline void
load_ds(u_short sel)
{
	__asm __volatile("movw %0,%%ds" : : "rm" (sel));
}

static __inline void
load_es(u_short sel)
{
	__asm __volatile("movw %0,%%es" : : "rm" (sel));
}

static __inline void
cpu_monitor(const void *addr, u_long extensions, u_int hints)
{

	__asm __volatile("monitor"
	    : : "a" (addr), "c" (extensions), "d" (hints));
}

static __inline void
cpu_mwait(u_long extensions, u_int hints)
{

	__asm __volatile("mwait" : : "a" (hints), "c" (extensions));
}

/* Functions defined only for i386. */
#ifdef __i386__
static __inline uint64_t
rgdt(void)
{
	uint64_t gdtr;
	__asm __volatile("sgdt %0" : "=m" (gdtr));
	return (gdtr);
}

static __inline uint64_t
ridt(void)
{
	uint64_t idtr;
	__asm __volatile("sidt %0" : "=m" (idtr));
	return (idtr);
}

static __inline u_short
rldt(void)
{
	u_short ldtr;
	__asm __volatile("sldt %0" : "=g" (ldtr));
	return (ldtr);
}

static __inline u_short
rtr(void)
{
	u_short tr;
	__asm __volatile("str %0" : "=g" (tr));
	return (tr);
}
#endif

#if defined(_KERNEL) && defined(__amd64__)
/* This is defined in <machine/specialreg.h> but is too painful to get to */
#ifndef	MSR_FSBASE
#define	MSR_FSBASE	0xc0000100
#endif
static __inline void
load_fs(u_short sel)
{
	/* Preserve the fsbase value across the selector load */
	__asm __volatile("rdmsr; movw %0,%%fs; wrmsr"
	    : : "rm" (sel), "c" (MSR_FSBASE) : "eax", "edx");
}

#ifndef	MSR_GSBASE
#define	MSR_GSBASE	0xc0000101
#endif
static __inline void
load_gs(u_short sel)
{
	/*
	 * Preserve the gsbase value across the selector load.
	 * Note that we have to disable interrupts because the gsbase
	 * being trashed happens to be the kernel gsbase at the time.
	 */
	__asm __volatile("pushfq; cli; rdmsr; movw %0,%%gs; wrmsr; popfq"
	    : : "rm" (sel), "c" (MSR_GSBASE) : "eax", "edx");
}
#else
/* Usable by userland and i386 kernel */
static __inline void
load_fs(u_short sel)
{
	__asm __volatile("movw %0,%%fs" : : "rm" (sel));
}

static __inline void
load_gs(u_short sel)
{
	__asm __volatile("movw %0,%%gs" : : "rm" (sel));
}
#endif

static __inline void
lidt(struct region_descriptor *addr)
{
	__asm __volatile("lidt (%0)" : : "r" (addr));
}

static __inline void
lldt(u_short sel)
{
	__asm __volatile("lldt %0" : : "r" (sel));
}

static __inline void
ltr(u_short sel)
{
	__asm __volatile("ltr %0" : : "r" (sel));
}

static __inline register_t
rdr0(void)
{
	register_t data;
	__asm __volatile(CR_MOV " %%dr0,%0" : "=r" (data));
	return (data);
}

static __inline void
load_dr0(register_t dr0)
{
	__asm __volatile(CR_MOV " %0,%%dr0" : : "r" (dr0));
}

static __inline register_t
rdr1(void)
{
	register_t data;
	__asm __volatile(CR_MOV " %%dr1,%0" : "=r" (data));
	return (data);
}

static __inline void
load_dr1(register_t dr1)
{
	__asm __volatile(CR_MOV " %0,%%dr1" : : "r" (dr1));
}

static __inline register_t
rdr2(void)
{
	uint64_t data;
	__asm __volatile(CR_MOV " %%dr2,%0" : "=r" (data));
	return (data);
}

static __inline void
load_dr2(register_t dr2)
{
	__asm __volatile(CR_MOV " %0,%%dr2" : : "r" (dr2));
}

static __inline register_t
rdr3(void)
{
	register_t data;
	__asm __volatile(CR_MOV " %%dr3,%0" : "=r" (data));
	return (data);
}

static __inline void
load_dr3(register_t dr3)
{
	__asm __volatile(CR_MOV " %0,%%dr3" : : "r" (dr3));
}

static __inline register_t
rdr4(void)
{
	register_t data;
	__asm __volatile(CR_MOV " %%dr4,%0" : "=r" (data));
	return (data);
}

static __inline void
load_dr4(register_t dr4)
{
	__asm __volatile(CR_MOV " %0,%%dr4" : : "r" (dr4));
}

static __inline register_t
rdr5(void)
{
	register_t data;
	__asm __volatile(CR_MOV " %%dr5,%0" : "=r" (data));
	return (data);
}

static __inline void
load_dr5(uint64_t dr5)
{
	__asm __volatile(CR_MOV " %0,%%dr5" : : "r" (dr5));
}

static __inline register_t
rdr6(void)
{
	register_t data;
	__asm __volatile(CR_MOV " %%dr6,%0" : "=r" (data));
	return (data);
}

static __inline void
load_dr6(register_t dr6)
{
	__asm __volatile(CR_MOV " %0,%%dr6" : : "r" (dr6));
}

static __inline register_t
rdr7(void)
{
	register_t data;
	__asm __volatile(CR_MOV " %%dr7,%0" : "=r" (data));
	return (data);
}

static __inline void
load_dr7(register_t dr7)
{
	__asm __volatile(CR_MOV " %0,%%dr7" : : "r" (dr7));
}

static __inline register_t
intr_disable(void)
{
	register_t flags;

	flags = read_flags();
	disable_intr();
	return (flags);
}

static __inline void
intr_restore(register_t flags)
{
	write_flags(flags);
}

#ifdef __i386__
static __inline u_char
read_cyrix_reg(u_char reg)
{
	outb(0x22, reg);
	return inb(0x23);
}

static __inline void
write_cyrix_reg(u_char reg, u_char data)
{
	outb(0x22, reg);
	outb(0x23, data);
}
#endif

#else /* !(__GNUCLIKE_ASM && __CC_SUPPORTS___INLINE) */

int			breakpoint(void);
u_int		bsfl(u_int mask);
u_int		bsrl(u_int mask);
void		clflush(u_long addr);
void		clts(void);
void		cpuid_count(u_int ax, u_int cx, u_int *p);
void		disable_intr(void);
void		do_cpuid(u_int ax, u_int *p);
void		enable_intr(void);
void		halt(void);
void		ia32_pause(void);
u_char		inb(u_int port);
u_int		inl(u_int port);
void		insb(u_int port, void *addr, size_t count);
void		insl(u_int port, void *addr, size_t count);
void		insw(u_int port, void *addr, size_t count);
register_t	intr_disable(void);
void		intr_restore(register_t rf);
void		invd(void);
void		invlpg(u_int addr);
void		invltlb(void);
u_short		inw(u_int port);
void		lidt(struct region_descriptor *addr);
void		lldt(u_short sel);
void		load_cr0(register_t cr0);
void		load_cr3(register_t cr3);
void		load_cr4(register_t cr4);
void		load_dr0(register_t dr0);
void		load_dr1(register_t dr1);
void		load_dr2(register_t dr2);
void		load_dr3(register_t dr3);
void		load_dr4(register_t dr4);
void		load_dr5(register_t dr5);
void		load_dr6(register_t dr6);
void		load_dr7(register_t dr7);
void		load_fs(u_short sel);
void		load_gs(u_short sel);
void		ltr(u_short sel);
void		outb(u_int port, u_char data);
void		outl(u_int port, u_int data);
void		outsb(u_int port, const void *addr, size_t count);
void		outsl(u_int port, const void *addr, size_t count);
void		outsw(u_int port, const void *addr, size_t count);
void		outw(u_int port, u_short data);
register_t	rcr0(void);
register_t	rcr2(void);
register_t	rcr3(void);
register_t	rcr4(void);
uint64_t	rdmsr(u_int msr);
uint64_t	rdpmc(u_int pmc);
register_t	rdr0(void);
register_t	rdr1(void);
register_t	rdr2(void);
register_t	rdr3(void);
register_t	rdr4(void);
register_t	rdr5(void);
register_t	rdr6(void);
register_t	rdr7(void);
uint64_t	rdtsc(void);
register_t	read_flags(void);
#ifdef __i386__
u_char	read_cyrix_reg(u_char reg);
uint64_t rgdt(void);
uint64_t ridt(void);
u_short	rldt(void);
u_short	rtr(void);
void	write_cyrix_reg(u_char reg, u_char data);
#endif
u_int	rfs(void);
u_int	rgs(void);
void	wbinvd(void);
void	write_flags(register rf);
void	wrmsr(u_int msr, uint64_t newval);

#endif	/* __GNUCLIKE_ASM && __CC_SUPPORTS___INLINE */

void	reset_dbregs(void);

#ifdef _KERNEL
int	rdmsr_safe(u_int msr, uint64_t *val);
int	wrmsr_safe(u_int msr, uint64_t newval);
#endif

#ifdef CR_MOV
#undef CR_MOV
#endif

#endif /* !_MACHINE_CPUFUNC_H_ */
