#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstring>

#define BUFSIZE 100

std::string keywords[] = {
    "define",
    "macro",
    "end",
};

enum TokenType {
    TOK_DEFINE,
    TOK_MACRO,
    TOK_END,

    TOK_TEXT,
};


class Token {
public:
    enum TokenType m_type;
    std::string m_value;

    Token(enum TokenType type, std::string value)
        : m_type(type)
        , m_value(value)
    {}
};



std::vector<char>
read_file(std::string filename) {

    std::ifstream file(filename);
    std::vector<char> lines;

    char c;
    while (c != EOF) {
        c = file.get();
        lines.push_back(c);
    }

    file.close();

    return lines;
}


int main(int argc, char **argv) {
    (void) argc; (void) argv;

    auto src = read_file("file.script");
    auto src_size = src.size();

    std::vector<Token> tokenlist;


    for (size_t current=0; current<src_size; ++current) {

        char c = src[current];

        enum TokenType type;
        std::string value;

        switch (c) {

            case '$': {

                size_t skip = 0;
                for (size_t j=current ;; ++j) {

                    if (src[j] == ' ') {
                        skip = j;
                        break;
                    }
                }
                char buf[BUFSIZE];
                std::strncpy(buf, &src[current], current-skip);
                std::cout << buf << std::endl;




            } break;

            default: {
                type = TOK_TEXT;
            } break;
        }

        Token newtok(type, value);

    }


    return 0;

}
