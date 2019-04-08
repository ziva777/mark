#include "str_filter.h"

#include <locale>
#include <codecvt>
#include <algorithm>

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
    std::locale utf8_loc(locale_name);
    std::wstring_convert<
        std::codecvt_utf8<wchar_t>
    > conv;
    std::wstring ws = conv.from_bytes(data);

    std::transform(
        ws.begin(), ws.end(),
        ws.begin(),
        [&](wchar_t c) {
            return (wchar_t)std::tolower(c, utf8_loc);
        }
    );

    data = conv.to_bytes(ws);
}
