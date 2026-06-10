#include <iostream>
#include "account.h"

void open_account(sqlite3 *db, struct account *account);

int main() {
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Hello, and Welcome to the bank." << std::endl;
    std::cout << "-------------------------------" << std::endl;
    std::cout << std::endl;
    int inp = -101;

    sqlite3 *db;
    init_db(&db);


    std::cout << "1. Create Account\n"
                "2. Open Existing Account\n"
                "0. Quit\n"
                "Enter your choice: ";
    std::cin >> inp;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch (inp) {
        case 1:
            open_account(db, create_account(db));
            break;
        case 2:
            open_account(db, switch_account(db));
            break;
    }
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Thanks! for Visiting the Bank" << std::endl;
    std::cout << "-------------------------------" << std::endl;

    return 0;
}

void open_account(sqlite3 *db, struct account *account) {
    int inp = -101;
    while (inp != 0) {
        std::cout << "-------------------------------" << std::endl;
        std::cout << "1. Withdraw\n"
                     "2. Deposit\n"
                     "3. Transaction History\n"
                     "4. Account Details\n"
                     "5. Change Account\n"
                     "0. Quit\n"
                     "9. Delete Account\n"
                     "Enter your choice: ";
        std::cin >> inp;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (inp) {
            case 1:
                withdraw(db, account);
                break;
            case 2:
                deposit(db, account);
                break;
            case 3:
                print_transaction_history(db, account);
                break;
            case 4:
                print_acc_details(db, account);
                break;
            case 5:
                open_account(db, switch_account(db));
                break;
            case 9:
                open_account(db, delete_account(db, account));
                break;
        }
        std::cout << "-------------------------------" << std::endl;
    }
}