/*-
 * Copyright (c) 2012 Daniel McGregor
 * Copyright (c) 2002 Mitsuru IWASAKI
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
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
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

#ifndef _X86_ACPICA_MACHDEP_H__
#define _X86_ACPICA_MACHDEP_H__

#ifdef __amd64__

#define	ACPI_MACHINE_WIDTH             64
#define	COMPILER_DEPENDENT_INT64       long
#define	COMPILER_DEPENDENT_UINT64      unsigned long

#else

#define	ACPI_MACHINE_WIDTH             32
#define	COMPILER_DEPENDENT_INT64       long long
#define	COMPILER_DEPENDENT_UINT64      unsigned long long
#define	ACPI_USE_NATIVE_DIVIDE

#endif

void	acpi_SetDefaultIntrModel(int model);
void	acpi_cpu_c1(void);
void	*acpi_map_table(vm_paddr_t pa, const char *sig);
void	acpi_unmap_table(void *table);
vm_paddr_t acpi_find_table(const char *sig);

#endif /* _X86_ACPICA_MACHDEP_H__ */
