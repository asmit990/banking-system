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
        ifstream infile("Bank.data");
        if (!infile) {
            return; // If file doesn't exist, just return
        }
        Account account;
        while (infile >> account) {  // Properly read till EOF
            accounts.insert(make_pair(account.getAccNo(), account));
        }
        infile.close();
        if (!accounts.empty()) {
            Account::setLastAccountNumber(accounts.rbegin()->second.getAccNo() + 1); // Update the next account number
        }
    }

    Account OpenAccount(string fname, string lname, float balance) {
        Account account(fname, lname, balance);
        accounts.insert(make_pair(account.getAccNo(), account));
        SaveAccountsToFile();
        return account;
    }

    Account BalanceEnquiry(long accountNumber) {
        auto itr = accounts.find(accountNumber);
        if (itr != accounts.end()) {
            return itr->second;
        } else {
            throw runtime_error("Account not found.");
        }
    }

    Account Deposit(long accountNumber, float amount) {
        auto itr = accounts.find(accountNumber);
        if (itr != accounts.end()) {
            itr->second.Deposit(amount);
            SaveAccountsToFile();
            return itr->second;
        } else {
            throw runtime_error("Account not found.");
        }
    }

    Account Withdraw(long accountNumber, float amount) {
        auto itr = accounts.find(accountNumber);
        if (itr != accounts.end()) {
            itr->second.Withdraw(amount);
            SaveAccountsToFile();
            return itr->second;
        } else {
            throw runtime_error("Account not found.");
        }
    }

    void CloseAccount(long accountNumber) {
        auto itr = accounts.find(accountNumber);
        if (itr != accounts.end()) {
            cout << "Account Deleted: " << itr->second << endl;
            accounts.erase(accountNumber);
            SaveAccountsToFile();
        } else {
            throw runtime_error("Account not found.");
        }
    }

    void ShowAllAccounts() {
        for (const auto& pair : accounts) {
            cout << "Account " << pair.first << endl << pair.second << endl;
        }
    }

private:
    void SaveAccountsToFile() {
        ofstream outfile("Bank.data", ios::trunc);
        for (const auto& pair : accounts) {
            outfile << pair.second;
        }
        outfile.close();
    }
};

int main() {
    Bank b;
    Account acc1 = b.OpenAccount("John", "Doe", 1000);
    cout << acc1;
    
    b.Deposit(acc1.getAccNo(), 500);
    cout << "After deposit: " << b.BalanceEnquiry(acc1.getAccNo()) << endl;

    try {
        b.Withdraw(acc1.getAccNo(), 1200);
        cout << "After withdrawal: " << b.BalanceEnquiry(acc1.getAccNo()) << endl;
    } catch (InsufficientFunds &e) {
        cout << "Insufficient funds for withdrawal!" << endl;
    }
    
    b.ShowAllAccounts();
    
    b.CloseAccount(acc1.getAccNo());

    return 0;
}




