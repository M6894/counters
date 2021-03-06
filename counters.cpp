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
    Date() {
        year = 0;
        month = 0;
        day = 0;
    }
    Date(int y, int m, int d) {
        year = y;
        month = m;
        day = d;
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

bool operator==(const Date& lhs, const Date& rhs) {
    return (lhs.GetDay() == rhs.GetDay()) && (lhs.GetMonth() == rhs.GetMonth() && (lhs.GetYear() == rhs.GetYear()));
}

struct MonthlyCounters {
    Date date = {0, 0, 0};
    double gvs = 0;
    double hvs = 0;
    double t1 = 0;
    double t2 = 0;
};

bool operator<(const MonthlyCounters& lhs, const MonthlyCounters& rhs) {
    return lhs.date < rhs.date;
}

bool operator==(const MonthlyCounters& lhs, const MonthlyCounters& rhs) {
    return lhs.date == rhs.date;    
}

// TODO
ostream& operator<<(ostream& output, MonthlyCounters counters) {
    cout << counters.gvs << endl;
    return output;
}

Date ParseDate(string& input_date) {
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
                return Date(y, m, d); // TODO: Check
            }                
        }
    } catch (const runtime_error& ex) {
        cerr << "Wrong date format: " << ex.what() << endl;
    } catch (const invalid_argument& arg) {
        cerr << arg.what() << endl;
    }
    return Date(0, 0, 0); // TODO: Check
}

MonthlyCounters ParseCounters(istream& line, MonthlyCounters new_month) {
    bool ok = true;
    char d; 
    ok = ok && (line >> new_month.gvs >> d) && (d == ',');
    ok = ok && (line >> new_month.hvs >> d) && (d == ',');
    ok = ok && (line >> new_month.t1 >> d) && (d == ',');
    ok = ok && (line >> new_month.t2);
    if (ok && (line.peek() == EOF)) {
        return new_month;
    }
    throw logic_error("Input syntax error in counters followed correct date.");
}

class Db {
public:
    Db();
    Db(const string& path) {
        ReadFile(path);
    }
    void ReadFile(const string& path) {
        ifstream input(path);
        MonthlyCounters new_month;
        if (input) {
            // Error message line number init
            int line_count = 0;
            while (!input.eof()) {
                // Error message line number increasing
                ++line_count;
                // Line gathering
                string line_str;
                getline(input, line_str);
                stringstream line(line_str);
                // date string gathering
                string raw_date;
                getline(line, raw_date, ','); // Eats the ','
                // Parse and check the stored date 
                // (the error may appear if the file was edited manually)
                Date date = ParseDate(raw_date);
                if (date.GetMonth() != 0 && line) {
                    new_month.date = date;
                    // TODO: Reuse AddNewMonth 
                    bool ok = true;
                    char d; 
                    ok = ok && (line >> new_month.gvs >> d) && (d == ',');
                    ok = ok && (line >> new_month.hvs >> d) && (d == ',');
                    ok = ok && (line >> new_month.t1 >> d) && (d == ',');
                    ok = ok && (line >> new_month.t2);
                    // Saving the results or show the error
                    if (ok && (line.eof() || (line.peek() == '\r') || (line.peek() == '\n'))) {
                        counters.push_back(new_month);
                    } else {
                        cerr << "Error in line " << line_count << ": " << line_str
                        << "\nSkipping this line." << endl;
                    }
                } else {
                    cerr << "Line " << line_count << endl;
                }
            }
        }
    }

    void AddNewMonth(istream& line) {
        MonthlyCounters new_month;
        string raw_date;
        getline(line, raw_date, ','); // Eats the ','
        // Parse and check input date 
        Date date = ParseDate(raw_date);
        if (line && date.GetMonth() != 0) {
            new_month.date = date;
            try {
                new_month = ParseCounters(line, new_month);
                counters.push_back(new_month);
            } catch(const exception& e) {
                cerr << e.what() << endl;
            }
        }
    }

    double CalculatePayment() {
        if (counters.size() >= 2) {
            double sum = 0;
            sum += (counters[static_cast<int>(counters.size()) - 1].gvs - 
                    counters[static_cast<int>(counters.size()) - 2].gvs) * gvs_rate;
            sum += (counters[static_cast<int>(counters.size()) - 1].hvs - 
                    counters[static_cast<int>(counters.size()) - 2].hvs) * hvs_rate;
            sum += (counters[static_cast<int>(counters.size()) - 1].t1 - 
                    counters[static_cast<int>(counters.size()) - 2].t1) * t1_rate;
            sum += (counters[static_cast<int>(counters.size()) - 1].t2 - 
                    counters[static_cast<int>(counters.size()) - 2].t2) * t2_rate;
            return sum;
        } else {
            return -1; // Not enough data to calculate payment
        }
    }
    
    // void SaveFileData(const string& path) {

    // }

private:
    vector<MonthlyCounters> counters = {};
    const double gvs_rate = 188.53;
    const double hvs_rate = 38.06;
    const double t1_rate = 4.95;
    const double t2_rate = 1.35;  
};


int main() {
    const string path = "counters-data.txt";
    Db db(path);

    string command_line;
    while (getline(cin, command_line)) {
        stringstream ss(command_line);
        string command;
        ss >> command;
        if (command == "add") {
            db.AddNewMonth(ss);
            if (double amount = db.CalculatePayment(); amount == -1) {
                cout << "Please enter at least one more month data to calculate payment." << endl;
            } else {
                cout << "Payment amount: " << amount << endl;
            }
        } else {
            cout << "Unknown command." << endl;
        }
    }
    return 0;
}