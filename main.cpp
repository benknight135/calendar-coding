#include <iostream> /* cout, cerr, endl */
#include <fstream> /* ofstream */
#include <math.h> /* floor */
#include <sstream> /* stringstream */

bool isLeapYear(int year){
    // year number must be divisible by four
    // except for end-of-century years, which must be divisible by 400
    if (year % 4 == 0) return true;
    if (year % 100 == 0) return false;
    if (year % 400 == 0) return true;
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

int weekNumber(int day, int month, int year){
    // ISO-8601
    // 2.2.10 calendar week number
    // Ordinal number which identifies a calendar week within its calendar year
    // according to the rule that the first calendar week of a year is that one
    // which includes the first Thursday of that year and that the last calendar
    // week of a calendar year is the week immediately preceding the first calendar
    // week of the next calendar year

    bool skip_first_week = false;
    // account for first week in Jan being after Thursday
    int first_day_in_week = dayOfWeek(year, 1, 1);
    int days_in_first_week = 7 - (first_day_in_week - 1);
    std::cout << first_day_in_week << std::endl;
    std::cout << days_in_first_week << std::endl;
    skip_first_week = (days_in_first_week < 4);
    if (skip_first_week && day <= days_in_first_week && month == 1){
        // week number is last week of previous year
        return weekNumber(31, 12, year-1);
    }
    int week_number = 1;
    if (skip_first_week){
        // start counting from 0 so that 1 is the first week
        // when the first week was numbered from the previous year
        week_number = 0;
    }
    for (int m = 1; m <= month; m++){
        int month_days = daysInMonth(year, m);
        if (m == month)
            month_days = day;
        for (int day_in_month = 1; day_in_month <= month_days; day_in_month++){
            int day_in_week = dayOfWeek(year, m, day_in_month);
            if (day_in_week == 7){
                // increase week number at end of week
                week_number++;
            }
        }
    }
    return week_number;
}

std::string genHeaderHTML(){
    // generate HTML header
    std::stringstream html;
    html << "<!DOCTYPE html>" << std::endl;
    html << "<html>" << std::endl;
    html << "  <body>" << std::endl;
    return html.str();
}

std::string genMonthHTML(int year, int month){
    // generate HTML for chosen month and year
    std::string month_names[12] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    std::string month_header[8] = {"Wk", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    std::string month_name = month_names[month-1];
    // create title for month table
    std::stringstream html;
    html << "    <h2 align=\"center\" style=\"color: black;\">";
    html << month_name << " " << std::to_string(year) << "</h2>" << std::endl;
    html << "    <br />" << std::endl;
    // start to create month table
    html << "    <table bgcolor=\"lightgrey\" align=\"center\" cellspacing=\"21\" cellpadding=\"21\">" << std::endl;
    html << "    <caption align=\"top\"></caption>" << std::endl;
    // create table header text (days of the week and week number)
    html << "    <thead>" << std::endl;
    html << "      <tr>" << std::endl;
    for (int h = 0; h <= 7;  h++){
        html << "        <th>" << month_header[h] + "</th>" << std::endl;
    }
    html << "      </tr>" << std::endl;
    html << "    </thead>" << std::endl;
    html << "    <tbody>" << std::endl;
    html << "      <tr>" << std::endl;
    // create empty space where the first day is not a Monday
    int first_day_in_week = dayOfWeek(year, month, 1);
    if (first_day_in_week != 1){
        int week_number = weekNumber(1, month, year);
        html << "        <td>" << week_number << "</td>" << std::endl;
        for (int day_in_week = 1; day_in_week <= first_day_in_week - 1; day_in_week++){
            html <<  "        <td></td>" << std::endl;
        }
    }
    // fill table with calendar date
    int month_days = daysInMonth(year, month);
    int final_day_in_week;
    for (int day_in_month = 1; day_in_month <= month_days; day_in_month++){
        int day_in_week = dayOfWeek(year, month, day_in_month);
        // Monday (1st day) will be start of new row
        if (day_in_week == 1){
            html << "      <tr>" << std::endl;
            int week_number = weekNumber(day_in_month, month, year);
            html << "        <td>" << std::to_string(week_number) << "</td>" << std::endl;
        }
        html << "        <td>" << std::to_string(day_in_month) << "</td>" << std::endl;
        // Sunday (7th day) will be end of the row
        if (day_in_week == 7){
            html << "      </tr>" << std::endl;
        }
        final_day_in_week = day_in_week;
    }
    // add spaces if last day of the week is not a Sunday
    if (final_day_in_week < 7){
        for (int day_in_week = final_day_in_week; day_in_week < 7; day_in_week++){
            html << "        <td></td>" << std::endl;
        }
        html << "      </tr>" << std::endl;
    }
    // end month table
    html << "    </tbody>" << std::endl;
    html << "    </table>" << std::endl;
    return html.str();
}

std::string genFooterHTML(){
    // generate HTML footer
    std::stringstream html;
    html << "  </body>" << std::endl;
    html << "</html>" << std::endl;
    return html.str();
}

std::string genYearHTML(int year){
    // generate HTML for months in a year
    std::string html;
    for (int month = 1; month <= 12; month++){
        html += genMonthHTML(year, month);
    }
    return html;
}

std::string genCalendarHTML(int middle_year){
    // generate calendar html for year provided and year either side
    // e.g. 2000 would generate 1999, 2000, and 2001.
    std::string html;
    html = genHeaderHTML();
    for (int year = middle_year-1; year <= middle_year+1; year++){
        html += genYearHTML(year);
    }
    html += genFooterHTML();
    return html;
}

int exportHTML(std::string html_data, std::string filepath){
    // export html data to file
    std::ofstream myfile;
    myfile.open(filepath);
    myfile << html_data;
    myfile.close();
    // check write was successfuly
    if(!myfile){
        return 1;
    }
    return 0;
}

int main(int argc,char* argv[]){
    // Generates HTML calendar for year provided and years either side.
    // e.g. 2000 would generate 1999, 2000, and 2001.
    // Arguments:
    //      year (int)
    //      filepath (string)

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
        // check for tailing characters in number
        if (pos < year_str.size()) {
            std::cerr << "Trailing characters after number: " << year_str << std::endl;
            return 1;
        }
        // check year is after 1582
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

    // generate html
    std::string calendar_html = genCalendarHTML(year);
    // export html to file
    int ret = exportHTML(calendar_html, filepath);
    if (ret == 0){
        std::cout << "File saved to: " << filepath << std::endl;
    } else {
        std::cerr << "Failed to save file to: " << filepath << std::endl;
        return 1;
    }

    std::cout << weekNumber(1, 1, 2023) << std::endl;

    return 0;
}