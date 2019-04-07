#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <locale>
#include <codecvt>
#include <sstream>
#include <vector>
#include <map>

#include "curl_pipe.h"


class StrFilter {
public:
    void process(
            std::string &data,
            std::string locale_name);
private:
    void _remove_punct(
            std::string &data);
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

using Histogram = std::map<std::string, size_t>;

void 
fill_histogram(
    Histogram &histogram,
    std::string &data)
{
    using namespace std;

    istringstream iss{data};
    vector<string> 
        results(
            istream_iterator<string>{iss},
            istream_iterator<string>());

    for (auto &&item : results) {
        histogram[item] += 1;
    }
}

int main()
{
    CurlPipe pipe;
    CurlPipe::ResultCodes code;
    std::string data;
    StrFilter filter;
    Histogram histogram;

    std::tie(code, data) = pipe.get("file:///Users/ziva/Desktop/dub.txt");
    filter.process(data, "ru_RU.UTF-8");
    fill_histogram(histogram, data);

    /*std::tie(code, data) = pipe.get("file:///Users/ziva/Desktop/belkin.txt");
    filter.process(data, "ru_RU.UTF-8");
    fill_histogram(histogram, data);*/
    
    {
        size_t i = 0;
        for (auto const &x : histogram) {
            std::cout
                << ++i << " - " 
                << x.first << ':' << x.second
                << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
