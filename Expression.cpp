//
// Created by imcjohn on 8/7/2019.
//

#include "Expression.h"

#include <algorithm>
#include <iostream>

Expression::Expression(std::string *s, std::map<std::string,int> *m){
    content = s;
    vars = m;
}

int Expression::parse_expression(std::string *expression){
    expression->erase(remove(expression->begin(),expression->end(), ' '),expression->end()); // TODO: Handle whitespace better
    int len = expression->length();
    int start = 0;
    int symb = 1;
    int val = 0;
    int in_parens = 0;
    for (int i=0; i<len; i++){
        char c = expression->at(i);
        if (c == '(')
            in_parens+=1;
        else if (c == ')')
            in_parens-=1;
        if ((c == '+' || c == '-') && !in_parens){
            std::string term = expression->substr(start,i-start);
            val = val + (symb * parse_term(&term));
            symb = c == '-' ? -1 : 1;
            start = i+1;
        }
        else if (i == len-1) {
            std::string term = expression->substr(start, len); // read till end
            val = val + (symb * parse_term(&term));
        }
    }
    return val;
}

int Expression::parse_term(std::string *term) {
    if (term->empty()) return 0;
    int len = term->length();
    int start = 0;
    bool symb = true; // true for mult, false for div
    int val = 1;
    int in_parens = 0;
    for (int i=0; i<len; i++){
        char c = term->at(i);
        if (c == '(')
            in_parens+=1;
        else if (c == ')')
            in_parens-=1;
        if ((c == '*' || c == '/') && !in_parens){
            std::string factor = term->substr(start,i-start);
            int res = parse_factor(&factor);
            val = val * (symb ? res : 1/res);
            symb = (c == '*');
            start = i+1;
        }
        else if (i == len-1) {
            std::string factor = term->substr(start, len); // read till end
            int res = parse_factor(&factor);
            val = val * (symb ? res : 1/res);
        }
    }
    return val;
}

int Expression::parse_factor(std::string *factor){
    char zero = factor->at(0);
    if (zero == '('){ // handle expressions recursively
        factor->erase(0,1);
        factor->pop_back();
        return parse_expression(factor);
    }
    else if (zero >= 'A' && zero <= 'Z'){ // numerical variable
        return vars->at(*factor);
    }
    else { // treat it as a number because why not
        return std::strtol(factor->c_str(), NULL, 10);
    }
}

std::string Expression::to_str(){
    return std::to_string(to_int());
}

int Expression::to_int(){
    return parse_expression(content);
}