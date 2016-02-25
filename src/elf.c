/*
** elf.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Feb 17 16:15:24 2016 Adrien WERY
** Last update	Thu Feb 25 13:41:10 2016 Adrien WERY
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
     }
     else
         elf->type = ELFCLASS32;
    return (true);
}

bool    initElf(t_elf *elf, char *name)
{
    if (!getFile(elf, name) || !validELF(elf))
        return (false);
    return (true);
}

char    *to_lower(const char *s1)
{
  char      *dest;
  size_t    i;
  size_t    size;

  i = 0;
  size = strlen(s1);
  if (!(dest = malloc(sizeof(char) * size)))
    return (NULL);
  while (i  < size)
  {
      dest[i] = LOWER(s1[i]);
      ++i;
  }
  return (dest);
}

int     compare(const void *s1, const void *s2)
{
    int     ret;

    char *ss1 = to_lower(((t_sym *)(s1))->name);
    char *ss2 = to_lower(((t_sym *)(s2))->name);
    ret = strcoll(ss1, ss2);
    free(ss1);
    free(ss2);
  return (ret);
}

char    getType(char *name)
{
    R_CUSTOM(!strncmp(name, ".bss", 4), 'b');
    R_CUSTOM(!strncmp(name, "code", 4), 't');
    R_CUSTOM(!strncmp(name, ".data", 5), 'd');
    R_CUSTOM(!strncmp(name, "*DEBUG*", 7), 'N');
    R_CUSTOM(!strncmp(name, ".debug", 6), 'N');
    R_CUSTOM(!strncmp(name, ".drectve", 8), 'i');
    R_CUSTOM(!strncmp(name, ".edata", 6), 'e');
    R_CUSTOM(!strncmp(name, ".fini", 5), 't');
    R_CUSTOM(!strncmp(name, ".idata", 6), 'i');
    R_CUSTOM(!strncmp(name, ".init", 5), 't');
    R_CUSTOM(!strncmp(name, ".pdata", 6), 'p');
    R_CUSTOM(!strncmp(name, ".rdata", 6), 'r');
    R_CUSTOM(!strncmp(name, ".rodata", 7), 'r');
    R_CUSTOM(!strncmp(name, ".sbss", 5), 's');
    R_CUSTOM(!strncmp(name, ".scommon", 8), 'c');
    R_CUSTOM(!strncmp(name, ".sdata", 6), 'g');
    R_CUSTOM(!strncmp(name, ".text", 5), 't');
    R_CUSTOM(!strncmp(name, "vars", 4), 'd');
    R_CUSTOM(!strncmp(name, "zerovars", 8), 'b');
    return ('?');
}
