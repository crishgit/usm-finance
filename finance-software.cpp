#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <iostream>
using namespace std;

/*
Use:
    Debt class that stores the debt data and calculates the debt evolution
    * need 'cmath' librery to use pow function

Use Case:
// debt with 1000€, 10% interest, 12 months to pay and 3 extraordinary pays
Debt my_debt(1000.0, 10.0, 12, {
    {1, 100},
    {3, 200},
    {6, 300}
});
// to evolution the debt 
my_debt.passMonth();
// to get the data of the current months
map<string, double> current_month = my_debt.getCurrentMonthData();
// to get the initial data of the debt
my_debt.getInitialData();
// to access to the data of current months
current_month["time_passed"]; 
current_month["current_interest_paid"];


Template Parameters:
    double ammount: the ammount of money that the user owes
    double interest: the interest rate of the debt (n%, 'n' it's the value)
    double time: the time that the user has to pay the debt (in months)
    map<int, double> extraordinary_pays: 
        a map with the extraordinary pays of the debt
        input: {
            month: ammount,
            month: ammount,
            ...
        }

Public Methods:
    map<string,double> getInitialData: returns the initial data of the debt
        output: {
            "initial_debt": ,
            "time": ,
            "interest": ,
            "payment": the payment that the user has to pay each month,
            "total_extraordinary_pay": total sum of the extraordinary pays
        }

    map<string,double> getCurrentMonthData: returns data of the current month 
        output: {
            "time_passed": month that has passed paying the debt,
            "current_interest_paid": interest paid that month,
            "current_debt_paid": debt paid that month,
            "total_paid": total debt substract to that month,
            "total_interest_paid": total interest paid to that month,
            "ammount": debt left to pay,
            "current_extraordinary_pay": extraodinary of that mounth,
            "real_extraordinary_pay": amount used of the extraordinary pay,
            "left_extraordinary_pay": extraordinary pay left to use
        }

    void passMonth: pass a month in the debt evolution
    bool haveDebt: returns true if the user still have debt, false otherwise
    map<int,double> getExtraordinaryPays: returns all the extraordinary pays of the debt

Notes:
    I need to include a epsilon amount in passMonth to avoid the error of the double when the debt reach 0 after the pay.

*/

class Debt {
private:
    // init values -> 
    double initial_debt;
    double interest;
    double time;
    double payment;
    map<int, double> extraordinary_pays;
    double total_extraordinary_pay;

    // current values ->
    double ammount;
    // actual refers to the interest/debt paid for the current month ->
    double current_interest_paid;
    double current_debt_paid;
    double time_passed;
    double current_extraordinary_pay;
    double real_extraordinary_pay;
    double left_extraordinary_pay;
    double total_paid;
    double total_interest_paid;

    double getExtraordinaryTotal() {
        double total = 0;
        for(auto const& pair : this->extraordinary_pays) {
            total += pair.second;
        }
        return total;
    }

public:
    Debt (double ammount, double interest, double time, map<int, double> extraordinary_pays = {}){
        this->initial_debt= ammount;
        this->interest = interest / 100.0;
        this->time = time;
        this->payment = (ammount * (this->interest)) 
            / (1 - pow(1 + (this->interest), -time));
        this->extraordinary_pays = extraordinary_pays;
        this->total_extraordinary_pay = this->getExtraordinaryTotal();

        this->ammount = ammount;
        this->current_interest_paid = 0;
        this->current_debt_paid = 0;
        this->time_passed = 0;
        this->current_extraordinary_pay = 0;
        this->real_extraordinary_pay = 0;
        this->left_extraordinary_pay = this->total_extraordinary_pay;
        this->total_paid = 0;
        this->total_interest_paid = 0;
    }

    void passMonth() {
        if( (this->time_passed) > (this->time) or (this->ammount) <= 0){
            return;
        }

        this->time_passed++;
        this->current_interest_paid = (this->ammount) * (this->interest);
        this->current_debt_paid = (this->payment) - (this->current_interest_paid);

        int debt_result = abs(this->ammount - this->current_debt_paid);
        double epsilon_error_range = 0.00001;

        if (debt_result < epsilon_error_range) {
          this->current_debt_paid = this->ammount;
          this->ammount = 0;
        } else {
          this->ammount -= this->current_debt_paid;
        }

        this->current_extraordinary_pay = extraordinary_pays[this->time_passed];
        debt_result = abs(this->ammount - this->current_extraordinary_pay);
        if (debt_result < epsilon_error_range) {
          this->real_extraordinary_pay = this->ammount;
          this->ammount = 0;
        } else {
          this->real_extraordinary_pay = this->current_extraordinary_pay;
          this->ammount -= this->current_extraordinary_pay;
        }

        this->left_extraordinary_pay -= this->real_extraordinary_pay;
        this->total_paid += this->current_debt_paid + this->real_extraordinary_pay;
        this->total_interest_paid += this->current_interest_paid;
    }

    map<string, double> getCurrentMonthData() {
        map<string, double> data;

        data["time_passed"] = this->time_passed;
        data["current_interest_paid"] = this->current_interest_paid;
        data["current_debt_paid"] = this->current_debt_paid;
        data["total_paid"] = this->total_paid;
        data["total_interest_paid"] = this->total_interest_paid;
        data["ammount"] = this->ammount;
        data["current_extraordinary_pay"] = this->current_extraordinary_pay;
        data["real_extraordinary_pay"] = this->real_extraordinary_pay;
        data["left_extraordinary_pay"] = this->left_extraordinary_pay;

        return data;
    }

    map<string, double> getInitialData() {
        map<string, double> data;

        data["initial_debt"] = this->initial_debt;
        data["time"] = this->time;
        data["interest"] = this->interest;
        data["payment"] = this->payment;
        data["total_extraordinary_pay"] = this->total_extraordinary_pay;

        return data;
    }

    map<int, double> getExtraordinaryPays() {
        return this->extraordinary_pays;
    }

    bool haveDebt() {
        return (this->ammount > 0) ? true : false;
    }
};

int main(){
    map<int, double> extraordinary_pays = {
        {12, 3000.0},
    };
    Debt user_debt(20000, 3, 24, extraordinary_pays);

    cout << "Initial Values: " << endl;
    map<string, double> initial_data = user_debt.getInitialData();
    cout << "Initial debt: " << initial_data["initial_debt"] << endl;
    cout << "Time: " << initial_data["time"] << endl;
    cout << "Interest: " << initial_data["interest"] << endl;
    cout << "Payment: " << initial_data["payment"] << endl;
    cout << endl;

    for(int month = 1; user_debt.haveDebt() ; month++){
        user_debt.passMonth();
        map<string, double> data = user_debt.getCurrentMonthData();
        cout << "Month: " << data["time_passed"] << endl;
        cout << "Interest paid: " << data["current_interest_paid"] << endl;
        cout << "Debt paid: " << data["current_debt_paid"] << endl;
        cout << "Total paid: " << data["total_paid"] << endl;
        cout << "Total interest paid: " << data["total_interest_paid"] << endl;
        cout << "Ammount: " << data["ammount"] << endl;
        cout << "Current extraordinary pay: " << data["current_extraordinary_pay"] << endl;
        cout << "Real extraordinary pay: " << data["real_extraordinary_pay"] << endl;
        cout << "Left extraordinary pay: " << data["left_extraordinary_pay"] << endl;
        cout << endl;
    }


    return 0;
}
