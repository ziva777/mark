#include <stdexcept>
#include <iostream>
#include <string>
#include <ctime>
#include <list>

#include "misc.h"
#include "curl_pipe.h"
#include "str_filter.h"
#include "markov_model.h"
#include "markov_model_trainer.h"


// class MarkovModelSerializer {
// public:
//     std::pair<bool, MarkovModel> model load(
//             std::string file);
//     bool save(
//             std::string file,
//             const MarkovModel &model) const;
// private:
// };


int main()
{
    const char locale[] = "ru_RU.UTF-8";
    const size_t order = 2;
    CurlPipe::ResultCode code;
    CurlPipe pipe;
    std::string data;
    std::string file;
    StrFilter filter;
    MarkovModelTrainer markov;
    MarkovModel markov_automat(order);

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

    MarkovModel::State s{{"one"}, {"fish"}};
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
