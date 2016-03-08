/*
** main.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Feb 17 00:48:10 2016 Adrien WERY
** Last update	Tue Mar 08 22:21:24 2016 Adrien WERY
*/

#include "elfi.h"

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

void    display(char *s)
{
    t_elf   elf;

    if (!(initElf(&elf, s)))
        return;
    if (elf.type == ELFCLASS64)
        display64(&elf);
    else
        display32(&elf);
    closeFile(&elf);
}

int     main(int ac, char **av)
{
    size_t  i;

    i = 0;
    setlocale(LC_COLLATE, "fr_FR.utf8");
    while (++i < (size_t)ac)
    {
        if (ac > 2)
            printf("\n%s:\n", av[i]);
        display(av[i]);
    }
    if (i == 1)
        display("./a.out");
    return (0);
}
