/*
** sym64.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Mon Feb 22 21:47:49 2016 Adrien WERY
** Last update	Mon Feb 22 23:16:34 2016 Adrien WERY
*/

#include "elfi.h"

void    sortSym64(t_elf *elf, Elf64_Sym **syms, char *symstr)
{
    int     i;
    bool    ok;
    char	*s1;
    char	*s2;
    Elf64_Sym   *tmp;

    ok = true;
    while (ok)
    {
        ok = false;
        i = 1;
        while (syms[i])
        {
            s1 = getSymName(elf, syms[i - 1]->st_name, symstr);
            s2 = getSymName(elf, syms[i]->st_name, symstr);
            if (strcmpcase_espace(s1, s2, '_') > 0)
            {
                tmp = syms[i - 1];
                syms[i - 1] = syms[i];
                syms[i] = tmp;
                ok = true;
            }
            i++;
        }
    }
}

void    dumpSym64(t_elf *elf, Elf64_Sym *sym, char *symstr)
{
    char    type = '?';

    if (OVER(symstr + sym->st_name) || symstr[sym->st_name] == '\0'
        || sym->st_info == STT_NOTYPE || sym->st_info == STT_FILE)
        return;
    if (sym->st_value)
        printf(DUMPSYM64, sym->st_value, type, getSymName(elf, sym->st_name, symstr));
    else
        printf(DUMPSYMNS64, type, getSymName(elf, sym->st_name, symstr));

}

void    displaySym64(t_elf *elf, void *ptr, char *symstr)
{
    size_t      i;
    Elf64_Sym   **syms;

    i = 0;
    if (!(syms = malloc((elf->size / elf->symSize + 1) * sizeof(void*))))
        return;
    while (elf->symSize * i < elf->size)
    {
        syms[i] = (Elf64_Sym*)(ptr + (elf->symSize * i));
        i++;
    }
    syms[i] = NULL;
    sortSym64(elf, syms, symstr);
    i = 0;
    while (syms[i])
        dumpSym64(elf, syms[i++], symstr);
    free(syms);

}

void    getSym64(t_elf *elf)
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
            displaySym64(elf, elf->data + Shdr[i].sh_offset,
                    elf->data + Shdr[Shdr[i].sh_link].sh_offset);
        }
        ++i;
    }
}
