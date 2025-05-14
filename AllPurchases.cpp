#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "AllPurchases.h"
using namespace std;

AllPurchases::AllPurchases(string inputLine) {
    istringstream ss(inputLine);

    ss >> accountNumber >> item >> date >> ammount;
}

void AllPurchases::printPurchase() const {
    cout << left << setw(12) << date
        << setw(30) << item
        << "$" << setw(9) << fixed << setprecision(2) << ammount
        << endl;
}

// Getters
int AllPurchases::getAccountNumber() const { return accountNumber; }
std::string AllPurchases::getItem() const { return item; }
std::string AllPurchases::getDate() const { return date; }
double AllPurchases::getAmmount() const { return ammount; }

// Setters
void AllPurchases::setItem(string item) { this->item = item; }
void AllPurchases::setDate(string date) { this->date = date; }
void AllPurchases::setAccountNumber(int accountNumber) { this->accountNumber = accountNumber; }
void AllPurchases::setAmmount(double ammount) { this->ammount = ammount; }