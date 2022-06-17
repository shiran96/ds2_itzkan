#ifndef DYNAMIC_HASH_TABLE_H
#define DYNAMIC_HASH_TABLE_H

#include <stdlib.h>
#include "linked_list.h"
#include "employee.h"
#include <memory>
using std::shared_ptr;
                            
const int INITIAL_SIZE=10;
class DynamicHashTable //HashTable of type chains
{
    private:
    //helper function- change the size of the table
    // -make it smaller or bigger
    void makeDifferentSizeTable(int old_table_size, int new_table_size);

    public:
    LinkedList<shared_ptr<Employee>>** arr; //every cell in the array is a linked list
                                        //of type shared_ptr of type Player
    int playersNum; 
    int tableSize;

    DynamicHashTable();

    ~DynamicHashTable();

    DynamicHashTable(const DynamicHashTable& other):
        arr(new LinkedList<shared_ptr<Employee>>*[other.tableSize]),
        playersNum(other.playersNum),tableSize(other.tableSize)
    {
        for (int i = 0; i < other.tableSize; i++)
        {
            arr[i]=other.arr[i]; 
        }
    }

    DynamicHashTable& operator=(const DynamicHashTable& other)=delete;
    int activateHashTableFunction(int playerId) const;
    Node<shared_ptr<Employee>>* getEmployeeById(int playerId) const;

    /*operations of hashTable*/
    bool find(int playerId) const;
    void insert(const shared_ptr<Employee>& player);
    void remove(int playerId);
    int getNumEmployeesAtHash() const;
    int getTableSize() const;
    void setEmployeesCompanyId(int groupId);

//    //for merging 2 hash of players
//    //notice : the players's GroupId didnt changed- on the user to call a func to fix this
//    DynamicHashTable(const DynamicHashTable& first, const DynamicHashTable& second);
//
//    //note: it's on the user that "this" will be empty
//    void insertFromOtherHashTable(const DynamicHashTable& src);
//
//    void cleanHashTable(); //deplete

    void print() const;
    void mergeHash(DynamicHashTable& src);

};


#endif /*DYNAMIC_HASH_TABLE_H*/