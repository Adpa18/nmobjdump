/*
** main.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Thu Feb 25 15:30:06 2016 Adrien WERY
** Last update	Thu Feb 25 23:38:35 2016 Adrien WERY
*/

#include <stdio.h>
#include "elfi.h"

bool    checkType(size_t type, size_t flags, char *name, size_t e_type)
{
    static bool sh = true;

    R_CUSTOM(!name, false);
    if (e_type == ET_DYN && sh && !strcmp(".shstrtab", name))
    {
        sh = false;
        return (true);
    }
    return ((type != SHT_NULL && type != SHT_NOBITS
        && type != SHT_STRTAB
        && type != SHT_SYMTAB && type != SHT_RELA)
        || (type == SHT_STRTAB && flags & SHF_ALLOC)
        ||  (type == SHT_RELA && flags & SHF_ALLOC));
}

void    display64(t_elf *elf)
{
    Elf64_Ehdr  *Ehdr;
    char        *name;
    size_t      i;

    Ehdr = elf->data;
    elf->Shdr = elf->data + Ehdr->e_shoff;
    elf->shstrtab = elf->data + G_SHDR64[Ehdr->e_shstrndx].sh_offset;
    i = 0;
    while (i < Ehdr->e_shnum)
    {
        if (checkType(G_SHDR64[i].sh_type, G_SHDR64[i].sh_flags,
            (name = &elf->shstrtab[G_SHDR64[i].sh_name]), Ehdr->e_type)
        && G_SHDR64[i].sh_size > 0)
        {
            printf("Contents of section %s:\n", name);
            dumpMem(elf->data + G_SHDR64[i].sh_offset,
                    G_SHDR64[i].sh_size, G_SHDR64[i].sh_addr);
        }
        ++i;
    }
}

void    display32(t_elf *elf)
{
    Elf32_Ehdr  *Ehdr;
    char        *name;
    size_t      i;

    Ehdr = elf->data;
    elf->Shdr = elf->data + Ehdr->e_shoff;
    elf->shstrtab = elf->data + G_SHDR32[Ehdr->e_shstrndx].sh_offset;
    i = 0;
    while (i < Ehdr->e_shnum)
    {
        if (checkType(G_SHDR32[i].sh_type, G_SHDR32[i].sh_flags,
        (name = &elf->shstrtab[G_SHDR32[i].sh_name]), Ehdr->e_type)
        && G_SHDR32[i].sh_size > 0)
        {
            printf("Contents of section %s:\n", name);
            dumpMem(elf->data + G_SHDR32[i].sh_offset,
                    G_SHDR32[i].sh_size, G_SHDR32[i].sh_addr);
        }
        ++i;
    }
}

void    display(char *s)
{
    t_elf   elf;

    if (!(initElf(&elf, s)))
        return;
    printf("\n");
    displayFlags(&elf);
    printf("\n");
    X64(elf.type, display64(&elf), display32(&elf));
    closeFile(&elf);
}

int     main(int ac, char **av)
{
    size_t  i;

    i = 0;
    setlocale(LC_COLLATE, "fr_FR.utf8");
    while (++i < (size_t)ac)
        display(av[i]);
    if (i == 1)
        display("./a.out");
}
