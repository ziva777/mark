#ifndef __curl_pipe_h__
#define __curl_pipe_h__

#include <string>
#include <tuple>

#include "pipe.h"

/*
    Class to read curl output via pipe.
*/

class CurlPipe {
public:
    enum class ResultCodes {
        OK, OPEN_FAIL, READ_FAIL 
    };
    using Values = std::tuple<
        ResultCodes, 
        std::string
    >;

    CurlPipe();
    ~CurlPipe();

    Values get(std::string url);

private:
    static const int IO_BUFF_SIZE;
    static const std::string CURL_CMD;
    static const std::string CURL_ARG;

    std::string _build_url(std::string url);
};


#endif // __curl_pipe_h__
