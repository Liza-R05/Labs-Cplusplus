#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// класс исключения
class MyError {
public:
    string userInput;  // то, что ввел пользователь

    // запоминаем ввод
    MyError(string input) {
        userInput = input;
    }
};

// возвращает true, если строка состоит только из цифр
bool isNumber(string s) {
    // строка пустая - не число
    if (s == "") {
        return false;
    }

    // проверка каждый символ
    for (int i = 0; i < s.length(); i++) {
        // символ не цифра - это не число
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }

    // дошли до конца - все символы цифры
    return true;
}

// функция ввода
// читает число, если не получается - вызывает ошибку
int readNumber() {
    string input;
    getline(cin, input);  // читаем то, что ввел пользователь

    // ввели не число
    if (isNumber(input) == false) {
        MyError err(input);
        throw err;  // случилась ошибка
    }

    // число - превращаем строку в число и возвращаем
    return stoi(input);
}

int main() {
    setlocale(LC_ALL, "Russian");

    int number;
    bool done = false;

    // повторять, пока не введут число
    while (done == false) {

        try {
            cout << "Введите число: ";
            number = readNumber();

            // дошли до этой строки - значит ошибки не было
            done = true;
            cout << endl << "Вы ввели число: " << number << endl;

        }
        // ловим ошибку
        catch (MyError& e) {
            // попадаем сюда, если случился throw --- readNumber() 
            cout << "Вы ввели не число" << endl;
            cout << "Вы ввели: \"" << e.userInput << "\"" << endl;
            cout << "Попробуйте еще раз" << endl << endl;
        }

    }
    return 0;
}