#pragma once
#include <string>
#include <vector>
#include "AllPurchases.h"

class AllCustomers {
    public:
        AllCustomers() = default;
        AllCustomers(std::string inputLine);

        void printAccountInformation();
        void addPurchase(AllPurchases Purchase);

        int getAccountNumber();

        void setFirstName(std::string firstName);
        void setLastName(std::string lastName);
        void setStreetAddress(std::string streetAddress);
        void setCity(std::string city);
        void setState(std::string state);
        void setAccountNumber(int accountNumber);
        void setZipCode(int zipCode);
        void setPhoneNumber(int phoneNumber);


    private:
        std::string firstName, lastName, streetAddress, city, state;
        int accountNumber, zipCode, phoneNumber;
        std::vector<AllPurchases> purchases;
};