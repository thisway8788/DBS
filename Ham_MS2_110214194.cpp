//--***********************
//--Name: Jeung Hak Ham
//-- ID : 110214194
//--Date : 12 / 08 / 20
//--Purpose : MS2
//-- * **********************

#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iomanip>
#include <string>
#include <iostream>
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

// Employee struct
struct Employee {
    int employeeNumber;
    char lastName[50];
    char firstName[50];
    char email[100];
    char phone[50];
    char extension[10];
    char reportsTo[100];
    char jobTitle[50];
    char city[50];
};

// prototypes
void run(Connection* conn);
int getInt();
int getIntRange(int min, int max);
int menu();
int findEmployee(Connection* conn, int empNo, Employee* emp);
void displayEmployee(Connection* conn, Employee emp);
void displayAllEmployees(Connection* conn);
void insertEmployee(Connection* conn, Employee emp);
void updateEmployee(Connection* conn, int employeeNumber);
void deleteEmployee(Connection* conn, int employeeNumber);

int main(void) {
	Environment* env = nullptr;
	Connection* conn = nullptr;

	string str;
	string usr = "dbs211_203f13";
	string pass = "18524163";
	string srv = "myoracle12c.senecacollege.ca:1521/oracle12c";

	try {
		env = Environment::createEnvironment(Environment::DEFAULT);
		conn = env->createConnection(usr, pass, srv);
		
        int option;
        Employee emp = { 0 };
        do
        {
            int empNo;
            option = menu();
            switch (option)
            {
            case 1:
                displayEmployee(conn, emp);
                break;
            case 2:
                displayAllEmployees(conn);
                break;
            case 3:
                insertEmployee(conn, emp);
                break;
            case 4:
                cout << "\nEmployee Number: ";
                cin >> empNo;
                updateEmployee(conn, empNo);
                break;
            case 5:
                cout << "\nEmployee Number: ";
                cin >> empNo;
                deleteEmployee(conn, empNo);
                break;
            default:
                break;
            }
        } while (option != 0);
		
		env->terminateConnection(conn);
		Environment::terminateEnvironment(env);
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
	return 0;
}

void run(Connection* conn)
{
	int option;
    Employee emp = { 0 };
	do
	{
        option = menu();
		switch(option)
		{
        case 1:
            displayEmployee(conn, emp);
            break;
        case 2:
            displayAllEmployees(conn);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
		default:
            break;
		}
	} while (option != 0);
}

int getInt() {
    int value;
    bool badEntry;
    char nextChar;
    do {
        badEntry = false;
        cin >> value;
        if (cin.fail()) {
            cout << "Bad integer value, try again: ";
            cin.clear();
            cin.ignore(3000, '\n');
            badEntry = true;
        }
        else {
            nextChar = cin.get();
            if (nextChar != '\n') {
                cout << "Only enter an integer, try again: ";
                cin.ignore(3000, '\n');
                badEntry = true;
            }
        }
    } while (badEntry);
    return value;
}

int getIntRange(int min, int max) {
    int number = getInt();
    while (number < min || number > max) {
        cout << "Invalid value enterd, retry[" << min << " <= value <= " << max << "]: ";
        number = getInt();
    }
    return number;
}

int menu()
{
    cout << "********************* HR Menu *********************" << endl;
    cout << "1) Find Employee" << endl;
    cout << "2) Employees Report" << endl;
    cout << "3) Add Employee" << endl;
    cout << "4) Update Employee" << endl;
    cout << "5) Remove Employee" << endl;
    cout << "0) Exit" << endl;
    cout << "> ";
    return getIntRange(0, 5);
}

int findEmployee(Connection* conn, int empNo, Employee* emp)
{
    Statement* stmt = conn->createStatement();
    const string sql = string("SELECT e.employeenumber, e.lastname, e.firstname, e.email, o.phone, e.extension, e.reportsto, e.jobtitle, o.city FROM dbs211_employees e JOIN offices o ON e.officecode = o.officecode WHERE employeenumber=:1");
    stmt->setSQL("SELECT e.employeenumber, e.lastname, e.firstname, e.email, o.phone, e.extension, e.reportsto, e.jobtitle, o.city FROM dbs211_employees e JOIN offices o ON e.officecode = o.officecode WHERE employeenumber=:1");
    stmt->setInt(1, empNo);

    ResultSet* rs = stmt->executeQuery();
    while (rs->next())
    {
        emp->employeeNumber = rs->getInt(1);
        strcpy(emp->lastName, rs->getString(2).c_str());
        strcpy(emp->firstName, rs->getString(3).c_str());
        strcpy(emp->email, rs->getString(4).c_str());
        strcpy(emp->phone, rs->getString(5).c_str());
        strcpy(emp->extension, rs->getString(6).c_str());
        strcpy(emp->reportsTo, rs->getString(7).c_str());
        strcpy(emp->jobTitle, rs->getString(8).c_str());
        strcpy(emp->city, rs->getString(9).c_str());
    }
    return emp->employeeNumber != 0;

}

void displayEmployee(Connection* conn, Employee emp)
{
    
    cout << endl << "Employee Number: ";
    int empNo = getInt();
	if(!findEmployee(conn, empNo, &emp))
	{
        cout << endl << "Employee " << empNo << " does not exist." << endl;
	} else
	{
        cout << endl << "employeeNumber = " << emp.employeeNumber << endl;
        cout << "lastName = " << emp.lastName << endl;
        cout << "firstName = " << emp.firstName << endl;
        cout << "email = " << emp.email << endl;
        cout << "phone = " << emp.phone << endl;
        cout << "extension = " << emp.extension << endl;
        cout << "reportsTo = " << emp.reportsTo << endl;
        cout << "jobTitle = " << emp.jobTitle << endl;
        cout << "city = " << emp.city << endl << endl;
	}
}

void displayAllEmployees(Connection* conn)
{
    Statement* stmt = conn->createStatement();
    const string sql = string("SELECT e.employeenumber, e.firstname, e.lastname, e.email, o.phone, e.extension, m.firstname || ' ' || m.lastname from dbs211_employees e join offices o on e.officecode = o.officecode left outer join dbs211_employees m on e.reportsto = m.employeenumber");
    stmt->setSQL(sql);

    ResultSet* rs = stmt->executeQuery();
	
    cout << endl << left << setw(8) << "E" << setw(20) << "Employee Name" << setw(34) << "Email" << setw(24) << "Phone" << setw(8) << "Ext" << setw(20) << "Manager" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------" << endl;
    if (!rs->next())
    {
        cout << "no employee data!" << endl;
    } else
    {
        do {
            cout << left << setw(8) << rs->getInt(1)
                << setw(20) << rs->getString(2) + " " + rs->getString(3)
                << setw(34) << rs->getString(4)
                << setw(24) << rs->getString(5)
                << setw(8) << rs->getString(6)
                << setw(20) << rs->getString(7) << endl;
        } while (rs->next());
        cout << endl;
    }
    cout << endl;
}

void insertEmployee(Connection* conn, Employee emp)
{
    int empNo;
    cout << "\nEmployee Number: ";
    cin >> empNo;
    if (findEmployee(conn, empNo, &emp))
    {
        cout << "\nAn employee with the same employee number exists.\n" << endl;
    }
    else
    {
        emp.employeeNumber = empNo;
        cout << "Last Name: ";
        cin >> emp.lastName;
        cout << "First Name: ";
        cin >> emp.firstName;
        cout << "Email: ";
        cin >> emp.email;
        cout << "extension: ";
        cin >> emp.extension;
        cin.ignore();
        cout << "Job Title: ";
        cin.getline(emp.jobTitle, 50);
        
        Statement* stmt = conn->createStatement();
        stmt->setSQL("insert into dbs211_employees (employeenumber, lastname, firstname, email, extension, jobtitle, reportsto, officecode) values(:1, :2, :3, :4, :5, :6, :7, :8)");
        stmt->setInt(1, emp.employeeNumber);
        stmt->setString(2, emp.lastName);
        stmt->setString(3, emp.firstName);
        stmt->setString(4, emp.email);
        stmt->setString(5, emp.extension);
        stmt->setString(6, emp.jobTitle);
        stmt->setString(7, "1002");
        stmt->setString(8, "1");

        stmt->executeUpdate();
        cout << "\nThe new employee is added successfully.\n" << endl;
    }
}

void updateEmployee(Connection* conn, int employeeNumber)
{
    Employee emp;
    if (findEmployee(conn, employeeNumber, &emp))
    {
        cout << "\nNew Extension: ";
        cin >> emp.extension;

        Statement* stmt = conn->createStatement();
        stmt->setSQL("update dbs211_employees set extension = :1 where employeenumber = :2");
        stmt->setString(1, emp.extension);
        stmt->setInt(2, employeeNumber);
        stmt->executeUpdate();
        cout << "\nThe employe is updated.\n" << endl;
    }
}

void deleteEmployee(Connection* conn, int employeeNumber)
{
    Employee emp;
    if (findEmployee(conn, employeeNumber, &emp))
    {
        Statement* stmt = conn->createStatement();
        stmt->setSQL("delete from dbs211_employees where employeenumber = :1");
        stmt->setInt(1, employeeNumber);
        stmt->executeUpdate();
        cout << "\nThe employee is deleted.\n" << endl;
    }
    else
    {
        cout << "\nThe employee does not exist.\n" << endl;
    }
}