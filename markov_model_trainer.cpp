#include "markov_model_trainer.h"

#include <iterator>
#include <sstream>


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
    MarkovModel &markov_automat)
{
    TOKENS_ITERATOR itr1, itr2;

    for (itr1 = begin; itr1 != end; ++itr1) {
        auto &state = *itr1;
        auto &transitions = markov_automat.transitions(state);

        if (++(itr2 = itr1) != end) {
            bool match = false;

            for (auto &tr : transitions) {
                MarkovModel::State &next_state = 
                    std::get<MarkovModel::STATE_ID>(tr);
                
                if (next_state == *itr2) {
                    size_t &counter = 
                        std::get<MarkovModel::CONTER_ID>(tr);

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
    MarkovModel &markov_automat)
{
    Corpus corpus = _make_corpus(std::move(data));
    Tokens tokens = _make_tokens(std::move(corpus), markov_automat.order());

    if (tokens.empty()) {
        throw std::logic_error("Corpus is empty, nothin' to learn!");
    }

    if (tokens.begin()->size() < markov_automat.order()) {
        throw std::logic_error("Order of model more than corpus!");
    }

    _make_train(tokens.begin(), tokens.end(), markov_automat);
}
