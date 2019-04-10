#ifndef __markov_model_h__
#define __markov_model_h__

#include <list>
#include <tuple>
#include <string>
#include <cstddef>
#include <numeric>
#include <unordered_map>


class MarkovModel {
public:
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
    
    MarkovModel(
            size_t order = 1);

    size_t order() const { return _order; }

    Transitions & transitions(
            State &state);

    void print() const;
    void cals_transitions_probs();

    std::string genarate(
            State origin,
            size_t n) const;

private:
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

    using ModelCore = std::unordered_map<
        State,
        Transitions,
        StateHash
    >;

    ModelCore _model_core;
    size_t _order;
};


#endif // __markov_model_h__
