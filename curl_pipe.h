#ifndef __curl_pipe_h__
#define __curl_pipe_h__

#include <string>
#include <tuple>

#include "pipe.h"

/*
    Class to read curl output via pipe.
*/

#define STATIC_IO_BUFFER


class CurlPipe {
public:
    enum class ResultCode {
        OK, OPEN_FAIL, READ_FAIL 
    };
    using Values = std::tuple<
        ResultCode, 
        std::string
    >;

    CurlPipe();
    ~CurlPipe();

    Values get(std::string url);

private:
    static const int IO_BUFF_SIZE;
    static const char CURL_CMD[];
    static const char CURL_ARG[];
    static const char CURL_CONV[];

    std::string _build_url(std::string url);
};


#endif // __curl_pipe_h__
