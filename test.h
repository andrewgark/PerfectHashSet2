#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <set>
#include "perfecthashtable.h"

std::default_random_engine gen;

typedef unsigned int ui32;
typedef unsigned long long ui64;

enum EQueryType
    {
        INSERT,
        ERASE,
        FIND,
        POSSIBLE,
        SIZE
    };
ui32 const QUERY_TYPE_SIZE = 5;

class Query
{
public:
    EQueryType type;
    ui32 key;
    Query(EQueryType type, ui32 key) : type(type), key(key){}
    Query(){}
};


class StdHashTable: public IHashTable
{
    std::set<ui32> possibleKeys;
    std::set<ui32> allKeys;

public:
    StdHashTable(){};

    void init(std::vector<ui32> &keys)
    {
        for(ui32 i = 0; i != keys.size(); ++i)
        //for(std::vector<ui32>::const_iterator i = keys.begin(); i != keys.end(); ++i)
        {
            //std::cout << keys[i] << "xx";
            if (!possibleKeys.count(keys[i]))
                possibleKeys.insert(keys[i]);
            else
                throw EqualKeysException(keys[i]);
        }
    }

    void insert(ui32 &key)
    {
        if (possible(key))
            allKeys.insert(key);
        else
            throw ImpossibleKeyException(key);
    }

    void erase(ui32 &key)
    {
        if (possible(key))
            allKeys.erase(key);
        else
            throw ImpossibleKeyException(key);
    }

    bool find(ui32 &key)
    {
        if (possible(key))
            return (allKeys.count(key));
        else
            throw ImpossibleKeyException(key);
    }

    ui32 size()
    {
        //std::cout << "IM IN SIZE";
        return allKeys.size();
    }

    bool possible(ui32 &key)
    {
        //std::cout << "IM IN POSSIBLE";
        return (possibleKeys.count(key));
    }
};


bool test(std::vector<ui32> &keys, std::vector<Query> &queries)
{
    PerfectHashTable PHT;
    StdHashTable SHT;
    //std::cout << "\n" << keys.size() << "\n";
    //for (ui32 i = 0; i != keys.size(); ++i)
    //    std::cout << keys[i] << " ";
    //std::cout << "\n" << queries.size() << "\n";
    //for (ui32 i = 0; i != queries.size(); ++i)
    //    std::cout << (queries[i].type == POSSIBLE) << " ";

    //std::cout << "\n";
    PHT.init(keys);
    //std::cout << "Init1\n";
    //std::cout << "\n" << keys.size() << "\n";
    //for (ui32 i = 0; i != keys.size(); ++i)
    //    std::cout << keys[i] << " ";
    //std::cout << "\n";
    SHT.init(keys);
    //std::cout << "Init2\n";
    //std::cout << PHT.size() << "\n";
    //std::cout << SHT.size() << "\n";
    for(ui32 q = 0; q != queries.size(); ++q)
    //for(std::vector<Query>::const_iterator q = queries.begin(); q != queries.end(); ++q)
    {

        //std::cout << "begin";
        switch (queries[q].type)
        {
            case INSERT:
                //std::cout << "INSERT " << queries[q].key << "\n";
            break; case ERASE:
                //std::cout << "ERASE " << queries[q].key << "\n";
            break; case POSSIBLE:
                //std::cout << "POSSIBLE " << queries[q].key << "\n";
            break; case SIZE:
                //std::cout << "SIZE\n";
            break; case FIND:
                //std::cout << "FIND " << queries[q].key << "\n";
            break;
        }
        switch (queries[q].type)
        {
            case INSERT: {
                PHT.insert(queries[q].key);
                SHT.insert(queries[q].key);
            break; } case ERASE: {
                PHT.erase(queries[q].key);
                SHT.erase(queries[q].key);
            break; } case FIND: {
                if (PHT.find(queries[q].key) != SHT.find(queries[q].key))
                {
                    std::cout << "\nWrong answer in operation FIND\n";
                    return 0;
                }
            break; } case POSSIBLE: {
                if (PHT.possible(queries[q].key) != SHT.possible(queries[q].key))
                {
                    std::cout << "\nWrong answer in operation POSSIBLE\n";
                    return 0;
                }
                //else
                //    std::cout << "Possible works right\n";
            break; } case SIZE: {
                if (PHT.size() != SHT.size())
                {
                    //std::cout << "\nWrong answer in operation SIZE\n";
                    return 0;
                }
            break; }
        }
        //std::cout << "end" << q;
    }
    //std::cout << "return";
    return 1;
}

std::vector<ui32> genRandomDifferentKeys(ui32 &numKeys)
{
    std::uniform_int_distribution<ui32> distributionKeys(0, UINT_MAX);
    std::set<ui32> Set;
    for (ui32 i = 0; i != numKeys; ++i)
       Set.insert(distributionKeys(gen));
    std::vector<ui32> Vector;
    Vector.reserve(Set.size());
    std::copy(Set.begin(), Set.end(), std::back_inserter(Vector));
    return Vector;
}

std::vector<Query> genRandomCorrectQueries(ui32 &numQueries, std::vector<ui32> &keys)
{
    std::vector<Query> Vector;
    std::vector<bool> used;
    Vector.reserve(numQueries);
    used.assign(keys.size(), 0);
    std::uniform_int_distribution<ui32> distributionQueries(1, QUERY_TYPE_SIZE);
    std::uniform_int_distribution<ui32> distributionNumKeys(0, keys.size() - 1);
    std::uniform_int_distribution<ui32> distributionKeys(0, UINT_MAX);
    while(Vector.size() != numQueries)
    {
        ui32 randomQueryTypeNum = distributionQueries(gen);
        Query randomQuery;
        bool wrongErase = false;
        ui32 idKey = distributionNumKeys(gen);
        switch(randomQueryTypeNum)
        {
            case 1: {
                randomQuery.type = INSERT;
                randomQuery.key = keys[idKey];
                used[idKey] = true;
            break; } case 2: {
                randomQuery.type = ERASE;
                if (!used[idKey])
                    wrongErase = true;
                else
                    std::cout << "++++ERASE++++";
                randomQuery.key = keys[idKey];
                used[idKey] = false;
            break; } case 3:
                randomQuery.type = FIND;
                randomQuery.key = keys[idKey];
            break; case 4:
                randomQuery.type = POSSIBLE;
                randomQuery.key = distributionKeys(gen);
            break; case 5:
                randomQuery.type = SIZE;
            break;
        }
        if (!wrongErase)
            Vector.push_back(randomQuery);
    }
    return Vector;
}

void RandomCorrectTest(ui32 &numTests, ui32 &maxNumKeys, ui32 &maxNumQueries)
{
    std::uniform_int_distribution<ui32> distributionNumKeys(1, maxNumKeys);
    std::uniform_int_distribution<ui32> distributionNumQueries(1, maxNumQueries);
    for (ui32 i = 0; i < numTests; ++i)
    {
        ui32 numKeys =  distributionNumKeys(gen);
        ui32 numQueries =  distributionNumQueries(gen);
        //std::cout << "startGen ";
        std::vector<ui32> keys = genRandomDifferentKeys(numKeys);
        //std::cout << "\n" << keys.size() << "\n";
        //for (ui32 i = 0; i != keys.size(); ++i)
        //    std::cout << keys[i] << " ";
        //std::cout << "\n";
        //std::cout << "genKeys ";
        std::vector<Query> queries = genRandomCorrectQueries(numQueries, keys);
        //std::cout << "genQueries\n";
        if (test(keys, queries))
            std::cout << "+";
        else
            std::cout << "-";
    }
}
