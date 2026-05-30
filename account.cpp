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

void init_db(sqlite3 *db) {
    const fs::path dbPath = fs::path(std::getenv("HOME")) / "Library/Application Support/bankingSystem/bankData.db";
    if (!fs::exists(fs::path("~/Library/Application Support/bankingSystem"))) {
        fs::create_directory(fs::path("~/Library/Application Support/bankingSystem"));
    }
    sqlite3_open(dbPath.string().c_str(), &db);
    const char* createtable = "CREATE TABLE IF NOT EXISTS accounts(acc_num INTEGER PRIMARY KEY, password TEXT, balance INT, name VARCHAR(50)";
    sqlite3_exec(db, createtable, nullptr, nullptr, nullptr);
}

void reset_db(sqlite3 *db) {
    if (fs::exists(fs::path("~/Library/Application Support/bankingSystem/bankData.db"))) {
        fs::remove(fs::path("~/Library/Application Support/bankingSystem/bankData.db"));
    }
    init_db(db);
}

void create_account() {

}

int switch_account(int acc_num) {

}

void change_acc_num(int acc_num) {

}

void print_acc_details() {

}

void print_transaction_history(int no) {

}

void withdraw() {

}

void deposit() {

}