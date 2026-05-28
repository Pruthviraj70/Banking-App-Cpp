#ifndef ACCOUNT_H
#define ACCOUNT_H
//#include <string>

void create_account();
int switch_account(int acc_num);
void change_account_details(int acc_num);

class account {
    public:
        account();
        void print_acc_details();
        void print_transaction_history(int no = 5);
        void withdraw();
        void deposit();
        ~account();
    private:
        int acc_num;
        int balance;
        str::string name;
};


#endif //ACCOUNT_H
