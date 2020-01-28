//
// Created by juice on 8/11/19.
//

#include <string>
#include <sstream>

bool if_eval(int x, int y, std::string relop){
    if (relop == "=") // idk why basic syntax is an = not ==, but it is and that is dumb
        return x == y;
    if (relop == "<=")
        return x <= y;
    if (relop == ">=")
        return x >= y;
    if (relop == ">")
        return x > y;
    if (relop == "<")
        return x < y;
}

std::string upperCase(std::string* input){
    // yes i know locale has a function for this but I don't like it
    std::stringstream out;
    for (int i=0; i<input->length(); i++){
        char c = input->at(i);
        if (c > 'Z')
            out << (char) (c - 32);
        else
            out << c;
    }
    return out.str();
}