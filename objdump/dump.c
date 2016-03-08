/*
** dump.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Thu Feb 25 16:39:36 2016 Adrien WERY
** Last update	Thu Feb 25 23:43:09 2016 Adrien WERY
*/

#include "elfi.h"

size_t  hexLength(int value)
{
    int len;

    len = 1;
    while (value > 15)
    {
        ++len;
        value /= 16;
    }
  return (len);
}

void    dumpLine(unsigned char *data, size_t size)
{
    size_t  i;
    i = 0;
    while (i < BPL)
     {
         if (i < size)
            printf("%02x", (unsigned) (data[i]));
        else
            printf("  ");
        if ((i & 3) == 3)
            printf(" ");
         ++i;
     }
     printf(" ");
     i = 0;
     while (i < BPL)
     {
         if (i < size)
            printf("%c", isprint(data[i]) ? data[i] : '.');
   	    else
            printf(" ");
        ++i;
     }
     printf("\n");
}

void    dumpMem(void *data, size_t size, size_t start_addr)
{
    size_t  i;
    size_t  div;
    size_t  nb_lines;
    static char    format[7] = " %04lx ";

    i = 0;
    div = (size / BPL);
    nb_lines = div + ((size % BPL > 0) ? 1 : 0);
    format[3] = (MAX(4, hexLength(start_addr + size))) + 48;
    while (i < nb_lines)
    {
        printf(format, start_addr + (i * BPL));
        dumpLine(data + (i * BPL), (i >= div) ? (size % BPL) : BPL);
        ++i;
    }
}

void    displayFlags(t_elf *elf)
{
    size_t  start;
    size_t  type;

    printf("%s:     file format %s\n", elf->name, GET_ARCH(elf->type));
    start = X64(elf->type, G_EHDR64->e_entry, G_EHDR32->e_entry);
    type = X64(elf->type, G_EHDR64->e_type, G_EHDR32->e_type);
    if (elf->type == ELFCLASS64)
        printf("architecture: i386:x86-64, flags ");
    else
        printf("architecture: i386, flags ");
    if (type == ET_NONE || type == ET_CORE)
        printf("0x%08x:\n%s\n", 0x0, "");
    else if (type == ET_REL)
        printf("0x%08x:\n%s\n", 0x11, "HAS_RELOC, HAS_SYMS");
    else if (type == ET_EXEC)
        printf("0x%08x:\n%s\n", 0x112, "EXEC_P, HAS_SYMS, D_PAGED");
    else if (type == ET_DYN)
        printf("0x%08x:\n%s\n", 0x150, "HAS_SYMS, DYNAMIC, D_PAGED");
    X64(elf->type, printf("start address 0x%016lx\n", start),
        printf("start address 0x%08lx\n", start));
}
