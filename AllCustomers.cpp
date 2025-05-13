#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "AllCustomers.h"
using namespace std;

AllCustomers::AllCustomers(string inputLine) {
    istringstream ss(inputLine);
    ss >> firstName >> lastName >> accountNumber >> streetAddress >> city >> state >> zipCode >> phoneNumber;
}

// Other Fuctions
void AllCustomers::printAccountInformation() {
    // Print customer basic info
    cout << left << setw(15) << "Name:" << firstName  << " " << lastName << endl;
    cout << left << setw(15) << "Account #:" << accountNumber << endl;
    cout << left << setw(15) << "Address:" << streetAddress << ", " << city << ", " << state << " " << zipCode << endl;
    cout << left << setw(15) << "Phone:" << phoneNumber << "\n" << endl;

    // Print purchases header
    cout << left << setw(12) << "Date"
         << setw(30) << "Item"
         << setw(10) << "Amount" << endl;
    cout << string(52, '-') << endl;

    // Print each purchase
    for (AllPurchases purchase : purchases) {
        purchase.printPurchase();
    }
}
void AllCustomers::addPurchase(AllPurchases Purchase) {
    purchases.push_back(Purchase);
}

// Getters
int AllCustomers::getAccountNumber() {return accountNumber;}

// Setters
void AllCustomers::setFirstName(std::string firstName) {}
void AllCustomers::setLastName(std::string lastName) {}
void AllCustomers::setStreetAddress(std::string streetAddress) {}
void AllCustomers::setCity(std::string city) {}
void AllCustomers::setState(std::string state) {}
void AllCustomers::setAccountNumber(int accountNumber) {}
void AllCustomers::setZipCode(int zipCode) {}
void AllCustomers::setPhoneNumber(int phoneNumber) {}