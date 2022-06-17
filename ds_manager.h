#ifndef DS_MANAGER_H
#define DS_MANAGER_H

#include "rank_tree.h"
#include "employee.h"
#include "dynamic_hash_table.h"
#include "exception"
#include "library2.h"
#include "unionFind.h"
#include <memory>
using std::shared_ptr;

class DsManager
{
    public:
    int num_of_companies;
    int num_of_zero_emploies;
    long long sum_of_zero_bump_grade;
    DynamicHashTable AllEmploiesHashTable;
    unionFind unionFindCompanies;
    AvlRankTree rankTree; 

    DsManager(int k):num_of_companies(k), num_of_zero_emploies(0), sum_of_zero_bump_grade(0),
        AllEmploiesHashTable(DynamicHashTable()), unionFindCompanies(unionFind(k)),
        rankTree(AvlRankTree()){}

    ~DsManager() = default;

    
    StatusType DSacquireCompany(int AcquirerID, int TargetID, double Factor);
    StatusType DSaddEmployee(int EmployeeID, int CompanyID, int Grade);
    StatusType DSremoveEmployee(int PlayerID);
    StatusType DSemployeeSalaryIncrease(int EmployeeID, int SalaryIncrease);
    StatusType DSpromoteEmployee(int EmployeeID, int BumpGrade);
    StatusType DSavregeGradeInBounds(int CompanyID, int
        lowerSalary, int higherSalary);
    StatusType DSsumOfGradeTopEmployee(int CompanyID, int m);
    StatusType DScompanyValue(int company_id);
    

};


#endif /*DS_MANAGER_H*/