#include <iostream>
#include <string>
#include <vector>
#include <list>

#include <sstream>
#include <iterator>
#include <unordered_map>

#include "curl_pipe.h"
#include "str_filter.h"

using Histogram = std::unordered_map<
    std::string, 
    size_t
>;

template<
    typename ITERATOR
>
size_t 
fill_histogram_from_itr(
    Histogram &histogram,
    ITERATOR begin, 
    ITERATOR end)
{
    size_t n = 0;
    ITERATOR itr;

    for (itr = begin; itr != end; ++itr) {
        auto res = histogram.emplace(std::move(*itr), 0);
        auto item = res.first;

        ++item->second;
        ++n;
    }

    return n;
}

size_t  
fill_histogram(
    Histogram &histogram,
    std::string &&data)
{
    std::istringstream iss(std::move(data));
    return fill_histogram_from_itr(
        histogram, 
        std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>()
    );
}


class MarkovChain {
public:
    using Chain = std::vector<std::string>;

    MarkovChain(
        size_t order = 1);

    void train(std::string &&data);
    bool load(std::string file);
    bool save(std::string file);

    Chain random_seq();
    Chain generate_seq(Chain &seq, size_t n);

private:
    size_t _order;
};

MarkovChain::MarkovChain(
    size_t order)
: _order{order}
{
}

void 
MarkovChain::train(
    std::string &&data)
{
    // using StreamIter = std::istream_iterator<std::string>;
    // using MoveStreamIter = std::move_iterator<StreamIter>;
    // using Tokens = std::list<std::string>;
    // using TokensIter = Tokens::iterator;

    // std::istringstream iss(std::move(data));
    // StreamIter begin{iss}, end;
    // Tokens tokens;

    // /*std::copy(
    //     MoveIter(begin),
    //     MoveIter(end),
    //     tokens.begin()
    // );*/

    // TokensIter itr1, itr2, itr3;

    // for (itr1 = begin; itr1 != end; ++itr) {
    //     tokens.emplace_back(*itr);
    // }

    // for (auto &token : tokens) {
    //     std::cout << token << " ";
    // }
    // std::cout << std::endl;

    // for (auto itr1 = tokens.begin(); itr != tokens.end(); ++itr1) {

    // }

    // for (itr; itr != end; ++itr) {
    // }

    // using Iter = std::istream_iterator<std::string>;

    // std::istringstream iss(std::move(data));
    // Iter itr{iss}, end;
    // Chain window;
    // size_t i = 0;

    // // fill the window
    // while (i++ != _order && itr != end)
    //     window.emplace_back(*itr++);

    // if (i != _order) {
    //     // Nothin' to learn
    //     return;
    // }

    // for (size_t j = 0; j != _order; ++j) {
    //     std::cout << window[j] << " ";
    // }
    
    // if (itr != end)
    //     std::cout << "->" << *itr << std::endl;
    // else
    //     std::cout << "->" << "$" << std::endl;

    // while (itr != end) {
    //     for (size_t j = 1; j < _order; ++j) {
    //         window[j-1] = window[j];
    //     }

    //     window[_order-1] = *itr++;

    //     for (size_t j = 0; j != _order; ++j) {
    //         std::cout << window[j] << " ";
    //     }

    //     if (itr != end)
    //         std::cout << "->" << *itr << std::endl;
    //     else
    //         std::cout << "->" << "$" << std::endl;
    // }

    // std::istringstream iss(std::move(data));
    // Iter itr_up{iss}, itr_down, end;
    // Iter itr;
    // bool up = true;
    // size_t i = 0;

    // for (itr_up; itr_up != end; ++itr_up) {
    //     std::cout << *itr_up << " ";
    // }
    // std::cout << "\n";

    // for (itr_down; itr_down != end; ++itr_down) {
    //     std::cout << *itr_down << " ";
    // }
    // std::cout << "\n";
    
    /*for (; itr_up != end && itr_down != end; (up ? ++itr_up : ++itr_down)) {
        itr = (up ? itr_up : itr_down);
        i = 0;

        for (itr; itr != end && i != _order; ++itr, ++i) {
            std::cout << *itr << " ";
        }
        std::cout << "\n";      

        up = !up;
    }*/
    // Iter begin{iss}, end;
    // Iter itr, itr2, itr3;

    // for (itr = begin; itr != end; ++itr) {
    //     // std::cout << *itr << "\n";
    //     // Chain key(_order);
    //     // std::string value = "$";
    //     size_t i = 0;

    //     for (itr2 = itr; itr2 != end /*&& i != _order*/; ++itr2, ++i) {
    //         std::cout << *itr2 << " ";
    //     //     key[i] = *itr2;
    //     }
    //     std::cout << "\n";

    //     // // itr3 = 

    //     // if (itr2 != end) {
    //     //     itr3 = itr2;
    //     //     if (itr3 != end) {
    //     //         value = *itr3;
    //     //     }
    //     // }


    //     // std::cout << " -> " << value << "\n";
        
    // }
}

int main()
{
    const char locale[] = "ru_RU.UTF-8";
    const size_t order = 2;
    CurlPipe::ResultCode code;
    CurlPipe pipe;
    std::string data;
    StrFilter filter;
    Histogram histogram;
    MarkovChain markov(order);

    std::tie(code, data) = 
        pipe.get(
            "file:///home/marina/programming/git/mark/case0.txt");
    filter.process(data, locale);
    markov.train(std::move(data));
    // fill_histogram(histogram, std::move(data));

    // std::tie(code, data) = 
    //     pipe.get(
    //         "file:///home/marina/programming/git/mark/case1.txt");
    // filter.process(data, locale);
    // fill_histogram(histogram, std::move(data));

    /*std::tie(code, data) = 
        pipe.get(
            "file:///Users/ziva/Desktop/dub.txt");
    filter.process(data, locale);
    fill_histogram(histogram, data);*/

    /*std::tie(code, data) = 
        pipe.get(
            "file:///Users/ziva/Desktop/belkin.txt");
    filter.process(data, locale);
    fill_histogram(histogram, data);*/
    
    {
        size_t i = 0;
        for (auto const &x : histogram) {
            // if (x.second > 1)
                std::cout
                    << ++i << " - " 
                    << x.first << ":\t" << x.second
                    << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
