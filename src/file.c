/*
** file.c for PSU_2015_nmobjdump
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Feb 17 16:17:18 2016 Adrien WERY
** Last update	Mon Feb 22 15:16:30 2016 Adrien WERY
*/

#include "elfi.h"

void    closeFile(t_elf *file)
{
    munmap(file->data, file->size);
    close(file->fd);
}

bool    getFile(t_elf *file, char *name)
{
    struct stat f_stat;

    if ((file->fd = open(name, O_RDONLY)) == -1)
        return (false);
    file->name = name;
    if (fstat(file->fd, &f_stat) < 0)
    {
        close(file->fd);
        return (false);
    }
    file->size = f_stat.st_size;
    file->data = mmap(NULL, file->size, PROT_READ, MAP_PRIVATE, file->fd, 0);
    return (true);
}
