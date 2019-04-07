#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <locale>
#include <codecvt>


#include "curl_pipe.h"

void 
remove_punct(
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
to_lower(
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

void 
beautify_str(
    std::string &data,
    std::string locale_name)
{
    remove_punct(data);
    to_lower(data, locale_name);
}

int main()
{
    CurlPipe pipe;
    CurlPipe::ResultCodes code;
    std::string data;

    std::tie(code, data) = pipe.get("file:///Users/ziva/Desktop/dub.txt");

    beautify_str(data, "ru_RU.UTF-8");

    std::cout << data;


    return EXIT_SUCCESS;
}
