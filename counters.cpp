#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <set>

using namespace std;

class Date {
public:
    Date();
    Date(vector<int> input_date) {
        year = input_date[0];
        month = input_date[1];
        day = input_date[2];
    }
    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }
private:
    int year = 0;
    int month = 0;
    int day = 0;
};

ostream& operator<<(ostream& output, const Date& date) {
    cout << setw(4) << setfill('0') << date.GetYear() << "-" << setw(2) << date.GetMonth() << "-" << setw(2) << date.GetDay();
    return output;
}

bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() != rhs.GetYear()) {
        return lhs.GetYear() < rhs.GetYear();
    } else if (lhs.GetMonth() != rhs.GetMonth()) {
        return lhs.GetMonth() < rhs.GetMonth();
    } else {
        return lhs.GetDay() < rhs.GetDay();
    }
}

struct MonthlyCounters {
    Date date;
    double gvs = 0;
    double hvs = 0;
    double t1 = 0;
    double t2 = 0;
};

bool operator<(MonthlyCounters lhs, MonthlyCounters rhs) {
    return lhs.date < rhs.date;
}

vector<int> ParseDate(istream& input) {
    if (input) {
        // Сохраняем введенную дату для вывода ошибки
        string input_date;
        getline(input, input_date, ' ');

        stringstream user_date(input_date);

        int y, m, d;
        char def1, def2;
        try {
            user_date >> y >> def1 >> m >> def2 >> d;
            if (!user_date) {
                throw runtime_error(input_date);
            } else if (def1 != '-' || def2 != '-') {
                throw runtime_error(input_date); 
            } else if (user_date.peek() != EOF) {
                throw runtime_error(input_date); 
            } else {
                if (m > 12 || m < 1) {
                    throw invalid_argument("Month value is invalid: " + to_string(m));
                } else if (d > 31 || d < 1) {
                    throw invalid_argument("Day value is invalid: " + to_string(d));
                } else {
                    return {y, m, d};
                }                
            }
        } catch (const runtime_error& ex) {
            cerr << "Wrong date format: " << ex.what() << endl;
        } catch (const invalid_argument& arg) {
            cerr << arg.what() << endl;
        }
    }
    return {0, 0, 0};
}

/*
class Db {
public:
    void GetFileData(const string& path) {
        ifstream input(path);
        MonthlyCounters new_month;
        if (input) {
            while (input.peek() != EOF) {
                string line_str;
                getline(input, line_str);
                stringstream line(line_str);
                string tmp_date;
                getline(line, tmp_date, ',');
                stringstream date_to_parse(tmp_date);
                // Check the date was ok in file 
                // (the error may appear if the file was edited manually)
                vector<int> date = ParseDate(date_to_parse);
                if (!date.empty()) {
                    // Add temp new_month? to be checked whether i can assign vector to date.
                    Date tmp_date(date);
                    new_month.date = tmp_date;
                    char d; // Delimiter
                    line >> d;
                    if (d == ',') line >> new_month.gvs >> d;
                    if (d == ',') line >> new_month.hvs >> d;
                    if (d == ',') line >> new_month.t1 >> d;
                    if (d == ',') line >> new_month.t2;
                    if (input) {
                        counters.insert(new_month);
                    }
                }
            }
        }
    }

    // void GetNewMonthData() {

    // }

    // void CalculatePayment() {

    // }
    
    // void SaveFileData(const string& path) {

    // }

private:
    set<MonthlyCounters> counters;
    const int gvs_rate = 188.53;
    const int hvs_rate = 38.06;
    const int t1_rate = 4.95;
    const int t2_rate = 1.35;  
};
*/



int main() {
    const string path = "counters-data.txt";
    // Db db;
    // db.GetFileData(path);
    return 0;
}