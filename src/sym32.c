/*
** sym32.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Mon Feb 22 21:47:49 2016 Adrien WERY
** Last update	Thu Feb 25 16:09:30 2016 Adrien WERY
*/

#include "elfi.h"

char    getSectionType32(Elf32_Shdr *st, char *name)
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

char    getSymType32(Elf32_Sym *sym)
{
    if (sym->st_shndx == SHN_UNDEF)
    {
        R_CUSTOM(ELF32_ST_BIND(sym->st_info) != STB_WEAK, 'U');
        R_CUSTOM(ELF32_ST_TYPE(sym->st_info) == STT_OBJECT, GLOBAL('v'));
        return (GLOBAL('w'));
    }
    R_CUSTOM(sym->st_shndx == SHN_ABS, GLOBAL('a'));
    R_CUSTOM(sym->st_shndx == SHN_COMMON, 'C');
    R_CUSTOM(ELF32_ST_TYPE(sym->st_info) == STT_GNU_IFUNC, 'i');
    R_CUSTOM(ELF32_ST_TYPE(sym->st_info) == (1 << 13), 'I');
    R_CUSTOM(ELF32_ST_BIND(sym->st_info) == STB_WEAK, 'W');
    R_CUSTOM(ELF32_ST_TYPE(sym->st_info) == STT_FUNC, GLOBAL('t'));
    return ('?');
}

void    dumpSym32(t_sym *syms, size_t nb_syms)
{
    size_t  i;

    i = 0;
    qsort(syms, nb_syms, sizeof(t_sym), compare);
    while (i < nb_syms)
    {
        if (syms[i].value)
            printf("%08x %c %s\n", (int)syms[i].value,
                syms[i].type, syms[i].name);
        else
            printf("         %c %s\n", syms[i].type, syms[i].name);
        ++i;
    }
}

void    displaySym32(t_elf *elf, size_t max, size_t syms_ptr, void *ptr)
{
    Elf32_Sym   *sym;
    t_sym       syms[max];
    char        *name;
    size_t      i;
    size_t      nb_syms;

    nb_syms = 0;
    i = 0;
    while (i < max)
    {
        sym = &((Elf32_Sym *)(syms_ptr))[i];
        name = (char *)ptr + sym->st_name;
        if (sym->st_shndx != SHN_ABS && name[0] != '\0')
        {
            syms[nb_syms].name = name;
            syms[nb_syms].value = (sym->st_shndx == SHN_UNDEF)?0:sym->st_value;
            if ((syms[nb_syms].type = getSymType32(sym)) == '?')
                syms[nb_syms].type = GLOBAL(getSectionType32(
                        &G_SHDR32[sym->st_shndx],
                        &elf->shstrtab[G_SHDR32[sym->st_shndx].sh_name]));
            ++nb_syms;
        }
        ++i;
    }
    dumpSym32(syms, nb_syms);
}
