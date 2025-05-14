#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <sstream>
#include <limits>
#include <regex>
#include <random>

#include "AllCustomers.h"
#include "AllPurchases.h"
using namespace std;

// Regex for strict date format m/d/YY
static const regex dateRx(R"(^\d{1,2}/\d{1,2}/\d{2}$)");

// static generator for 8-digit account numbers
static std::mt19937 acctRng(std::random_device{}());
static std::uniform_int_distribution<int> acctDist(10000000, 99999999);

// Validate m/d/YY date with correct day/month logic
bool isValidDate(const string& date) {
    int m, d, y;
    char slash1, slash2;
    istringstream ss(date);
    if (!(ss >> m >> slash1 >> d >> slash2 >> y)) return false;
    if (slash1 != '/' || slash2 != '/' || m < 1 || m > 12 || d < 1 || y < 0 || y > 99) return false;
    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (d > daysInMonth[m-1]) return false;
    return true;
}

void printAllCustomers(const map<int, AllCustomers>& customerList) {
    // Print table header
    cout << left << setw(7) << "No."
         << setw(13) << "Account #"
         << setw(25) << "Name" << endl;
    cout << string(7 + 13 + 25, '-') << endl;

    int count = 1;
    for (const auto& pair : customerList) {
        const AllCustomers& c = pair.second;
        cout << left << setw(7) << count
             << setw(13) << c.getAccountNumber()
             << setw(25) << (c.getFirstName() + " " + c.getLastName())
             << endl;
        ++count;
    }
}

template<typename Compare>
void sortAndPrintCustomers(const map<int, AllCustomers>& customerList,
                           Compare comp)
{
    // 1) Gather pointers to each customer
    vector<const AllCustomers*> roster;
    roster.reserve(customerList.size());
    for (const auto& kv : customerList)
        roster.push_back(&kv.second);

    // 2) Sort by the user‐supplied comparator
    sort(roster.begin(), roster.end(),
        [&](const AllCustomers* a, const AllCustomers* b){
            return comp(*a, *b);
        });

    // 3) Print header
    cout << left
              << setw(15) << "\nAccount #"
              << setw(25) << "Name" << "\n"
              << string(40, '-') << "\n";

    // 4) Print rows
    for (const auto* cptr : roster) {
        cout << left
                  << setw(15) << cptr->getAccountNumber()
                  << setw(25)
                     << (cptr->getFirstName() + " " + cptr->getLastName())
                  << "\n";
    }
}

// Function to read an integer in specific range
int readIntInRange(const string& prompt, int min, int max) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);
        stringstream ss(line);
        int x;
        if (ss >> x && ss.eof()) {
            if (x >= min && x <= max)
                return x;
            else
                cout << "Please enter a number between " << min << " and " << max << ".\n";
        } else {
            cout << "Invalid input, please enter a number.\n";
        }
    }
}

// Recursive function to add multiple customers
void addCustomersRecursive(int count, map<int, AllCustomers>& customerList) {
    if (count <= 0) return;
    AllCustomers newCust;
    string s;
    // First name (no spaces)
    do {
        cout << "First name: ";
        getline(cin, s);
        if (s.find(' ') != string::npos)
            cout << "Invalid input: spaces not allowed.\n";
    } while (s.find(' ') != string::npos);
    newCust.setFirstName(s);
    // Last name (no spaces)
    do {
        cout << "Last name: ";
        getline(cin, s);
        if (s.find(' ') != string::npos)
            cout << "Invalid input: spaces not allowed.\n";
    } while (s.find(' ') != string::npos);
    newCust.setLastName(s);
    // Street address (use - for spaces; no literal spaces allowed)
    do {
        cout << "Street address (use - for spaces): ";
        getline(cin, s);
        if (s.find(' ') != string::npos)
            cout << "Invalid input: spaces not allowed, use '-' instead.\n";
    } while (s.find(' ') != string::npos);
    newCust.setStreetAddress(s);
    // Zip Code (5 digits)
    string zipStr;
    regex zipRx(R"(^[0-9]{5}$)");
    do {
        cout << "Zip code (5 digits): ";
        getline(cin, zipStr);
        if (!regex_match(zipStr, zipRx)) cout << "Invalid: must be 5 digits.\n";
    } while (!regex_match(zipStr, zipRx));
    newCust.setZipCode(zipStr);
    regex phoneRx(R"(^[0-9]{3}-[0-9]{3}-[0-9]{4}$)");
    string phone;
    do {
        cout << "Phone number (xxx-xxx-xxxx): ";
        getline(cin, phone);
        if (!regex_match(phone, phoneRx)) cout << "Invalid phone format. ";
    } while (!regex_match(phone, phoneRx));
    newCust.setPhoneNumber(phone);
    // City (use - for spaces; no literal spaces allowed)
    do {
        cout << "City (use - for spaces): ";
        getline(cin, s);
        if (s.find(' ') != string::npos)
            cout << "Invalid input: spaces not allowed, use '-' instead.\n";
    } while (s.find(' ') != string::npos);
    newCust.setCity(s);
    // State (use - for spaces; no literal spaces allowed)
    do {
        cout << "State (use - for spaces): ";
        getline(cin, s);
        if (s.find(' ') != string::npos)
            cout << "Invalid input: spaces not allowed, use '-' instead.\n";
    } while (s.find(' ') != string::npos);
    newCust.setState(s);
    // Generate unique account number
    int acct;
    do { acct = acctDist(acctRng); } while (customerList.count(acct));
    newCust.setAccountNumber(acct);
    customerList[newCust.getAccountNumber()] = newCust;
    cout << "\nCustomer added. Account # is " << acct << "\n";
    addCustomersRecursive(count - 1, customerList);
}

// Recursive function to add multiple purchases
void addPurchasesRecursive(int count, AllCustomers& cust) {
    if (count <= 0) return;
    string date;
    do {
        cout << "Purchase date (m/d/YY): ";
        getline(cin, date);
        if (!isValidDate(date)) cout << "Invalid date. Please enter a valid date (m/d/YY).\n";
    } while (!isValidDate(date));
    string item;
    // Item (no spaces)
    do {
        cout << "Item (use - for spaces): ";
        getline(cin, item);
        if (item.find(' ') != string::npos)
            cout << "Invalid input: spaces not allowed, use '-' instead.\n";
    } while (item.find(' ') != string::npos);
    double amt;
    while (true) {
        cout << "Amount: ";
        string line;
        getline(cin, line);
        stringstream ss(line);
        if (ss >> amt && ss.eof()) break;
        cout << "Invalid amount, please enter a number.\n";
    }
    AllPurchases p;
    p.setAccountNumber(cust.getAccountNumber());
    p.setDate(date);
    p.setItem(item);
    p.setAmmount(amt);
    cust.addPurchase(p);
    cout << "\nPurchase added.\n";
    addPurchasesRecursive(count - 1, cust);
}

// Function to update a customer's information
void updateCustomerInfo(map<int, AllCustomers>& customerList) {
    printAllCustomers(customerList);
    int custIndex = readIntInRange("Select customer to update (No.): ", 1, customerList.size());
    auto it = customerList.begin();
    advance(it, custIndex - 1);
    AllCustomers& c = it->second;
    int fieldChoice;
    do {
        cout << "\nSelect field to update:\n"
             << "1. First Name\n"
             << "2. Last Name\n"
             << "3. Street Address\n"
             << "4. City\n"
             << "5. State\n"
             << "6. Zip Code\n"
             << "7. Phone\n"
             << "0. Back\n";
        fieldChoice = readIntInRange("Enter choice: ", 0, 7);
        switch (fieldChoice) {
            case 1: {
                string v;
                do {
                    cout << "New first name: "; getline(cin, v);
                    if (v.find(' ') != string::npos)
                        cout << "Invalid input: spaces not allowed.\n";
                } while (v.find(' ') != string::npos);
                c.setFirstName(v); break;
            }
            case 2: {
                string v;
                do {
                    cout << "New last name: "; getline(cin, v);
                    if (v.find(' ') != string::npos)
                        cout << "Invalid input: spaces not allowed.\n";
                } while (v.find(' ') != string::npos);
                c.setLastName(v); break;
            }
            case 3: {
                // Street address validation
                string v;
                do {
                    cout << "New street address (use - for spaces): "; getline(cin, v);
                    if (v.find(' ') != string::npos)
                        cout << "Invalid input: spaces not allowed, use '-' instead.\n";
                } while (v.find(' ') != string::npos);
                c.setStreetAddress(v); break;
            }
            case 4: {
                string v;
                do {
                    cout << "New city (use - for spaces): "; getline(cin, v);
                    if (v.find(' ') != string::npos)
                        cout << "Invalid input: spaces not allowed, use '-' instead.\n";
                } while (v.find(' ') != string::npos);
                c.setCity(v); break;
            }
            case 5: {
                string v;
                do {
                    cout << "New state (use - for spaces): "; getline(cin, v);
                    if (v.find(' ') != string::npos)
                        cout << "Invalid input: spaces not allowed, use '-' instead.\n";
                } while (v.find(' ') != string::npos);
                c.setState(v); break;
            }
            case 6: {
                string newZip;
                regex zipRxUpdate(R"(^[0-9]{5}$)");
                do {
                    cout << "New zip code (5 digits): ";
                    getline(cin, newZip);
                    if (!regex_match(newZip, zipRxUpdate))
                        cout << "Invalid: must be 5 digits.\n";
                } while (!regex_match(newZip, zipRxUpdate));
                c.setZipCode(newZip);
                break;
            }
            case 7: {
                string v;
                regex phoneRx(R"(^[0-9]{3}-[0-9]{3}-[0-9]{4}$)");
                do {
                    cout << "New phone (xxx-xxx-xxxx): "; getline(cin, v);
                    if (!regex_match(v, phoneRx)) cout << "Invalid phone format. ";
                } while (!regex_match(v, phoneRx));
                c.setPhoneNumber(v); break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    } while (fieldChoice != 0);
}

// Function to delete a customer
bool deleteCustomer(map<int, AllCustomers>& customerList) {
    printAllCustomers(customerList);
    int custIndex = readIntInRange("Select customer to delete (No.): ", 1, customerList.size());
    auto it = customerList.begin();
    advance(it, custIndex - 1);
    int key = it->first;
    cout << "Are you sure you want to delete '"
         << it->second.getFirstName() << " " << it->second.getLastName()
         << "'? (y/n): ";
    char yn; cin >> yn;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (yn == 'y' || yn == 'Y') {
        customerList.erase(key);
        cout << "Customer deleted.\n";
        return true;
    } else {
        cout << "Deletion canceled.\n";
        return false;
    }
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
        cout << "\n[=============================================]" << endl;
        cout << "  Welcome to FitFlex Apparel Customer Manager" << endl;
        cout << "[=============================================]\n" << endl;

        cout << "1. View Customers" << endl;
        cout << "2. Manage Customers" << endl;
        cout << "3. Manage Purchases" << endl;
        cout << "4. File Operations" << endl;
        cout << "5. Exit" << endl;

        input = readIntInRange("\nEnter your choice: ", 1, 5);

        switch (input) {
            case 1: {
                int viewChoice;
                do {
                cout << "\n[-- VIEW CUSTOMERS --]\n\n";
                cout << "1. List all customers" << endl;
                    cout << "2. List customers sorted" << endl;
                cout << "3. View a customer's full account & purchase history" << endl;
                cout << "0. Back" << endl;

                    viewChoice = readIntInRange("\nEnter your choice: ", 0, 4);

                    switch (viewChoice) {
                    case 1:
                        printAllCustomers(customerList);
                        break;
                        case 2: {
                            int sortChoice;
                            do {
                                cout << "\n[-- SORT CUSTOMERS --]\n\n";
                                cout << "1. First Name ↑" << endl;
                                cout << "2. First Name ↓" << endl;
                                cout << "3. Last Name ↑" << endl;
                                cout << "4. Last Name ↓" << endl;
                                cout << "5. Account # ↑" << endl;
                                cout << "6. Account # ↓" << endl;
                                cout << "0. Back" << endl;

                                sortChoice = readIntInRange("\nEnter your choice: ", 0, 6);
                                switch (sortChoice) {
                                    case 1: sortAndPrintCustomers(customerList, [](auto& a, auto& b){ return a.getFirstName() < b.getFirstName(); }); break;
                                    case 2: sortAndPrintCustomers(customerList, [](auto& a, auto& b){ return a.getFirstName() > b.getFirstName(); }); break;
                                    case 3: sortAndPrintCustomers(customerList, [](auto& a, auto& b){ return a.getLastName() < b.getLastName(); }); break;
                                    case 4: sortAndPrintCustomers(customerList, [](auto& a, auto& b){ return a.getLastName() > b.getLastName(); }); break;
                                    case 5: sortAndPrintCustomers(customerList, [](auto& a, auto& b){ return a.getAccountNumber() < b.getAccountNumber(); }); break;
                                    case 6: sortAndPrintCustomers(customerList, [](auto& a, auto& b){ return a.getAccountNumber() > b.getAccountNumber(); }); break;
                                    case 0: break;
                                    default: cout << "Invalid choice.\n"; break;
                                }
                            } while (sortChoice != 0);
                        } break;
                        case 3: {
                            printAllCustomers(customerList);

                            int custIndex = readIntInRange("\nChoose a customer (Specify No.): ", 1, customerList.size());
                            auto it = customerList.begin();
                            advance(it, custIndex - 1);
                            AllCustomers& picked = it->second;
                            picked.printAccountInformation();
                        } break;
                        case 0:
                        break;
                        default:
                            cout << "Invalid choice.\n";
                        break;
                }
                } while (viewChoice != 0);
            } break;
            case 2: {
                int manageChoice;
                do {
                cout << "\n[-- MANAGE CUSTOMERS --]\n\n";
                cout << "1. Add a new customer" << endl;
                cout << "2. Add multiple customers" << endl;
                cout << "3. Update a customer's information" << endl;
                cout << "4. Delete a customer's information" << endl;
                cout << "0. Back" << endl;

                    manageChoice = readIntInRange("\nEnter your choice: ", 0, 4);
                    switch (manageChoice) {
                        case 1: {
                            // Add a single new customer via recursive helper
                            addCustomersRecursive(1, customerList);
                        } break;
                        case 2: {
                            // Add multiple new customers
                            int cnt = readIntInRange("How many customers to add? ", 1, 1000);
                            addCustomersRecursive(cnt, customerList);
                        } break;
                        case 3:
                            updateCustomerInfo(customerList);
                            break;
                        case 4: {
                            // Delete a customer's information
                            deleteCustomer(customerList);
                        } break;
                        case 0:
                break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                } while (manageChoice != 0);
            } break;
            case 3: {
                int purchaseChoice;
                do {
                cout << "\n[-- MANAGE PURCHASES --]\n\n";
                cout << "1. Add a purchase for a customer" << endl;
                cout << "2. Add multiple purchases for a customer" << endl;
                cout << "0. Back" << endl;

                    purchaseChoice = readIntInRange("\nEnter your choice: ", 0, 2);
                    switch (purchaseChoice) {
                        case 1: {
                            // Add a single purchase via recursive helper
                            printAllCustomers(customerList);
                            int custIndex = readIntInRange("Select customer to add purchase to (No.): ", 1, customerList.size());
                            auto it = customerList.begin();
                            advance(it, custIndex - 1);
                            addPurchasesRecursive(1, it->second);
                        } break;
                        case 2: {
                            // Add multiple purchases via recursive helper
                            printAllCustomers(customerList);
                            int custIndex2 = readIntInRange("Select customer to add purchases to (No.): ", 1, customerList.size());
                            auto it2 = customerList.begin();
                            advance(it2, custIndex2 - 1);
                            int count2 = readIntInRange("How many purchases to add? ", 1, 1000);
                            addPurchasesRecursive(count2, it2->second);
                        } break;
                        case 0:
                            break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                } while (purchaseChoice != 0);
            } break;
            case 4: {
                int fileChoice;
                do {
                cout << "\n[-- FILE OPERATIONS --]\n\n";
                cout << "1. Save changes (overwrite existing files)" << endl;
                cout << "2. Save changes to a new file." << endl;
                cout << "0. Back" << endl;

                    fileChoice = readIntInRange("\nEnter your choice: ", 0, 2);
                    switch (fileChoice) {
                        case 1: {
                            // Overwrite existing customer and purchase files
                            ofstream outCust("AllCustomers.txt");
                            if (!outCust.is_open()) {
                                cerr << "Error: Could not open AllCustomers.txt for writing!" << endl;
                                break;
                            }
                            for (auto& kv : customerList) {
                                auto& c = kv.second;
                                outCust << c.getFirstName() << " "
                                        << c.getLastName() << " "
                                        << c.getAccountNumber() << " "
                                        << c.getStreetAddress() << " "
                                        << c.getCity() << " "
                                        << c.getState() << " "
                                        << c.getZipCode() << " "
                                        << c.getPhoneNumber() << "\n";
                            }
                            outCust.close();
                            ofstream outPur("AllPurchases.txt");
                            if (!outPur.is_open()) {
                                cerr << "Error: Could not open AllPurchases.txt for writing!" << endl;
                                break;
                            }
                            for (auto& kv : customerList) {
                                for (auto& p : kv.second.getPurchases()) {
                                    outPur << p.getAccountNumber() << " "
                                           << p.getItem() << " "
                                           << p.getDate() << " "
                                           << p.getAmmount() << "\n";
                                }
                            }
                            outPur.close();
                            cout << "Files saved to AllCustomers.txt and AllPurchases.txt.\n";
                            break;
                        }
                        case 2: {
                            // Save changes to new files
                            cout << "Enter new customers filename: ";
                            string custFile;
                            getline(cin, custFile);
                            cout << "Enter new purchases filename: ";
                            string purFile;
                            getline(cin, purFile);
                            ofstream outCust2(custFile);
                            if (!outCust2.is_open()) {
                                cerr << "Error: Could not open " << custFile << " for writing!" << endl;
                                break;
                            }
                            for (auto& kv : customerList) {
                                auto& c = kv.second;
                                outCust2 << c.getFirstName() << " "
                                         << c.getLastName() << " "
                                         << c.getAccountNumber() << " "
                                         << c.getStreetAddress() << " "
                                         << c.getCity() << " "
                                         << c.getState() << " "
                                         << c.getZipCode() << " "
                                         << c.getPhoneNumber() << "\n";
                            }
                            outCust2.close();
                            ofstream outPur2(purFile);
                            if (!outPur2.is_open()) {
                                cerr << "Error: Could not open " << purFile << " for writing!" << endl;
                                break;
                            }
                            for (auto& kv : customerList) {
                                for (auto& p : kv.second.getPurchases()) {
                                    outPur2 << p.getAccountNumber() << " "
                                            << p.getItem() << " "
                                            << p.getDate() << " "
                                            << p.getAmmount() << "\n";
                                }
                            }
                            outPur2.close();
                            cout << "Files saved to " << custFile << " and " << purFile << ".\n";
                            break;
                        }
                        case 0:
                break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                } while (false);
            } break;
            case 5:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (input != 5);



    return 0;
}