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
    bool in_quotes = false;
    bool in_escape = false;
    while ((buf = getc(stdin))) {
        if(!in_escape && buf == '\\') {
            in_escape = true;
            goto end_read;
        }
        if(!in_escape && buf == '"') {
            in_quotes = !in_quotes;
            goto end_read;
        } else if(buf == '\n') {
            line++;
            column = 0;
            goto end_read;
        } else if(!in_quotes && buf==';') break;
        input+=buf;
        in_escape = false;
        end_read:
        column++;
    }
    return input;
}

void ignore_arg(std::string & command, ssize_t separators[], bool first){
    separators[0] = command.find_first_not_of(' ', first ? 0 : separators[1]+1);
    separators[1] = command.find_first_of(' ', separators[0]+1);
}


void get_nt_arg(std::string & arg, std::string & command, ssize_t separators[]){
    separators[0] = command.find_first_not_of(' ', separators[1]+1);
    separators[1] = command.find_first_of(' ', separators[0]+1);
    
    arg = separators[0] == - 1 || separators [1] == -1 ? "" :  command.substr(separators[0], separators[1]-separators[0]);
}

void get_last_arg(std::string & arg, std::string & command, ssize_t separators[]){
    separators[0] = command.find_first_not_of(' ', separators[1]+1);
    arg = separators[0] == - 1 ? "" : command.substr(separators[0], command.size() - separators[0]);
}

int main(){
    MappedStack<std::string, std::string> ms;
    
    const std::regex insert_cmd =   std::regex("\\s*(i|(ins)|(insert)|(inserir))\\s+(\\w|\\.)+\\s+.*", std::regex::icase);
    const std::regex rinsert_cmd = std::regex("\\s*r(i|(ins)|(insert)|(inserir))\\s+(\\w|\\.)+\\s+.*", std::regex::icase);
    const std::regex count_cmd = std::regex("\\s*(c|(cnt)|(count)|(contar))\\s+(\\w|\\.)+\\s*", std::regex::icase);
    const std::regex at_cmd =   std::regex("\\s*(f|a|(at)|(find)|(encontrar))\\s+(\\w|\\.)+\\s*", std::regex::icase);
    const std::regex rat_cmd = std::regex("\\s*r(f|a|(at)|(find)|(encontrar))\\s+(\\w|\\.)+\\s*", std::regex::icase);
    const std::regex remove_cmd =   std::regex("\\s*(r|(rm)|(rem)|(remove)|(remover))\\s+(\\w|\\.)+\\s*", std::regex::icase);
    const std::regex rremove_cmd = std::regex("\\s*r(r|(rm)|(rem)|(remove)|(remover))\\s+(\\w|\\.)+\\s*", std::regex::icase);
    const std::regex  silent_cmd = std::regex("\\s*(s|(shh)|(silent)|(silent_mode))\\s*", std::regex::icase);
    const std::regex rsilent_cmd = std::regex("\\s*r(s|(shh)|(silent)|(silent_mode))\\s*", std::regex::icase);
    const std::regex exit_cmd = std::regex("\\s*(q|(exit)|(quit)|(sair))\\s*", std::regex::icase);
    const std::regex list_cmd = std::regex("\\s*(l|(list)|(listar))\\s*", std::regex::icase);
    const std::regex      select_cmd = std::regex("\\s*(s|(select)|(selecionar))\\s*(\\w|\\.)+\\s*", std::regex::icase);
    const std::regex select_offset_cmd = std::regex("\\s*--(s|(select)|(selecionar))-((compensar)|(offset)|(o))\\s+\\d+\\s*", std::regex::icase);
    const std::regex select_limit_cmd = std::regex("\\s*--(s|(select)|(selecionar))-((limite)|(limit)|(l))\\s+\\d+\\s*", std::regex::icase);
    const std::regex select_default_cmd = std::regex("\\s*--(s|(select)|(selecionar))-((default)|(padrao)|(d))\\s*", std::regex::icase);
    const std::regex select_like_cmd = std::regex("\\s*(s|(select)|(selecionar))\\s+\\w+\\s+((regex)|(like)|(como))\\s+.*", std::regex::icase);
    const std::regex version_cmd = std::regex("\\s*(v|(version)|(versao))\\s*", std::regex::icase);
    const std::regex help_cmd = std::regex("\\s*(h|(help)|(ajuda))\\s*", std::regex::icase);
    const std::regex push_cmd = std::regex("\\s*(push)\\s*.*", std::regex::icase);
    const std::regex rpush_cmd = std::regex("\\s*(rpush)\\s*.*", std::regex::icase);
    const std::regex pop_cmd = std::regex("\\s*(pop)\\s*", std::regex::icase);
    const std::regex rpop_cmd = std::regex("\\s*(rpop)\\s*", std::regex::icase);

    std::string command, where, what;
    std::string * what_res;
    std::regex what_regx;
    ssize_t separators[2];
    size_t offset{0},limit{0};
    // i a 1; i a 2; i a 3; i a 4; i a 5;
    while (true) {
        command = get_command("ms-cli");
        if(std::regex_match(command, push_cmd)){
            ignore_arg(command, separators, true);
            get_last_arg(what, command, separators);
            ms.rinsert(".", what);
            if(!silent_mode)
                std::cout << "ok\n";
        } else if(std::regex_match(command, rpush_cmd)){
            ignore_arg(command, separators, true);
            get_last_arg(what, command, separators);
            ms.insert(".", what);
            if(!silent_mode)
                std::cout << "ok\n";
        } else if(std::regex_match(command, pop_cmd)){
            if(!silent_mode)
                std::cout << ((what_res = ms.at(".")) ? *what_res : "") << '\n';
            ms.remove(".");
        } else if(std::regex_match(command, rpop_cmd)){
            if(!silent_mode)
                std::cout << ((what_res = ms.rat(".")) ? *what_res : "") << '\n';
            ms.rremove(".");
        } else if(std::regex_match(command, insert_cmd)){
            ignore_arg(command, separators, true);
            get_nt_arg(where, command, separators);
            get_last_arg(what, command, separators);
            ms.insert(where, what);
            if(!silent_mode)
                std::cout << "ok\n";
        } else if(std::regex_match(command, rinsert_cmd)){
            ignore_arg(command, separators, true);
            get_nt_arg(where, command, separators);
            get_last_arg(what, command, separators);
            ms.rinsert(where, what);
            if(!silent_mode)
                std::cout << "ok\n";
        } else if(std::regex_match(command, count_cmd)){
            ignore_arg(command, separators, true);
            get_last_arg(where, command, separators);
            if(!silent_mode)
                std::cout << ms.count(where) << '\n';
        } else if(std::regex_match(command, at_cmd)){
            ignore_arg(command, separators, true);
            get_last_arg(where, command, separators);
            if(!silent_mode)
                std::cout << ((what_res = ms.at(where)) ? *what_res : "") << '\n';
        } else if(std::regex_match(command, rat_cmd)){
            ignore_arg(command, separators, true);
            get_last_arg(where, command, separators);
            if(!silent_mode)
                std::cout << ((what_res = ms.rat(where)) ? *what_res : "") << '\n';
        } else if(std::regex_match(command, remove_cmd)){
            ignore_arg(command, separators, true);
            get_last_arg(where, command, separators);
            if(!silent_mode)
                std::cout << (ms.remove(where) ? "ok\n" : "\n");
        } else if(std::regex_match(command, rremove_cmd)){
            ignore_arg(command, separators, true);
            get_last_arg(where, command, separators);
            if(!silent_mode)
                std::cout << (ms.rremove(where) ? "ok\n" : "\n");
        } else if(std::regex_match(command, exit_cmd)){
            if(!silent_mode)
                std::cout << "ok\n";
            break;
        } else if(std::regex_match(command, list_cmd)){
            if(!silent_mode)
                ms.print();
        } else if(std::regex_match(command, select_cmd)){
            ignore_arg(command, separators, true);
            get_last_arg(where, command, separators);
            if(!silent_mode){
                if((what_res = ms.at(where))){
                    for (size_t _ = offset; _ < ms.count(where) && (!limit || _ - offset < limit); _++) {
                        std::cout << *(what_res+_) << '\n';
                    }
                } else {
                    std::cout << '\n';
                }
            }
        } else if(std::regex_match(command, select_offset_cmd)){
            ignore_arg(command, separators, true);
            get_last_arg(what, command, separators);
            offset = atoll(what.c_str());
            if(!silent_mode)
                std::cout << "ok\n";
        } else if(std::regex_match(command, select_limit_cmd)){
            ignore_arg(command, separators, true);
            get_last_arg(what, command, separators);
            limit = atoll(what.c_str());
            if(!silent_mode)
                std::cout << "ok\n";
        } else if(std::regex_match(command, select_default_cmd)){
            offset = limit = 0;
            if(!silent_mode)
                std::cout << "ok\n";
        } else if(std::regex_match(command, select_like_cmd)){
            ignore_arg(command, separators, true);
            get_nt_arg(where, command, separators);
            ignore_arg(command, separators, false);
            get_last_arg(what, command, separators);
            try {
                what_regx = std::regex(what);
                if(!silent_mode){
                    if((what_res = ms.at(where))){
                        for (size_t _ = offset; _ < ms.count(where) && (!limit || _ - offset < limit); _++) {
                            if(std::regex_match(*(what_res+_), what_regx))
                                std::cout << *(what_res+_) << '\n';
                        }
                    } else {
                        std::cout << '\n';
                    }
                }
            } catch (std::__1::regex_error err){
                std::cerr << "syntax error in regex \"" <<  what << "\" " << "(" << line << "," << column << ").\n"
                << err.what() << '\n';
            }
        } else if(std::regex_match(command, version_cmd)){
            if(!silent_mode)
                std::cout <<"© 2022 Rene Muala (renemuala@icloud.com) \nMappedStack CLI 1.0 , libms " << MAPPED_STACK_VERSION << '\n';
        } else if(std::regex_match(command, silent_cmd)){
            silent_mode = true;
        } else if(std::regex_match(command, rsilent_cmd)){
            silent_mode = false;
        } else if(std::regex_match(command, help_cmd)){
            if(!silent_mode){
                std::cout << "© 2022 Rene Muala (renemuala@icloud.com) \nMappedStack CLI 1.1 , libms " << MAPPED_STACK_VERSION << '\n'
                << "Note: <where> means stack key / <onde> significa chave de pilha.\n"
                << "(i|(ins)|(insert)|(inserir)) <where/onde> <data/dado>" << "\t inserts data to the end of <where> / insere dados ao fim <onde>.\n"
                << "r(i|(ins)|(insert)|(inserir)) <where/onde> <data/dado>" << "\t inserts data to the top of <where> / insere dados ao topo <onde>.\n"
                << "(c|(cnt)|(count)|(contar)) <where/onde>" << "\t counts <where> / conta onde.\n"
                << "(f|a|(at)|(find)|(encontrar)) <where/onde>" << "\t returns the element on the top of <where> / retorna o elemento no topo de <onde>.\n"
                << "r(f|a|(at)|(find)|(encontrar)) <where/onde>" << "\t returns the element on the bottom of <where> / retorna o elemento no fim de <onde>.\n"
                << "(r|(rm)|(rem)|(remove)|(remover)) <where/onde>" << "\t removes the element on the top of <where> / remove o elemento no top de <onde>.\n"
                << "r(r|(rm)|(rem)|(remove)|(remover)) <where/onde>" << "\t removes the element on the bottom of <where> / remove o elemento no fim de <onde>.\n"
                << "(s|(shh)|(silent)|(silent_mode))" << "\t enables silent mode / activa o modo silencioso.\n"
                << "r(s|(shh)|(silent)|(silent_mode))" << "\t disables silent mode / desactiva o modo silencioso.\n"
                << "(l|(list)|(listar))" << "\t lists all <where> / mostra todos os <onde>.\n"
                << "(s|(select)|(selecionar)) <where/onde>" << "\t displays all elements in <where> / apresenta todos elemetos de <onde>.\n"
                << "(s|(select)|(selecionar)) <where> ((regex)|(like)|(como)) <regex>" << "\t displays all elements in <where> thax match <regex> / apresenta todos elemetos de <onde> que respeitam a <regex>.\n"
                << "--(s|(select)|(selecionar))-((compensar)|(offset)|(o)) <size/tamanho>" << "\t sets <size> as the offset for all select routines / configura <tamanho> como a comprensacao para todas as rotinas select.\n"
                << "--(s|(select)|(selecionar))-((limite)|(limit)|(l)) <size/tamanho>" << "\t sets <size> as the limit for all select routines / configura <tamanho> como o limite para todas as rotinas select.\n"
                << "--(s|(select)|(selecionar))-((default)|(padrao)|(d))" << "\t resets the configurations for all select routines / reseta as configuracoes para todas as rotinas select.\n"
                << "(h|(help)|(ajuda))" << "\t ...\n"
                << "(push) <what>" << "\t inserts data to the top of the default (.) stack / insere dados ao topo da pilha principal (.).\n"
                << "(rpush) <what>" << "\t inserts data to the end of the default (.) stack / insere dados ao fim da pilha principal (.).\n"
                << "(pop)" << "\t returns and removes data from the top of the default (.) stack / retorna e remove dados do topo da pilha principal (.).\n"
                << "(rpop)" << "\t returns and removes data from the top of the default (.) stack / retorna e remove dados do topo da pilha principal (.).\n"
                << "(q|(exit)|(quit)|(sair))" << "\t ...\n"
                << "(v|(version)|(versao))" << "\t ...\n";
            }
        } else {
            if(!silent_mode)
                std::cerr << "syntax error in command \"" <<  command << "\" " << "(" << line << "," << column << ").\n";
        }
    }
}
