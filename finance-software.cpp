#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include <typeinfo>
using namespace std;

/*  
## Use:
    Debt class that stores the debt data and calculates the debt evolution
    * need 'cmath' librery to use pow function

## Use Case:
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


## Template Parameters:
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

## Public Methods:
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

## Notes:
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
    double total_debt_paid;
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
        this->total_debt_paid = 0;
        this->total_interest_paid = 0;
    }

    void passMonth() {
        if( (this->time_passed) > (this->time) or (this->ammount) <= 0.0){
            return;
        }

        this->time_passed++;
        this->current_interest_paid = (this->ammount) * (this->interest);
        this->current_debt_paid = (this->payment) - (this->current_interest_paid);

        int debt_result = abs(this->ammount - this->current_debt_paid);
        double epsilon_error_range = 0.00001;

        if (debt_result < epsilon_error_range or 
            (this->current_debt_paid > this->ammount)) {

          this->current_debt_paid = this->ammount;
          this->ammount = 0.0;
        } else {
          this->ammount -= this->current_debt_paid;
        }

        this->current_extraordinary_pay = extraordinary_pays[this->time_passed];
        debt_result = abs(this->ammount - this->current_extraordinary_pay);
        if (debt_result < epsilon_error_range or 
            (this->current_extraordinary_pay > this->ammount) ) {

          this->real_extraordinary_pay = this->ammount;
          this->ammount = 0.0;
        }
        else {
          this->real_extraordinary_pay = this->current_extraordinary_pay;
          this->ammount -= this->current_extraordinary_pay;
        }

        this->left_extraordinary_pay -= this->real_extraordinary_pay;
        this->total_debt_paid +=
            this->current_debt_paid + this->real_extraordinary_pay;
        this->total_interest_paid += this->current_interest_paid;
    }
    map<string, double> getCurrentMonthData() {
        map<string, double> data;

        data["time_passed"] = this->time_passed;
        data["current_interest_paid"] = this->current_interest_paid;
        data["current_debt_paid"] = this->current_debt_paid;
        data["total_debt_paid"] = this->total_debt_paid;
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

void printTable(double ptDebt, int ptInterest, int ptMonth, map<int, double> &extraordinary_pays){

     // ------------------ table ------------------ // 

    Debt user_debt(ptDebt, ptInterest, ptMonth, extraordinary_pays);
    map<string, double> intial_data = user_debt.getInitialData();

    // Reference format:
    // Préstamo de 20000.00$, a un interés del 3.00% durante 3 meses. Cuota inicial de 583.33$.
    // Cuota extraordinaria de 3000.00$ en el mes 12.
    cout <<endl << " Préstamo de " 
        << fixed << setprecision(2) << intial_data["initial_debt"] << "$, "
        << "a un interés del "
        << fixed << setprecision(2) << intial_data["interest"] * 100.0 << "% "
        << "durante "
        << fixed << setprecision(0) << intial_data["time"] << " meses. "
        << "Cuota inicial de "
        << fixed << setprecision(2) << intial_data["payment"] << "$. " << endl;

    // ------ Header ------- // 
    // Put Total Paid first  
    cout << setfill(' ') << setw(4) << "Mes";
    cout << setfill(' ') << setw(15) << "Deuda pagada";
    cout << setfill(' ') << setw(17) << "Interes pagado";
    cout << setfill(' ') << setw(12) << "Deuda";
    // cout << setfill(' ') << setw(27) << " Current extraordinary pay ";
    cout << setfill(' ') << setw(22) << "Pago extraordinario";
    // cout << setfill(' ') << setw(24) << " Left extraordinary pay ";
    cout << setfill(' ') << setw(22) << "Total interes pagado";
    cout << setfill(' ') << setw(15) << "Total pagado";
    cout << endl;

    int fTime;
    double fInterest;
    double fdebt;
    double fxpay = 0;

    for(int month = 1; user_debt.haveDebt() ; month++){
        user_debt.passMonth();
        map<string, double> data = user_debt.getCurrentMonthData();

        fTime = data["time_passed"];
        fInterest = data["total_interest_paid"];
        fxpay += data["real_extraordinary_pay"];  


        cout << setfill(' ') << setw(4) << setprecision(0) << fTime;
        cout << setw(15) << setprecision(2) << data["current_debt_paid"];
        cout << setw(17) << setprecision(2) << data["current_interest_paid"];
        cout << setw(12) << setprecision(2) << data["ammount"];
        // cout << "Current extraordinary pay: " << data["current_extraordinary_pay"];
        cout << setw(22) << setprecision(2) << data["real_extraordinary_pay"];
        // cout << << data["left_extraordinary_pay"];
        cout << setw(22) << setprecision(2) << fInterest;
        cout << setw(15) << setprecision(2) << data["total_debt_paid"] + data["total_interest_paid"];
       cout << endl;
    }

    cout << "La deuda de " << intial_data["initial_debt"] << "$ se pago en " << setprecision(0)<< fTime << " meses de "<< intial_data["time"] << " disponibles." << endl;
    cout << "Fue pagado un total de " << setprecision(2)<<  fInterest << "$ en intereses" << endl;
    cout << "y ademas se pago " << fxpay << "$ en pagos extraordinarios" << endl;
     
}

    // create a report of the debt, with the following data:
    // In what time the debt was paid, what time left 
    // How much interest was paid 
    // How much debt was paid 
    // How much was paid in total 
    // How much was paid in extraordinary payments 
    // What ammount of extraordinary payments was left 

int main(){

    // ------------------ input ------------------ //
    
    // ------ variables ------- //
    bool run = true;
    double debt;
    double interest;
    int time;
    // --- extraordinary pays variables --- //
    string xpay;
    int xpaytime;
    double xpayvalue;
    map<int, double>extraordinary_pays;

    // -------- input variables ---------- //
    bool debtCorrect = false;
    bool timeCorrect = false;
    bool interestCorrect = false;
    bool keyCorrect = false;
    bool valueCorrect = false;
    
    // ---------- starts program -------------- // 
    cout << endl << "Este programa te ayuda a calcular cuanto debes pagar para saldar una deuda pagando mensualmente, tomando en cuenta el interes y los pagos extraordinarios" << endl;

    while (run){

      // remember put in a function later, maybe 
        while (debtCorrect == false){
        cout << "Ingrese monto de la deuda: ";
        cin >> debt;
        cin.clear();
        if (typeid(debt) == typeid(double) and debt > 0) {
          debtCorrect = true;
        } else {
          cout << endl <<"valor invalido, ingrese un valor correcto"<<endl;
          cin.ignore(100, '\n');
          }
        } 
      
 
      // remember put in a function later, maybe 
        while (interestCorrect == false){
        cout << "Ingrese porcentaje de interes: ";
        cin >> interest;
        cin.clear();
        if (typeid(interest) == typeid(double) and interest > 0) {
          interestCorrect= true;
        } else {
          cout << endl <<"valor invalido, ingrese un valor correcto"<< endl;
          cin.ignore(100, '\n');
          }
        }         
          
        
      // remember put in a function later, maybe 
        while (timeCorrect == false){
        cout << "Ingrese numero de meses para pagar la deuda: ";
        cin >> time;
        cin.clear();
        if (typeid(time) == typeid(int) and debt > 0) {
          timeCorrect = true;
        } else {
          cout << endl <<"valor invalido, ingrese un valor correcto" <<endl;
          cin.ignore(100, '\n');
          }
        } 
          
        cout << "Se realizo algun pago extraordinario? (si/no): ";
        cin >> xpay;
        
        while(xpay == "si" or xpay == "s" or xpay == "y" or xpay == "Y"){
            int key;
            double value;
            

            // remember put in a function later, maybe 
            keyCorrect = false; 
            while (keyCorrect == false){
              cout << "Ingrese numero del mes: ";
              cin >> key;
              cin.clear();
              if (typeid(key) == typeid(int) and key <= time) {
                keyCorrect = true;
              } else {
              cout << endl <<"valor invalido, ingrese un valor correcto" <<endl;
              cin.ignore(100, '\n');
                }
            }

            // remember put in a function later, maybe
            valueCorrect = false;
            while (valueCorrect == false){
            cout << "Ingrese monto correspondiente al mes: ";
            cin >> value;
            cin.clear();
            if (typeid(value) == typeid(double) and value <= debt) {
              valueCorrect = true;
            } else {
              cout << endl <<"valor invalido, ingrese un valor correcto" <<endl;
              cin.ignore(100, '\n');
              }
            }

            if (extraordinary_pays.count(key) == 0) {
              extraordinary_pays[key] = value;
            } else {
              extraordinary_pays[key] += value;
            }

            extraordinary_pays.insert(make_pair(key, value));
            cout << "Quieres añadir otro pago extraordinario? (si/no):";
            cin >> xpay;
            };

        run = false;
        printTable(debt, interest, time, extraordinary_pays);

    } 
    
    return 0;
}
