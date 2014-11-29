#include "hash.h"
#include "innerhashtable.h"

typedef unsigned int ui32;
typedef unsigned long long ui64;

class IHashTable
{
    virtual void init(std::vector<ui32> &keys) = 0;
    virtual void erase(ui32 &key) = 0;
    virtual void insert(ui32 &key) = 0;
    virtual bool find(ui32 &key) = 0;
    virtual ui32 size() = 0;
};

class PerfectHashTable: public IHashTable
{
private:
    std::vector<InnerHashTable> innerHashTables;
    std::vector<std::vector<ui32>> innerTableHashes;
    Hash hash;
    ui32 tableSize;
    ui32 keysSize;

    bool badHash(std::vector<ui32> &keys)
    {
        innerTableHashes.assign(tableSize, std::vector<ui32> ());

        for (ui32 it = 0; it != keys.size(); ++it)
        //for (std::vector<ui32>::const_iterator it = keys.begin(); it != keys.end(); ++it)
            innerTableHashes[hash(keys[it])].push_back(keys[it]);

        ui64 sumOfSquaresOfSizes = 0;

        for (ui32 itTable = 0; itTable != innerTableHashes.size(); ++itTable)
        //for (std::vector<std::vector<ui32>>::iterator itTable = innerTableHashes.begin(); itTable != innerTableHashes.end(); ++itTable)
        {

            //std::cout << "\n(*itTable).size(): " << (*itTable).size() << " " << tableSize;
            for (ui32 i = 1; i < innerTableHashes[itTable].size(); ++i)
                if (innerTableHashes[itTable][i - 1] == innerTableHashes[itTable][i])
                    throw EqualKeysException(innerTableHashes[itTable][i]);

            if ((innerTableHashes[itTable].size() == 3) && (innerTableHashes[itTable].front() == innerTableHashes[itTable].back()))
                throw EqualKeysException(innerTableHashes[itTable].front());

            sumOfSquaresOfSizes += innerTableHashes[itTable].size() * innerTableHashes[itTable].size();
        }

        //std::cout << "\nsumsquares: " << sumOfSquaresOfSizes << " " << tableSize;
        return (sumOfSquaresOfSizes > 3 * tableSize);
    }

    void initInnerHashTables()
    {
        innerHashTables.clear();
        innerHashTables.reserve(tableSize);

        for (ui32 it = 0; it != innerTableHashes.size(); ++it)
        //for (std::vector<std::vector<ui32>>::iterator it = innerTableHashes.begin(); it!= innerTableHashes.end(); ++it)
        {
            innerHashTables.push_back(InnerHashTable(innerTableHashes[it]));
        }
    }

public:
    PerfectHashTable(){};

    void init(std::vector<ui32> &keys)
    {
        //std::cout << "\n keys[0]==" << keys[0] << "\n";
        tableSize = keys.size();
        hash.setSize(tableSize);
        do
        {
            hash.genCoefficients();
        }
        while (badHash(keys));
        //std::cout << "\n keys[0]==" << keys[0] << "\n";
        initInnerHashTables();

        keysSize = 0;
        //std::cout << "\n keys[0]==" << keys[0] << "\n";
    }

    void insert(ui32 &key)
    {
        if (possible(key))
            keysSize += !innerHashTables[hash(key)].insert(key);
        else
            throw ImpossibleKeyException(key);
    }

    void erase(ui32 &key)
    {
        if (possible(key))
            keysSize -= innerHashTables[hash(key)].erase(key);
        else
            throw ImpossibleKeyException(key);
    }

    bool find(ui32 &key)
    {
        if (possible(key))
            return innerHashTables[hash(key)].find(key);
        else
            throw ImpossibleKeyException(key);
    }

    ui32 size()
    {
        return keysSize;
    }

    bool possible(ui32 &key)
    {
        //std::cout << "!" << key <<"!" <<std::endl;
        return innerHashTables[hash(key)].possible(key);
    }
};
