/*
** sym64.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Mon Feb 22 21:47:49 2016 Adrien WERY
** Last update	Wed Feb 24 23:17:02 2016 Adrien WERY
*/

#include "elfi.h"

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

void    dumpSym64(t_sym *syms, size_t nb_syms)
{
    size_t  i;

    i = 0;
    qsort(syms, nb_syms, sizeof(t_sym), compare);
    while (i < nb_syms)
    {
        if (syms[i].value)
            printf(DUMPSYM64, syms[i].value, syms[i].type, syms[i].name);
        else
            printf(DUMPSYMNS64, syms[i].type, syms[i].name);
        ++i;
    }
}

void    displaySym64(size_t syms_ptr, void *ptr, size_t max)
{
    Elf64_Sym   *symbol;
    t_sym       syms[max];
    char        *name;
    size_t      i;
    size_t      nb_syms;

    nb_syms = 0;
    i = 0;
    while (i < max)
    {
        symbol = &((Elf64_Sym *)(syms_ptr))[i];
        name = (char *)ptr + symbol->st_name;
        if (symbol->st_shndx != SHN_ABS && name[0] != '\0')
        {
            syms[nb_syms].name = name;
            syms[nb_syms].type = '?';
            if (symbol->st_shndx == SHN_UNDEF)
                syms[nb_syms].value = 0;
            else
                syms[nb_syms].value = symbol->st_value;
            ++nb_syms;
        }
        ++i;
    }
    dumpSym64(syms, nb_syms);
}

void    display64(t_elf *elf)
{
    Elf64_Ehdr  *Ehdr;
    Elf64_Shdr  *Shdr;
    size_t      i;

    Ehdr = elf->data;
    Shdr = elf->data + Ehdr->e_shoff;
    i = 0;
    while (i < Ehdr->e_shnum)
    {
        if (Shdr[i].sh_type == SHT_SYMTAB)
        {
            displaySym64((size_t)elf->data + Shdr[i].sh_offset,
                    elf->data + Shdr[Shdr[i].sh_link].sh_offset,
                    Shdr[i].sh_size / Shdr[i].sh_entsize);

        }
        ++i;
    }
}
