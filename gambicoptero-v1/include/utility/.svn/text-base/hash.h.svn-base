// EPOS Hash Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __hash_h
#define	__hash_h

#include "list.h"
#include "vector.h"

__BEGIN_SYS

// Hash Table with a single Synonym List
// In order to change the hash function, simply redefine the operator % for
// objects of type T and Key.
template <typename T, unsigned int SIZE, typename Key = int>
class Simple_Hash
{
public:
    typedef T Object_Type;
    typedef typename List_Elements::Singly_Linked_Ordered<T, Key> Element;

public:
    Simple_Hash() {}

    bool empty() const {
	return (_vector.size() == 0) && (_synonyms.size() == 0);
    }
    unsigned int size() const {
	return _vector.size() + _synonyms.size();
    }
    
    void insert(Element * e) {
	if(!_vector.insert(e, e->key() % SIZE))
	    _synonyms.insert(e);
    }
    
    Element * remove(Element * e) {
	if(_vector.remove(e))
	    return e;
	return _synonyms.remove(e);
    }
    Element * remove(const Object_Type * obj) {
	Element * e = _vector.remove(obj);
	if(e)
	    return e;
	return _synonyms.remove(obj);
    }
    
    Element * search(const Object_Type * obj) {
	Element * e = _vector.search(obj);
	if(e)
	    return e;
	return _synonyms.search(obj);
    }
    
    Element * search_key(const Key & key) {
	Element * e = _vector.get(key % SIZE);
	if(e && (e->key() == key))
	    return e;
	return _synonyms.search_rank(key);
    }
    
    Element * remove_key(const Key & key) {
	Element * e = _vector.get(key % SIZE);
	if(e && (e->key() == key))
	    return _vector.remove(key % SIZE);
	return _synonyms.remove_rank(key);
    }

private:
    Vector<T, SIZE, Element> _vector;
    Simple_Ordered_List<T, Key, Element> _synonyms;
};


// Hash Table with a Synonym List for each Key
template <typename T,
         unsigned int SIZE,
         typename Key = int,
         typename El = List_Elements::Singly_Linked_Ordered<T, Key>,
         typename L = Simple_Ordered_List<T, Key, El> >
class Hash
{
public:
    typedef T Object_Type;
    typedef El Element;
    typedef L List;

public:
    Hash() {}

    bool empty() const {
	for(int i; i < SIZE; i++)
	    if(!_table[i].empty())
		return false;
	return true;
    }
    
    unsigned int size() const {
	int size = 0;
	for(int i; i < SIZE; i++)
	    size += _table[i].size();
	return size;
    }

    void insert(Element * e) {
	_table[e->key() % SIZE].insert(e);
    }
    
    Element * remove(Element * e) {
	return _table[e->key() % SIZE].remove(e);
    }
    Element * remove(const Object_Type * obj) {
	for(int i = 0; i < SIZE; i++) {
	    Element * e = _table[i].remove(obj);
	    if(e)
		return e;
	}
	return 0;
    }
    
    Element * search(const Object_Type * obj) {
	for(int i = 0; i < SIZE; i++) {
	    Element * e = _table[i].search(obj);
	    if(e)
		return e;
	}
	return 0;
    }
    
    Element * search_key(const Key & key) {
	return _table[key % SIZE].search_rank(key);
    }
    
    Element * remove_key(int key) {
	return _table[key % SIZE].remove_rank(key);
    }

protected:
    List * operator[](const Key & key) {
        return &_table[key % SIZE];
    }

private:
    List _table[SIZE];
};

__END_SYS
 
#endif
