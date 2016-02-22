/*
** file.h for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Feb 17 01:19:21 2016 Adrien WERY
** Last update	Mon Feb 22 23:08:01 2016 Adrien WERY
*/

#ifndef ELFI_H
# define ELFI_H

# include <elf.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <stdio.h>

# define DUMPSYM64 "%016lx %c %s\n"
# define DUMPSYM32 "%08x %c %s\n"
# define DUMPSYMNS64 "                 %c %s\n"
# define DUMPSYMNS32 "         %c %s\n"

# define x64(type, x, y) ((type == ELFCLASS64) ? x : y)
# define OVER(x) (((void*)(x) > elf->data + elf->size) ? true : false)
# define LOWER(x) ((x >= 'A' && x <= 'Z') ? x - 'A' + 'a' : x)
# define R_CUSTOM(x, y) if (x) {return (y);}

typedef __SIZE_TYPE__ size_t;
typedef enum { false, true } bool;

typedef struct  s_elf
{
    int         fd;
    char        *name;
    size_t      size;
    size_t      symSize;
    char        type;
    void        *data;
    void        *Ehdr;
    void        *Shdr;
    char        *strtab;
}               t_elf;

bool    initElf(t_elf *elf, char *name);
void    closeFile(t_elf *file);
bool    getFile(t_elf *file, char *name);
void    getSym64(t_elf *elf);
char    *getSymName(t_elf *elf, int pos, char *symstr);
int     strcmpcase_espace(char *s1, char *s2, char esc);

#endif /* !ELFI_H */
