#ifndef __str_filter_h__
#define __str_filter_h__

#include <string>


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


#endif // __str_filter_h__
