#ifndef __markov_model_trainer_h__
#define __markov_model_trainer_h__

#include <list>
#include <string>
#include <cstddef>

#include "markov_model.h"


class MarkovModelTrainer {
public:
    using Atom = MarkovModel::Atom;
    using Corpus = std::list<Atom>;
    using CorpusItr = Corpus::iterator;
    using Tokens = std::list<MarkovModel::State>;
    using TokensItr = Tokens::iterator;

    void train(
            std::string &&data,
            MarkovModel &markov_automat);

private:
    Corpus _make_corpus(
            std::string &&data);
    Tokens _make_tokens(
            Corpus &&corpus,
            size_t window_size);

    template<typename TOKENS_ITERATOR>
    void _make_train(
            TOKENS_ITERATOR begin,
            TOKENS_ITERATOR end,
            MarkovModel &markov_automat);
};


#endif // __markov_model_trainer_h__
