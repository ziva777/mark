#include "pipe.h"

#include <errno.h>
#include <sys/types.h> 
#include <sys/wait.h>

int  
pipe_init(
    struct pipe_context *desc,
    const char *cmd, 
    const char *mode) 
{
    desc->handle = popen(cmd, mode);
    desc->is_open = desc->handle != NULL;
    desc->error = 0;
    desc->code = 0;

    return desc->is_open;
}

void 
pipe_finit(
    struct pipe_context *desc) 
{
    desc->code = pclose(desc->handle);
    desc->is_open = 0;
    desc->error = errno;
}

char * 
pipe_read(
    struct pipe_context *desc,
    char *s, 
    int size)
{
    char *p = fgets(s, size, desc->handle);
    desc->error = errno;
    return p;
}

int 
pipe_write(
    struct pipe_context *desc,
    const char *s)
{
    int r = fputs(s, desc->handle);
    desc->error = errno;
    return r;
}
