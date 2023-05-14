#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

void compress(ifstream& input, ofstream& output) {
    unordered_map<string, int> dictionary;
    for (int i = 0; i < 256; i++) {
        dictionary[string(1, i)] = i;
    }
    int next_code = 256;
    string current;
    char c;
    while (input.get(c)) {
        string next = current + c;
        if (dictionary.count(next)) {
            current = next;
        } else {
            output.write(reinterpret_cast<const char*>(&dictionary[current]), sizeof(int));
            dictionary[next] = next_code++;
            current = string(1, c);
        }
    }
    if (!current.empty()) {
        output.write(reinterpret_cast<const char*>(&dictionary[current]), sizeof(int));
    }
}

void decompress(ifstream& input, ofstream& output) {
    vector<string> dictionary(256);
    for (int i = 0; i < 256; i++) {
        dictionary[i] = string(1, i);
    }
    int next_code = 256;
    int current_code, previous_code;
    input.read(reinterpret_cast<char*>(&previous_code), sizeof(int));
    output << dictionary[previous_code];
    while (input.read(reinterpret_cast<char*>(&current_code), sizeof(int))) {
        string current;
        if (dictionary.size() > current_code) {
            current = dictionary[current_code];
        } else if (current_code == dictionary.size()) {
            current = dictionary[previous_code] + dictionary[previous_code][0];
        } else {
            throw runtime_error("Invalid compressed data");
        }
        output << current;
        dictionary.push_back(dictionary[previous_code] + current[0]);
        previous_code = current_code;
    }
}

int main() {
    ifstream input("C:\\Users\\nasty\\CLionProjects\\Kurs\\input.txt", ios::binary);
    ofstream output("C:\\Users\\nasty\\CLionProjects\\Kurs\\output.txt", ios::binary);
    compress(input, output);
    input.close();
    output.close();
    input.open("put.txt", ios::binary);
    output.open("decompressed.txt", ios::binary);
    decompress(input, output);
    input.close();
    output.close();
    return 0;
}