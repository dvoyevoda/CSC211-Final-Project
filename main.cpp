#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "AllCustomers.h"
#include "AllPurchases.h"
using namespace std;

void printAllCustomers(map<int, AllCustomers> customerList) {
   // for (const auto& pair : customerList) {
    
  // }
}

int main () {

    // FitFlex Apparel
    ifstream AllCustomersData("AllCustomers.txt");
    ifstream AllPurchasesData("AllPurchases.txt");
    string line;
    map<int, AllCustomers> customerList;

    if (!AllCustomersData) {
        cerr << "Error opening AllCustomers.txt." << endl;
        return 1;
    }
    if (!AllPurchasesData) {
        cerr << "Error opening AllPurchases.txt." << endl;
        return 1;
    }

    while (getline(AllCustomersData, line)) {
        AllCustomers Customer(line);
        customerList[Customer.getAccountNumber()] = Customer;
    }

    while(getline(AllPurchasesData, line)) {
        AllPurchases Purchase(line);
        customerList[Purchase.getAccountNumber()].addPurchase(Purchase);
    }

    int input;

    do {
        cout << "\n===============================" << endl;
        cout << " Welcome to FitFlex Apparel Customer Manager" << endl;
        cout << "===============================\n" << endl;
        cout << "1. (option 1)" << endl;
        cout << "2. (option 2)" << endl;
        cout << "3. (option 3)" << endl;
        cout << "4. (option 4)" << endl;
        cout << "5. (option 5)" << endl;
        cout << "0. (option 0)" << endl;
        cout << "Enter your choice: ";
        cin >> input;
    } while (input != 0);



    return 0;
}