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
#include <random>
#include <ctime>
#include <cstdlib>

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


class MarkovAutomat {
public:
    struct StateHash;

    static const int STATE_ID = 0;
    static const int PROBABILITY_ID = 1;
    static const int CONTER_ID = 2;

    using Atom = std::string;
    using State = std::list<Atom>;
    using Transition = std::tuple<
        State,  // next state
        float,  // probability of transition
        size_t  // counter
    >;
    using Transitions = std::list<
        Transition
    >;
    using AutomatCore = std::unordered_map<
        State,
        Transitions,
        StateHash
    >;

    struct StateHash {
        size_t operator()(const State &state) const 
        {
            std::string s = std::accumulate(
                state.begin(), state.end(), std::string(),
                [](
                    const std::string &a,
                    const std::string &b
                ) {
                    return a + '_' + b;
                }
            );
            return std::hash<std::string>()(s);
        };
    };

    Transitions & transitions(
            State &state);

    void print() const;
    void cals_transitions_probs();

    std::string genarate(
            State origin,
            size_t n) const;

private:
    AutomatCore _automat_core;
};


MarkovAutomat::Transitions & 
MarkovAutomat::transitions(
        MarkovAutomat::State &state)
{
    return _automat_core[state];
}

void 
MarkovAutomat::print() const
{
    /* Print out automat */
    auto str_sum = [](
        const std::string &a,
        const std::string &b
    ) {
        if (a.empty())
            return b;
        return a + "," + b;
    };

    for (auto &item : _automat_core) {
        auto &state = item.first;
        auto &transitions = item.second;

        std::string k = std::accumulate(
            state.begin(), state.end(), std::string(),
            str_sum
        );

        std::string v;
        for (auto &transition : transitions) {
            auto &next_state = std::get<STATE_ID>(transition);
            auto &count = std::get<CONTER_ID>(transition);
            auto &prob = std::get<PROBABILITY_ID>(transition);

            v = std::accumulate(
                next_state.begin(), next_state.end(), v,
                str_sum
            );
            v += ":" + std::to_string(count);
            v += ":" + std::to_string(prob);
        }

        std::cout << "[" << k << "] : <" << v << ">";
        std::cout << std::endl;
    }
}

void 
MarkovAutomat::cals_transitions_probs()
{
    for (auto &item : _automat_core) {
        auto &state = item.first;
        auto &transitions = item.second;
        size_t n = transitions.size();

        for (auto &transition : transitions) {
            auto &count = std::get<CONTER_ID>(transition);
            auto &prob = std::get<PROBABILITY_ID>(transition);

            prob = static_cast<float>(count) / static_cast<float>(n);
        }
    }
}

std::string 
MarkovAutomat::genarate(
    MarkovAutomat::State origin,
    size_t n) const
{
    auto str_sum = [](
        const std::string &a,
        const std::string &b
    ) {
        if (a.empty())
            return b;
        return a + " " + b;
    };

    bool stop = false;

    while (n-- && !stop) {
        bool goon = true;

        for (auto itr = _automat_core.begin(); 
            itr != _automat_core.end() && goon; 
            ++itr)
        {
            auto &item = *itr;
            auto &state = item.first;
            auto &transitions = item.second;

            if (origin == state) {
                std::cout <<
                    std::accumulate(
                        state.begin(), state.end(), std::string(),
                        str_sum)
                    << " ";

                if (!transitions.empty()) {
                    size_t tr_idx = rand() % transitions.size();
                    auto tr_itr = transitions.begin();

                    while (tr_idx--) {
                        ++tr_itr;
                    }

                    origin = std::get<STATE_ID>(*tr_itr);
                    goon = false;
                } else {
                    goon = false;
                    stop = true;
                }
            }
        }
    }

    std::string res;
    return res;
}


class MarkovModelTrainer {
public:
    using Atom = MarkovAutomat::Atom;
    using Corpus = std::list<Atom>;
    using CorpusItr = Corpus::iterator;
    using Tokens = std::list<MarkovAutomat::State>;
    using TokensItr = Tokens::iterator;

    MarkovModelTrainer(
            size_t order = 1);

    void train(
            std::string &&data,
            MarkovAutomat &markov_automat);
    // bool load(
    //         std::string file);
    // bool save(
    //         std::string file) const;
    // void print() const;

    // std::string genarate(
    //         MarkovAutomat::State origin,
    //         size_t n) const;

private:
    size_t _order;
    // MarkovAutomat _markov_automat;

    Corpus _make_corpus(
            std::string &&data);
    Tokens _make_tokens(
            Corpus &&corpus,
            size_t window_size);

    template<typename TOKENS_ITERATOR>
    void _make_train(
            TOKENS_ITERATOR begin,
            TOKENS_ITERATOR end,
            MarkovAutomat &markov_automat);
};

MarkovModelTrainer::MarkovModelTrainer(
    size_t order)
: _order{order}
{
}

/* [1, 2, ,3 ,4] -> [[1, 2], [2, 3], [3, 4]] */
MarkovModelTrainer::Tokens
MarkovModelTrainer::_make_tokens(
    Corpus &&corpus,
    size_t window_size)
{
    CorpusItr begin = corpus.begin();
    CorpusItr end = corpus.end();
    CorpusItr itr1, itr2;
    Tokens tokens;
    size_t i;

    for (itr1 = begin; itr1 != end; ++itr1) {
        Corpus chunk;

        for (itr2 = itr1, i = 0; 
            itr2 != end && i != window_size; 
            ++itr2, ++i) 
        {
            chunk.emplace_back(*itr2);
        }

        tokens.emplace_back(std::move(chunk));
    }

    return tokens;
}

MarkovModelTrainer::Corpus  
MarkovModelTrainer::_make_corpus(
    std::string &&data)
{
    using Iss = std::istringstream;
    using IssIter = std::istream_iterator<std::string>;

    Iss iss(std::move(data));
    return Corpus(std::make_move_iterator(IssIter{iss}),
                  std::make_move_iterator(IssIter()));
}

template<typename TOKENS_ITERATOR>
void 
MarkovModelTrainer::_make_train(
    TOKENS_ITERATOR begin,
    TOKENS_ITERATOR end,
    MarkovAutomat &markov_automat)
{
    TOKENS_ITERATOR itr1, itr2;

    for (itr1 = begin; itr1 != end; ++itr1) {
        auto &state = *itr1;
        auto &transitions = markov_automat.transitions(state);

        if (++(itr2 = itr1) != end) {
            bool match = false;

            for (auto &tr : transitions) {
                MarkovAutomat::State &next_state = 
                    std::get<MarkovAutomat::STATE_ID>(tr);
                
                if (next_state == *itr2) {
                    size_t &counter = 
                        std::get<MarkovAutomat::CONTER_ID>(tr);

                    ++counter;
                    match = true;
                    break;
                }
            }

            if (!match) {
                transitions.emplace_back(
                    std::make_tuple(*itr2, 0.0f, 1)); 
            }
        }
    }

    markov_automat.cals_transitions_probs();
}

void 
MarkovModelTrainer::train(
    std::string &&data,
    MarkovAutomat &markov_automat)
{
    Corpus corpus = _make_corpus(std::move(data));
    Tokens tokens = _make_tokens(std::move(corpus), _order);

    if (tokens.empty()) {
        throw std::logic_error("Corpus is empty, nothin' to learn!");
    }

    if (tokens.begin()->size() < _order) {
        throw std::logic_error("Order of model more than corpus!");
    }

    _make_train(tokens.begin(), tokens.end(), markov_automat);
}

// void 
// MarkovModelTrainer::print() const
// {
//     _markov_automat.print();
// }

// std::string 
// MarkovModelTrainer::genarate(
//     MarkovAutomat::State &origin,
//     size_t n) 
// {
//     return ";"
//     // return _markov_automat.genarate(origin, n);
// }


int main()
{
    const char locale[] = "ru_RU.UTF-8";
    const size_t order = 1;
    CurlPipe::ResultCode code;
    CurlPipe pipe;
    std::string data;
    std::string file;
    StrFilter filter;
    MarkovModelTrainer markov(order);
    MarkovAutomat markov_automat;

    std::srand(std::time(nullptr));

    file = "file:///home/marina/programming/git/mark/case0.txt";
    std::tie(code, data) = pipe.get(file);

    if (code == CurlPipe::ResultCode::OK) {
        filter.process(data, locale);
        markov.train(std::move(data), markov_automat);
    } else {
        throw std::domain_error("Can't read file: " + file);
    }

    markov_automat.print();

    MarkovAutomat::State s{{"one"}};
    std::cout << markov_automat.genarate(s, 10) << std::endl;

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
