#include "unionFind.h"

unionFind::unionFind(int groupsNum):groupsNum(groupsNum),
    parentArr(new int[groupsNum +1]()),groupSizeArr(new int[groupsNum + 1]()),
    valueArr(new double[groupsNum + 1]),
    maxArr(new double[groupsNum +1]),
    grArray(new Company*[groupsNum +1])
{
    for (int i = 1; i < groupsNum+1; ++i) {
        parentArr[i] = -1; //represent empty cell
        groupSizeArr[i] = 1;
        grArray[i] = new Company(i);
        valueArr[i] = i;
        maxArr[i] = i;
    }
}

unionFind::~unionFind() {
    delete[] parentArr;
    delete[] groupSizeArr;
    delete[] valueArr;
    for(int i=1;i<groupsNum+1;i++)
    {
        delete grArray[i];
    }
    delete[] grArray;
}

//void unionFind::Makeset(int x) {
//    this->parents[x] = -1;
//    this->size_[x] = 1;
//}

int unionFind::Find(int x) {   //assume x valid
    if (parentArr[x] == -1)
        return x;
    int parent = this->parentArr[x];
    int old_p = x;
    double relative = 0;
    while (parent > -1) {
        old_p = parent;
        parent = this->parentArr[parent];
        relative += valueArr[old_p];
    }
    relative -= valueArr[old_p];
    //parent = old_p;  //shrink paths
    int temp = x;
    int new_temp=0;
    while (parentArr[temp] != parent) {
        new_temp = parentArr[temp];
        parentArr[temp] = parent;
        valueArr[temp] += relative;
        temp = new_temp;
        relative -= valueArr[temp]; 
    }
    return new_temp;
}

void unionFind::Union(int p_curr, int q_curr, double factor) {//p buyer , q target
    int p = this->Find(p_curr);
    int q = this->Find(q_curr);
    if (p == q) return;
    valueArr[p] += factor * maxArr[q];
    maxArr[p] += factor * maxArr[q];
    if (groupSizeArr[p] > groupSizeArr[q]) {
        //valueArr[p] += factor * valueArr[q];
        //valueArr[q] -= valueArr[p];
        groupSizeArr[p] += groupSizeArr[q];
        groupSizeArr[q] = 0;
        parentArr[q] = p;
        grArray[p]->mergeCompanys(grArray[q]);
        if (valueArr[p] > maxArr[p])
            maxArr[p] = valueArr[p];

        if (valueArr[q] > maxArr[p]) {
            if (valueArr[q] > maxArr[q])
                maxArr[p] = valueArr[q];
            else
                maxArr[p] = maxArr[q];
        }
        else {
            if (maxArr[p] < maxArr[q])
                maxArr[p] = maxArr[q];
        }

        valueArr[q] -= valueArr[p];
    }
    else if (groupSizeArr[p] <= groupSizeArr[q]) {
        
        
        groupSizeArr[q] += groupSizeArr[p];
        groupSizeArr[p] = 0;
        parentArr[p] = q;
        grArray[q]->mergeCompanys(grArray[p]);

        if (valueArr[p] > maxArr[q]) {
            if (valueArr[p] > maxArr[p])
                maxArr[q] = valueArr[p];
            else
                maxArr[q] = maxArr[p];
        }
        else {
            if (maxArr[q] < maxArr[p])
                maxArr[q] = maxArr[p];
        }
        valueArr[p] -= valueArr[q];
        //rootSwap(p, q);
        //double temp_val = valueArr[p];
        //double old_q_val = valueArr[q];
        //valueArr[p] = valueArr[q];
        //valueArr[q] = temp_val;
        //valueArr[p] += factor * old_q_val;
        //valueArr[q] -= valueArr[p];

    }
    //else {
    //    if (p > q) {
    //        groupSizeArr[p] += groupSizeArr[q];
    //        groupSizeArr[q] = 0;
    //        parentArr[q] = p;
    //        MgrArray[q]->MergeGroups(grArray[p]);

    //        //            merged_dc_to_delete[q] = dc_array[q];
    //        //            dc_array[q] = dc_array[p];
    //    }
    //    else {
    //        groupSizeArr[q] += size_[p];
    //        groupSizeArr[p] = 0;
    //        parentArr[p] = q;
    //        MgrArray[q]->MergeGroups(grArray[p]);

    //        //            merged_dc_to_delete[p] = dc_array[p];
    //        //            dc_array[p] = dc_array[q];
    //    }
 /*   }*/
}

Company* unionFind::getCompany(int CompanyId)
{
    int realGroup=this->Find(CompanyId);
    return grArray[realGroup];
}

void unionFind::rootSwap(int companyId1, int companyId2)//id1= root of bigger company
{
    int temp_parent = parentArr[companyId1];
    this->parentArr[companyId1] = companyId2;
    this->parentArr[companyId2] = temp_parent;
}

double unionFind::getVal(int companyId)
{
    double val = 0;
    int curr_company = companyId;
    while (curr_company > -1) {
        val += valueArr[curr_company];
        curr_company = parentArr[curr_company];
    }
    return val;
}