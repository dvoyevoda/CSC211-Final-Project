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

        std::string getFirstName();
        std::string getLastName();
        std::string getStreetAddress();
        std::string getCity();
        std::string getState();
        int getAccountNumber();
        int getZipCode();
        std::string getPhoneNumber();

        void setFirstName(std::string firstName);
        void setLastName(std::string lastName);
        void setStreetAddress(std::string streetAddress);
        void setCity(std::string city);
        void setState(std::string state);
        void setAccountNumber(int accountNumber);
        void setZipCode(int zipCode);
        void setPhoneNumber(const std::string& phoneNumber);


    private:
        std::string firstName, lastName, streetAddress, city, state;
        int accountNumber, zipCode;
        std::string phoneNumber;
        std::vector<AllPurchases> purchases;
};