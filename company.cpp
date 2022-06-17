 #include "company.h"

Company::Company(int company_id): company_id(company_id), current_value(company_id),
    company_salary_tree(AvlRankTree()), company_employies_hash_table(DynamicHashTable()) {};

int Company::getCompanyId() const
{
	return this->company_id;
}

void Company::increaseNumZeroSalaryEmployees()
{
    this->num_of_zero_emploies++;
}

void Company::increaseSumZeroSalaryEmployeesGrades(int Grade)
{
    this->sum_of_zero_bump_grade += Grade;
}

void Company::decreaseNumZeroSalaryEmployees()
{
    this->num_of_zero_emploies--;
}

void Company::decreaseSumZeroSalaryEmployeesGrades(int Grade)
{
    this->sum_of_zero_bump_grade -= Grade;
}

void Company::mergeCompanys(Company* Company) //merge group-->this (group is smaller)
{
    if (this->getTotalNumPeopleAtCompany() == 0 && Company->getTotalNumPeopleAtCompany() == 0)
    {
        return;
    }
    if(this->company_employies_hash_table.getNumEmployeesAtHash() == 0
      && Company->company_employies_hash_table.getNumEmployeesAtHash() == 0) //nothing to merge
    {
        return;
    }
  
    //merge hash
    this->company_employies_hash_table.mergeHash(Company->company_employies_hash_table);
    this->company_employies_hash_table.setEmployeesCompanyId(this->company_id);
   // merge rankTrees
    AvlRankTree merged_tree(Company->company_salary_tree,this->company_salary_tree);
    this->company_salary_tree.empty();
    this->company_salary_tree.fillFromTree(merged_tree);
    merged_tree.empty();
}



//may throw bad alloc
void Company::removeEmployeeFromRankTree(int salary,int employee_id)
{
    this->company_salary_tree.remove(salary,employee_id);
}

void Company::insertEmployeeFromRankTree(int salary, int employee_id,int company_id, int grade)
{
    this->company_salary_tree.insert( salary, employee_id ,company_id, grade);
}

int Company::getCompanySize()
{
    return this->size;
}

void Company::increaseCompanySize()
{
    this->size++;
}

void Company::decreaseCompanySize()
{
    this->size--;
}

int Company::getTotalNumPeopleAtCompany()
{
    return this->company_employies_hash_table.getNumEmployeesAtHash();
}
int Company::getNumPeopleWithSalaryZero()
{
    return this->num_of_zero_emploies;
}

int Company::getSumGradesEmployeesWithSalaryZero()
{
    return this->sum_of_zero_bump_grade;
}

int Company::getTotalNumPeopleAtTree()
{
    return this->company_salary_tree.getTotalNumPeopleAtTree();
}

double Company::getAverageBumpGradeBetweenSalaryByGroup(int lowerLevel,int higherLevel,
                   int* error, int numPeopleAtZero, long long numPeopleAtZeroWithScore)
{
    return this->company_salary_tree.getAverageBumpGradeBetweenSalaryByGroup(lowerLevel,
                         higherLevel,error,numPeopleAtZero,numPeopleAtZeroWithScore);
}

void Company::removeEmployeerFromHash(int employee_id)
{
    this->company_employies_hash_table.remove(employee_id);
}
void Company::addEmployeeToHash(const shared_ptr<Employee>& new_player)
{
    this->company_employies_hash_table.insert(new_player);
}
int Company::getCompanySumOfHighestMPlayerLevel(int sum,int m)
{
    return this->company_salary_tree.getSumOfHighestMPlayerLevel(sum,m, company_salary_tree.getRoot());
}

void Company::printCompanyTree()
{
  this->company_salary_tree.printTree();
}


