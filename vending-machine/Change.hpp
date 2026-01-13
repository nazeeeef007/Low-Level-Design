#ifndef CHANGE_HPP
#define CHANGE_HPP

class Change {
private:
    int amount; // Changed to int (cents)
public:
    Change(int amount) : amount(amount) {}
    Change() : amount(0) {}

    int getAmount() const { return amount; }

    struct compareByChangeAmount {
        bool operator()(const Change& a, const Change& b) const {
            return a.amount < b.amount; // map sorts ascending; rbegin() gives largest
        }
    };

    Change operator-(const Change& a) const {
        return Change(amount - a.amount);
    }
};
#endif