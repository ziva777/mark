#include "curl_pipe.h"

const int CurlPipe::IO_BUFF_SIZE = 1024;
const std::string CurlPipe::CURL_CMD = "curl";
const std::string CurlPipe::CURL_ARG = "-s";


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
    struct pipe_context pipe;
    ResultCodes code;
    std::string data;
    char buff[IO_BUFF_SIZE];
    std::string cmd = _build_url(url);


    if (pipe_init(&pipe, cmd.c_str(), PIPE_MODE_R)) {
        while (pipe_read(&pipe, buff, IO_BUFF_SIZE)) {
            data.append(buff);
        }

        pipe_finit(&pipe);

        if (pipe.code == 0) {
            code = ResultCodes::OK;
        } else {
            code = ResultCodes::READ_FAIL;
        }
    } else {
        code = ResultCodes::OPEN_FAIL;
    }

    return std::make_tuple(code, data);
}

std::string 
CurlPipe::_build_url(
    std::string url)
{
    std::string cmd;
    cmd += CURL_CMD + " ";
    cmd += CURL_ARG + " ";
    cmd += url + " ";
    return cmd;
}
