#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <locale>
#include <codecvt>

#include "curl_pipe.h"


class StrFilter {
public:
    void process(
            std::string &data,
            std::string locale_name);
private:
    void _remove_punct(std::string &data);
    void _to_lower(
            std::string &data,
            std::string locale_name);
};


void 
StrFilter::process(
    std::string &data,
    std::string locale_name)
{
    _remove_punct(data);
    _to_lower(data, locale_name);
}

void 
StrFilter::_remove_punct(
    std::string &data)
{
    std::replace_if(
            data.begin(), data.end(),
            [] (int c) {
                return std::ispunct(c);
            }, 
            ' ');
}

void 
StrFilter::_to_lower(
    std::string &data,
    std::string locale_name)
{
    using namespace std;

    locale utf8_loc(locale_name);
    wstring_convert<
        codecvt_utf8<wchar_t>
    > conv;
    wstring ws = conv.from_bytes(data);

    transform(
        ws.begin(), ws.end(),
        ws.begin(),
        [&](wchar_t c) {
            return (wchar_t)tolower(c, utf8_loc);
        }
    );

    data = conv.to_bytes(ws);
}


int main()
{
    CurlPipe pipe;
    CurlPipe::ResultCodes code;
    std::string data;
    StrFilter filter;

    std::tie(code, data) = pipe.get("file:///Users/ziva/Desktop/dub.txt");
    filter.process(data, "ru_RU.UTF-8");

    std::cout << data;

    return EXIT_SUCCESS;
}
