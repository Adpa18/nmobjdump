/*
** sym32.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Mon Feb 22 21:47:49 2016 Adrien WERY
** Last update	Thu Feb 25 13:32:01 2016 Adrien WERY
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
                syms[nb_syms].type = GLOBAL(getSectionType32(&G_SHDR32[sym->st_shndx],
                        &elf->shstrtab[G_SHDR32[sym->st_shndx].sh_name]));
            ++nb_syms;
        }
        ++i;
    }
    dumpSym32(syms, nb_syms);
}

void    display32(t_elf *elf)
{
    Elf32_Ehdr  *Ehdr;
    size_t      i;

    Ehdr = elf->data;
    elf->Shdr = elf->data + Ehdr->e_shoff;
    elf->shstrtab = elf->data + G_SHDR32[Ehdr->e_shstrndx].sh_offset;
    i = 0;
    while (i < Ehdr->e_shnum)
    {
        if (G_SHDR32[i].sh_type == SHT_SYMTAB)
        {
            displaySym32(elf, G_SHDR32[i].sh_size / G_SHDR32[i].sh_entsize,
                (size_t)elf->data + G_SHDR32[i].sh_offset,
                elf->data + G_SHDR32[G_SHDR32[i].sh_link].sh_offset);

        }
        ++i;
    }
}