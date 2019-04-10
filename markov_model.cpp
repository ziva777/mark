#include "markov_model.h"

#include <random>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include "misc.h"

MarkovModel::MarkovModel(
    size_t order)
: _order(order)
{
}

MarkovModel::Transitions & 
MarkovModel::transitions(
    MarkovModel::State &state)
{
    return _model_core[state];
}

void 
MarkovModel::print() const
{
    misc::StringSum str_sum{","};

    for (auto &item : _model_core) {
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
MarkovModel::cals_transitions_probs()
{
    for (auto &item : _model_core) {
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
MarkovModel::genarate(
    MarkovModel::State origin,
    size_t n) const
{
    if (origin.size() != _order) {
        throw std::logic_error("Origin state not eq to order of model!");
    }

    misc::StringSum str_sum{" "};
    std::stringstream ss;
    bool stop = false;

    ss << *origin.begin() << " ";

    while (n-- && !stop) {
        bool goon = true;

        for (auto itr = _model_core.begin(); 
            itr != _model_core.end() && goon; 
            ++itr)
        {
            auto &item = *itr;
            auto &state = item.first;
            auto &transitions = item.second;

            if (origin == state) {
                if (!transitions.empty()) {
                    ss << 
                        std::accumulate(
                            ++state.begin(), state.end(), std::string(),
                            str_sum)
                    << " ";

                    size_t tr_idx = rand() % transitions.size();
                    auto tr_itr = transitions.begin();

                    while (tr_idx--) {
                        ++tr_itr;
                    }

                    {
                        origin = std::get<STATE_ID>(*tr_itr);
                    }
                    goon = false;
                } else {
                    goon = false;
                    stop = true;
                }
            }
        }
    }

    return ss.str();
}
