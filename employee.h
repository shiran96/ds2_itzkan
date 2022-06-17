#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee
{
    private:
    int employeeId;
    int employeeGrade;
    int employeeSalary;
    int employeeCompanyId;

    public:
    Employee(int employeeId, int employeeCompanyId, int grade);
    ~Employee() = default;
    Employee(const Employee& employee) = default;
    Employee& operator=(const Employee& other) = default;
    
    //get the current information of the Employee
    int getEmployeeId() const;
    int getEmployeeCompanyId() const;
    int getEmployeeSalary() const;
    int getEmployeeGrade() const;
    
    //update the information of the EMPLOYEE
    
    // TODO: chack when function is relvent
    void setEmployeeCompanyId(int groupId);
    
    void setEmployeeSalary(int salary);
    void setEmployeeGrade(int garde);

    // //operation for sorting players by level for AVL tree
    // //maybe class instead with operator()
    // friend bool operator<(const Player& p1, const Player& p2);
    // friend bool operator>(const Player& p1, const Player& p2);

};

#endif /*EMPLOYEE_H*/