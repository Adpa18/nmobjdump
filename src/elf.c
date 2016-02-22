/*
** elf.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Feb 17 16:15:24 2016 Adrien WERY
** Last update	Mon Feb 22 23:11:23 2016 Adrien WERY
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

int         strcmpcase_espace(char *s1, char *s2, char esc)
{
    char    a;
    char	b;

    R_CUSTOM(!s2, 0);
    R_CUSTOM(!s1, 1);
    while (s1[0] && s2[0])
    {
        while (s1[0] == esc && ++s1);
        while (s2[0] == esc && ++s2);
        a = LOWER(s1[0]);
        b = LOWER(s2[0]);
        R_CUSTOM(a != b, a - b);
        ++s1;
        ++s2;
    }
    return (a - b);
}

char    *getSymName(t_elf *elf, int pos, char *symstr)
{
    if (OVER(symstr + pos) || (symstr[pos] == '\0'))
        return (NULL);
  return (&symstr[pos]);
}

bool    initElf(t_elf *elf, char *name)
{
    if (!getFile(elf, name) || !validELF(elf))
        return (false);
    return (true);
}
