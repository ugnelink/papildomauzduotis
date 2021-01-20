#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <vector>

std::string getFileName() {
    bool badFile;
    std::string fileName;
    std::cout << "iveskite failo pavadinima (formatu: pavadinimas.txt):\n";
    std::cin >> fileName;
    do {
        try {
            std::ifstream in (fileName);
            if (!in.good())
                throw 404;
            else if (fileName.substr(fileName.length()-4,4) != ".txt") {
                throw 321;
            } else {
                in.close();
                badFile = false;
            }
        } catch (int exception) {
            badFile = true;

            std::cout << "File " << fileName;
            if (exception == 404)
                std::cout << " toks  failas neegzistuoja ";
            else std::cout << " formatas nepalaikomas ";
            std::cout << "iveskite kito failo pavadinima:\n";

            std::cin.clear();
            std::cin.ignore(256,'\n');
            std::cin >> fileName;
        }
    } while (badFile);
    return fileName;
}

void read(std::map<std::string, std::vector<int>>& words, std::vector<std::string>& urls) {
    std::string row, w;
    std::istringstream rowStream;
    int numOfLine = 0;

    std::ifstream in (getFileName());
    while (std::getline(in, row)) {
        rowStream.clear();
        rowStream.str(row);
        numOfLine ++;

        while (rowStream >> w) {
            if (w.find("www.") != std::string::npos || w.find("http") != std::string::npos) {
                urls.push_back(w);
            } else {
                // paliekame tik raides
                w.erase(std::remove_if(w.begin(), w.end(), [](char c) { return !isalpha(c); } ), w.end());

                if (w != "") {   // jeigu yra raidziu
                    // visas raides paversti mazosiomis raidemis
                    std::transform(w.begin(), w.end(), w.begin(), [](unsigned char c){ return std::tolower(c); });
                    // eilutes pabaigoje pridedame eilutes numeri
                    words[w].push_back(numOfLine);
                }
            }
        }
    }
    in.close();
}

void print(std::map<std::string, std::vector<int>>& words, std::vector<std::string>& urls) {
    std::ofstream out ("rezultatai.txt");

    out << std::left << std::setw(15) << "zodis" << std::setw(15)
        << "kartai" << std::setw(15) << "eilutes" << "\n";
    for (int i = 0; i < 50; i ++)
        out << "-";

    for (auto w : words)
        if (w.second.size() > 1) {                   // jei zodis pasikartoja daugiau, negu karta
            out << "\n" << std::setw(15) << w.first  //isvedame zodi
                << std::setw(15) << w.second.size(); // atspausdiname dazni
            for (auto line : w.second)       // atspausdiname eiluciu numerius, kuriuose yra tas zodis
                out << line << " ";
        }


    out << "\n\nURL\n";
    for (int i = 0; i < 50; i ++)
        out << "-";
    out << "\n";
    for (std::string u : urls)
        out << u << "\n";

    out.close();
}

int main() {
    std::vector<std::string> urls;
    std::map<std::string, std::vector<int>> words;

    read(words, urls);
    print(words, urls);

    return 0;
}
