//
//  MappedStack.hpp
//  random
//
//  Created by René Descartes Domingos Muala on 24/05/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef MappedStack_hpp
#define MappedStack_hpp

#include <stdio.h>
#include <unordered_map>
#include <deque>
#ifdef MS_PRINTABLE_KEYS
#include <iostream>
#endif

#define  MAPPED_STACK_VERSION "1.0"

template <typename T1, typename T2>
class MappedStack {
    std::unordered_map<T1, std::deque<T2>> data;
public:
        
#ifdef MS_PRINTABLE_KEYS
    void print(){
        for (std::pair<T1, std::deque<T2>> t : data) {
            std::cout << t.first << " (" << count(t.first) << ")" <<  "\n";
        }
    }
#endif
    
    void insert(T1 where, T2 what){
        data[where].push_back(what);
    }
    
    void rinsert(T1 where, T2 what){
        data[where].push_front(what);
    }
    
    size_t count(T1 where){
        return data[where].size();
    }
    
    T2* at(T1 where) {
        if(data[where].size()){
            return &data[where].front();
        } return nullptr;
    }
    
    T2* rat(T1 where) {
        if(data[where].size()){
            return &data[where].back();
        } return nullptr;
    }
    
    bool remove(T1 where){
        if(data[where].size()){
            data[where].pop_front();
            return true;
        } return false;
    }
    
    bool rremove(T1 where){
        if(data[where].size()){
            data[where].pop_back();
            return true;
        } return false;
    }
};

#endif /* MappedStack_hpp */
