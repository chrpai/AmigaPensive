#ifndef NOVIA_REGISTERS_H
#define NOVIA_REGISTERS_H

#define __aligned
#define __asm
#define __regargs
#define __saveds
#define __stdargs
#define REG(r)    register __ ## r
#define GNUCREG(r)
#define SAVEDS
#define ASM
#define REGARGS
#define STDARGS
#define ALIGNED
#include <dos/dos.h>
#define SEGLISTPTR BPTR
//  typedef struct SegList * SEGLISTPTR;
#endif