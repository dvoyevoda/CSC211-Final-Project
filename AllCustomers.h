#pragma once
#include <string>
#include <vector>
#include "AllPurchases.h"

class AllCustomers {
    public:
        AllCustomers() = default;
        AllCustomers(std::string inputLine);

        void printAccountInformation() const;
        void addPurchase(AllPurchases Purchase);
        double getTotalSpend() const;

        std::string getFirstName() const;
        std::string getLastName() const;
        std::string getStreetAddress() const;
        std::string getCity() const;
        std::string getState() const;
        int getAccountNumber() const;
        std::string getZipCode() const;
        std::string getPhoneNumber() const;

        void setFirstName(std::string firstName);
        void setLastName(std::string lastName);
        void setStreetAddress(std::string streetAddress);
        void setCity(std::string city);
        void setState(std::string state);
        void setAccountNumber(int accountNumber);
        void setZipCode(const std::string& zipCode);
        void setPhoneNumber(const std::string& phoneNumber);
        const std::vector<AllPurchases>& getPurchases() const;

    private:
        std::string firstName, lastName, streetAddress, city, state;
        int accountNumber;
        std::string zipCode;
        std::string phoneNumber;
        std::vector<AllPurchases> purchases;
};