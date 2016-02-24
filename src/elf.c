/*
** elf.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Feb 17 16:15:24 2016 Adrien WERY
** Last update	Wed Feb 24 23:14:19 2016 Adrien WERY
*/

#include "elfi.h"

bool    validELF(t_elf *elf)
{
    unsigned char   *magic;

    if (elf->size < sizeof(Elf32_Ehdr))
        return (false);
    magic = (unsigned char *)elf->data;
    if (!(magic[EI_MAG0] == ELFMAG0 && magic[EI_MAG1] == ELFMAG1
        && magic[EI_MAG2] == ELFMAG2 && magic[EI_MAG3] == ELFMAG3))
        return (false);
    if (magic[EI_CLASS] == ELFCLASS64)
     {
         if (elf->size < sizeof(Elf64_Ehdr))
             return (false);
         elf->type = ELFCLASS64;
         elf->symSize = sizeof(Elf64_Sym);
     }
     else
     {
         elf->type = ELFCLASS32;
         elf->symSize = sizeof(Elf32_Sym);
     }
    return (true);
}

bool    initElf(t_elf *elf, char *name)
{
    if (!getFile(elf, name) || !validELF(elf))
        return (false);
    return (true);
}
