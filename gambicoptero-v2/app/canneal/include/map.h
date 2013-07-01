/* This is a simple STD map interface that uses EPOS Vector class.
 * Only the methods used by canneal are implemented.
 * Author: Giovani Gracioli <giovani@lisha.ufsc.br>
 */

#ifndef MAP_H
#define MAP_H

#include <utility/vector.h>
//#include "input.h"

#define MAP_SIZE 100
#define MAX_STRING 20

template <typename key, typename T>
class map
{
private:
    struct map_node {
        key name;
        T value;
    };
public:
    typedef typename System::Vector<map_node, MAP_SIZE>::Element Element;
    
    class iterator : private map<key, T> {
    public:
        unsigned int current;
        key first;
        T second;
        bool _end;
        System::Vector<map_node, MAP_SIZE> *tmp;
    public:
        bool operator==(const iterator& b) { 
          //cout << "first = " << first << " second = " << second << " b.first = " << b.first << " b.second = " << b.second  << "\n";
          if((strcmp(first, b.first) == 0) && second == b.second && _end == b._end)
            return true;  
          return false;
        }
        
        bool operator!=(const iterator& b) { 
          if((strcmp(first, b.first) != 0) || second != b.second)
            return true; 
          return false;
        } 
        
        iterator *operator->() {
            return this;
        }
        
        void operator++() { 
          //cout << "OPERATOR++ current = " << this->current << " tmp->size = " << tmp->size() << endl;
          this->current++;
          if(this->current == tmp->size()) { // end
              //cout << "OPERATOR++ END" << endl;
              this->_end = true;
              //first = (key) malloc (sizeof(char)*4);
              strcpy(this->first, "\0");
              second = 0; 
              return;
          }
          map_node *node = tmp->get(this->current)->object();
          this->first = node->name;
          //strcpy(this->first, node->name);
          this->second = node->value;
          this->_end = false;
          //cout << "OPERATOR++ current = " << this->current << " this->first = " << this->first << endl;
        }
        
        static iterator end() {
            iterator it;
            it._end = true;
            it.first = (key) malloc(sizeof(char)*20);
            strcpy(it.first, (char *)'\0');
            it.second = 0;
            return it;
        }
    };
    
    iterator begin() {
        iterator it;
        for(unsigned int i = 0 ; i < _v.size(); i++) {
            map_node *node = _v.get(i)->object();
            if(node == 0) continue;
            it.current = i;
            it.first = node->name;
            it.second = node->value;
            it.tmp = &_v;
            return it;
        }
        it.current = 0;
        it.first = 0;
        it.second = 0;
        it.tmp = &_v;
        return it;
    } 
    
    iterator end() {
        return iterator::end();
    }
    
    T& operator[](const char *name) {
        map_node *node;
        unsigned int i;
        //Element *e; 
        for(i = 0; i < _v.size(); i++) {
              node =  _v.get(i)->object();
              if(node == 0) continue;
              if(strcmp(node->name, name) == 0) {
                  return node->value;
              }
        }
        node = (map_node *) malloc(sizeof(map_node));
        node->name = new char[20];
        strcpy(node->name, name);
        //cout << "OPERATOR [] adding in [" << i << "] name = " << name << endl;
        //e = new Element(node);
        //_v.insert(e, i);
        insert_sorted(node);
        return node->value;
    }
        
    iterator find(const char *name) { //TODO
        iterator it;
        for(unsigned int i = 0; i < _v.size(); i++) {
              map_node *node = _v.get(i)->object();
              if(node == 0) continue;
              //cout << "**find() element[" << i << "] = " << node->name << " name = " << name << endl; 
              if(strcmp(node->name, name) == 0) {
                  //cout << "**find() element found = " << name << " node->name = " << node->name << endl; 
                  it.first = node->name;
                  it.second = node->value;
                  it.current = i;
                  return it;
              }
        }
        //cout << "**find() returning END\n";
        return iterator::end();
    }
    
    int size() { return _v.size(); }
    
private:
    void insert_sorted(map_node *new_node) {
        Element *e;
        e = new Element(new_node);
        map_node *old_node;
        int i;
        
        for(i = 0; i < _v.size(); i++) { //found the new position
            old_node = _v.get(i)->object();
            if(strcmp(old_node->name, new_node->name) > 0) {
                break;
            }
        }
        
        Element *tmp;
        for(;i < _v.size(); i++) {
            tmp = _v.remove(i);
            _v.insert(e, i);
            e = tmp;
        }   
        
        _v.insert(e, i); //insert last element
    }
    
private:
    System::Vector<map_node, MAP_SIZE> _v;
};

#endif
