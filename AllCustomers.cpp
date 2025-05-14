#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "AllCustomers.h"
using namespace std;

// Helper to convert date "m/d/YY" into an integer YYYYMMDD for sorting
static int dateToInt(const string& dateStr) {
    int m, d, y;
    char slash;
    istringstream ss(dateStr);
    ss >> m >> slash >> d >> slash >> y;
    return y * 10000 + m * 100 + d;
}

AllCustomers::AllCustomers(string inputLine) {
    istringstream ss(inputLine);
    ss >> firstName >> lastName >> accountNumber >> streetAddress >> city >> state; // Read up to state
    // Manually extract zipCode as string, then phoneNumber
    std::string tempZip, tempPhone;
    if (ss >> tempZip) zipCode = tempZip;
    if (ss >> tempPhone) phoneNumber = tempPhone; // This assumes phone is last and might have internal hyphens
}

// Other Fuctions
void AllCustomers::printAccountInformation() const {
    // Print customer basic info
    cout << endl << left << setw(15) << "Name:" << firstName  << " " << lastName << endl;
    cout << left << setw(15) << "Account #:" << accountNumber << endl;
    cout << left << setw(15) << "Address:" << streetAddress << ", " << city << ", " << state << " " << zipCode << endl;
    cout << left << setw(15) << "Phone:";
    if (phoneNumber.size() == 12) {
        cout << "(" << phoneNumber.substr(0,3) << ")" << phoneNumber.substr(3) << "\n" << endl;
    } else {
        cout << phoneNumber << "\n" << endl;
    }

    // Print purchases header
    cout << left << setw(12) << "Date"
         << setw(30) << "Item"
         << setw(10) << "Amount" << endl;
    cout << string(50, '-') << endl;

    // Check if there are purchases
    if (purchases.empty()) {
        cout << "No purchases on file for this customer." << endl;
    } else {
        // Sort purchases by date and print
        vector<AllPurchases> sortedPurchases = purchases;
        sort(sortedPurchases.begin(), sortedPurchases.end(),
             [](const AllPurchases& a, const AllPurchases& b) {
                 return dateToInt(a.getDate()) > dateToInt(b.getDate());
             });
        for (auto& purchase : sortedPurchases) {
            purchase.printPurchase();
        }
        
        // Calculate and display total spend
        double total = getTotalSpend();
        cout << string(42, '-') << endl;
        cout << left << setw(42) << "Total Spend:" 
             << "$" << fixed << setprecision(2) << total << endl;
    }
}

// Calculate total spend across all purchases
double AllCustomers::getTotalSpend() const {
    double total = 0.0;
    for (const auto& purchase : purchases) {
        total += purchase.getAmmount();
    }
    return total;
}

void AllCustomers::addPurchase(AllPurchases Purchase) {
    purchases.push_back(Purchase);
}

// Getters
std::string AllCustomers::getFirstName() const { return firstName; }
std::string AllCustomers::getLastName() const { return lastName; }
std::string AllCustomers::getStreetAddress() const { return streetAddress; }
std::string AllCustomers::getCity() const { return city; }
std::string AllCustomers::getState() const { return state; }
std::string AllCustomers::getZipCode() const { return zipCode; }
int AllCustomers::getAccountNumber() const { return accountNumber; }
std::string AllCustomers::getPhoneNumber() const { return phoneNumber; }

// Setters
void AllCustomers::setFirstName(std::string firstName) { this->firstName = firstName; }
void AllCustomers::setLastName(std::string lastName) { this->lastName = lastName; }
void AllCustomers::setStreetAddress(std::string streetAddress) { this->streetAddress = streetAddress; }
void AllCustomers::setCity(std::string city) { this->city = city; }
void AllCustomers::setState(std::string state) { this->state = state; }
void AllCustomers::setAccountNumber(int accountNumber) { this->accountNumber = accountNumber; }
void AllCustomers::setZipCode(const std::string& zipCode) { this->zipCode = zipCode; }
void AllCustomers::setPhoneNumber(const std::string& phoneNumber) { this->phoneNumber = phoneNumber; }

// Getter for purchases
const vector<AllPurchases>& AllCustomers::getPurchases() const {
    return purchases;
}