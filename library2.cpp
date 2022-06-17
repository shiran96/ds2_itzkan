
#include "library2.h"
#include "ds_manager.h"

void* Init(int k)
{
    if (k <= 0  )
    {
        return NULL;
    }
    DsManager* CDS = new DsManager(k);
    return (void*)CDS;
}

StatusType AcquireCompany(void* DS, int companyID1, int companyID2, double factor)
{
    if (DS == NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DSacquireCompany(companyID1, companyID2,factor);
}

StatusType AddEmployee(void* DS, int employeeID, int companyID, int grade)
{
    if (DS == NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DSaddEmployee(employeeID, companyID, grade);
}

StatusType RemoveEmployee(void* DS, int employeeID)
{
    if (DS == NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DSremoveEmployee(employeeID);
}

StatusType EmployeeSalaryIncrease(void* DS, int employeeID, int salaryIncrease)
{
    if (DS == NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DSemployeeSalaryIncrease(employeeID, salaryIncrease);
}

StatusType PromoteEmployee(void* DS, int employeeID, int bumpGrade)
{
    if (DS == NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DSpromoteEmployee(employeeID, bumpGrade);
}

StatusType AverageBumpGradeBetweenSalaryByGroup(void* DS, int companyID,
                                        int lowerSalary, int higherSalary)
{
    if (DS == NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DSavregeGradeInBounds(companyID,lowerSalary, higherSalary);
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void* DS, int companyID, int m)
{
    if (DS == NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DSsumOfGradeTopEmployee(companyID, m);
}

StatusType CompanyValue(void* DS, int companyID)
{
    if (DS == NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DScompanyValue(companyID);
}

void Quit(void** DS)
{
    delete (DsManager*)*DS;
    *DS = NULL;
}