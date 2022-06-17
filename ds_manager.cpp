
#include "ds_manager.h"

StatusType DsManager::DSacquireCompany(int AcquirerID, int TargetID, double Factor)
{
    if(AcquirerID <=0 || TargetID <=0 || AcquirerID >this->num_of_companies || AcquirerID== TargetID
        || TargetID > this->num_of_companies ||Factor<=0.0)
    {
        return INVALID_INPUT;
    }
    try
    {
        unionFindCompanies.Union(AcquirerID, TargetID, Factor);//merging Hash may throw bad alloc
         
    }
    catch (std::bad_alloc &e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType DsManager::DSaddEmployee( int EmployeeID, int CompanyID, int Grade)

{
    if (CompanyID > this->num_of_companies || CompanyID <= 0 || EmployeeID <= 0 ||
        Grade <0)
    {
        return INVALID_INPUT;
    }
    
    if (AllEmploiesHashTable.find(EmployeeID) == true)
    {
        return FAILURE;
    }
    try
    {
        shared_ptr<Employee> new_player(new Employee(EmployeeID, CompanyID, Grade));
        AllEmploiesHashTable.insert(new_player); //may throw bad_alloc
        this->num_of_zero_emploies++;
        this->sum_of_zero_bump_grade += Grade;
        Company* gr = this->unionFindCompanies.getCompany(CompanyID);
        gr->increaseNumZeroSalaryEmployees();
        gr->increaseSumZeroSalaryEmployeesGrades(Grade);
        gr->addEmployeeToHash(new_player);
        gr->increaseCompanySize();
    }
    catch (const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;

}

StatusType DsManager::DSremoveEmployee(int employeeId)
{
    if (employeeId <= 0)
    {
        return INVALID_INPUT;
    }
    if (AllEmploiesHashTable.find(employeeId) == false)
    {
        return FAILURE;
    }
    try
    {
        Node<shared_ptr<Employee>>* player_data = AllEmploiesHashTable.getEmployeeById(employeeId);
        int employee_salary = player_data->data->getEmployeeSalary();
        int employee_company_id = player_data->data->getEmployeeCompanyId();
        int employee_id = player_data->data->getEmployeeId();
        int employee_grade = player_data->data->getEmployeeGrade();
        Company* gr = this->unionFindCompanies.getCompany(employee_company_id); //change- to be logn,
        //go to index in arr directly
        gr->decreaseCompanySize();
        if (employee_salary == 0)
        {
            gr->decreaseSumZeroSalaryEmployeesGrades(employee_grade);
            gr->decreaseNumZeroSalaryEmployees();
            this->num_of_zero_emploies--;
            this->sum_of_zero_bump_grade -= employee_grade;
            
        }
        else
        {
            gr->removeEmployeeFromRankTree(employee_salary, employee_id); //may throw bad_alloc
            this->rankTree.remove(employee_salary, employee_id); //may throw bad_alloc
        }
        gr->removeEmployeerFromHash(employee_id); //may throw bad_alloc
        AllEmploiesHashTable.remove(employee_id); //may throw bad_alloc
    }
    catch (const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


StatusType DsManager::DSemployeeSalaryIncrease(int EmployeeID, int SalaryIncrease)
{
    if(EmployeeID <=0 || SalaryIncrease <=0)
    {
        return INVALID_INPUT;
    }
    if(AllEmploiesHashTable.find(EmployeeID)==false)
    {
        return FAILURE;
    }
    //the player exist
    Node<shared_ptr<Employee>>* player=
                                AllEmploiesHashTable.getEmployeeById(EmployeeID);
    int employeeOldSalary=player->data->getEmployeeSalary();
    int employeeGrade=player->data->getEmployeeGrade();
    int employeeCompanyId = player->data->getEmployeeCompanyId();
    int employeeNewSalary= employeeOldSalary + SalaryIncrease;
    player->data->setEmployeeSalary(employeeNewSalary); //thanks to shared ptr the player
                                                    //level got updated in all of his locations

    //remove old data regard the level
    if(employeeOldSalary ==0)
    {
        this->num_of_zero_emploies--;
        this->sum_of_zero_bump_grade -= employeeGrade;
    }
    else //old level != 0
    {
        try
        {
            rankTree.remove(employeeOldSalary,EmployeeID);
        }
        catch(std::bad_alloc& e)
        {
            return ALLOCATION_ERROR;
        }
    }
    //update the new data regard the level
    try
    {
        rankTree.insert(employeeNewSalary, EmployeeID, employeeCompanyId,employeeGrade);
       // rankTree.printTree(); // added
    }
    catch (std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    //find the group of the player
    
    Company* playerGroup=unionFindCompanies.getCompany(employeeCompanyId);//to change - go to index in arr
    //for logn
    //remove old info regard the player's level
    if(employeeOldSalary==0)
    {
        playerGroup->decreaseNumZeroSalaryEmployees();
        playerGroup->decreaseSumZeroSalaryEmployeesGrades(employeeGrade);
    }
    else
    {
        try
        {
            playerGroup->removeEmployeeFromRankTree(employeeOldSalary,EmployeeID);
        }
        catch(std::bad_alloc& e)
        {
            return ALLOCATION_ERROR;
        }

    }
    //update the new info
    try
    {
        playerGroup->insertEmployeeFromRankTree(employeeNewSalary, EmployeeID,
            employeeCompanyId,employeeGrade);
    }
    catch(std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType DsManager::DSpromoteEmployee(int EmployeeID, int BumpGrade)
{
    if(EmployeeID <=0 )
    {
        return INVALID_INPUT;
    }
    
    if(AllEmploiesHashTable.find(EmployeeID)==false)
    {
        return FAILURE;
    }
    if (BumpGrade <= 0)
    {
        return SUCCESS;
    }
    Node<shared_ptr<Employee>>* playerData=
            AllEmploiesHashTable.getEmployeeById(EmployeeID);
    int playerOldGrade=playerData->data->getEmployeeGrade();
    int playerSalary=playerData->data->getEmployeeSalary();
    int playerCompanyId = playerData->data->getEmployeeCompanyId();
    playerData->data->setEmployeeGrade(playerOldGrade +BumpGrade);
    if(playerSalary ==0)
    {
        this->sum_of_zero_bump_grade += BumpGrade;
    }
    else
    {
        try
        {
            rankTree.remove(playerSalary, EmployeeID);
            rankTree.insert(playerSalary, EmployeeID, playerCompanyId, (playerOldGrade + BumpGrade));
        }
        catch(std::bad_alloc& e)
        {
            return ALLOCATION_ERROR;
        }
    }
    //update the data in the playerGroup:
    int playerGroupId=playerData->data->getEmployeeCompanyId();
    Company* playerGroup=unionFindCompanies.getCompany(playerGroupId);//go to index directly to stay in logn
    if(playerSalary==0)
    {
        playerGroup->increaseSumZeroSalaryEmployeesGrades(BumpGrade);
    }
    else
    {
        try
        {
            playerGroup->removeEmployeeFromRankTree(playerSalary,EmployeeID);
            playerGroup->insertEmployeeFromRankTree(playerSalary, EmployeeID ,
                playerCompanyId,playerOldGrade+BumpGrade);
        }
        catch (std::bad_alloc& e)
        {
            return ALLOCATION_ERROR;
        }
    }
    return SUCCESS;
}

StatusType DsManager::DSavregeGradeInBounds (int CompanyID, int lowerSalary, int higherSalary)
{
    if(CompanyID<0 || CompanyID > this->num_of_companies || higherSalary<0 || lowerSalary<0
        || lowerSalary> higherSalary )
    {
        return INVALID_INPUT;
    }
    double average=0;
    int error=0;
    int numPeopleAtSalarylZero=0;
    long long sum_grades_zero=0;
    if(CompanyID ==0)
    {
        if(lowerSalary ==0) //need to count the players at level zero which aren't in the tree
        {
            int total=AllEmploiesHashTable.getNumEmployeesAtHash();
            int notLevelZero=rankTree.getTotalNumPeopleAtTree();
            numPeopleAtSalarylZero =total-notLevelZero;
            sum_grades_zero = this->sum_of_zero_bump_grade;
        }
        try
        {
            // the next line may throw bad alloc
            average =this->rankTree.getAverageBumpGradeBetweenSalaryByGroup(lowerSalary,
                           higherSalary,&error, numPeopleAtSalarylZero, sum_grades_zero);

            if(error==-1)
            {
                return FAILURE;
            }
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", average);
            return SUCCESS;
        }
        catch (std::bad_alloc& e)
        {
            average =0;
            return ALLOCATION_ERROR;
        }
    }
    else
    {
        Company* group=unionFindCompanies.getCompany(CompanyID);
        if(lowerSalary<=0) //need to count the players at level zero which aren't in the tree
        {
            int total=group->getTotalNumPeopleAtCompany();
            int notLevelZero=group->getTotalNumPeopleAtTree();
            numPeopleAtSalarylZero=total-notLevelZero;
            sum_grades_zero =group->getSumGradesEmployeesWithSalaryZero();
        }
        try
        {
            average=group->getAverageBumpGradeBetweenSalaryByGroup(lowerSalary,
              higherSalary,&error,numPeopleAtSalarylZero,sum_grades_zero);
            if(error==-1)
            {
                return FAILURE;
            }
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", average);
            return SUCCESS;
        }
        catch (std::bad_alloc& e)
        {
            average=0;
            return ALLOCATION_ERROR;
        }

    }
}

StatusType DsManager::DScompanyValue(int company_id)
{
    if (company_id <= 0 || company_id > this->num_of_companies )
    {
        return INVALID_INPUT;
    }
    else
    {
        printf("CompanyValue: %.1f\n", this->unionFindCompanies.getVal(company_id));
        return SUCCESS;
    }
    
   

}

StatusType DsManager::DSsumOfGradeTopEmployee(int CompanyID, int m)
{
    if( CompanyID >this->num_of_companies || CompanyID <0 || m<=0  )
    {
        return INVALID_INPUT;
    }
    long long sum_grades = 0;
    
    if(CompanyID ==0)
    {
        if(this->rankTree.getTotalNumPeopleAtTree() < m)
        {
            sum_grades =0;
            return FAILURE;
        }
        try
        {
            
            int numPeopleAtTree=rankTree.getTotalNumPeopleAtTree();
            int updated_m=(m < numPeopleAtTree)?m:numPeopleAtTree; //we will sum only not level0
            sum_grades =this->rankTree.getSumOfHighestMPlayerLevel(0,updated_m,rankTree.getRoot());
            printf("SumOfBumpGradeBetweenTopWorkersByGroup: %llu\n", sum_grades);
            return SUCCESS;
        }
        catch(std::bad_alloc& e) //should not get here - nothing is allocated!
        {
            sum_grades=0;
            return ALLOCATION_ERROR;
        }
    }
    else
    {
        Company* group=unionFindCompanies.getCompany(CompanyID);
        if(group ==nullptr||group ->getCompanySize() <m ||group->getTotalNumPeopleAtTree() <m)
        {
            sum_grades=0;
            return FAILURE;
        }
        try
        {
            
            //num people at tree > 0
            int numPeopleAtTree=group->getTotalNumPeopleAtTree();
            int updated_m=(m < numPeopleAtTree)?m:numPeopleAtTree; //we will sum only not level0
            sum_grades =group->getCompanySumOfHighestMPlayerLevel(0,updated_m);
            printf("SumOfBumpGradeBetweenTopWorkersByGroup: %llu\n", sum_grades);
            return SUCCESS;
        }
        catch(std::bad_alloc& e) //should not get here - nothing is allocated!
        {
            sum_grades=0;
            return ALLOCATION_ERROR;
        }
    }

    

}
