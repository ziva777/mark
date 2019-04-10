#ifndef __pipe_h__
#define __pipe_h__

#include <string>


class Pipe {
public:
    enum class Mode {
        R, W
    };

    Pipe();
    ~Pipe();

    int error() const { return _error; }
    int code() const { return _code; }
    bool is_open() const { return _pipe; }
    bool is_success() const { return _code == 0; }

    bool 
    open(
        std::string cmd, 
        Mode mode = Mode::R);
    void 
    close();

    char * 
    read(
        char *buff, 
        int size);
    int 
    write(
        const char *buff);
private:
    FILE *_pipe {nullptr};
    int _error {0};
    int _code {0};
};


#endif /* __pipe_h__ */
