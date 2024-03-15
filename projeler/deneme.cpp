#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <clocale>
#include <windows.h>
#include <stdexcept>
#include <sstream>
#include "eval.h" //Matematiksel işlemler için
using namespace std;

void printVect(vector<string> inp, char pchar = '\0') {
    for (const string& w : inp) {
        if (pchar != '\0') { cout << w << pchar; }
        else { cout << w; }
    }cout << "\n";
}
float toInt(const string& metin) {
    try {
        float sonuc = std::stof(metin);
        return sonuc;
    }
    catch (const exception& err) {
        throw (err.what());
    }
}float toInt(float num) { return num; }

bool checkMath(string text) {
    char operators[] = { '+','-','*','/',')','(' };
    for (const char& character : text) {
        for (const char& mathOperator : operators) {
            if (character == mathOperator) { return true; }
        }
    } return false;
}
tuple<int, int, int> countToken(vector<string> code, long int start = 0, long int end = 0, string token = "") {
    int count = 0;
    long int konum1 = 0, konum2 = 0;
    if (start == 0 && end == 0) {
        for (int i = 0; i < code.size(); i++) {
            if (code[i] == token) {
                if (konum1 == 0) {
                    konum1 = i;
                }
                else if (konum2 == 0 && konum1 != 0) {
                    konum2 = i;
                }
                count++;
            }
        }
        return make_tuple(count, konum1, konum2);
    }
    else {
        for (long int startIndex = start; startIndex <= end; startIndex++) {
            if (code[startIndex] == token) {
                if (konum1 == 0) {
                    konum1 = startIndex;
                }
                else if (konum2 == 0 && konum1 != 0) {
                    konum2 = startIndex;
                }
                count++;
            }
        }
    }
    return make_tuple(count, konum1, konum2 - 1/*konum2'den 1 çıkardım yoksa son tırnağıda veriyordu*/);
}

long int findToken(vector<string> text, long int startIndex, string ftext) {
    for (startIndex = 0; startIndex <= text.size(); startIndex++) {
        if (text[startIndex] == ftext) {
            return startIndex;
        }
    }
    return -1; // bulunamazsa döndürülecek
}

long int charTo(vector<string> code, long int startIndex, string character) {
    for (;;) {
        startIndex++;
        if (code[startIndex] != character) {
            return startIndex;
        }
    }
}

string readTwoIndex(vector<string> code, long int start, long int end) {
    string value = "";
    for (long int index = start; index <= end; index++) {
        value += code[index];
    }
    return value;
}

vector<string> Parser(string code, string charset) {
    vector<string> parsingCode;
    string guncelCode;
    for (char c : code) {
        bool found = false;
        for (char checkChar : charset) {
            if (checkChar == c) {
                if (!guncelCode.empty()) {
                    parsingCode.push_back(guncelCode);
                }
                parsingCode.push_back(string(1, c));
                guncelCode = "";
                found = true;
                break;
            }
        }
        if (!found) { guncelCode += c; }
    }
    return parsingCode;
}

void interpreter(vector<string> code) {
    unordered_map<string, string> strVariables;
    unordered_map<string, float> intVariables;
    for (long int counter = 0; counter < code.size(); counter++) {
        /*Print başlangıç*/
        if (code[counter] == "write") {
            long int openParenIndex = findToken(code, counter, "(");
            if (openParenIndex != -1) {
                long int closeParenIndex = findToken(code, openParenIndex, ")");
                tuple<int, int, int> counterTuple = countToken(code, openParenIndex, closeParenIndex, "\"");
                int tokNum, fKonum, sKonum;
                tie(tokNum, fKonum, sKonum) = counterTuple;
                if (tokNum == 2) {// Tırnak, yani string kontrolü
                    if (fKonum + 1 <= sKonum) {
                        for (int writeCount = fKonum + 1; writeCount <= sKonum; writeCount++) {
                            cout << code[writeCount];
                        }
                    }
                }
                /*Print değişken bastırma başlangıç*/
                else if (tokNum == 0) {// Tırnak yok ise değişken bastıracak
                    long int goToVar = charTo(code, openParenIndex, " ");
                    string val = readTwoIndex(code, goToVar, closeParenIndex - 1);
                    if (checkMath(val)) {
                        try {
                            cout << eval(val, intVariables);
                            continue;
                        }
                        catch (const exception& err) {
                            throw invalid_argument(err.what());
                        }
                    }
                    auto strVarsFindStat = strVariables.find(val);
                    auto intVarsFindStat = intVariables.find(val);
                    if (strVarsFindStat != strVariables.end()) {//String Değişken bulundu ise
                        cout << strVariables[val];
                    }
                    else if (intVarsFindStat != intVariables.end()) { // Integer değişken bulundu ise
                        cout << intVariables[val];
                    }
                    else {
                        throw invalid_argument("Error : '" + val + "' not found!");
                    }
                }/*Print değişken bastırma bitiş*/
            }
            else { throw invalid_argument("Hata: Brackets not found."); }
        }
        /*Print Bitiş*/

        /*Değişken oluşturma başlangıç*/
        if (code[counter] == "var") {
            string varName, varVal;

            long int charEndIndex = charTo(code, counter, " ");
            varName = code[charEndIndex];
            counter = charEndIndex;
            charEndIndex = findToken(code, counter, "=");
            counter = charEndIndex;
            tuple<int, int, int> countQuote = countToken(code, charEndIndex, findToken(code, charEndIndex, ";"), "\"");
            // String Değişken
            if (get<0>(countQuote) == 2) { // Tırnak işareti kontrolü
                charEndIndex = charTo(code, counter, " ") + 1;
                long int endQuoteCheck = findToken(code, charEndIndex, "\"") - 1;
                varVal = readTwoIndex(code, charEndIndex, endQuoteCheck);

                // Değişken varsa güncelliyeceğiz, yoksa yeni bir değişken olarak ekliyoz
                if (strVariables.find(varName) != strVariables.end()) {
                    strVariables[varName] = varVal; // Değişken adı ve değeri güncellendi.
                }
                else {
                    strVariables[varName] = varVal; // Değişken adı ve değeri atandı.
                }
                counter = endQuoteCheck;
            }
            //Integer değişken
            else if (get<0>(countQuote) == 0) {
                long int endPuncCheck = findToken(code, charEndIndex, ";") - 1;
                varVal = readTwoIndex(code, charEndIndex, endPuncCheck);
                // Değişken varsa güncelliyeceğiz, yoksa yeni bir değişken olarak ekliyoz
                if (intVariables.find(varName) != intVariables.end()) {
                    intVariables[varName] = eval(varVal, intVariables); // Değişken adı ve değeri güncellendi.
                }// Bu if else sonra kaldırılacak. Aynı işleve denk geliyor çünkü.
                else {
                    intVariables[varName] = eval(varVal, intVariables); // Değişken adı ve değeri atandı.
                }
                counter = endPuncCheck++;
                cout << code[counter] << endl;
                for (auto var : intVariables) cout << "Var Name: " << var.first << "\nVar Value: " << var.second << endl << "-----------------\n";
            }
        }
        /* Değişken oluşturma bitiş */
    }
}

int main() {
    setlocale(LC_ALL, "turkish");
    string code = "var x=7+1; var y=4+2";
    string charset = "=)(\"; ";
    vector<string> output = Parser(code, charset);
    try {
        interpreter(output);
    }
    catch (const exception& error) {
        cerr << error.what() << endl;
    }

    return 0;
}