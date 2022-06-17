
#include "employee.h"

Employee::Employee(int employee_id, int employee_company_id,int employee_Grade):
    employeeId(employee_id), employeeGrade(employee_Grade), employeeSalary(0),
    employeeCompanyId(employee_company_id)
    {};

int Employee::getEmployeeId() const
{
    return this->employeeId;
}
int Employee::getEmployeeCompanyId() const
{
    return this->employeeCompanyId;
}

int Employee::getEmployeeSalary() const
{
    return this->employeeSalary;
}
int Employee::getEmployeeGrade() const
{
    return this->employeeGrade;
}

//TODO: is relevent?
void Employee::setEmployeeCompanyId(int companyId)
{
    this->employeeCompanyId= companyId;
}

void Employee::setEmployeeSalary(int salary)
{
    this->employeeSalary= salary;
}
void Employee::setEmployeeGrade(int grade)
{
    this->employeeGrade = grade;
}

