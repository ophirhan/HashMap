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
    int hash(const KeyT &key) const;
    void reHash(int newSize);
public:
    HashMap();
    HashMap(const std::vector<KeyT> &keys, const std::vector<ValueT> &values);
    HashMap(HashMap<KeyT, ValueT> &other);
    ~HashMap();

    //functions
    int size() const;
    int capacity() const;
    bool empty() const;
    bool insert(const KeyT &key, const ValueT &val);
    bool containsKey(const KeyT &key) const;
    ValueT &at(const KeyT &key);
    ValueT at(const KeyT &key) const;
    bool erase(const KeyT &key);
    double getLoadFactor() const;
    int bucketSize(const KeyT &key) const;
    int bucketIndex(const KeyT &key) const;
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
                    current = &(vec[i][0]);
                    return;
                }
            }
        }

        reference operator*()const
        {
            return *current;
        }

        pointer operator->() const
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
            current = &(vec[bucket][indexInBucket]);
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
            return other.current != current;
        }
    };

    const_iterator begin() const
    {
        return const_iterator(vec, count);
    }

    const_iterator end() const
    {
        return const_iterator(nullptr, 0);
    }

    const_iterator cbegin() const
    {
        return const_iterator(vec, count);
    }

    const_iterator cend() const
    {
        return const_iterator(nullptr, 0);
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
HashMap<KeyT, ValueT>::HashMap(const std::vector<KeyT> &keys,const std::vector<ValueT> &values):
        maxCapacity(DEFAULT_CAPACITY),
        count(0)
{
    if(keys.size() != values.size())
    {
        throw std::exception();//todo error;
    }
    vec = new pairVector[maxCapacity];
    for(int i = 0; i < keys.size(); i++)
    {
//        insert(keys[i], values[i]);
        (*this)[keys[i]] = values[i];
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
int HashMap<KeyT, ValueT>::hash(const KeyT &key) const
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
bool HashMap<KeyT, ValueT>::insert(const KeyT &key,const ValueT &val)
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
int HashMap<KeyT, ValueT>::size() const
{
    return count;
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::capacity() const
{
    return maxCapacity;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::empty() const
{
    return count == 0;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::containsKey(const KeyT &key) const
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
ValueT &HashMap<KeyT, ValueT>::at(const KeyT &key)
{
    int place = hash(key);
    for(auto &i: vec[place])
    {
        if(i.first == key)
        {
            return i.second;
        }
    }
    throw std::exception();//todo exception
}


template<class KeyT, class ValueT>
ValueT HashMap<KeyT, ValueT>::at(const KeyT &key) const
{
    int place = hash(key);
    for(auto &i: vec[place])
    {
        if(i.first == key)
        {
            return i.second;
        }
    }
    throw std::exception();//todo exception
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::erase(const KeyT &key)
{
    int index = hash(key);
    for(auto iter=vec[index].begin();iter!= vec[index].end() ; iter++)
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
double HashMap<KeyT, ValueT>::getLoadFactor() const
{
    return (double) size()/capacity();
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucketSize(const KeyT &key) const
{
    if(!containsKey(key))
    {
        throw std::exception();//TODO ERROR
    }
    return (int) vec[hash(key)].size();
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucketIndex(const KeyT &key) const
{
    if(!containsKey(key))
    {
        throw std::exception();//TODO ERROR
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
    if(other == *this)
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
    for(auto &pair: other)
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
    return true;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator!=(const HashMap<KeyT, ValueT> &other) const
{
    return !(other == *this);
}



#endif //CPP_EX3_HASHMAP_HPP
