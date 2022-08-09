#include <iostream>
#include <fstream>
#include <vector>
#include <math.h> /* floor */

bool isLeapYear(int year){
    // year number must be divisible by four
    // except for end-of-century years, which must be divisible by 400
    if (year%4 == 0) return true;
    if (year%100 == 0) return false;
    if (year%400 == 0) return true;
    return false;
}

int daysInMonth(int year, int month){
    // get days in month from array (0 Jan, 1 Feb, etc...)
    int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // leap year feb has 29 days
    if (month == 2 && isLeapYear(year)) return 29;
    return monthDays[month-1];
}

int dayOfWeek(int year, int month, int day){
    // use Zeller’s Algorithm to calculate day of the week
    // get month code
    int month_code;
    if (month <= 2){
        // Jan, Feb result in 13, 14
        // Year is -1
        month_code = (12 + month); 
        year--;
    } else {
        // all other months code is the same as month index
        month_code = month;
    }
    // last two digits of year
    int y = year % 100;
    // first two digits of year
    int c = year / 100;
    // calculate day code
    int day_code = (day + floor((13*(month_code+1))/5) + y + floor(y/4) + floor(c/4) + (5*c));
    day_code = day_code % 7;
    int day_indices[7] = { 6, 7, 1, 2, 3, 4, 5};
    return day_indices[day_code];
}

std::string generateCal(int year){   
    std::cout << year << std::endl;
    std::string day_names[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    for (int month = 1; month <= 12;  month++){
        for (int day_in_week = 1; day_in_week <= 7;  day_in_week++){
            std::cout << day_names[day_in_week - 1] << " ";
        }
        std::cout << std::endl;
        int first_day_in_week = dayOfWeek(year, month, 1);
        for (int day_in_week = 1; day_in_week <= first_day_in_week - 1; day_in_week++){
            std::cout << "[  ]";
        }
        int numDays = daysInMonth(year, month);
        for (int day_in_month = 1; day_in_month < numDays; day_in_month++){
            int day_in_week = dayOfWeek(year, month, day_in_month);
            if (day_in_month < 10){
                std::cout << "[0" << day_in_month << "]";
            } else {
                std::cout << "[" << day_in_month << "]";
            }
            if (day_in_week == 7){
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // TODO format in HTML string
    std::string html_data;
    return html_data;
}

int exportCal(std::vector<std::string> year_data, std::string filepath){
    // export calendar data to html file
    std::string test_data = "";
    std::ofstream myfile;
    myfile.open(filepath);
    myfile << test_data;
    myfile.close();
    return 0;
}

int main(int argc,char* argv[]){
    // Inputs:
    //      year (int)
    //      filepath (string)
    // Outputs:
    //      HTML file

    // Check valid number of arguments provided
    if(argc!=3){
        std::cerr << "Invalid number of arguments." << std::endl;
        std::cerr << "Required format: 'CalGen year filepath'." << std::endl;
        std::cerr << "Example 'CalGen 2000 Calendar2000.html'" << std::endl;
        return 1;
    }
    
    // Get filepath and year from arguments
    std::string filepath = argv[2];
    std::string year_str = argv[1];
    int year = 0;
    try {
        std::size_t pos;
        year = std::stoi(year_str, &pos);
        if (pos < year_str.size()) {
            std::cerr << "Trailing characters after number: " << year_str << std::endl;
            return 1;
        }
        if (year < 1582){
            std::cerr << "Year must be greater than 1582: " << year_str << std::endl;
            return 1;
        }
    } catch (const std::invalid_argument & e) {
        std::cerr << "Invalid argument in year: " << year_str << std::endl;
        return 1;
    } catch (const std::out_of_range & e) {
        std::cerr << "Year out of range: " << year_str << std::endl;
        return 1;
    }

    std::vector<std::string> years_data;
    for (int i = year-1; i <= year+1; i++){
        years_data.push_back(generateCal(i));
    }

    int ret = exportCal(years_data, filepath);
    if (ret == 0){
        std::cout << "File saved to: " << filepath << std::endl;
    } else {
        std::cerr << "Failed to save file to: " << filepath << std::endl;
        return 1;
    }

    return 0;
}