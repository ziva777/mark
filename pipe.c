#include "pipe.h"

#include <errno.h>

int  
pipe_init(
    struct pipe_context *desc,
    const char *cmd, 
    const char *mode) 
{
    desc->handle = popen(cmd, mode);
    desc->is_open = desc->handle != NULL;
    desc->error = 0;

    return desc->is_open;
}

void 
pipe_finit(
    struct pipe_context *desc) 
{
    if (desc->is_open)
        pclose(desc->handle);

    desc->is_open = 0;
    desc->error = 0;
}

char * 
pipe_read(
    struct pipe_context *desc,
    char *str, 
    int size)
{
    char *p = fgets(str, size, desc->handle);
    desc->error = errno;
    return p;
}

int 
pipe_write(
    struct pipe_context *desc,
    const char *str)
{
    int r = fputs(str, desc->handle);
    desc->error = errno;
    return r;
}
