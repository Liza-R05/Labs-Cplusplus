#include <iostream>
#include <clocale>

using namespace std;

const int N = 4;

// Функция заполнения матрицы по схеме варианта 4
double fillMatrix(double matrix[N][N], int n) {
    int num = 1;

    // Инициализация нулями
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }

    // Заполняем по диагоналям, параллельным побочной
    // Всего 2*n-1 диагоналей
    for (int diag = 0; diag < 2 * n - 1; diag++) {
        // Определяем начальную точку диагонали
        int start_row, start_col;

        if (diag < n) {
            // Диагонали, начинающиеся в первой строке
            start_row = 0;
            start_col = n - 1 - diag;
        }
        else {
            // Диагонали, начинающиеся в последнем столбце
            start_row = diag - n + 1;
            start_col = 0;
        }

        // Считаем длину диагонали
        int length;
        if (diag < n) {
            length = diag + 1;
        }
        else {
            length = 2 * n - 1 - diag;
        }

        // Временный массив для хранения чисел диагонали
        int temp[16];
        for (int i = 0; i < length; i++) {
            temp[i] = num++;
        }

        // Заполняем диагональ
        if (diag % 2 == 0) {
            // Четная диагональ - сверху вниз
            for (int i = 0; i < length; i++) {
                matrix[start_row + i][start_col + i] = temp[i];
            }
        }
        else {
            // Нечетная диагональ - снизу вверх
            for (int i = 0; i < length; i++) {
                matrix[start_row + length - 1 - i][start_col + length - 1 - i] = temp[i];
            }
        }
    }

    return 0;
}

double printMatrix(double matrix[N][N], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%8.0f ", matrix[i][j]);
        }
        cout << "\n";
    }
    return 0;
}

// Функция нахождения обратной матрицы
double inverseMatrix(double a[N][N], double inv[N][N], int n, int* success) {
    double aug[N][2 * N];

    *success = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            aug[i][j] = a[i][j];
            aug[i][j + n] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (int i = 0; i < n; i++) {
        double maxVal = aug[i][i];
        if (maxVal < 0) maxVal = -maxVal;
        int maxRow = i;

        for (int k = i + 1; k < n; k++) {
            double val = aug[k][i];
            if (val < 0) val = -val;
            if (val > maxVal) {
                maxVal = val;
                maxRow = k;
            }
        }

        if (maxVal < 0.0001) {
            *success = 0;
            return 0;
        }

        if (maxRow != i) {
            for (int j = 0; j < 2 * n; j++) {
                double temp = aug[i][j];
                aug[i][j] = aug[maxRow][j];
                aug[maxRow][j] = temp;
            }
        }

        double div = aug[i][i];
        for (int j = 0; j < 2 * n; j++) {
            aug[i][j] = aug[i][j] / div;
        }

        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = aug[k][i];
                for (int j = 0; j < 2 * n; j++) {
                    aug[k][j] = aug[k][j] - factor * aug[i][j];
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv[i][j] = aug[i][j + n];
        }
    }

    return 0;
}

// Функция умножения матриц
double multiplyMatrices(double a[N][N], double b[N][N], double result[N][N], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result[i][j] = result[i][j] + a[i][k] * b[k][j];
            }
        }
    }
    return 0;
}

int main() {
    setlocale(LC_ALL, "RUS");

    double matrix[N][N];
    double invMatrix[N][N];
    double product[N][N];
    int success;

    fillMatrix(matrix, N);

    cout << "\nИсходная матрица:\n";
    printMatrix(matrix, N);

    inverseMatrix(matrix, invMatrix, N, &success);

    if (success == 0) {
        cout << "\nОшибка: матрица вырождена\n";
        return 1;
    }

    cout << "\nОбратная матрица:\n";
    printMatrix(invMatrix, N);

    multiplyMatrices(invMatrix, matrix, product, N);

    cout << "\nПроверка (обратная * исходная = единичная):\n";
    printMatrix(product, N);

    return 0;
}
