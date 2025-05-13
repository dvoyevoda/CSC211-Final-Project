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
    cout << endl << left << setw(15) << "Name:" << firstName  << " " << lastName << endl;
    cout << left << setw(15) << "Account #:" << accountNumber << endl;
    cout << left << setw(15) << "Address:" << streetAddress << ", " << city << ", " << state << " " << zipCode << endl;
    cout << left << setw(15) << "Phone:" << "+1 " + phoneNumber << "\n" << endl;

    // Print purchases header
    cout << left << setw(12) << "Date"
         << setw(30) << "Item"
         << setw(10) << "Amount" << endl;
    cout << string(50, '-') << endl;

    // Print each purchase
    for (AllPurchases purchase : purchases) {
        purchase.printPurchase();
    }
}
void AllCustomers::addPurchase(AllPurchases Purchase) {
    purchases.push_back(Purchase);
}

// Getters
std::string AllCustomers::getFirstName() { return firstName; }
std::string AllCustomers::getLastName() { return lastName; }
std::string AllCustomers::getStreetAddress() { return streetAddress; }
std::string AllCustomers::getCity() { return city; }
std::string AllCustomers::getState() { return state; }
int AllCustomers::getZipCode() { return zipCode; }
int AllCustomers::getAccountNumber() { return accountNumber; }
std::string AllCustomers::getPhoneNumber() { return phoneNumber; }

// Setters
void AllCustomers::setFirstName(std::string firstName) { this->firstName = firstName; }
void AllCustomers::setLastName(std::string lastName) { this->lastName = lastName; }
void AllCustomers::setStreetAddress(std::string streetAddress) { this->streetAddress = streetAddress; }
void AllCustomers::setCity(std::string city) { this->city = city; }
void AllCustomers::setState(std::string state) { this->state = state; }
void AllCustomers::setAccountNumber(int accountNumber) { this->accountNumber = accountNumber; }
void AllCustomers::setZipCode(int zipCode) { this->zipCode = zipCode; }
void AllCustomers::setPhoneNumber(const std::string& phoneNumber) { this->phoneNumber = phoneNumber; }