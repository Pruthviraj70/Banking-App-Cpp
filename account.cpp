#include "account.h"
#include <filesystem>

namespace fs = std::filesystem;

void init_db(sqlite3 *db) {
    const fs::path dbPath = fs::path(std::getenv("HOME")) / "Library/Application Support/bankingSystem/bankData.db";
    if (!fs::exists(fs::path("~/Library/Application Support/bankingSystem"))) {
        fs::create_directory(fs::path("~/Library/Application Support/bankingSystem"));
    }
    sqlite3_open(dbPath.string().c_str(), &db);
    const char* createtable = "CREATE TABLE IF NOT EXISTS accounts(acc_num INT PRIMARY KEY, balance INT, name VARCHAR(50), "
}

void create_account() {

}

int switch_account(int acc_num) {

}

void change_acc_num(int acc_num) {

}

account::account() {

}

void account::print_acc_details() {

}

void account::print_transaction_history(int no = 5) {

}

void account::deposit() {

}

void account::withdraw() {

}

account::~account() {

}