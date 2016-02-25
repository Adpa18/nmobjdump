/*
** main.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Feb 17 00:48:10 2016 Adrien WERY
** Last update	Thu Feb 25 11:54:40 2016 Adrien WERY
*/

#include <stdio.h>
#include "elfi.h"

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
}
