#include "pipe.h"

#include <errno.h>
#include <sys/types.h> 
#include <sys/wait.h>

#include <unordered_map>

namespace {
    static const char READ_MODE[] = "r";
    static const char WRITE_MODE[] = "w";
}


Pipe::Pipe()
{
}

Pipe::~Pipe()
{
    close();
}

bool Pipe::open(
        std::string cmd, 
        Pipe::Mode mode)
{
    std::unordered_map<Mode, std::string> modes = {
        {Mode::R, READ_MODE},
        {Mode::W, WRITE_MODE},
    };
    _pipe = popen(cmd.c_str(), modes[mode].c_str());
    _error = errno;
    _code = 0;
}

void 
Pipe::close()
{
    if (_pipe) {
        _code = pclose(_pipe);
        _error = errno;
    }
}

char * 
Pipe::read(
    char *buff, 
    int size)
{
    char *s = fgets(buff, size, _pipe);
    _error = errno;
    return s;
}

int 
Pipe::write(
    const char *buff)
{
    int ret = fputs(buff, _pipe);
    _error = errno;
    return ret;
}
