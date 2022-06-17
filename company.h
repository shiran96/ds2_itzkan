#ifndef COMPANY_H
#define COMPANY_H

#include "dynamic_hash_table.h"
#include "rank_tree.h"

class Company{
private:

    int company_id;
    int current_value;  
    int size;//num of players in company
    int num_of_zero_emploies;
    long long sum_of_zero_bump_grade;
    AvlRankTree company_salary_tree;
    DynamicHashTable company_employies_hash_table;

public:
    explicit Company(int company_id);
    ~Company() = default ;
    Company(const Company& Company) = default;
    void mergeCompanys(Company* Company);
    Company& operator=(const Company& Company)=delete;
    int getCompanyId() const;
    void increaseNumZeroSalaryEmployees();
    void increaseSumZeroSalaryEmployeesGrades(int Grade);
    void increaseCompanySize();
    void decreaseCompanySize();
    void decreaseNumZeroSalaryEmployees();
    void decreaseSumZeroSalaryEmployeesGrades(int Grade);
    void removeEmployeeFromRankTree(int salary, int employee_id);
    void insertEmployeeFromRankTree(int salary, int employee_id, int company_id, int grade);
    int getTotalNumPeopleAtCompany();
    int getTotalNumPeopleAtTree();
    void removeEmployeerFromHash(int employee_id);
    void addEmployeeToHash(const shared_ptr<Employee>& new_player);
    int getCompanySize();
    int getNumPeopleWithSalaryZero();
    int getSumGradesEmployeesWithSalaryZero();
    double getAverageBumpGradeBetweenSalaryByGroup(int lowerLevel,int higherLevel,
                          int* error, int numPeopleAtZero, long long numPeopleAtZeroWithScore);
    int getCompanySumOfHighestMPlayerLevel(int sum,int m);
    void printCompanyTree() ; //for debug
};

#endif /*Company_H*/