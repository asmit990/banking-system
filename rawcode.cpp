
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>
using namespace std;

#define MIN_BALANCE 500

class InsufficientFunds{};

class Account {
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;

public:
    Account(){}
    Account(string fname, string lname, float balance) {
        this->firstName = fname;
        this->lastName = lname;
        this->balance = balance;
        this->accountNumber = NextAccountNumber++;
    }

    long getAccNo() { return accountNumber; }
    string getFirstName() { return firstName; }
    string getLastName() { return lastName; }
    float getBalance() { return balance; }

    void Deposit(float amount) {
        balance += amount;
    }

    void Withdraw(float amount) {
        if (balance - amount < MIN_BALANCE)
            throw InsufficientFunds();
        balance -= amount;
    }

    static void setLastAccountNumber(long accountNumber) {
        NextAccountNumber = accountNumber;
    }

    static long getLastAccountNumber() {
        return NextAccountNumber;
    }

    friend ofstream& operator<<(ofstream& ofs, Account& acc) {
        ofs << acc.accountNumber << endl;
        ofs << acc.firstName << endl;
        ofs << acc.lastName << endl;
        ofs << acc.balance << endl;
        return ofs;
    }

    friend ifstream& operator>>(ifstream& ifs, Account& acc) {
        ifs >> acc.accountNumber;
        ifs >> acc.firstName;
        ifs >> acc.lastName;
        ifs >> acc.balance;
        return ifs;
    }

    friend ostream& operator<<(ostream& os, Account& acc) {
        os << "First Name: " << acc.getFirstName() << endl;
        os << "Last Name: " << acc.getLastName() << endl;
        os << "Account Number: " << acc.getAccNo() << endl;
        os << "Balance: " << acc.getBalance() << endl;
        return os;
    }
};

long Account::NextAccountNumber = 0;

class Bank {
    map<long, Account> accounts;

public:
    Bank() {
        ifstream infile;
        infile.open("Bank.data");
        if (!infile) {
            return;
        }
        Account account;
        while (!infile.eof()) {
            infile >> account;
            accounts.insert(pair<long, Account>(account.getAccNo(), account));
        }
        Account::setLastAccountNumber(account.getAccNo());
        infile.close();
    }

    Account OpenAccount(string fname, string lname, float balance) {
        Account account(fname, lname, balance);
        accounts.insert(pair<long, Account>(account.getAccNo(), account));
        ofstream outfile;
        outfile.open("Bank.data", ios::trunc);
        map<long, Account>::iterator itr;
        for (itr = accounts.begin(); itr != accounts.end(); itr++) {
            outfile << itr->second;
        }
        outfile.close();
        return account;
    }

    Account BalanceEnquiry(long accountNumber) {
        map<long, Account>::iterator itr = accounts.find(accountNumber);
        return itr->second;
    }

    Account Deposit(long accountNumber, float amount) {
        map<long, Account>::iterator itr = accounts.find(accountNumber);
        itr->second.Deposit(amount);
        ofstream outfile;
        outfile.open("Bank.data", ios::trunc);
        map<long, Account>::iterator itr2;
        for (itr2 = accounts.begin(); itr2 != accounts.end(); itr2++) {
            outfile << itr2->second;
        }
        outfile.close();
        return itr->second;
    }

    Account Withdraw(long accountNumber, float amount) {
        map<long, Account>::iterator itr = accounts.find(accountNumber);
        itr->second.Withdraw(amount);
        ofstream outfile;
        outfile.open("Bank.data", ios::trunc);
        map<long, Account>::iterator itr2;
        for (itr2 = accounts.begin(); itr2 != accounts.end(); itr2++) {
            outfile << itr2->second;
        }
        outfile.close();
        return itr->second;
    }

    void CloseAccount(long accountNumber) {
        map<long, Account>::iterator itr = accounts.find(accountNumber);
        cout << "Account Deleted" << itr->second;
        accounts.erase(accountNumber);
        ofstream outfile;
        outfile.open("Bank.data", ios::trunc);
        map<long, Account>::iterator itr2;
        for (itr2 = accounts.begin(); itr2 != accounts.end(); itr2++) {
            outfile << itr2->second;
        }
        outfile.close();
    }

    void ShowAllAccounts() {
        map<long, Account>::iterator itr;
        for (itr = accounts.begin(); itr != accounts.end(); itr++) {
            cout << "Account " << itr->first << endl <<




