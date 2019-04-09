#include <iostream>
#include <string>
#include <vector>
#include <list>

#include <sstream>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <stdexcept>
#include <tuple>

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


class MarkovModel {
public:
    using Atom = std::string;
    using Corpus = std::list<Atom>;
    using Window = std::list<Atom>;
    using Tokens = std::list<Window>;

    using CorpusItr = Corpus::iterator;
    using WindowItr = Window::iterator;
    using TokensItr = Tokens::iterator;

    struct WindowHash {
        size_t operator()(const Window &w) const {
            auto str_sum = [](
                const std::string &a,
                const std::string &b
            ) {
                return a + '_' + b;
            };
            std::string s = std::accumulate(
                w.begin(), w.end(), std::string(),
                str_sum
            );
            return std::hash<std::string>()(s);
        };
    };

    /*
        token : [
            <token, count>,
            <token, count>, ...
        ], ...
    */
    using Transition = std::tuple<
        Window, 
        size_t
    >;
    using Transitions = std::list<
        Transition
    >;
    using Automat = std::unordered_map<
        Window,
        Transitions,
        WindowHash
    >;

    MarkovModel(
        size_t order = 1);

    void train(
        std::string &&data);
    bool load(
        std::string file);
    bool save(
        std::string file);

private:
    size_t _order;
    Automat _automat;

    Corpus _make_corpus(
        std::string &&data);
    Tokens _make_tokens(
        Corpus &&corpus,
        size_t window_size);
};

MarkovModel::MarkovModel(
    size_t order)
: _order{order}
{
}

/* [1, 2, ,3 ,4] -> [[1, 2], [2, 3], [3, 4]] */
MarkovModel::Tokens
MarkovModel::_make_tokens(
    Corpus &&corpus,
    size_t window_size)
{
    using Tokens = std::list<
        std::list<std::string>
    >;
    using Corpus = std::list<
        std::string
    >;
    using Chunk = std::list<
        std::string
    >;

    Tokens tokens;
    Corpus::iterator begin = corpus.begin();
    Corpus::iterator end = corpus.end();
    Corpus::iterator itr1, itr2;
    size_t i;

    for (itr1 = begin; itr1 != end; ++itr1) {
        Chunk chunk;

        for (itr2 = itr1, i = 0; itr2 != end && i != window_size; ++itr2, ++i) {
            chunk.emplace_back(*itr2);
        }

        tokens.emplace_back(std::move(chunk));
    }

    corpus.clear();
    return tokens;
}

MarkovModel::Corpus  
MarkovModel::_make_corpus(
    std::string &&data)
{
    using Iss = std::istringstream;
    using IssIter = std::istream_iterator<std::string>;

    Iss iss(std::move(data));
    return Corpus(std::make_move_iterator(IssIter{iss}),
                  std::make_move_iterator(IssIter()));
}

void 
MarkovModel::train(
    std::string &&data)
{
    Corpus corpus = _make_corpus(std::move(data));
    Tokens tokens = _make_tokens(std::move(corpus), _order);

    if (tokens.empty()) {
        throw std::logic_error("Corpus is empty, nothin' to learn!");
    }

    if (tokens.begin()->size() < _order) {
        throw std::logic_error("Order of model more than corpus!");
    }

    // for (auto &token : tokens) {
    //     for (auto &word : token)
    //         std::cout << word << " ";
    //     std::cout << std::endl;
    // }

    {
        TokensItr begin = tokens.begin();
        TokensItr end = tokens.end();
        TokensItr itr1, itr2;

        for (itr1 = begin; itr1 != end; ++itr1) {
            Transitions &trs = _automat[*itr1];

            if (++(itr2 = itr1) != end) {
                bool match = false;

                for (auto &tr : trs) {
                    Window &tr_window = std::get<0>(tr);
                    size_t &count = std::get<1>(tr);

                    if (tr_window == *itr2) {
                        ++count;
                        match = true;
                        break;
                    }
                }

                if (!match) {
                    trs.emplace_back(
                        std::make_tuple(*itr2, 1)
                    );
                }
            }
        }
    }

    {
        /* Print out automat */
        auto str_sum = [](
            const std::string &a,
            const std::string &b
        ) {
            if (a.empty())
                return b;
            return a + ',' + b;
        };


        for (auto &item : _automat) {
            auto &w_key = item.first;
            auto &values = item.second;

            std::string k = std::accumulate(
                w_key.begin(), w_key.end(), std::string(),
                str_sum
            );
            std::string v;
            {
                for (auto &value : values) {
                    auto &window = std::get<0>(value);
                    auto count = std::get<1>(value);
                    v = std::accumulate(
                        window.begin(), window.end(), v,
                        str_sum
                    );
                    v += ":" + std::to_string(count);
                }
            }

            std::cout << "[" << k << "] : <" << v << ">";
            std::cout << std::endl;
        }
    }
}

int main()
{
    const char locale[] = "ru_RU.UTF-8";
    const size_t order = 1;
    CurlPipe::ResultCode code;
    CurlPipe pipe;
    std::string data;
    std::string file;
    StrFilter filter;
    MarkovModel markov(order);

    file = "file:///home/marina/programming/git/mark/case0.txt";
    std::tie(code, data) = pipe.get(file);

    if (code == CurlPipe::ResultCode::OK) {
        filter.process(data, locale);
        markov.train(std::move(data));
    } else {
        throw std::domain_error("Can't read file: " + file);
    }

    // file = "file:///home/marina/programming/git/mark/case1.txt";
    // std::tie(code, data) = pipe.get(file);

    // if (code == CurlPipe::ResultCode::OK) {
    //     filter.process(data, locale);
    //     markov.train(std::move(data));
    // } else {
    //     throw std::domain_error("Can't read file: " + file);
    // }

    // Histogram histogram;
    // fill_histogram(histogram, std::move(data));
    // {
    //     size_t i = 0;
    //     for (auto const &x : histogram) {
    //         // if (x.second > 1)
    //             std::cout
    //                 << ++i << " - " 
    //                 << x.first << ":\t" << x.second
    //                 << std::endl;
    //     }
    // }

    return EXIT_SUCCESS;
}
