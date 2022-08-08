#include <iostream>
#include <fstream>

std::string generateCal(int year){
    // TODO calculate calendar

    // TODO format in HTML string
    std::string html_data;
    return html_data;
}

int exportHTML(std::string html_data, std::string filepath){
    // export data to html file
    std::ofstream myfile;
    myfile.open(filepath);
    myfile << html_data;
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
        }
    } catch (const std::invalid_argument & e) {
        std::cout << "Invalid number for year: " << year_str << std::endl;
        return 1;
    } catch (const std::out_of_range & e) {
        std::cout << "Year out of range: " << year_str << std::endl;
        return 1;
    }

    std::string html_data = generateCal(year);
    int ret = exportHTML(html_data, filepath);
    if (ret == 0){
        std::cout << "File saved to: " << filepath << std::endl;
    } else {
        std::cerr << "Failed to save file to: " << filepath << std::endl;
        return 1;
    }

    return 0;
}