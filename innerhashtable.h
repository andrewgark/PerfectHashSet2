#include "exceptions.h"

typedef unsigned int ui32;
typedef unsigned long long ui64;

struct InnerHashTable
{
    std::vector<bool> used;
    std::vector<ui32> keyByHash;
    std::vector<bool> keyExists;
    Hash hash;
    ui32 size;

    InnerHashTable(std::vector<ui32> &keys)
    {
        init(keys);
    }

    bool badHash(std::vector<ui32> &keys)
    {
        used.assign(size, false);
        keyByHash.resize(size);

        for (ui32 it = 0; it != keys.size(); ++it)
        //for (std::vector<ui32>::const_iterator it = keys.begin(); it!= keys.end(); ++it)
        {
            ui32 curHash = hash(keys[it]);
            //std::cout << (*it) << " -> " << hash(*it) << " and ";
            if (used[curHash])
            {

                if (used[curHash] && (keyByHash[curHash] == keys[it]))
                    throw EqualKeysException(keys[it]);
                return true;
            }

            keyByHash[curHash] = keys[it];
            used[curHash] = true;
        }
        return false;
    }

    void init(std::vector<ui32> &keys)
    {

        //std::cout << "\n" << keys.size() << "\n";
        //for (ui32 i = 0; i != keys.size(); ++i)
        //    std::cout << keys[i] << " ";
        //std::cout << "\n";
        size = keys.size();
        hash.setSize(size * size);
        do
        {
            hash.genCoefficients();
        }
        while (badHash(keys));

        used.assign(size, false);
        keyExists.assign(size, false);
        for (ui32 it = 0; it != keys.size(); it++)
        //for (std::vector<ui32>::const_iterator it = keys.begin(); it!= keys.end(); ++it)
            keyExists[hash(keys[it])] = true;
    }


    bool insert(ui32 &key)
    {
        ui32 h = hash(key);
        bool isUsed = used[h];
        used[h] = 1;
        return isUsed;
    }

    bool erase(ui32 &key)
    {
        ui32 h = hash(key);
        bool isUsed = used[h];
        used[h] = 0;
        return isUsed;
    }

    bool find(ui32 &key)
    {
        return used[hash(key)];
    }

    bool possible(ui32 &key)
    {
        return ((hash.getSize()) && keyExists[hash(key)] && (keyByHash[hash(key)] == key));
    }
};

