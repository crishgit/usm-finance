#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <iostream>
using namespace std;

// add extraordinary ammounts to a given time of the month
// the extraordinary ammounts are (ammount, month) and substract them from the debt
// extraordinary months can't be in a month greather than the time of the debt
// cláusula cuando en un mes, el monto extraodinario + pago mensual es mayor a la deuda restante
// test first
class Debt {
private:
    // init values -> 
    double initial_debt;
    double interest;
    double time;
    double payment;
    map<int, double> extraordinary_pays;

    // current values ->
    double ammount;
    // actual refers to the interest/debt paid for the current month ->
    double current_interest_paid;
    double current_debt_paid;
    double time_passed;
    double current_extraordinary_pay;
    double total_paid;
    double total_interest_paid;

public:
    Debt (double ammount, double interest, double time, map<int, double> extraordinary_pays = {}){
        this->initial_debt= ammount;
        this->interest = interest / 100.0;
        this->time = time;
        this->payment = (ammount * (this->interest)) 
            / (1 - pow(1 + (this->interest), -time));
        this->extraordinary_pays = extraordinary_pays;

        this->ammount = ammount;
        this->current_interest_paid = 0;
        this->current_debt_paid = 0;
        this->time_passed = 0;
        this->current_extraordinary_pay = 0;
        this->total_paid = 0;
        this->total_interest_paid = 0;
    }

    void passMonth() {
        if( (this->time_passed) > (this->time) ){
            return;
        }

        this->time_passed++;
        this->current_interest_paid = (this->ammount) * (this->interest);
        this->current_debt_paid = (this->payment) - (this->current_interest_paid);
        this->ammount -= this->current_debt_paid;

        this->current_extraordinary_pay = extraordinary_pays[this->time_passed];
        if(this->current_extraordinary_pay > 0){
            this->ammount -= this->current_extraordinary_pay;
        }
        
        this->total_paid += this->current_debt_paid;
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

        return data;
    }

    map<string, double> getInitialData() {
        map<string, double> data;

        data["initial_debt"] = this->initial_debt;
        data["time"] = this->time;
        data["interest"] = this->interest;
        data["payment"] = this->payment;

        return data;
    }

    map<int, double> getExtraordinaryPays() {
        return this->extraordinary_pays;
    }
};

int main(){
    // test case: 
    // Debt user_debt(20000, 3, 24);
    Debt user_debt(20000, 3, 24);

    cout << "Initial Values: " << endl;
    map<string, double> initial_data = user_debt.getInitialData();
    cout << "Initial debt: " << initial_data["initial_debt"] << endl;
    cout << "Time: " << initial_data["time"] << endl;
    cout << "Interest: " << initial_data["interest"] << endl;
    cout << "Payment: " << initial_data["payment"] << endl;
    cout << endl;

    for(int month = 1; month <= 24; month++){
        user_debt.passMonth();
        map<string, double> data = user_debt.getCurrentMonthData();
        cout << "Month: " << data["time_passed"] << endl;
        cout << "Interest paid: " << data["current_interest_paid"] << endl;
        cout << "Debt paid: " << data["current_debt_paid"] << endl;
        cout << "Total paid: " << data["total_paid"] << endl;
        cout << "Total interest paid: " << data["total_interest_paid"] << endl;
        cout << "Ammount: " << data["ammount"] << endl;
        cout << "Extraordinary pay: " << data["current_extraordinary_pay"] << endl;
        cout << endl;
    }


    return 0;
}
