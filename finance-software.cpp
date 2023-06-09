#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include <typeinfo>
using namespace std;


// ------------------ Classes ------------------ //
class Debt  {
private:
    // Initial Variables
    double initial_debt = 0.0;
    double interest = 0.0;
    double months = 0.0;
    double payment = 0.0;
    map<int, double> extraordinary_pays = {};
    double total_extraordinary_pay = 0.0;

    // Current Variables
    double months_paying = 0.0;
    double current_debt = 0.0;
    double current_interest_pay = 0.0;
    double current_debt_pay = 0.0;
    double current_extraordinary_pay = 0.0;

    // Report Variables
    double total_interest_pay = 0.0;
    double total_pay = 0.0;
    double left_extraordinary_pay = 0.0;

    // ------------------ Private Methods ------------------ //
    double getTotalExtraordinaryPay(){
        double total = 0.0;
        for (auto const &pair : this->extraordinary_pays) {
            total += pair.second;
        }
        return total;
    }

    void actualizeCurrentPays(){
        this->current_interest_pay = this->current_debt * this->interest;
        this->current_debt_pay = this->payment - this->current_interest_pay;
        this->current_extraordinary_pay = this->extraordinary_pays[this->months_paying];
    }

    void adjustToMaxPossiblePay(double &pay){
        double abs_difference = abs(this->current_debt - pay);

        if(pay >= this->current_debt or abs_difference < 0.001){
            pay = this->current_debt;
        }
    }

    void payWith(double &pay){
        this->current_debt -= pay;
    }

public:
    // ------------------ Public Methods ------------------ //
    Debt (double initial_debt, double interest, double months, 
          map<int, double> extraordinary_pays){

        // Initial Variables
        this->initial_debt = initial_debt;
        this->interest = interest / 100.0;
        this->months = months;
        this->payment = (this->initial_debt * this->interest) / 
                        (1 - pow(1 + this->interest, -this->months) );
        this->extraordinary_pays = extraordinary_pays; 
        this->total_extraordinary_pay = this->getTotalExtraordinaryPay();

        // Current variables
        this->current_debt = this->initial_debt;
        this->current_interest_pay = 0.0;
        this->current_debt_pay = 0.0;
        this->current_extraordinary_pay = 0.0;
        this->months_paying = 0.0;
        this->total_interest_pay = 0.0;
        this->total_pay = 0.0;
        this->left_extraordinary_pay = this->total_extraordinary_pay;
    }

    void passMonth(){
        if(this->current_debt <= 0){
            return;
        }

        this->months_paying += 1;
        actualizeCurrentPays();
        this->total_interest_pay += this->current_interest_pay;

        adjustToMaxPossiblePay(this->current_debt_pay);
        payWith(this->current_debt_pay);

        adjustToMaxPossiblePay(this->current_extraordinary_pay);
        payWith(this->current_extraordinary_pay);
        this->left_extraordinary_pay -= this->current_extraordinary_pay;

        this->total_pay += this->current_debt_pay + 
                           this->current_extraordinary_pay + 
                           this->current_interest_pay;
    }

    map<string, double> getInitialData(){
        map<string, double> data = {
            {"initial_debt", this->initial_debt},
            {"interest", this->interest},
            {"months", this->months},
            {"payment", this->payment},
            {"total_extraordinary_pays", this->total_extraordinary_pay}
        };

        return data;
    }

    map<string, double> getCurrentMonthData(){
        map<string, double> data = {
            {"months_paying", this->months_paying},
            {"current_debt_paid", this->current_debt_pay},
            {"current_interest_paid", this->current_interest_pay},
            {"current_extraordinary_pay", this->current_extraordinary_pay},
            {"current_debt", this->current_debt},
        };

        return data;
    }

    map<string, double> getReportData(){
        map<string, double> data = {
            {"total_interest_paid", this->total_interest_pay},
            {"total_paid", this->total_pay},
            {"left_extraordinary_pays", this->left_extraordinary_pay},
        };

        return data;
    }

    bool haveDebt(){
        return (this->current_debt > 0) ? true : false;
    }

};

// ------------------ Functions ------------------ //

class Table {
private:
    // vector_rows[row][column] -> string of that column
    vector< vector<string> > vector_rows = {};
    vector<int> columns_width = {};
    int columns_ammount = 0;
    int columns_separation = 3;
    char columns_separator = '|';
    int rows_ammount = 0;

    // ------------------ Private Methods ------------------ // 
    void calculateColumnsWidth(){
        this->columns_width = {};

        for(int i = 0; i < this->columns_ammount; i++){
            int max_width = 0;

            for(int j = 0; j < this->rows_ammount; j++){
                int current_width = this->vector_rows[j][i].size();

                if(current_width > max_width){
                    max_width = current_width;
                }
            }

            this->columns_width.push_back(max_width);
        }
    }

    void printRow(int row_index){
        // format:
        // 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10
        vector<string> row = this->vector_rows[row_index];
        for(int column_i = 0; column_i < this->columns_ammount; column_i++){
            string column = row[column_i];
            int column_width = this->columns_width[column_i];

            cout << setw(column_width + this->columns_separation) 
                 << column << this->columns_separator;
        }

        cout << endl;
    }

public:
    Table(int columns_ammount, int columns_separation = 3, 
          char columns_separator = '|'){

        if(columns_ammount <= 0){
            throw invalid_argument("Columns ammount must be greater than 0");
        }
        if(columns_separation <= 0){
            throw invalid_argument("Columns separation must be greater than 0");
        }

        this->columns_ammount = columns_ammount;
        this->columns_separation = columns_separation;
        this->columns_separator = columns_separator;
    }

    void addRow (vector<string> row){
        if(row.size() != this->columns_ammount){
            throw invalid_argument("Row size must be equal to columns ammount");
        }

        this->vector_rows.push_back(row);
        this->rows_ammount += 1;
    }

    void printTable(){
        this->calculateColumnsWidth();
        for(int row_i = 0; row_i < this->rows_ammount; row_i++){
            this->printRow(row_i);
        }
    }

    vector<int> getColumnWidth(){
        return this->columns_width;
    }
};

string to_string_with_N_decimals(double number, int decimals = 2){
    // flujo de string, el fixed le avisa que es de punto fijo para 
    // los decimales, el setprecision es la cantidad de decimales
    // luego le pasa el número. Devuelve el string del número.
    stringstream ss;
    ss << fixed << setprecision(decimals) << number;
    return ss.str();
}

int main () {
    double ptDebt = 20000.0;
    double ptInterest = 3.0;
    int ptMonth = 24;
    map<int, double> extraordinary_pays = {
        {12, 3000.0}
    };

    Debt user_debt(ptDebt, ptInterest, ptMonth, extraordinary_pays);
    Table table_debt(7, 2);
    vector<string> header_debt_table = {"Mes", "Deuda Pagada", "Interes Pagado", "Deuda", 
                          "Pago Extraordinario", "Interes Total", 
                          "Total Pagado"};
    table_debt.addRow(header_debt_table);

    while(user_debt.haveDebt() ){
        user_debt.passMonth();
        map<string, double> current_month_data = user_debt.getCurrentMonthData();
        map<string, double> report_data = user_debt.getReportData();

        table_debt.addRow(
            {
                "Mes "+ to_string_with_N_decimals( current_month_data["months_paying"], 0), 
                to_string_with_N_decimals( current_month_data["current_debt_paid"] ),
                to_string_with_N_decimals( current_month_data["current_interest_paid"] ),
                to_string_with_N_decimals( current_month_data["current_debt"] ),
                to_string_with_N_decimals( current_month_data["current_extraordinary_pay"] ),
                to_string_with_N_decimals( report_data["total_interest_paid"] ),
                to_string_with_N_decimals( report_data["total_paid"] ),
            }
        );
    }
    
    table_debt.printTable();

    return 0;
}

















/*
void printTable(double ptDebt, int ptInterest, int ptMonth, map<int, double> &extraordinary_pays){

     // ------------------ table ------------------ // 

    Debt user_debt(ptDebt, ptInterest, ptMonth, extraordinary_pays);
    map<string, double> intial_data = user_debt.getInitialData();

    // Reference format:
    // Préstamo de 20000.00$, a un interés del 3.00% durante 3 meses. Cuota inicial de 583.33$.
    // Cuota extraordinaria de 3000.00$ en el mes 12.
    cout << endl << " Préstamo de " 
        << fixed << setprecision(2) << intial_data["initial_debt"] << "$, "
        << "a un interés del "
        << fixed << setprecision(2) << intial_data["interest"] * 100.0 << "% "
        << "durante "
        << fixed << setprecision(0) << intial_data["time"] << " meses. "
        << "Cuota inicial de "
        << fixed << setprecision(2) << intial_data["payment"] << "$. ";

    cout << endl << endl << endl;

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

    for(int month = 1; user_debt.haveDebt() ; month++){
        user_debt.passMonth();
        map<string, double> data = user_debt.getCurrentMonthData();

        fTime = data["time_passed"];


        cout << setfill(' ') << setw(4) << setprecision(0) << fTime;
        cout << setw(15) << setprecision(2) << data["current_debt_paid"];
        cout << setw(17) << setprecision(2) << data["current_interest_paid"];
        cout << setw(12) << setprecision(2) << data["ammount"];
        // cout << "Current extraordinary pay: " << data["current_extraordinary_pay"];
        cout << setw(22) << setprecision(2) << data["real_extraordinary_pay"];
        // cout << << data["left_extraordinary_pay"];
        cout << setw(22) << setprecision(2) << data["total_interest_paid"];
        cout << setw(15) << setprecision(2) << data["total_debt_paid"] + data["total_interest_paid"];
      
       cout << endl;
    }

    cout << endl << endl << "La deuda se pago en " << setprecision(0)<< intial_data["time"] - fTime << " meses de "<< fTime << " disponibles" << endl;

     
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
        cout << "Ingrese procentaje de interes: ";
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

            extraordinary_pays.insert(make_pair(key, value));
            cout << "Quieres añadir otro pago extraordinario? (si/no):";
            cin >> xpay;
            };

        run = false;
        printTable(debt, interest, time, extraordinary_pays);
    } 
    
    return 0;
}

*/
