#include <iostream>
#include <algorithm>

using namespace std;

class IntArray {
private:
    int* arr;      // указатель на массив
    int size;      // размер массива

public:
    // конструктор с параметром n (размер массива)
    IntArray(int n) {
        size = n;
        arr = new int[size];  // выделяем память
        for (int i = 0; i < size; i++) {
            arr[i] = 0;  // инициализируем нулями
        }
    }

    // деструктор (освобождаем память)
    ~IntArray() {
        delete[] arr;
    }

    void input() {
        cout << "Введите " << size << " элементов массива:\n";
        for (int i = 0; i < size; i++) {
            cout << "Элемент [" << i << "]: ";
            cin >> arr[i];
        }
    }

    void output() {
        cout << "Элементы массива: ";
        for (int i = 0; i < size; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

    // пузырьковая сортировка
    void sort() {
        // внешний — проходы по массиву
        for (int i = 0; i < size - 1; i++) {

            // внутренний — сравнение соседних элементов
            for (int j = 0; j < size - 1 - i; j++) {

                // левый больше правого — меняем местами
                if (arr[j] > arr[j + 1]) {
                    // обмен
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }

            }

        }
        cout << "Массив отсортирован.\n";
    }

    // свойство только для чтения — размер массива
    int getSize() {
        return size;
    }

    // свойство только для записи — умножение на скаляр
    void setMultiply(int scalar) {
        for (int i = 0; i < size; i++) {
            arr[i] = arr[i] * scalar;
        }
    }

    // доступ по индексу
    int& operator[](int index) {
        if (index < 0 || index >= size) {
            cout << "Ошибка: индекс вне диапазона!\n";
            return arr[0];  // возвращаем первый элемент при ошибке
        }
        return arr[index];
    }

    // перегрузка ++
    IntArray& operator++() {
        for (int i = 0; i < size; i++) {
            arr[i]++;
        }
        return *this;
    }

    // перегрузка --
    IntArray& operator--() {
        for (int i = 0; i < size; i++) {
            arr[i]--;
        }
        return *this;
    }

    // проверка на упорядоченность по возрастанию
    bool operator!() {
        for (int i = 0; i < size - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                return true;  // не упорядочен
            }
        }
        return false;  // упорядочен
    }

    // перегрузка умножение на скаляр
    IntArray operator*(int scalar) {
        IntArray result(size);
        for (int i = 0; i < size; i++) {
            result.arr[i] = arr[i] * scalar;
        }
        return result;
    }

    // Преобразование класса в одномерный массив (int*)
    operator int* () {
        return arr;
    }

    // преобразование из одномерного массива
    void fromArray(int* source, int n) {
        if (n != size) {
            delete[] arr;
            size = n;
            arr = new int[size];
        }
        for (int i = 0; i < size; i++) {
            arr[i] = source[i];
        }
    }
};

int main() {
    setlocale(LC_ALL, "RUS");

    cout << "1. Создание массива:\n";
    IntArray arr(5);
    cout << "Размер массива: " << arr.getSize() << endl;

    cout << "\n2. Ввод элементов:\n";
    arr.input();
    cout << "\n3. Вывод элементов:\n";
    arr.output();

    cout << "\n4. Проверка упорядоченности:\n";
    if (!arr) {
        cout << "Массив НЕ упорядочен по возрастанию\n";
    }
    else {
        cout << "Массив упорядочен по возрастанию\n";
    }

    cout << "\n5. Сортировка массива:\n";
    arr.sort();
    arr.output();

    if (!arr) {
        cout << "Массив НЕ упорядочен\n";
    }
    else {
        cout << "Массив упорядочен\n";
    }

    cout << "\n6. Работа с индексатором:\n";
    cout << "Элемент с индексом 2: " << arr[2] << endl; // какой индекс
    arr[2] = 100; // на что заменим
    cout << "После изменения:\n";
    arr.output();

    cout << "\n7. Оператор ++ (увеличение всех на 1):\n";
    ++arr;
    arr.output();

    cout << "\n8. Оператор -- (уменьшение всех на 1):\n";
    --arr;
    arr.output();

    // Свойство умножения на скаляр (только запись)
    cout << "\n9. Умножение всех элементов на 3:\n";
    arr.setMultiply(3);
    arr.output();

    // Оператор бинарный *
    cout << "\n10. Оператор * (умножение на 2):\n";
    IntArray arr2 = arr * 2;
    arr2.output();

    // Преобразование в массив int*
    cout << "\n11. Преобразование в обычный массив:\n";
    int* ptr = (int*)arr;
    cout << "Элементы через указатель: ";
    for (int i = 0; i < 5; i++) {
        cout << ptr[i] << " ";
    }
    cout << endl;

    // Преобразование из массива
    cout << "\n12. Загрузка из обычного массива:\n";
    int source[] = { 10, 20, 30, 40, 50 };
    arr.fromArray(source, 5);
    arr.output();


    return 0;
}