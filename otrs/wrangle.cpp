#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int main() {

    string input_file_path = "results.csv";
    string output_file_path = "results_avg.csv";
    ifstream input_file(input_file_path);
    
    string line;
    map<string, long> groupingMap;

    if (input_file.is_open()) {
        // Header
        getline(input_file, line);
        while (getline(input_file, line)) {
            // cout << line << endl;
            int idx;
            if ((idx = line.find_last_of(",")) != string::npos) {
                groupingMap[line.substr(0, idx-1)] += stol(line.substr(idx+1));
            }
        }
    } else {
        cout << "Unable to open file " << input_file_path << endl;
    }

    input_file.close();

    ofstream output_file(output_file_path);
    if(output_file.is_open()) {
        output_file << "Container,Data type,Operation,Elements,Time" << endl;
        for (auto it = groupingMap.begin(); it != groupingMap.end(); ++it) {
            it->second /= 10;
            cout << it->first  << " -> " << it->second  / 1'000'000.0 << " ms" << endl;
            output_file << it->first << "," << it->second << endl;
        }
    }
     else {
        cout << "Unable to open file " << output_file_path << endl;
    }

    output_file.close();

}