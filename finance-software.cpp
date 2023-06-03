#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <iostream>
using namespace std;

// add extraordinary ammounts to a given time of the month
// the extraordinary ammounts are (ammount, month) and substract them from the debt
// cláusula cuando en un mes, el monto extraodinario + pago mensual es mayor a la deuda restante
// falta: total interes pagados
// test first
class Debt {
private:
    // init values -> 
    int initial_debt;
    int interest;
    int time;
    int payment;

    // current values ->
    int ammount;
    // actual refers to the interest/debt paid for the current month ->
    int current_interest_paid;
    int current_debt_paid;
    int time_passed;
    int total_paid;
    int total_interest_paid;
    // vector <int> extraordinary_ammounts;

public:
    Debt (int ammount, int interest, int time){
        this->initial_debt= ammount;
        this->ammount = ammount;
        this->interest = interest;
        this->time = time;
        this->payment = (ammount * interest) / (1 - pow(1 + interest, -time));
        this->current_interest_paid = 0;
        this->current_debt_paid = 0;
        this->time_passed = 0;
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

        this->total_paid += this->current_debt_paid;
        this->total_interest_paid += this->current_interest_paid;

        // extraordinary_ammounts there
    }

    map<string, int> getMonthData() {
        map<string, int> data;

        data["time_passed"] = this->time_passed;
        data["current_interest_paid"] = this->current_interest_paid;
        data["current_debt_paid"] = this->current_debt_paid;
        data["total_paid"] = this->total_paid;
        data["total_interest_paid"] = this->total_interest_paid;
        data["ammount"] = this->ammount;

        return data;
    }

    map<string, int> getInitialData() {
        map<string, int> data;

        data["initial_debt"] = this->initial_debt;
        data["time"] = this->time;
        data["interest"] = this->interest;
        data["payment"] = this->payment;

        return data;
    }
};

int main(){
    // test case: 
    // Debt user_debt(20000, 3, 24);
    return 0;
}
