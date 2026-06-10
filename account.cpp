#include "account.h"
#include <filesystem>
#include <openssl/sha.h>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(
        reinterpret_cast<const unsigned char*>(input.c_str()),
        input.size(),
        hash
    );

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(hash[i]);
    }
    return ss.str();
}

int callback(void* data, int argc, char** argv, char** colName){
    if (data) {
        struct account* account = static_cast<struct account*>(data);
        account->acc_num = std::stoi(argv[0]);
        account->name = argv[1];
        account->balance = std::stoi(argv[2]);
        account->password = argv[3];
        return 0;
    }

    for (int i = 0; i < argc; i++) {
        std::cout << colName[i] << ": " << (argv[i] ? argv[i] : "NULL") << '\n';
    }

    return 0;
}

int print_callback(void* data, int argc, char** argv, char** colName) {
    for (int i = 0; i < argc; i++){
        std::cout
            << colName[i]
            << ": "
            << argv[i]
            << '\n';
    }

    return 0;
}

void init_db(sqlite3 **db) {
    const fs::path folPath =
    fs::path(std::getenv("HOME")) /
    "Library/Application Support/bankingSystem";

    const fs::path dbPath = folPath / "bankData.db";

    if (!fs::exists(folPath)) {
        fs::create_directories(folPath);
    }
    sqlite3_open(dbPath.string().c_str(), db);
    const char* createtable = "CREATE TABLE IF NOT EXISTS accounts(acc_num INTEGER PRIMARY KEY, name VARCHAR(50), password TEXT, balance INT);";
    sqlite3_exec(*db, createtable, nullptr, nullptr, nullptr);
}

void reset_db(sqlite3 *db) {
    if (fs::exists(fs::path("~/Library/Application Support/bankingSystem/bankData.db"))) {
        fs::remove(fs::path("~/Library/Application Support/bankingSystem/bankData.db"));
    }
    init_db(&db);
}

struct account* create_account(sqlite3 *db) {
    struct account *new_account = new account();
    std::string temp_password;
    std::cout << "Please enter the full name of the account holder: ";
    std::getline(std::cin,new_account->name);

    std::cout << "Please enter the account password: ";
    std::getline(std::cin,temp_password);
    temp_password = sha256(temp_password);

    std::cout << "Please enter the initial deposit ammount: ";
    std::cin >> new_account->balance;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, "INSERT INTO accounts(name, password, balance) VALUES (?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, new_account->name.c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, temp_password.c_str(), -1, nullptr);
    sqlite3_bind_int(stmt, 3, new_account->balance);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;

    sqlite3_exec(db, "SELECT acc_num, name, balance, password FROM accounts WHERE acc_num = (SELECT MAX(acc_num) FROM accounts);", callback, new_account, nullptr);
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Account created." << std::endl;
    std::cout << new_account->acc_num << " is your account number." << std::endl;
    std::cout << "Please take note that all the future interactions with the bank, the account number is going to be used as you identification!!!" << std::endl;
    std::cout << "Current balance is " << new_account->balance <<std::endl;
    std::cout << "-------------------------------" << std::endl;

    std::string createtable = "CREATE TABLE IF NOT EXISTS ?_transactions(tid INTEGER PRIMARY KEY, ttype TEXT, tammount INT, acc_sent INT, ttime TEXT DEFAULT CURRENT_TIMESTAMP);";
    sqlite3_prepare_v2(db, createtable.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, new_account->acc_num);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;

    sqlite3_prepare_v2(db, "INSERT INTO ?_transactions(ttype, tammount) VALUES (?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, "Initial Deposit", -1, nullptr);
    sqlite3_bind_int(stmt, 2, new_account->balance);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;

    return new_account;
}

struct account* delete_account(sqlite3 *db, struct account *accountd) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM accounts WHERE acc_num = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, accountd->acc_num);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;

    std::string deletetable = "DROP TABLE IF EXISTS ?_transactions;";
    sqlite3_prepare_v2(db, deletetable.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, accountd->acc_num);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;

    std::cout << "Deleted Account." << std::endl;
    return switch_account(db);
}

struct account* switch_account(sqlite3 *db) {
    int acc_num;
    std::cout << "Enter the account number(Enter 0 to create new account): ";
    std::cin >> acc_num;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (acc_num) {
        std::string acc_password;
        std::cout << "Please enter the password of the account: ";
        std::getline(std::cin,acc_password);
        acc_password = sha256(acc_password);
        struct account *current_account = new account();
        std::string command = "SELECT acc_num, name, balance, password FROM accounts WHERE acc_num = " + std::to_string(acc_num) + ";";

        sqlite3_exec(db, command.c_str(), callback, current_account, nullptr);
        if (acc_password == current_account->password) {
            return current_account;
        }
        std::cout << "Incorrect account number or password!!" << std::endl;
        return switch_account(db);
    }
    else {
        return create_account(db);
    }
}

void print_acc_details(sqlite3 *db, struct account *accountd) {
    std::cout << "Account details." << std::endl;
    std::cout << "Account Number: " << accountd->acc_num << std::endl;
    std::cout << "Account Name: " << accountd->name << std::endl;
    std::cout << "Account Balance: " << accountd->balance << std::endl;
}

void print_transaction_history(sqlite3 *db, struct account *accountd) {
    std::string command = "SELECT * FROM " + std::to_string(accountd->acc_num) + "_transactions;";
    sqlite3_exec(db, command.c_str(), print_callback, nullptr, nullptr);
}

void withdraw(sqlite3 *db, struct account *accountd) {
    int ammount, acc_sent;
    std::cout << "Enter the amount to withdraw: ";
    std::cin >> ammount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    sqlite3_stmt* stmt;

    if (ammount < accountd->balance && ammount > 0) {
        accountd->balance -= ammount;

        sqlite3_prepare_v2(db, "UPDATE accounts SET balance = ? WHERE acc_num = ?;", -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, accountd->balance);
        sqlite3_bind_int(stmt, 2, accountd->acc_num);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;

        sqlite3_prepare_v2(db, "INSERT INTO &_transactions(ttype, tammount) VALUES (?, ?)", -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, accountd->acc_num);
        sqlite3_bind_text(stmt, 2, "Withdraw", -1, nullptr);
        sqlite3_bind_int(stmt, 3, ammount);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    else {
        std::cout << "Entered invalid ammount." << std::endl;
    }
}

void deposit(sqlite3 *db, struct account *accountd) {
    int ammount;
    std::cout << "Enter the deposite ammount: ";
    std::cin >> ammount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (ammount < 0) {
        std::cout << "Invalid ammount." << std::endl;
        return;
    }
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO &_transactions(ttype, tammount) VALUES (?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, accountd->acc_num);
    sqlite3_bind_text(stmt, 2, "Deposit", -1, nullptr);
    sqlite3_bind_int(stmt, 3, ammount);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;

    accountd->balance += ammount;
}