#pragma once
#include <string>

class AllPurchases {
    public:
        AllPurchases() = default;
        AllPurchases(std::string inputLine);

        void printPurchase() const;

        int getAccountNumber() const;
        std::string getItem() const;
        std::string getDate() const;
        double getAmmount() const;

        void setItem(std::string item);
        void setDate(std::string date);
        void setAccountNumber(int accountNumber);
        void setAmmount(double ammount);
        
    private:
        std::string item, date;
        int accountNumber;
        double ammount;
};