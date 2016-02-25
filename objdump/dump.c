/*
** dump.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Thu Feb 25 16:39:36 2016 Adrien WERY
** Last update	Thu Feb 25 18:34:36 2016 Adrien WERY
*/

#include "elfi.h"

void    dumpLine(unsigned char *data, size_t size)
{
    size_t  i;
    size_t  j;
    char    ascii[BPL + 1];
    char    octet[4 * (BPL + 1)];

    memset(ascii, 0, BPL + 1);
    i = 0;
    j = 0;
    while (i < size)
     {
         ascii[i] = isprint(data[i]) ? data[i] : '.';
         if (i % 4 == 0 && i != 0)
            octet[j++] = ' ';
         sprintf(&(octet[j]), "%02x", data[i]);
         j += 2;
         ++i;
     }
     printf("%-36s %-16s\n", octet, ascii);
}

void    dumpMem(void *data, size_t size, size_t start_addr)
{
    size_t  i;
    size_t  div;
    size_t  nb_lines;

    i = 0;
    div = (size / BPL);
    nb_lines = div + ((size % BPL > 0) ? 1 : 0);
    while (i < nb_lines)
    {
        printf(" %04lx ", start_addr + (i * BPL));
        dumpLine(data + (i * BPL), (i >= div) ? (size % BPL) : BPL);
        ++i;
    }
}

void    displayFlags(t_elf *elf)
{
    size_t  start;
    size_t  type;

    printf("%s:     file format %s\n", elf->name, GET_ARCH(elf->type));
    if (elf->type == ELFCLASS64)
    {
        start = ((Elf64_Ehdr*)elf->data)->e_entry;
        type = ((Elf64_Ehdr*)elf->data)->e_type;
    }
    else
    {
        start = ((Elf32_Ehdr*)elf->data)->e_entry;
        type = ((Elf32_Ehdr*)elf->data)->e_type;
    }
    if (type == ET_NONE || type == ET_CORE)
        printf(DYSFLAGS, 0x0, "", start);
    else if (type == ET_REL)
        printf(DYSFLAGS, 0x11, "HAS_RELOC, HAS_SYMS", start);
    else if (type == ET_EXEC)
        printf(DYSFLAGS, 0x112, "EXEC_P, HAS_SYMS, D_PAGED", start);
    else if (type == ET_DYN)
        printf(DYSFLAGS, 0x150, "HAS_SYMS, DYNAMIC, D_PAGED", start);
}
