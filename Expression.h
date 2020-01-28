//
// Created by juice on 8/11/19.
//

#ifndef TINY_BASIC_EXPRESSION_H
#define TINY_BASIC_EXPRESSION_H


#include <string>
#include <map>

class Expression {

public:
    Expression(std::string *s, std::map<std::string, int> *m);
    std::string to_str();
    int to_int();

private:
    std::string *content;
    std::map<std::string,int> *vars;
    int parse_expression(std::string *expression);
    int parse_term(std::string *term);
    int parse_factor(std::string *factor);

};


#endif //TINY_BASIC_EXPRESSION_H
