#pragma once

namespace arra 
{   

template<typename K, typename V> 
class SHashMap
{
    struct SPair
    {
        SPair(K key, V value)
        : key_(key)
        , value_(value)
        {
        }
        
        K key_;
        V value_;        
    };

public:
    // constructor, destructor
    SHashMap();
    SHashMap(int capacity);
    ~SHashMap();

    // access:
    V getValue(K key);

    // modifiers:
    void deleteNode(K key); 
    void insertNode(K key, V value);  
    int positiveRemainderHashCode(int hc);
    void rehash();  

    // info:
    int capacity();
    void goToNextHashCodeIndex(int& h);
    int hashCode(float key);
    int hashCode(int key);
    int hashCode(String key);
    bool isEmpty();
    void print();  
    int size();      

private:
    SPair** hashMap_;
    int capacity_;
    int size_;
};


const int DEFAULT_CAPACITY = 20; 

template<typename K, typename V> 
SHashMap<K, V>::SHashMap()
:hashMap_(new SPair* [DEFAULT_CAPACITY])
, capacity_(DEFAULT_CAPACITY) // Initial capacity of hashMap is 20 by default
, size_(0)
{
    // Initialise all elements of hashMap_ array as nullptr
    for (int i = 0; i < capacity_; ++i)
    {
        hashMap_[i] = nullptr;
    }
}

template<typename K, typename V> 
SHashMap<K, V>::SHashMap(int capacity)
: hashMap_(new SPair* [capacity])
, capacity_(capacity) 
, size_(0)
{
    for (int i = 0; i < capacity; ++i)
    {
        hashMap_[i] = nullptr;
    }
}

template<typename K, typename V>
SHashMap<K, V>::~SHashMap()
{
    for (int i = 0; i < capacity_; ++i)
    {
        delete hashMap_[i];
        hashMap_[i] = nullptr;
    }

    delete[] hashMap_;
    hashMap_ = nullptr;
}

template<typename K, typename V> 
int SHashMap<K, V>::positiveRemainderHashCode(int hc)
{
    // This assures us that the result of the mod operation between hc and capacity_ is positive even when hc is negative
    return (hc % capacity_ + capacity_) % capacity_;
}

template<typename K, typename V> 
int SHashMap<K, V>::hashCode(int key)
{    
    return positiveRemainderHashCode(key);  
}

template<typename K, typename V> 
int SHashMap<K, V>::hashCode(String key)
{
    // We shall use Polynomial Rolling algorithm 
    // to generate a hash code from a string input. 
    // It uses the following formula:
    // hashCode = x₁*A⁰ + x₂*Aⁱ + x₃*A² ... xₙ*Aⁿ⁻ⁱ
    // We get x from the ASCII code of a character in the string. 
    // We then multiply this ASCII code with A = PRIME_CONST, a prime number. 
    // We use a prime number to evenly distribute the values. 
    // The prime number should be close to number of available characters. 
    // For English alphabet 31 is a good empirical choice. 
    // The exponent ensures a different hash code for anagrams.
    const int PRIME_CONST = 31;    
    int hashCode = 0;
    int primeConstPower = 1;
    int keyLength = key.length();

    for (int i = 0; i < keyLength; ++i) 
    {
        hashCode += key[i] * primeConstPower;
        primeConstPower *= PRIME_CONST;
    }

    return positiveRemainderHashCode(hashCode);
}

template<typename K, typename V> 
int SHashMap<K, V>::hashCode(float key)
{
    int intKey = static_cast<int>(key);
    //std::cout << intKey << "\n";
    return SHashMap<K, V>::hashCode(intKey);
}

template<typename K, typename V> 
void SHashMap<K, V>::goToNextHashCodeIndex(int& h)
{
    ++h;
    h %= capacity_;
};

template<typename K, typename V> 
void SHashMap<K, V>::insertNode(K key, V value)
{    
    // if the hash map is occupied at maximum capacity, then rehash 
    if (size_ == capacity_)
    {
        rehash();
    }

    // Apply hash function to find index for given key
    int hashCodeIndex = hashCode(key);

    // Find next free space
    while (hashMap_[hashCodeIndex] != nullptr && hashMap_[hashCodeIndex]->key_ != key) 
    {
        goToNextHashCodeIndex(hashCodeIndex);
    }

    // If a new node will be inserted, increase the current size
    if (hashMap_[hashCodeIndex] == nullptr)
    {
        ++size_;
    }

    // Deallocate the previous memory corresponding to insertion index in the hash map
    delete hashMap_[hashCodeIndex];

    // Put the new information at the insertion index in the hash map
    hashMap_[hashCodeIndex] = new SPair(key, value);
}

template<typename K, typename V> 
void SHashMap<K, V>::rehash()
{
    // The current hash map is made temporary
    SPair** temporaryHashMap = hashMap_;
    int oldCapacity = capacity_;
    
    // New hash map of double the old capacity is created    
    capacity_ *= 2;
    hashMap_ = new SPair* [capacity_];
    size_ = 0;

    for (int i = 0; i < capacity_; ++i)
    {
        hashMap_[i] = nullptr;
    }

    // Looping through all the nodes in the original hash map (temporary) and inserting them into the new hash map
    for (int i = 0; i < oldCapacity; ++i) 
    {
        if (temporaryHashMap[i] != nullptr)
        {
            insertNode(temporaryHashMap[i]->key_, temporaryHashMap[i]->value_);
            delete temporaryHashMap[i];
            temporaryHashMap[i] = nullptr;
        }

    }

    // Deallocating the memory of the original hash map
    delete[] temporaryHashMap;
    temporaryHashMap = nullptr; 
}

template<typename K, typename V> 
void SHashMap<K, V>::deleteNode(K key)
{
    // Apply hash function to find index for given key
    int hashCodeIndex = hashCode(key);

    // finding the node with given key
    while (hashMap_[hashCodeIndex] != nullptr)
    {
        // if node found
        if (hashMap_[hashCodeIndex]->key_ == key)
        {
            delete hashMap_[hashCodeIndex];
            hashMap_[hashCodeIndex] = nullptr;

            // Reduce size
            --size_;
        }

        goToNextHashCodeIndex(hashCodeIndex);
    }
}
 
template<typename K, typename V>
V SHashMap<K, V>::getValue(K key)
{
    // Apply hash function to find index for given key
    int hashCodeIndex = hashCode(key);
    int counter = 0;

    // finding the node with the given key
    while (hashMap_[hashCodeIndex] != nullptr)
    {
        if (counter++ > capacity_) // to avoid an infinite loop
        {
            return nullptr; // Return null pointer if the key is not found (avoid throwing an exception)
        }

        // if the node is found, return its value
        if (hashMap_[hashCodeIndex]->key_ == key)
        {
            return hashMap_[hashCodeIndex]->value_;
        }

        goToNextHashCodeIndex(hashCodeIndex);
    }

    // If not found, return null pointer
    return nullptr;
}


template<typename K, typename V> 
void SHashMap<K, V>::print()
{
    for (int i = 0; i < capacity_; ++i)
    {
        if (hashMap_[i] != nullptr) 
        {
        //std::cout << "index = " << i << " key = " << hashMap_[i]->key_ << " value = " << hashMap_[i]->value_ << "\n";
        }
    }
}

template<typename K, typename V> 
int SHashMap<K, V>::size()
{
    return size_;
}

template<typename K, typename V> 
int SHashMap<K, V>::capacity()
{
    return capacity_;
}

template<typename K, typename V> 
bool SHashMap<K, V>::isEmpty()
{
    return (size_ == 0);
}

}