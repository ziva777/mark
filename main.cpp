#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <locale>
#include <codecvt>
#include <sstream>
#include <vector>
#include <unordered_map>

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

using Histogram = std::unordered_map<std::string, size_t>;

void 
fill_histogram(
    Histogram &histogram,
    std::string &data)
{
    std::istringstream iss{data};
    std::vector<std::string> 
        results(
            std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>());

    for (auto &&item : results) {
        histogram[item] += 1;
    }
}

void 
fill_histogram2(
    Histogram &histogram,
    std::string &data)
{
    std::istringstream iss(std::move(data));

    for (auto itr = std::istream_iterator<std::string>{iss};
        itr != std::istream_iterator<std::string>();
        ++itr) 
    {
        auto res = histogram.emplace(std::move(*itr), 0);
        auto item = res.first;
        auto exists = res.second;

        if (!exists)
            ++item->second;
        
        // std::istream_iterator<std::string> item;
        // bool created;
        // std::tie(item, created) = res;
        // std::tie(item, created) = histogram.emplace(std::move(*itr), 0);
        // auto [item, created] = histogram.emplace(std::move(*itr), 0);

        // if (!created) {
            // ++item->second;
        // }
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
    fill_histogram2(histogram, data);

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
