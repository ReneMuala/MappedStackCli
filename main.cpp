//
//  main.cpp
//  random
//
//  Created by René Descartes Domingos Muala on 24/05/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#include <iostream>
#include <regex>
#define MS_PRINTABLE_KEYS
#include "MappedStack.hpp"

size_t line{0}, column{0};
bool silent_mode{false};

std::string get_command(const std::string msg){
    char buf;
    std::string input;
    if(!silent_mode)
        std::cout << msg <<"$ ";
    
    while ((buf = getc(stdin))) {
        if(buf == '\n') {
            line++;
            column = 0;
            continue;
        } column++;
        if(buf!=';') {
            input+=buf;
        } else break;
    } return input;
}

void get_where(std::string & where, std::string & command, ssize_t separators[]){
    separators[0] = command.find_first_not_of(' ');
    separators[1] = command.find_first_of(' ', separators[0]+1);
    separators[0] = command.find_first_not_of(' ', separators[1]+1);
    separators[1] = command.find_first_of(' ', separators[0]+1);
    where = command.substr(separators[0], separators[1]-separators[0]);
}

void get_what(std::string & what, std::string & command, ssize_t separators[]){
    separators[0] = command.find_first_not_of(' ', separators[1]+1);
    what = command.substr(separators[0], command.size() - separators[0]);
}

int main(){
    MappedStack<std::string_view, std::string> ms;    
    const std::regex insert_cmd =   std::regex("\\s*(i|(ins)|(insert)|(inserir))\\s+\\w+\\s+.*", std::regex::icase);
    const std::regex rinsert_cmd = std::regex("\\s*r(i|(ins)|(insert)|(inserir))\\s+\\w+\\s+.*", std::regex::icase);
    const std::regex count_cmd = std::regex("\\s*(c|(cnt)|(count)|(contar))\\s+\\w+\\s*", std::regex::icase);
    const std::regex at_cmd =   std::regex("\\s*(f|a|(at)|(find)|(encontrar))\\s+\\w+\\s*", std::regex::icase);
    const std::regex rat_cmd = std::regex("\\s*r(f|a|(at)|(find)|(encontrar))\\s+\\w+\\s*", std::regex::icase);
    const std::regex remove_cmd =   std::regex("\\s*(r|(rm)|(rem)|(remove)|(remover))\\s+\\w+\\s*", std::regex::icase);
    const std::regex rremove_cmd = std::regex("\\s*r(r|(rm)|(rem)|(remove)|(remover))\\s+\\w+\\s*", std::regex::icase);
    const std::regex  silent_cmd = std::regex("\\s*(s|(shh)|(silent)|(silent_mode))\\s*", std::regex::icase);
    const std::regex rsilent_cmd = std::regex("\\s*r(s|(shh)|(silent)|(silent_mode))\\s*", std::regex::icase);
    const std::regex exit_cmd = std::regex("\\s*(q|(exit)|(quit)|(sair))\\s*", std::regex::icase);
    const std::regex list_cmd = std::regex("\\s*(l|(list)|(listar))\\s*", std::regex::icase);
    const std::regex version_cmd = std::regex("\\s*(v|(version)|(versao))\\s*", std::regex::icase);
    const std::regex help_cmd = std::regex("\\s*(h|(help)|(ajuda))\\s*", std::regex::icase);
    std::string command, where, what;
    std::string * what_res;
    ssize_t separators[2];
    while (true) {
        command = get_command("ms-cli");
        if(std::regex_match(command, insert_cmd)){
            get_where(where, command, separators);
            get_what(what, command, separators);
            ms.insert(where, what);
            if(!silent_mode)
                std::cout << "ok\n";
        } else if(std::regex_match(command, rinsert_cmd)){
            get_where(where, command, separators);
            get_what(what, command, separators);
            ms.rinsert(where, what);
            if(!silent_mode)
                std::cout << "ok\n";
        } else if(std::regex_match(command, count_cmd)){
            get_where(where, command, separators);
            if(!silent_mode)
                std::cout << ms.count(where) << '\n';
        } else if(std::regex_match(command, at_cmd)){
            get_where(where, command, separators);
            if(!silent_mode)
                std::cout << ((what_res = ms.at(where)) ? *what_res : "") << '\n';
        } else if(std::regex_match(command, rat_cmd)){
            get_where(where, command, separators);
            if(!silent_mode)
                std::cout << ((what_res = ms.rat(where)) ? *what_res : "") << '\n';
        } else if(std::regex_match(command, remove_cmd)){
            get_where(where, command, separators);
            if(!silent_mode)
                std::cout << (ms.remove(where) ? "ok\n" : "\n");
        } else if(std::regex_match(command, rremove_cmd)){
            get_where(where, command, separators);
            if(!silent_mode)
                std::cout << (ms.rremove(where) ? "ok\n" : "\n");
        } else if(std::regex_match(command, exit_cmd)){
            if(!silent_mode)
                std::cout << "ok\n";
            break;
        } else if(std::regex_match(command, list_cmd)){
            if(!silent_mode)
                ms.print();
        } else if(std::regex_match(command, version_cmd)){
            if(!silent_mode)
                std::cout <<"© 2022 Rene Muala (renemuala@icloud.com) \nMappedStack CLI 1.0 , libms " << MAPPED_STACK_VERSION << '\n';
        } else if(std::regex_match(command, silent_cmd)){
            silent_mode = true;
        } else if(std::regex_match(command, rsilent_cmd)){
            silent_mode = false;
        } else if(std::regex_match(command, help_cmd)){
            if(!silent_mode){
                std::cout << "© 2022 Rene Muala (renemuala@icloud.com) \nMappedStack CLI 1.0 , libms " << MAPPED_STACK_VERSION << '\n'
                << "Note: <where> means stack key / <onde> significa chave de pilha.\n"
                << "(i|(ins)|(insert)|(inserir)) <where/onde> <data/dado>;" << "\t inserts data to the end of <where> / insere dados ao fim <onde>.\n"
                << "r(i|(ins)|(insert)|(inserir)) <where/onde> <data/dado>;" << "\t inserts data to the top of <where> / insere dados ao topo <onde>.\n"
                << "(c|(cnt)|(count)|(contar)) <where/onde>;" << "\t counts <where> / conta onde.\n"
                << "(f|a|(at)|(find)|(encontrar)) <where/onde>;" << "\t returns the element on the top of <where> / retorna o elemento no topo de <onde>.\n"
                << "r(f|a|(at)|(find)|(encontrar)) <where/onde>;" << "\t returns the element on the bottom of <where> / retorna o elemento no fim de <onde>.\n"
                << "(r|(rm)|(rem)|(remove)|(remover)) <where/onde>;" << "\t removes the element on the top of <where> / remove o elemento no top de <onde>.\n"
                << "r(r|(rm)|(rem)|(remove)|(remover)) <where/onde>;" << "\t removes the element on the bottom of <where> / remove o elemento no fim de <onde>.\n"
                << "(s|(shh)|(silent)|(silent_mode));" << "\t enables silent mode / activa o modo silencioso.\n"
                << "r(s|(shh)|(silent)|(silent_mode));" << "\t disables silent mode / desactiva o modo silencioso.\n"
                << "(l|(list)|(listar));" << "\t lists all <where> / mostra todos os <onde>.\n"
                << "(h|(help)|(ajuda));" << "\t ...\n"
                << "(q|(exit)|(quit)|(sair));" << "\t ...\n"
                << "(v|(version)|(versao));" << "\t ...\n";
            }
        } else {
            if(!silent_mode)
                std::cerr << "syntax error in command \"" <<  command << "\" " << "(" << line << "," << column << ").\n";
        }
    }
}
