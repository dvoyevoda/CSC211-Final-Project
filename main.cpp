#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <sstream>
#include <limits>

#include "AllCustomers.h"
#include "AllPurchases.h"
using namespace std;

void printAllCustomers(map<int, AllCustomers>& customerList) {
    // Print table header
    cout << left << setw(7) << "No."
         << setw(13) << "Account #"
         << setw(25) << "Name" << endl;
    cout << string(7 + 13 + 25, '-') << endl;

    int count = 1;
    for (auto& pair : customerList) {
        AllCustomers& c = pair.second;
        cout << left << setw(7) << count
             << setw(13) << c.getAccountNumber()
             << setw(25) << (c.getFirstName() + " " + c.getLastName())
             << endl;
        ++count;
    }
}

template<typename Compare>
void sortAndPrintCustomers(map<int, AllCustomers>& customerList,
                           Compare comp)
{
    // 1) Gather pointers to each customer
    vector<AllCustomers*> roster;
    roster.reserve(customerList.size());
    for (auto& kv : customerList)
        roster.push_back(&kv.second);

    // 2) Sort by the user‐supplied comparator
    sort(roster.begin(), roster.end(),
        [&](AllCustomers* a, AllCustomers* b){
            return comp(*a, *b);
        });

    // 3) Print header
    cout << left
              << setw(15) << "\nAccount #"
              << setw(25) << "Name" << "\n"
              << string(40, '-') << "\n";

    // 4) Print rows
    for (auto cptr : roster) {
        cout << left
                  << setw(15) << cptr->getAccountNumber()
                  << setw(25)
                     << (cptr->getFirstName() + " " + cptr->getLastName())
                  << "\n";
    }
}

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
                            cout << "\nChoose a customer (Specify No.): ";

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
                        case 1:
                            // TODO: implement add new customer
                            break;
                        case 2:
                            // TODO: implement add multiple customers
                            break;
                        case 3:
                            // TODO: implement update customer
                            break;
                        case 4:
                            // TODO: implement delete customer
                            break;
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
                        case 1:
                            // TODO: implement add purchase
                            break;
                        case 2:
                            // TODO: implement add multiple purchases
                            break;
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
                        case 1:
                            // TODO: implement overwrite save
                            break;
                        case 2:
                            // TODO: implement save to new file
                            break;
                        case 0:
                            break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                } while (fileChoice != 0);
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