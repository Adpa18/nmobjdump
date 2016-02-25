/*
** sym64.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Mon Feb 22 21:47:49 2016 Adrien WERY
** Last update	Thu Feb 25 15:27:42 2016 Adrien WERY
*/

#include "elfi.h"

char    getSectionType64(Elf64_Shdr *st, char *name)
{
    R_CUSTOM(st->sh_flags & SHF_EXECINSTR, 't');
    R_CUSTOM(st->sh_flags & SHF_EXECINSTR, 't');
    if (st->sh_type == SHT_PROGBITS)
    {
        R_CUSTOM(st->sh_flags & SHF_ALLOC && !(st->sh_flags & SHF_WRITE), 'r');
        return ('d');
    }
    R_CUSTOM(st->sh_type == SHT_DYNAMIC, 'd');
    R_CUSTOM(st->sh_type == SHT_NOBITS, 'b');
    R_CUSTOM(st->sh_type == SHT_NULL && st->sh_type == SHT_ALPHA_DEBUG, 'N');
    R_CUSTOM(st->sh_type != SHT_NOBITS && st->sh_type == SHT_PROGBITS &&
      !(st->sh_flags & SHF_WRITE), 'n');
    return (getType(name));
}

char    getSymType64(Elf64_Sym *sym)
{
    if (sym->st_shndx == SHN_UNDEF)
    {
        R_CUSTOM(ELF64_ST_BIND(sym->st_info) != STB_WEAK, 'U');
        R_CUSTOM(ELF64_ST_TYPE(sym->st_info) == STT_OBJECT, GLOBAL('v'));
        return (GLOBAL('w'));
    }
    R_CUSTOM(sym->st_shndx == SHN_ABS, GLOBAL('a'));
    R_CUSTOM(sym->st_shndx == SHN_COMMON, 'C');
    R_CUSTOM(ELF64_ST_TYPE(sym->st_info) == STT_GNU_IFUNC, 'i');
    R_CUSTOM(ELF64_ST_TYPE(sym->st_info) == (1 << 13), 'I');
    R_CUSTOM(ELF64_ST_BIND(sym->st_info) == STB_WEAK, 'W');
    R_CUSTOM(ELF64_ST_TYPE(sym->st_info) == STT_FUNC, GLOBAL('t'));
    return ('?');
}

void    dumpSym64(t_sym *syms, size_t nb_syms)
{
    size_t  i;

    i = 0;
    qsort(syms, nb_syms, sizeof(t_sym), compare);
    while (i < nb_syms)
    {
        if (!syms[i].undef)
            printf("%016lx %c %s\n", syms[i].value, syms[i].type, syms[i].name);
        else
            printf("                 %c %s\n", syms[i].type, syms[i].name);
        ++i;
    }
}

void    displaySym64(t_elf *elf, int max, size_t syms_ptr, void *ptr)
{
    Elf64_Sym   *sym;
    t_sym       syms[max];
    char        *name;
    int         i;
    size_t      nb_syms;

    nb_syms = 0;
    i = -1;
    while (++i < max)
    {
        sym = &((Elf64_Sym *)(syms_ptr))[i];
        name = (char *)ptr + sym->st_name;
        if (sym->st_shndx != SHN_ABS && name[0] != '\0')
        {
            syms[nb_syms].name = name;
            syms[nb_syms].undef = (sym->st_shndx == SHN_UNDEF);
            syms[nb_syms].value = sym->st_value;
            if ((syms[nb_syms].type = getSymType64(sym)) == '?')
                syms[nb_syms].type = GLOBAL(getSectionType64(
                    &G_SHDR64[sym->st_shndx],
                    &elf->shstrtab[G_SHDR64[sym->st_shndx].sh_name]));
            ++nb_syms;
        }
    }
    dumpSym64(syms, nb_syms);
}

void    display64(t_elf *elf)
{
    Elf64_Ehdr  *Ehdr;
    size_t      i;

    Ehdr = elf->data;
    elf->Shdr = elf->data + Ehdr->e_shoff;
    elf->shstrtab = elf->data + G_SHDR64[Ehdr->e_shstrndx].sh_offset;
    i = 0;
    while (i < Ehdr->e_shnum)
    {
        if (G_SHDR64[i].sh_type == SHT_SYMTAB)
        {
            displaySym64(elf, G_SHDR64[i].sh_size / G_SHDR64[i].sh_entsize,
                (size_t)elf->data + G_SHDR64[i].sh_offset,
                elf->data + G_SHDR64[G_SHDR64[i].sh_link].sh_offset);

        }
        ++i;
    }
}
