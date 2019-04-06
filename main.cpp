#include <iostream>
#include <list>
#include <string>

#include "pipe.h"


/*
    Since popen is system C call leave it in pure C.
*/

// #include <stdio.h>
// #include <ctype.h>

// #define PIPE_MODE_R      "r"
// #define PIPE_MODE_W      "W"
// #define PIPE_MODE_RW     "r+"

// #define IO_BUFF_SIZE     20

// char *
// lower_filter(
//     char *s)
// {
//     char *p;

//     for (p = s; *p; ++p) 
//             *p = tolower(*p);

//     return s;
// }

// char *
// punctuation_filter(
//     char *s)
// {
//     char punctuation_marks[] = "'[](){}<>:,-!?\"/\\.&@#$%^*+`~\n";
//     char *p;
//     size_t n = sizeof(punctuation_marks) / sizeof(punctuation_marks[0]);
//     size_t i;

//     for (p = s; *p; ++p) {
//         for (i = 0; i != n; ++i) {
//             if (*p == punctuation_marks[i]) {
//                 *p = ' ';
//             }
//         }
//     }

//     return s;
// }

// void
// pipe_read(
//     FILE *pipe)
// {
//     char buff[IO_BUFF_SIZE];
//     // char tail[IO_BUFF_SIZE] = {'\0'};
//     char *p;
//     char *tmp;
        
//     while (fgets(buff, IO_BUFF_SIZE, pipe) != NULL) {
//         punctuation_filter(
//             lower_filter(
//                 buff));
//         // printf("%s\n", buff);
//         {
//             p = strtok_r(buff, " ", &tmp);

//             while (p != NULL) {
//                 printf("\n%s", p);
//                 p = strtok_r(NULL, " ", &tmp);
//             }
//         }
//     }
// }

// void
// pipe(
//     const char *cmd, 
//     const char *mode)
// {
//     FILE *p = popen(cmd, mode);

//     if (p != NULL) {
//         pipe_read(p);
//         pclose(p);    
//     }
// }


//////////////////////////////
// class Tokenizer {
// public:
//     void start(std::string url);
//     void stop();
//     std::string next();
// private:
// };

// void 
// Tokenizer::start(
//     std::string url)
// {
// }

// std::string 
// Tokenizer::next()
// {
// }

// void 
// Tokenizer::stop()
// {
// }
//////////////////////////////


int main()
{
    constexpr size_t BUFF_SIZE = 40;
    struct pipe_context p = {0};
    char buff[BUFF_SIZE];

    if (pipe_init(&p, "man ls", PIPE_MODE_R)) {
        while (pipe_read(&p, buff, BUFF_SIZE) != NULL) {
            printf("%s", buff);
        }

        pipe_finit(&p);
    }
    // pipe("man ls", PIPE_MODE_R);

    // using Urls = std::list<std::string>;
    // Urls urls = {
    //     "file:///Users/ziva/Desktop/dub.txt",
    //     "file:///Users/ziva/Desktop/belkin.txt",
    // };

    // for (auto &&url : urls) {
    //     Tokenizer tokenizer(url);

    //     for (auto &&token : tokenizer(url)) {
    //         std::cout << token << " ";
    //     }
    // }
    return EXIT_SUCCESS;
}

// using UrlsList = std::list<std::string>;

// UrlsList urls = {
//     "file:///Users/ziva/Desktop/dub.txt",
//     "file:///Users/ziva/Desktop/belkin.txt",
// };

// bool
// is_char_valid(char c)
// {
//     if (c == '.' || c == ',')
//         return false;
//     return true;
// }

// void 
// pipe(
//     std::string cmd, 
//     std::string mode)
// {
//     char buff[100];
//     char c;
//     std::string s;
//     int n, i;
//     FILE *p = popen(cmd.c_str(), mode.c_str());

//     if (p != NULL) {
//         while ((n = fgets(buff, sizeof(buff), p)) != NULL) {
//             s.clear();

//             for (i = 0; i != n; ++i) {
//                 c = buff[i];

//                 if (is_char_valid(c))
//                     s.push_back(c);
//             }
//             // s = buff;
//             // s.replace(s.begin(), s.end(), )
//             std::cout << s;
//         }
//     }
// }

// void
// curl_pipe(std::string url)
// {
//     std::string cmd;
//     cmd += "curl ";
//     cmd += url;

//     pipe(cmd, "r");
// }

// void
// read_urls(const UrlsList &urls) 
// {
//     for (auto &&url : urls) {
//         curl_pipe(url);
//         // std::cout << url << "\n";
//     }
// }

// int 
// main(
//     int argc, char 
//     const *argv[]) 
// {
//     read_urls(urls);
//     // auto lambda = [](auto x, auto y) {return x + y;};
//     // std::cout << "Hi there! It's " << lambda(1, 3) << " here." << std::endl;
//     // std::cout << "Hi there! It's " << lambda(1, 3) << " here." << std::endl;

//     return 0;
// }