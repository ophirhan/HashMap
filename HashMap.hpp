//
// Created by Ophir's laptop on 20/01/2020.
//
#include <vector>

#ifndef CPP_EX3_HASHMAP_HPP
#define CPP_EX3_HASHMAP_HPP

#define UPPER_LOAD_FACTOR 0.75

#define DEFAULT_CAPACITY 16

#define LOWER_LOAD_FACTOR 0.25

#define KEY_DOES_NOT_EXIST "The hashMap doesn't contain this key"

/**
 * @brief open HashMap holds ValueT object according to KeyT objects.
 * @tparam KeyT Objects to search ValueT by.
 * @tparam ValueT Object to hold.
 */
template<class KeyT, class ValueT>
class HashMap
{
private:
    typedef std::vector<std::pair<KeyT, ValueT>> pairVector;

    //private parameters
    int maxCapacity, count;
    pairVector *vec;

    //private funcs
    /**
     * @brief hashes given key to index number between 0 and maxCapcity.
     * @param key KeyT object to hash.
     * @return number between 0 and maxCapacity.
     */
    int _hash(const KeyT &key) const;

    /**
     * @brief Rehashes all keys in the HashMap to new HashMap of newCapacity capacity.
     * @param newCapacity number of buckets after operation is done.
     */
    void _reHash(int newCapacity);

public:
    /**
     * @brief Default HashMap constructor.
     */
    HashMap();

    /**
     * @brief Constructor that receives values in two separate vectors.
     * @param keys const reference to KeyT object vector.
     * @param values const reference to ValueT object vector.
     */
    HashMap(const std::vector<KeyT> &keys, const std::vector<ValueT> &values);

    /**
     * @brief Copy constructor
     * @param other HashMap to copy.
     */
    HashMap(HashMap<KeyT, ValueT> &other);

    /**
     * @brief HashMap destructor.
     */
    ~HashMap();

    //functions
    /**
     * @brief count getter.
     * @return number of items in HashMap.
     */
    int size() const;

    /**
     * maxCapacity getter.
     * @return number of buckets in HashMap.
     */
    int capacity() const;

    /**
     * @brief checks if the HashMap is empty.
     * @return true if the HashMap is empty, false otherwise.
     */
    bool empty() const;

    /**
     * @brief inserts a new value to the HashMap at a certain key location.
     * @param key to locate value by.
     * @param val value to input in the HashMap.
     * @return true if insertion was successful, false otherwise.
     */
    bool insert(const KeyT &key, const ValueT &val);

    /**
     * @brief checks if a given key is contained in the HashMap.
     * @param key the key to search for.
     * @return true if the HashMap contains the key, false otherwise.
     */
    bool containsKey(const KeyT &key) const;

    /**
     * @brief Get value by key.
     * @param key to search by.
     * @return reference to ValueT object if HashMap contains key, throws exception otherwise.
     */
    ValueT &at(const KeyT &key);

    /**
     * @brief Get value by key.
     * @param key to search by.
     * @return ValueT object if HashMap contains key, throws exception otherwise.
     */
    ValueT at(const KeyT &key) const;

    /**
     * @brief Erases key and value from HashMap.
     * @param key to erase.
     * @return true if erasure was successful, false otherwise.
     */
    bool erase(const KeyT &key);

    /**
     * @return gets current (double) load factor of the HashMap.
     */
    double getLoadFactor() const;

    /**
     * @brief Checks how many items were hashed to a certain bucket.
     * @param key to check size of bucket container.
     * @return number of items in bucket containing key.
     */
    int bucketSize(const KeyT &key) const;

    /**
     * @param key to search
     * @return index of bucket containing the key.
     */
    int bucketIndex(const KeyT &key) const;

    /**
     * @brief clears all items from HashMap, doesn't update size.
     */
    void clear();

    /**
     * @brief iterator object of HashMap.
     */
    class const_iterator
    {
    public:
        typedef int difference_type;

        typedef std::pair<KeyT, ValueT> value_type;

        typedef std::pair<KeyT, ValueT> *pointer;

        typedef std::pair<KeyT, ValueT> &reference;

        typedef std::forward_iterator_tag iterator_category;
    private:
        /**
         * @brief Index of bucket containing current item.
         */
        int bucket;

        /**
         * @brief Index of current item in it's containing bucket.
         */
        int indexInBucket;

        /**
         * @brief Number of buckets in HashMap.
         */
        int icapacity;

        /**
         * @brief Pointer to array of vector<KeyT, ValueT> containing items to iterate over.
         */
        pairVector *ivec;

        /**
         * @brief Pointer to current pair of KeyT object, ValueT object.
         */
        pointer current;

    public:

        /**
         * @brief Constructor of Iterator, finding first items.
         * @param mVec pointer to array of vectors to iterate over.
         * @param capacity number of vectors in the array.
         */
        const_iterator(pairVector *mVec, int capacity) :
                bucket(0), indexInBucket(0), icapacity(capacity), ivec(mVec), current(nullptr)
        {
            if (mVec == nullptr)
            {
                return;
            }
            for (int i = 0; i < capacity; ++i)
            {
                if (ivec[i].size() != 0)
                {
                    bucket = i;
                    current = &(ivec[i][0]);
                    return;
                }
            }
        }

        /**
         * @brief * operator overload when *<const_iter_name> is called.
         * @return dereference of current.
         */
        reference operator*()
        {
            return *current;
        }

        /**
         * @brief -> operator overload when <const_iter_name>-> is called.
         * @return current which is a pointer to std::pair of KeyT and ValueT objects
         */
        pointer operator->()
        {
            return current;
        }

        /**
         * @brief ++ operator overload when ++<const_iter_name> is called.
         * @return advances the current and indexes to the next item and returns the iterator,
         * when last is reached points to nullptr.
         */
        const_iterator &operator++()
        {
            if (ivec == nullptr)
            {
                current = nullptr;
                return *this;
            }
            if (indexInBucket + 1 < (int) ivec[bucket].size())
            {
                indexInBucket++;
            }
            else
            {
                indexInBucket = 0;
                bucket++;
                while (bucket < icapacity && ivec[bucket].size() == 0)
                {
                    bucket++;
                }
                if (bucket == icapacity)
                {
                    current = nullptr;
                    return *this;
                }
            }
            current = &(ivec[bucket][indexInBucket]);
            return *this;
        }

        /**
         * @brief ++ operator overload when <const_iter_name>++ is called.
         * @return advances the current and indexes to the next item and returns an iterator
         * pointing to the previous item.
         */
        const_iterator operator++(int)
        {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        /**
         * @brief == operator overload when <const_iter_name>==<other_const_iter_name> is called.
         * @return true if both iterator point to the same pair.
         */
        bool operator==(const_iterator const &other) const
        {
            return other.current == current;
        }

        /**
         * @brief != operator overload when <const_iter_name>==<other_const_iter_name> is called.
         * @return true if both iterator point to the a different pair.
         */
        bool operator!=(const_iterator const &other) const
        {
            return other.current != current;
        }
    };

    /**
     * @return A const iterator pointing to the first item in the HashMap.
     */
    const_iterator begin() const
    {
        return const_iterator(vec, maxCapacity);
    }

    /**
     * @return A const iterator pointing to nullptr.
     */
    const_iterator end() const
    {
        return const_iterator(nullptr, 0);
    }

    /**
     * @return A const iterator pointing to the first item in the HashMap.
     */
    const_iterator cbegin() const
    {
        return const_iterator(vec, maxCapacity);
    }

    /**
     * @return A const iterator pointing to nullptr.
     */
    const_iterator cend() const
    {
        return const_iterator(nullptr, 0);
    }

    //operators

    /**
     * @brief = operator overload when <HashMap_name>=<other_HashMap_name> is called,
     * Copies data from other HashMap to this HashMap.
     * @return Reference to current HashMap
     */
    HashMap<KeyT, ValueT> &operator=(const HashMap<KeyT, ValueT> &other);

    /**
     * @brief [] operator overload when <HashMap_name>[KeyT key] is called.
     * @return Reference to the ValueT item in the key place if it exists,
     * otherwise inserts default ValueT value and returns reference to it.
     */
    ValueT &operator[](const KeyT &key);

    /**
     * @brief [] operator overload when const <HashMap_name>[KeyT key] is called.
     * @return Reference to the ValueT item in the key place if it exists,
     * otherwise returns a reference to default ValueT.
     */
    ValueT operator[](const KeyT &key) const;

    /**
     * @brief == operator overload when const <HashMap_name>==<other_HashMap_name> is called.
     * @return true if the group of keyT, ValueT pairs in current is
     * equal to the group of other HashMap, false otherwise.
     */
    bool operator==(const HashMap<KeyT, ValueT> &other) const;

    /**
     * @brief != operator overload when const <HashMap_name>!=<other_HashMap_name> is called.
     * @return true if the group of keyT, ValueT pairs in current is
     * unequal to the group of other HashMap, false otherwise.
     */
    bool operator!=(const HashMap<KeyT, ValueT> &other) const;
};

/**
 * @brief Default HashMap constructor.
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap():
        maxCapacity(DEFAULT_CAPACITY),
        count(0),
        vec(new pairVector[DEFAULT_CAPACITY])
{
}

/**
 * @brief Constructor that receives values in two separate vectors.
 * @param keys const reference to KeyT object vector.
 * @param values const reference to ValueT object vector.
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(const std::vector<KeyT> &keys, const std::vector<ValueT> &values):
        maxCapacity(DEFAULT_CAPACITY),
        count(0)
{
    if (keys.size() != values.size())
    {
        throw std::runtime_error("Vector lengths aren't equal");
    }
    vec = new pairVector[maxCapacity];
    for (size_t i = 0; i < keys.size(); i++)
    {
        (*this)[keys[i]] = values[i];
    }
}

/**
 * @brief Copy constructor
 * @param other HashMap to copy.
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(HashMap<KeyT, ValueT> &other):
        maxCapacity(other.maxCapacity), count(other.count), vec(new pairVector[maxCapacity])
{
    for (int i = 0; i < maxCapacity; i++)
    {
        vec[i] = other.vec[i];
    }
}

//private funcs
/**
 * @brief hashes given key to index number between 0 and maxCapcity.
 * @param key KeyT object to hash.
 * @return number between 0 and maxCapacity.
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::_hash(const KeyT &key) const
{
    int hash = std::hash<KeyT>()(key);
    return hash & (maxCapacity - 1);
}

/**
 * @brief HashMap destructor.
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::~HashMap()
{
    delete[] vec;
}


/**
 * @brief inserts a new value to the HashMap at a certain key location.
 * @param key to locate value by.
 * @param val value to input in the HashMap.
 * @return true if insertion was successful, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::insert(const KeyT &key, const ValueT &val)
{
    if (containsKey(key))
    {
        return false;
    }
    int place = _hash(key);
    count++;
    vec[place].push_back(std::make_pair(key, val));
    if (getLoadFactor() > UPPER_LOAD_FACTOR)
    {
        _reHash(maxCapacity * 2);
    }
    return true;
}

/**
 * @brief Rehashes all keys in the HashMap to new HashMap of newCapacity capacity.
 * @param newCapacity number of buckets after operation is done.
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::_reHash(int newCapacity)
{
    auto newVec = new pairVector[newCapacity];
    int oldCapacity = maxCapacity;
    maxCapacity = newCapacity;
    for (int i = 0; i < oldCapacity; ++i)
    {
        for (auto &pair: vec[i])
        {
            newVec[_hash(pair.first)].push_back(pair);
        }
    }
    delete[] vec;
    vec = newVec;
}

/**
 * @brief count getter.
 * @return number of items in HashMap.
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::size() const
{
    return count;
}

/**
 * maxCapacity getter.
 * @return number of buckets in HashMap.
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::capacity() const
{
    return maxCapacity;
}

/**
 * @brief checks if the HashMap is empty.
 * @return true if the HashMap is empty, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::empty() const
{
    return count == 0;
}

/**
 * @brief checks if a given key is contained in the HashMap.
 * @param key the key to search for.
 * @return true if the HashMap contains the key, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::containsKey(const KeyT &key) const
{
    int place = _hash(key);
    for (auto &pair: vec[place])
    {
        if (pair.first == key)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Get value by key.
 * @param key to search by.
 * @return reference to ValueT object if HashMap contains key, throws exception otherwise.
 */
template<class KeyT, class ValueT>
ValueT &HashMap<KeyT, ValueT>::at(const KeyT &key)
{
    int place = _hash(key);
    for (auto &i: vec[place])
    {
        if (i.first == key)
        {
            return i.second;
        }
    }
    throw std::out_of_range(KEY_DOES_NOT_EXIST);
}

/**
 * @brief Get value by key.
 * @param key to search by.
 * @return ValueT object if HashMap contains key, throws exception otherwise.
 */
template<class KeyT, class ValueT>
ValueT HashMap<KeyT, ValueT>::at(const KeyT &key) const
{
    int place = _hash(key);
    for (auto &i: vec[place])
    {
        if (i.first == key)
        {
            return i.second;
        }
    }
    throw std::out_of_range(KEY_DOES_NOT_EXIST);
}

/**
 * @brief Erases key and value from HashMap.
 * @param key to erase.
 * @return true if erasure was successful, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::erase(const KeyT &key)
{
    int index = _hash(key);
    for (auto iter = vec[index].begin(); iter != vec[index].end(); iter++)
    {
        if (iter->first == key)
        {
            vec[index].erase(iter);
            count--;
            while (capacity() > 1 && getLoadFactor() < LOWER_LOAD_FACTOR)
            {
                _reHash(maxCapacity / 2);
            }
            return true;
        }
    }
    return false;
}

/**
 * @return gets current (double) load factor of the HashMap.
 */
template<class KeyT, class ValueT>
double HashMap<KeyT, ValueT>::getLoadFactor() const
{
    return (double) size() / capacity();
}

/**
 * @brief Checks how many items were hashed to a certain bucket.
 * @param key to check size of bucket container.
 * @return number of items in bucket containing key.
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucketSize(const KeyT &key) const
{
    if (!containsKey(key))
    {
        throw std::out_of_range(KEY_DOES_NOT_EXIST);
    }
    return (int) vec[_hash(key)].size();
}

/**
 * @param key to search
 * @return index of bucket containing the key.
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucketIndex(const KeyT &key) const
{
    if (!containsKey(key))
    {
        throw std::out_of_range(KEY_DOES_NOT_EXIST);
    }
    return _hash(key);
}

/**
 * @brief clears all items from HashMap, doesn't update size.
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::clear()
{
    for (int i = 0; i < maxCapacity; ++i)
    {
        vec[i].clear();
    }
    count = 0;
}

/**
 * @brief = operator overload when <HashMap_name>=<other_HashMap_name> is called,
 * Copies data from other HashMap to this HashMap.
 * @return Reference to current HashMap
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT> &HashMap<KeyT, ValueT>::operator=(const HashMap<KeyT, ValueT> &other)
{
    if (other == *this)
    {
        return *this;
    }
    delete[] vec;
    vec = new pairVector[other.capacity()];
    for (int i = 0; i < other.capacity(); i++)
    {
        vec[i] = other.vec[i];
    }
    maxCapacity = other.maxCapacity;
    count = other.count;
    return *this;
}

/**
 * @brief [] operator overload when <HashMap_name>[KeyT key] is called.
 * @return Reference to the ValueT item in the key place if it exists,
 * otherwise inserts default ValueT value and returns reference to it.
 */
template<class KeyT, class ValueT>
ValueT &HashMap<KeyT, ValueT>::operator[](const KeyT &key)
{
    try
    {
        return at(key);
    }
    catch (std::out_of_range &e)
    {
        insert(key, ValueT());
        return at(key);
    }
}

/**
 * @brief [] operator overload when const <HashMap_name>[KeyT key] is called.
 * @return Reference to the ValueT item in the key place if it exists,
 * otherwise returns a reference to default ValueT.
 */
template<class KeyT, class ValueT>
ValueT HashMap<KeyT, ValueT>::operator[](const KeyT &key) const
{
    try
    {
        return at(key);
    }
    catch (std::out_of_range &e)
    {
        return ValueT();
    }
}

/**
 * @brief == operator overload when const <HashMap_name>==<other_HashMap_name> is called.
 * @return true if the group of keyT, ValueT pairs in current is
 * equal to the group of other HashMap, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator==(const HashMap<KeyT, ValueT> &other) const
{
    if (size() != other.size())
    {
        return false;
    }
    for (auto &pair: other)
    {
        if (containsKey(pair.first))
        {
            if (at(pair.first) != pair.second)
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

/**
 * @brief != operator overload when const <HashMap_name>!=<other_HashMap_name> is called.
 * @return true if the group of keyT, ValueT pairs in current is
 * unequal to the group of other HashMap, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator!=(const HashMap<KeyT, ValueT> &other) const
{
    return !(other == *this);
}


#endif //CPP_EX3_HASHMAP_HPP
