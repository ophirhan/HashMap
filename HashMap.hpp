//
// Created by Ophir's laptop on 20/01/2020.
//
#include <vector>

#ifndef CPP_EX3_HASHMAP_HPP
#define CPP_EX3_HASHMAP_HPP

#define UPPER_LOAD_FACTOR 0.75

#define DEFAULT_CAPACITY 16

#define LOWER_LOAD_FACTOR 0.25

template<class KeyT, class ValueT>
class HashMap
{
private:
    typedef std::vector<std::pair<KeyT, ValueT>> pairVector;

    //private parameters
    int maxCapacity, count;
    pairVector *vec;

    //private funcs
    int hash(const KeyT &key);
    void reHash(int newSize);
public:
    HashMap();
    HashMap(std::vector<KeyT> &keys, std::vector<ValueT> &values);
    HashMap(HashMap<KeyT, ValueT> &other);
    ~HashMap();

    //functions
    int size();
    int capacity();
    bool empty();
    bool insert(KeyT &key, ValueT &val);
    bool containsKey(KeyT &key);
    ValueT &at(KeyT &key);
    ValueT at(KeyT &key) const;
    bool erase(KeyT &key);
    double getLoadFactor();
    int bucketSize(KeyT &key);
    int bucketIndex(KeyT &key);
    void clear();
    class const_iterator
    {
    public:
        typedef int difference_type;

        typedef std::pair<KeyT, ValueT>  value_type;

        typedef std::pair<KeyT, ValueT> *pointer;

        typedef std::pair<KeyT, ValueT> &reference;

        typedef std::forward_iterator_tag iterator_category;
    private:
        int bucket;
        int indexInBucket;
        pairVector *vec;
        int capacity;
        pointer current;

    public:
        const_iterator(pairVector *mVec, int capacity):
                bucket(0), indexInBucket(0), capacity(capacity), vec(mVec), current(nullptr)
        {
            if(mVec == nullptr)
            {
                return;
            }
            for (int i = 0; i < capacity; ++i)
            {
                if(vec[i].size() != 0)
                {
                    bucket = i;
                    current = &(vec[i].at(0));
                    return;
                }
            }
        }

        ValueT& operator*()const
        {
            return *current;
        }

        ValueT *operator->() const
        {
            return current;
        }

        const_iterator& operator++()
        {
            if(vec == nullptr)
            {
                current = nullptr;
                return *this; //todo yair?
            }
            if(indexInBucket + 1 < (int)vec[bucket].size())
            {
                indexInBucket++;
            }
            else
            {
                indexInBucket = 0;
                bucket++;
                while(vec[bucket].size() == 0 && bucket < capacity)
                {
                    bucket++;
                }
                if(bucket == capacity)
                {
                    current = nullptr;
                    return*this;
                }
            }
            current = &(vec[bucket].at(indexInBucket));
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const_iterator const &other) const
        {
            return other.current == current;
        }

        bool operator!=(const_iterator const &other) const
        {
            other.current != current;
        }
    };

    const_iterator begin() const
    {
        return const_iterator(vec);
    }

    const_iterator end() const
    {
        return const_iterator(nullptr);
    }

    //operators
    HashMap<KeyT, ValueT> &operator=(const HashMap<KeyT, ValueT> &other);
    ValueT &operator[](const KeyT &key);
    ValueT operator[](const KeyT &key) const;
    bool operator==(const HashMap<KeyT, ValueT> &other) const;
    bool operator!=(const HashMap<KeyT, ValueT> &other) const;
};
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap():
        maxCapacity(DEFAULT_CAPACITY),
        count(0),
        vec(new pairVector[DEFAULT_CAPACITY])
{}

template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(std::vector<KeyT> &keys, std::vector<ValueT> &values):
        maxCapacity(DEFAULT_CAPACITY),
        count(0)
{
    if(keys.size() != values.size())
    {
        //todo error;
    }
    vec = new pairVector[maxCapacity];
    for(int i = 0; i < keys.size(); i++)
    {
        insert(keys[i], values[i]); //ignore return value because all items come from a hash
    }
}

/**
 * @brief Copy constructor
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(HashMap<KeyT, ValueT> &other):
        maxCapacity(other.maxCapacity), count(other.count), vec(new pairVector[maxCapacity])
{
    for(int i = 0; i < maxCapacity; i++)
    {
        vec[i] = other.vec[i]; //todo use operator=?
    }
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::hash(const KeyT &key)
{
    int hash = std::hash<KeyT>()(key);
    return hash&(maxCapacity - 1);
}

template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::~HashMap()
{
    // todo clear?
    delete[] vec;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::insert(KeyT &key, ValueT &val)
{
    if(containsKey(key))
    {
        return false;
    }
    int place = hash(key);
    count++;
    vec[place].push_back(std::make_pair(key, val));
    if(getLoadFactor() > UPPER_LOAD_FACTOR)
    {
        reHash(maxCapacity*2);
    }
    return true;
}

template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::reHash(int newSize)
{
    auto newVec = new pairVector[newSize];
    int oldCapacity = maxCapacity;
    maxCapacity  = newSize ;
    for (int i = 0; i < oldCapacity; ++i)
    {
        for(auto &pair: vec[i])
        {
            newVec[hash(pair.first)].push_back(pair);
        }
    }
    // todo clear?
    delete[] vec;
    vec = newVec;
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::size()
{
    return count;
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::capacity()
{
    return maxCapacity;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::empty()
{
    return count == 0;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::containsKey(KeyT &key)
{
    int place = hash(key);
    for(auto &pair: vec[place])
    {
        if (pair.first == key)
        {
            return true;
        }
    }
    return false;
}

template<class KeyT, class ValueT>
ValueT &HashMap<KeyT, ValueT>::at(KeyT &key)
{
    int place = hash(key);
    for(const auto &i: vec[place])
    {
        if(i.first == key)
        {
            return i.second;
        }
    }
    throw std::exception();//todo exception
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::erase(KeyT &key)
{
    int index = hash(key);
    for(auto &iter :vec[index])
    {
        if(iter->first == key)
        {
            vec[index].erase(iter);
            count--;
            while(capacity() > 1 && getLoadFactor() < LOWER_LOAD_FACTOR)
            {
                reHash(maxCapacity/2);
            }
            return true;
        }
    }
    return false;
}

template<class KeyT, class ValueT>
double HashMap<KeyT, ValueT>::getLoadFactor()
{
    return (double) size()/capacity();
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucketSize(KeyT &key)
{
    if(!containsKey(key))
    {
        //TODO ERROR
    }
    return (int) vec[hash(key)].size();
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucketIndex(KeyT &key)
{
    if(!containsKey(key))
    {
        //TODO ERROR
    }
    return hash(key);
}

template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::clear()
{
    for (int i = 0; i < maxCapacity; ++i)
    {
        vec[i].clear();
    }
    count = 0;
}

template<class KeyT, class ValueT>
HashMap<KeyT, ValueT> &HashMap<KeyT, ValueT>::operator=(const HashMap<KeyT, ValueT> &other)
{
    if(other == this)
    {
        return *this;
    }
    //todo clear?
    delete [] vec;
    vec = new pairVector[other.capacity()];
    for(int i=0; i < other.capacity(); i++)
    {
        vec[i] = other.vec[i];
    }
    maxCapacity = other.maxCapacity;
    count = other.count;
    return *this;
}

template<class KeyT, class ValueT>
ValueT &HashMap<KeyT, ValueT>::operator[](const KeyT &key)
{
    try{
        return at(key);
    }
    catch (std::exception &e)
    {
        insert(key, ValueT());
        return at(key);
    }
}

template<class KeyT, class ValueT>
ValueT HashMap<KeyT, ValueT>::operator[](const KeyT &key) const
{
    try{
        return at(key);
    }
    catch (std::exception &e)
    {
        return ValueT();
    }
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator==(const HashMap<KeyT, ValueT> &other) const
{
    if(size() != other.size())
    {
        return false;
    }
    for(pairVector &vector: other)
    {
        for(auto &pair: vector)
        {
            if(containsKey(pair.first))
            {
                if(at(pair.first) != pair.second)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator!=(const HashMap<KeyT, ValueT> &other) const
{
    return !(other == *this);
}

template<class KeyT, class ValueT>
ValueT HashMap<KeyT, ValueT>::at(KeyT &key) const
{
    int place = hash(key);
    for(auto i: vec[place])
    {
        if(i.first == key)
        {
            return i.second;
        }
    }
    throw std::exception();//todo exception
}


#endif //CPP_EX3_HASHMAP_HPP
