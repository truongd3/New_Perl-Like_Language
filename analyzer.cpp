#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <iomanip>

#include "parserInt.h"

using namespace std;
namespace fs = std::__fs::filesystem;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <directory_path>" << endl;
        return 1;
    }

    string directoryPath = argv[1];

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        cerr << "Invalid directory." << endl;
        return 1;
    }

    double CPU_runtime = 0.0;
    int file_count = 0;
    int lineNumber;

    // Recursively traverse directory
    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            istream *in = NULL;
            ifstream file(entry.path());
            if (file.is_open() == false) {
                cerr << "CANNOT OPEN " << entry.path() << endl;
                continue;
            }

            lineNumber = 1;
            cout << "----------------\nProcessing file: " << entry.path() << endl;

            in = &file;

            clock_t start_time = clock(); // start CPU clock
            bool status = Prog(*in, lineNumber);
            clock_t end_time = clock(); // end CPU clock

            double elapsed = double(end_time - start_time) / CLOCKS_PER_SEC;
            CPU_runtime += elapsed;
            file_count++;

            if (!status) cout << "\nUnsuccessful Interpretation for " << entry.path() << endl << "Number of Errors: " << ErrCount() << endl;
            else cout << "\nSuccessful Execution for " << entry.path() << endl;
        }
    }

    // Update analyzation
    ofstream report("output/analyzer_report.txt", ios::app); // append mode

    if (!report.is_open()) {
        cerr << "Failed to open output/analyzer_report.txt." << endl;
        return 1;
    }

    // Get current date and time
    time_t now = time(0);
    tm *ltm = localtime(&now);

    report << "Date & Time: " 
           << 1900 + ltm->tm_year << "/"
           << setfill('0') << setw(2) << 1 + ltm->tm_mon << "/"
           << setfill('0') << setw(2) << ltm->tm_mday << ", "
           << setfill('0') << setw(2) << ltm->tm_hour << ":"
           << setfill('0') << setw(2) << ltm->tm_min << ":"
           << setfill('0') << setw(2) << ltm->tm_sec << endl;

    report << "Files Processed: " << file_count << endl;
    report << "Total CPU Runtime: " << CPU_runtime << " seconds" << endl;

    if (file_count > 0) report << "Average CPU Runtime: " << (CPU_runtime / file_count) << " seconds" << endl;
    else report << "Average CPU Runtime: N/A" << endl;

    report << "---------------------------------------------------\n" << endl;
    report.close();

    return 0;
}
