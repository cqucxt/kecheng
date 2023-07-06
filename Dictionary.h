#include "AList.h"
template <typename Key,typename E> 
class Dictionary {
private:
	void operator =(const Dictionary&) {}
	Dictionary(const Dictionary&) {}
public:
	Dictionary() {}// Default constructor
	virtual  ~Dictionary() {}// Base destructor
	virtual void clear() = 0;// Insert a record
	// k: The key for the record being inserted.
	// e: The record being inserted.
	virtual void insert (const Key& k, const E& e) = 0;
	// Remove and return a record.
	// k: The key of the record to be removed.
	// Return: A matching record. If multiple records match 
	// "k", remove an arbitrary one. Return NULL if no record// with key "k" exists.
	virtual E remove (const Key& k) = 0;
	// Remove and return an arbitrary record from dictionary.
	// Return: The record removed, or NULL if none exists.
	virtual E removeAny() = 0;
	// Return: A record matching "k" (NULL if none exists).
	// If multiple records match, return an arbitrary one.
	// k: The key of the record to findvirtual 
	E find(const Key& k) const= 0;
	// Return the number of records in the dictionary.
	virtual intsize() = 0;
};
template <typename Key, typename E>
class KVpair{
private:
	Key k;
	E e;
public:
// Constructors
	KVpair() {}
	KVpair(Key kval, E eval)
		{ k = kval; e = eval; }
	KVpair(const KVpair& o)// Copy constructor
	{ k = o.k; e = o.e; }
	void operator =(const KVpair& o)// Assignment operator
	{ k = o.k; e = o.e; }
	// Data member access functions
	Key key(){ return k; }
	void setKey(Key ink){ k = ink; }
	E value(){ return e; }
};

template <typename Key, typename E>
class UALdict: public Dictionary<Key, E>{
private:
	AList<KVpair<Key,E>>* list;
public:
	UALdict(intsize=defaultSize)// Constructor
	{ list = new AList<KVpair<Key,E> >(size); } 
	~UALdict(){ delete list; } // Destructor
	void clear(){ list->clear(); } // Reinitialize
	// Insert an element: append to list
	void insert(constKey&k, constE& e){
		KVpair<Key,E> temp(k, e);
		list->append(temp);
	}
	E remove(constKey& k){
		E temp = find(k); 
		// "find" will set list position
		if(temp != NULL) list->remove();
		return temp;
	}
	E removeAny(){ // Remove the last element
		Assert(size() != 0, "Dictionary is empty");
		list->moveToEnd();
		list->prev();
		KVpair<Key,E> e = list->remove();
		return e.value();
	}
	E find(constKey& k) const{
		for(list->moveToStart();list->currPos() < list->length(); list->next()) {
			KVpair<Key,E> temp = list->getValue();
			if (k == temp.key())
				return temp.value();
		}
		return NULL; // "k" does not appear in dictionary
	}
	int size()// Return list size
		{ return list->length(); }
};



