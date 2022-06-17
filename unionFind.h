#ifndef UNIONFIND_H
#define UNIONFIND_H


#include "company.h"

class unionFind {
    int groupsNum;
    int* parentArr;
    int* groupSizeArr;
    double* valueArr;
    double* maxArr;
    Company** grArray;
public:
    unionFind(int groupsNum); //will make k groups
    ~unionFind();
   
    //void Makeset(int x);//return singletone x
    
    void Union(int p, int q, double factor); //return a new union group, p&q deleted.
    
    int Find(int x); //return the group that x belong to

    double getVal(int companyId);

    //call this function after using find to get the real id of the group
    Company* getCompany(int companyId);

    void rootSwap(int companyId1,int companyId2);//we use to make sure the buyer company is the root of the bigger company-
        //we will make a swap otherwise
    
    
};
typedef unionFind* UnionFind;


#endif //UNIONFIND_H