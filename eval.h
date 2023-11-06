#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <sstream>
using namespace std;
// Ýþlem öncelikleri
int getPrecedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/') {
        return 2;
    }
    return 0; // Diðer karakterler için öncelik yok
}

// Ýþlem yapma fonksiyonu
float performOperation(float a, float b, char op) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0) {
            throw invalid_argument("Sýfýra bölme hatasý.");
        }
        return a / b;
    default:
        throw invalid_argument("Geçersiz iþlem: " + string(1, op));
    }
}

// Ýfadeyi hesaplayan fonksiyon
float eval(const string& expression, unordered_map<string, float>& variableMap) {
    istringstream input(expression);
    vector<float> values;
    vector<char> operators;

    char ch;
    while (input >> ch) {
        if (isalpha(ch)) {
            string variableName;
            variableName += ch;
            while (input.get(ch) && (isalnum(ch) || ch == '_')) {
                variableName += ch;
            }
            if (variableMap.find(variableName) != variableMap.end()) {
                values.push_back(variableMap[variableName]);
            }
            else {
                throw invalid_argument("Deðiþken bulunamadý: " + variableName);
            }
        }
        else if (isdigit(ch) || ch == '-') {
            input.putback(ch);
            float value;
            input >> value;
            values.push_back(value);
        }
        else if (ch == '(') {
            operators.push_back(ch);
        }
        else if (ch == ')') {
            while (!operators.empty() && operators.back() != '(') {
                char op = operators.back();
                operators.pop_back();
                if (values.size() < 2) {
                    throw invalid_argument("Ýþlem hatasý.");
                }
                float b = values.back();
                values.pop_back();
                float a = values.back();
                values.pop_back();
                values.push_back(performOperation(a, b, op));
            }
            if (operators.empty() || operators.back() != '(') {
                throw invalid_argument("Parantez hatasý.");
            }
            operators.pop_back();
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            while (!operators.empty() && getPrecedence(operators.back()) >= getPrecedence(ch)) {
                if (values.size() < 2) {
                    throw invalid_argument("Ýþlem hatasý.");
                }
                char op = operators.back();
                operators.pop_back();
                float b = values.back();
                values.pop_back();
                float a = values.back();
                values.pop_back();
                values.push_back(performOperation(a, b, op));
            }
            operators.push_back(ch);
        }
    }

    while (!operators.empty()) {
        if (operators.back() == '(') {
            throw invalid_argument("Parantez hatasý.");
        }
        if (values.size() < 2) {
            throw invalid_argument("Ýþlem hatasý.");
        }
        char op = operators.back();
        operators.pop_back();
        float b = values.back();
        values.pop_back();
        float a = values.back();
        values.pop_back();
        values.push_back(performOperation(a, b, op));
    }

    if (values.size() != 1) {
        throw invalid_argument("Ýþlem hatasý.");
    }

    return values[0];
}