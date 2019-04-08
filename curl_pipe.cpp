#include "curl_pipe.h"

const int CurlPipe::IO_BUFF_SIZE = 1024;
const char CurlPipe::CURL_CMD[] = "curl";
const char CurlPipe::CURL_ARG[] = "-s";


CurlPipe::CurlPipe()
{
}

CurlPipe::~CurlPipe()
{
}

CurlPipe::Values  
CurlPipe::get(
    std::string url)
{
#   ifdef STATIC_IO_BUFFER
    static
#   endif
    char buff[IO_BUFF_SIZE];

    std::string cmd = _build_url(url);
    std::string data;
    ResultCode code;
    Pipe pipe;

    if (pipe.open(cmd)) {
        while (pipe.read(buff, IO_BUFF_SIZE)) {
            data.append(buff);
        }

        pipe.close();

        if (pipe.is_success()) {
            code = ResultCode::OK;
        } else {
            code = ResultCode::READ_FAIL;
        }
    } else {
        code = ResultCode::OPEN_FAIL;
    }

    return std::make_tuple(code, data);
}

std::string 
CurlPipe::_build_url(
    std::string url)
{
    std::string cmd;
    cmd += std::string(CURL_CMD) + " ";
    cmd += std::string(CURL_ARG) + " ";
    cmd += url + " ";
    return cmd;
}
