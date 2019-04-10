#ifndef __misc_h__
#define __misc_h__

#include <string>
#include <cstddef>
#include <sstream>
#include <unordered_map>


namespace misc {
    struct StringSum {
        std::string spacer;
        std::string operator()(
                const std::string &a,
                const std::string &b) const
        {
            if (a.empty())
                return b;
            return a + "," + b;
        }
    };

    // using Histogram = std::unordered_map<
    //     std::string, 
    //     size_t
    // >;

    // template<
    //     typename ITERATOR
    // >
    // size_t 
    // fill_histogram_from_itr(
    //     Histogram &histogram,
    //     ITERATOR begin, 
    //     ITERATOR end)
    // {
    //     size_t n = 0;
    //     ITERATOR itr;

    //     for (itr = begin; itr != end; ++itr) {
    //         auto res = histogram.emplace(std::move(*itr), 0);
    //         auto item = res.first;

    //         ++item->second;
    //         ++n;
    //     }

    //     return n;
    // }

    // size_t  
    // fill_histogram(
    //     Histogram &histogram,
    //     std::string &&data)
    // {
    //     std::istringstream iss(std::move(data));
    //     return fill_histogram_from_itr(
    //         histogram, 
    //         std::istream_iterator<std::string>{iss},
    //         std::istream_iterator<std::string>()
    //     );
    // }
}


#endif // __misc_h__
