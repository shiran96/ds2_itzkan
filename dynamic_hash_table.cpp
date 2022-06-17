
#include "dynamic_hash_table.h"
#include "exceptions.h"
#include "iostream"


const int EXPANTION_RATE=2;

DynamicHashTable::DynamicHashTable():arr(new LinkedList<shared_ptr<Employee>>*[INITIAL_SIZE]),
                                                        playersNum(0),tableSize(INITIAL_SIZE)
{
    for (int i = 0; i < INITIAL_SIZE; i++)
    {
        arr[i] = new LinkedList<shared_ptr<Employee>>();
    }

};

DynamicHashTable::~DynamicHashTable()
{
    for(int i=0;i<tableSize;i++)
    {
        delete arr[i];
    }
    delete[] arr;
}



int DynamicHashTable::activateHashTableFunction(int playerId) const
{
    return playerId % tableSize;
}


Node<shared_ptr<Employee>>* DynamicHashTable::getEmployeeById(int playerId) const
{
    int place_at_arr=activateHashTableFunction(playerId);
    LinkedList<shared_ptr<Employee>>* curr=(arr[place_at_arr]);
    if(curr!= nullptr)
    {
        Node<shared_ptr<Employee>>* iter=curr->head;
        while (iter != nullptr)
        {
            if(iter->data->getEmployeeId()==playerId)
            {
                return iter;
            }
            iter=iter->next; //advanve in the list at arr[place_at_arr]
        }
    }
    return nullptr;
}

bool DynamicHashTable::find(int playerId) const
{
    Node<shared_ptr<Employee>>* player=this->getEmployeeById(playerId);
    return ( player != nullptr);
}

void DynamicHashTable::insert(const shared_ptr<Employee>& player)
{
    int player_id=player->getEmployeeId();
    if(this->find(player_id)==true)
    {
        throw playerAlreadyExist();
    }
    if(playersNum == tableSize -1) //need to expand the array
    {
        int old_table_size=tableSize;
        tableSize+=INITIAL_SIZE; //in order to expand by two
        makeDifferentSizeTable(old_table_size, tableSize);
    }
    int index_to_enter_player=activateHashTableFunction(player_id);
    arr[index_to_enter_player]->insert(player);
    playersNum=playersNum+1;
}

void DynamicHashTable::remove(int playerId)
{
    Node<shared_ptr<Employee>>* player=getEmployeeById(playerId);
    if(player==nullptr)
    {
        throw playerDoesntExist();
    }
    int shrink_indicator=(tableSize / (EXPANTION_RATE*EXPANTION_RATE) ) +1 ;
    if(playersNum==shrink_indicator) // the arr size will be 1/4 of the
    {               //tableSize after removing the player--> need to shrink
        int old_table_size=tableSize;
        tableSize= (tableSize / EXPANTION_RATE); //smaller by 2
        makeDifferentSizeTable(old_table_size,tableSize);
    }
    player= getEmployeeById(playerId);
    int index_of_player_in_arr=activateHashTableFunction(playerId);
    arr[index_of_player_in_arr]->remove(player);
    playersNum= playersNum-1;
}

void DynamicHashTable::makeDifferentSizeTable(int old_table_size, int new_table_size)
{
    LinkedList<shared_ptr<Employee>>** new_table;
    try
    {
        new_table=new LinkedList<shared_ptr<Employee>>*[new_table_size];
        for(int i=0;i<new_table_size;i++)
        {
            new_table[i]= new LinkedList<shared_ptr<Employee>>();
        }
    }
    catch(const std::bad_alloc &error)
    {
        tableSize=old_table_size; //couldn't make a bigger/smaller table
        //so the size of the table is the original size of it
        throw error; //don't change anything and report an error - will exit from here
    }
    for(int i=0;i<old_table_size;i++)
    {
        LinkedList<shared_ptr<Employee>>* curr=(arr[i]);
        Node<shared_ptr<Employee>>* list_iter= nullptr;
        if(curr!= nullptr)
        {
            list_iter=curr->head;
        }
        while(list_iter != nullptr)
        {

            shared_ptr<Employee> to_insert_player=list_iter->data;
            int to_insert_player_id=to_insert_player->getEmployeeId();
            try
            {
                //every insertion of a player could throw bad_alloc
                int index=activateHashTableFunction(to_insert_player_id);
                new_table[index]->insert(to_insert_player);
            }
            catch(const std::bad_alloc& error)
            {
                //undo the changes
                tableSize=old_table_size;
                delete[] new_table;
                //report an error
                throw error;
            }
            list_iter=list_iter->next;
        }
    }
    for(int i=0;i<old_table_size;i++)  //added
    {
        delete arr[i];
    }
    delete[] arr; //delete the old table
    arr=new_table;
}

int DynamicHashTable::getNumEmployeesAtHash() const
{
    return this->playersNum;
}
int DynamicHashTable::getTableSize() const
{
    return this->tableSize;
}
//DynamicHashTable::DynamicHashTable(const DynamicHashTable& first, const DynamicHashTable& second):
//        playersNum(first.playersNum+second.playersNum),tableSize(first.tableSize+second.tableSize),
//        arr(new LinkedList<shared_ptr<Player>>*[first.playersNum+second.playersNum])
//{
//    if(first.playersNum != 0)
//    {
//        for (int i = 0; i < first.tableSize; i++)
//        {
//            LinkedList<shared_ptr<Player>>* curr=(first.arr[i]);
//            if(curr!= nullptr)
//            {
//                Node<shared_ptr<Player>>* list_iter= (curr->head);
//                while(list_iter!= nullptr)
//                {
//                    shared_ptr<Player> to_insert_player=list_iter->data;
//                    int to_insert_player_id=to_insert_player->getPlayerId();
//                    int index=this->activateHashTableFunction(to_insert_player_id);
//                    this->arr[index]->insert(to_insert_player);
//                    list_iter=list_iter->next;
//                }
//            }
//        }
//    }
//    if(second.playersNum!=0)
//    {
//        for(int j=0;j<second.tableSize;j++)
//        {
//            LinkedList<shared_ptr<Player>>* curr=(second.arr[j]);
//            if(curr!= nullptr)
//            {
//                Node<shared_ptr<Player>>* list_iter= (curr->head);
//                while(list_iter!= nullptr)
//                {
//                    shared_ptr<Player> to_insert_player=list_iter->data;
//                    int to_insert_player_id=to_insert_player->getPlayerId();
//                    int index=this->activateHashTableFunction(to_insert_player_id);
//                    this->arr[index]->insert(to_insert_player);
//                    list_iter=list_iter->next;
//                }
//            }
//        }
//    }
//
//}

void DynamicHashTable::setEmployeesCompanyId(int groupId)
{
    for(int i=0;i<this->tableSize;i++)
    {
        LinkedList<shared_ptr<Employee>>* curr=(arr[i]);
        if(curr!= nullptr)
        {
            Node<shared_ptr<Employee>>* list_iter= (arr[i])->head;
            while(list_iter!= nullptr)
            {
                shared_ptr<Employee> player=list_iter->data;
                if(player!= nullptr)
                {
                    player->setEmployeeCompanyId(groupId);
                }
                list_iter=list_iter->next;
            }
        }
    }
}

//void DynamicHashTable::cleanHashTable()
//{
//    for(int i=0;i<this->tableSize;i++)
//    {
//        LinkedList<shared_ptr<Player>>* curr=(arr[i]);
//        if(curr!= nullptr)
//        {
//            Node<shared_ptr<Player>>* list_iter= curr->head;
//            while(list_iter!= nullptr)
//            {
//                Node<shared_ptr<Player>>* temp=list_iter;
//                list_iter=list_iter->next;
//                shared_ptr<Player> player=temp->data;
//                int player_id=player->getPlayerId();
//                this->remove(player_id); //remove temp
//                //   this->playersNum--;
//            }
//        }
//
//    }
//}
//
////this is empty
//void DynamicHashTable::insertFromOtherHashTable(const DynamicHashTable& src)
//{
//    this->playersNum=src.playersNum;
//    this->tableSize=src.tableSize;
//    for(int i=0;i<src.tableSize;i++)
//    {
//        LinkedList<shared_ptr<Player>>* curr=(src.arr[i]);
//        if(curr!= nullptr)
//        {
//            Node<shared_ptr<Player>>* list_iter=curr->head;
//            while(list_iter!= nullptr)
//            {
//                shared_ptr<Player> to_insert_player=list_iter->data;
//                int to_insert_player_id=to_insert_player->getPlayerId();
//                int index=this->activateHashTableFunction(to_insert_player_id);
//                this->arr[index]->insert(to_insert_player);
//                list_iter=list_iter->next;
//            }
//
//        }
//
//    }
//}

void DynamicHashTable::print() const  //for debug
{
    for(int i=0;i<tableSize;i++)
    {
        LinkedList<shared_ptr<Employee>>* curr=(arr[i]);
        if(curr!= nullptr)
        {
            Node<shared_ptr<Employee>>* list_iter=curr->head;
            while(list_iter!= nullptr)
            {
                shared_ptr<Employee> to_insert_player=list_iter->data;
                std::cout<<to_insert_player->getEmployeeId()<<std::endl;
                list_iter=list_iter->next;
            }
        }

    }
}

void DynamicHashTable::mergeHash(DynamicHashTable& src)
{
    for (int i = 0; i < src.tableSize; i++)
    {
        LinkedList<shared_ptr<Employee>>* list_iter=(src.arr[i]);
        if(list_iter!= nullptr)
        {
            Node<shared_ptr<Employee>>* curr=list_iter->head;
            while(curr!= nullptr)
            {
                this->insert(curr->data);
                curr=curr->next;
            }
        }
    }
}


