#include <iostream>
#include <fstream>
#include <unordered_map>

using namespace std;

void decompressLZW(string inputFile, string outputFile) {
    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);

    unordered_map<int, string> dictionary;
    for (int i = 0; i < 256; i++) {
        dictionary[i] = string(1, i);
    }

    int current_index, next_index, dictionary_size = 256;
    in.read((char*)&current_index, sizeof(current_index));
    out << dictionary[current_index];

    while (in.read((char*)&next_index, sizeof(next_index))) {
        string entry;
        if (dictionary.find(next_index) != dictionary.end()) {
            entry = dictionary[next_index];
        } else if (next_index == dictionary_size) {
            entry = dictionary[current_index] + dictionary[current_index][0];
        } else {
            throw "Invalid compressed file format";
        }

        out << entry;
        dictionary[dictionary_size++] = dictionary[current_index] + entry[0];
        current_index = next_index;
    }

    in.close();
    out.close();
}

int main() {
    decompressLZW("compressed.txt", "decompressed.txt");
    return 0;
}