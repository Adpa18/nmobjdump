/*
** file.h for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Feb 17 01:19:21 2016 Adrien WERY
** Last update	Thu Feb 25 23:40:49 2016 Adrien WERY
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
# include <locale.h>
# include <ctype.h>


# define BPL  16

# define OVER(x) (((void*)(x) > elf->data + elf->size) ? true : false)
# define LOWER(x) ((x >= 'A' && x <= 'Z') ? x - 'A' + 'a' : x)
# define R_CUSTOM(x, y) if (x) {return (y);}
# define GLOBAL(x) ((ELF64_ST_BIND(sym->st_info) == STB_GLOBAL) ? x - 32 : x)
# define GLOBAL_ST(x) ((ELF64_ST_BIND(st_info) == STB_GLOBAL) ? x - 32 : x)
# define G_SHDR64 ((Elf64_Shdr *)(elf->Shdr))
# define G_SHDR32 ((Elf32_Shdr *)(elf->Shdr))
# define G_EHDR64 ((Elf64_Ehdr *)(elf->data))
# define G_EHDR32 ((Elf32_Ehdr *)(elf->data))
# define X64(type, x, y) (type == ELFCLASS64) ? x : y
# define GET_ARCH(type) (type == ELFCLASS64) ? "elf64-x86-64" : "elf32-i386"
# define MAX(x, y) (x > y) ? x : y

typedef __SIZE_TYPE__ size_t;
typedef enum { false, true } bool;

typedef struct  s_elf
{
    int         fd;
    char        *name;
    size_t      size;
    char        type;
    void        *data;
    void        *Shdr;
    char        *shstrtab;
}               t_elf;

typedef struct  s_sym
{
    char        *name;
    size_t      value;
    char        type;
    bool        undef;
}               t_sym;

bool    initElf(t_elf *elf, char *name);
void    closeFile(t_elf *file);
bool    getFile(t_elf *file, char *name);
void    displaySym64(t_elf *elf, int max, size_t syms_ptr, void *ptr);
void    displaySym32(t_elf *elf, size_t max, size_t syms_ptr, void *ptr);
int     compare(const void *s1, const void *s2);
char    getType(char *name);

void    dumpMem(void *data, size_t size, size_t start_addr);
void    displayFlags(t_elf *elf);

#endif /* !ELFI_H */
