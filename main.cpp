#include <iostream>
#include <map>
#include <array>
#include <string>
#include <sstream>
#include <vector>
#include "Expression.h"
#include "helpers.h"
using namespace std;

// Tiny Basic Constants
#define MAX_LINES_ALLOWED 32768 // max number of lines allowed according to the Tiny Basic spec

// Core variables used by interpreter
int position = 0;
bool running = false;
bool sequential_running = false;
array<string, MAX_LINES_ALLOWED> lines;
vector<int> sub_stack;
map <string, int> vars;
bool parse_and_run(string line);

void clear_interpreter(){
    // set everything to default states
    position = 0;
    vars.clear();
    lines.fill(string());
    cout << "READY" << endl;
    running = true;
}

// basic commands
void statement_list(){
    for (int i = 0; i < MAX_LINES_ALLOWED; i++) {
        if (!lines[i].empty()) {
            auto line = lines[i];
            cout << i << " " << line << endl;
        }
    }
}

void statement_print(string *line){
    string expr("");
    stringstream out;
    line->push_back(','); // always get last expression
    int start = 0;
    int in_string = false;
    for (int i = 0; i < line->size(); i++){
        if (line->at(i) == '"'){
            if (in_string) {
                string in_quotes = line->substr(start, i-start);
                out << in_quotes;
                in_string = false;
                i = line->find(',',i); // this expression is done, so go to next semicolon
                start = i+1;
            }
            else {
                in_string = true;
                start = i+1;
            }
        }
        else if (line->at(i) == ',' && !in_string){
            string raw_exp = line->substr(start,i-start);
            Expression exp(&raw_exp, &vars);
            out << exp.to_str();
            start = i+1;
        }
    }
    cout << out.str() << endl;
}

void statement_let(string *line){
    int equals = line->find('=');
    int i=0;
    int first_space = abs((int)line->find(' '));
    int var_end = min(equals,first_space);
    string var = line->substr(0,var_end);
    string rest = line->substr(equals+1);
    Expression exp(&rest, &vars);
    int val =  exp.to_int();
    vars[var] = val;
}

bool statement_if(string *line){
    int first_space = line->find(' ');
    string exp_1 = line->substr(0,first_space);
    int relop_start = line->find_first_of("<=>");
    int relop_end = line->find_first_not_of("<=>",relop_start);
    string relop = line->substr(relop_start,relop_end-relop_start);
    int statement_loc = line->find("THEN");
    string exp_2 = line->substr(relop_end,statement_loc-relop_end);
    string statement = line -> substr(line->find_first_not_of(" ",statement_loc+4)); //  handle arbitrary spaces
    int exp_1_int = Expression(&exp_1,&vars).to_int();
    int exp_2_int = Expression(&exp_2,&vars).to_int();
    bool res = if_eval(exp_1_int,exp_2_int,relop);
    if (res)
        return parse_and_run(statement);
    return false;
}

void statement_run(){
    sequential_running = true;
    while (sequential_running){
        string command = lines[position];
        if (position == MAX_LINES_ALLOWED-1 || command == "END") {
            position = 0;
            sequential_running = false;
        }
        else if (command == "RETURN"){
            int pos_stack;
            if (sub_stack.size() > 0)
                pos_stack = sub_stack.back();
            else
                pos_stack = -1;
            sub_stack.pop_back();
            if (pos_stack == -1){
                position = 0;
                sequential_running = false;
            }
            else
                position = pos_stack;
        }
        else {
            bool exec = parse_and_run(command);
            if (!exec) {
                position += 1; // parse and run returns true if it updates position internally, otherwise just +1 it
            }
        }
    }
}

void statement_input(string *line){
    cout << "?";
    std::string input;
    cin >> input;
    line->push_back(',');
    input.push_back(',');
    // NOTE: this could be done simply by looking for letters, but want to keep possibility of multichar variables open
    int start = 0;
    int var_start = 0;
    for (int i=0; i<line->length(); i++){
        if (line->at(i) == ','){
            string var_name = line->substr(start,i-start);
            start=i+1;
            int next_comma = input.find(',', var_start);
            string var_val = input.substr(var_start, next_comma - var_start);
            int var_eval = Expression(&var_val,&vars).to_int();
            vars[var_name] = var_eval;
            var_start = next_comma+1;
        }
    }
}
//end of basic commands

bool parse_and_run(string line){
    if (line.empty())
        return false;
    locale loc;
    int first_space = line.find(" ");
    string command_unfiltered = line.substr(0,first_space);
    string command = upperCase(&command_unfiltered);
    string rest = line.substr(first_space+1);
    if (command == "LIST") {
        statement_list();
        position += 1;
    }
    else if (command == "RUN"){
        statement_run();
    }
    else if (command == "CLEAR"){
        clear_interpreter();
    }
    else if (command == "PRINT"){
        statement_print(&rest);
    }
    else if (command == "LET"){
        statement_let(&rest);
    }
    else if (command == "IF"){
        return statement_if(&rest);
    }
    else if (command == "GOTO"){
        position = Expression(&rest,&vars).to_int();
        if (!sequential_running) {
            statement_run();
        }
        return true;
    }
    else if (command == "GOSUB"){
        int old_pos = position+1;
        position = Expression(&rest,&vars).to_int();
        if (sequential_running){
            sub_stack.push_back(old_pos);
        }
        else{
            sub_stack.push_back(-1); // value to end
            statement_run();
        }
        return true;
    }
    else if (command == "INPUT"){
        statement_input(&rest);
    }
    return false;
}

bool to_be_stored(string line){
    char *out;
    int first_space = line.find(" ");
    if (first_space == string::npos) return false;
    string first_block = line.substr(0,first_space);
    const char * potential_int = first_block.c_str();
    int converted = strtol(potential_int,&out,10);
    if (*out) return false; // wasn't a number, go back to parsing line
    line.erase(0,first_space+1); // pop line number
    lines[converted] = line;
    return true;
}

void interpreter_poll(){
    while (running) {
        string input;
        cout << "> ";
        getline(cin, input);
        bool stored = to_be_stored(input);
        if (!stored)
            parse_and_run(input);
    }
}

int main() {
    clear_interpreter();
    interpreter_poll();
    return 0;
}