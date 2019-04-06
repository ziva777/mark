#ifndef __pipe_h__
#define __pipe_h__

/*
    Since popen is system C call leave it in pure C.
*/

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PIPE_MODE_R      "r"
#define PIPE_MODE_W      "W"
#define PIPE_MODE_RW     "r+"

/* Pipe descriptor */

struct pipe_context {
    FILE *handle;
    int error;
    int is_open;
};

/* Pipe functions */

int  
pipe_init(
    struct pipe_context *desc,
    const char *cmd, 
    const char *mode);

void 
pipe_finit(
    struct pipe_context *desc);

char * 
pipe_read(
    struct pipe_context *desc,
    char *str, 
    int size);

int  
pipe_write(
    struct pipe_context *desc,
    const char *str);

#ifdef __cplusplus
}
#endif

#endif /* __pipe_h__ */
