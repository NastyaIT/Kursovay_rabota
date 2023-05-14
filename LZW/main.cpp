#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

void compress(std::string input_file, std::string output_file) {
    std::unordered_map<std::string, int> dictionary;
    int dict_size = 256;
    for (int i = 0; i < 256; i++) {
        dictionary[std::string(1, i)] = i;
    }

    std::ifstream in(input_file, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Error opening input file.\n";
        return;
    }

    std::ofstream out(output_file, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Error opening output file.\n";
        return;
    }

    std::string current_str;
    char c;
    while (in.get(c)) {
        std::string next_str = current_str + c;
        if (dictionary.count(next_str)) {
            current_str = next_str;
        } else {
            out.write((char*)&dictionary[current_str], sizeof(int));
            dictionary[next_str] = dict_size++;
            current_str = std::string(1, c);
        }
    }
    if (!current_str.empty()) {
        out.write((char*)&dictionary[current_str], sizeof(int));
    }
    int end_of_file = 256;
    out.write((char*)&end_of_file, sizeof(int));

    in.close();
    out.close();
}

int main() {
    compress("C:\\Users\\nasty\\CLionProjects\\LZW\\vvod.txt", "C:\\Users\\nasty\\CLionProjects\\LZW\\vivod.lzw");
    return 0;
}