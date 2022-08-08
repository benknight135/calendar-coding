#include <iostream>
#include <fstream>

std::string generateCal(int year){
    // TODO calculate calendar

    // TODO format in HTML string
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
    //      Year (int)
    //      Filepath (string)
    // Outputs:
    //      HTML file

    // Check valid number of arguments provided
    if(argc!=3){
        std::cerr << "Invalid number of arguments." << std::endl;
        std::cerr << "Required format: 'gen_cal year filepath'." << std::end;
        std::cerr << "Example 'gen_cal.exe 2000 Calendar2000.html'" << std::endl;
        return -1;
    }

    // TODO Get filpath from arguments
    std::string filepath = "some interesting data (garbage)";
    // TODO get year from arguements
    int year = 2000;

    std::string html_data = generateCal(year);
    int ret = exportHTML(html_data, filepath);
    if (ret == 0){
        std::cout << "File saved to: " << filepath << std::endl;
    } else {
        std::cerr << "Failed to save file to: " << filepath << std::endl;
    }
}