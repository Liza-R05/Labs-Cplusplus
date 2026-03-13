#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>
#include <locale>

using namespace std;

const int MAX = 20;

// Абстрактный базовый класс
class Forma {
protected:
    string nazvanie;

public:
    Forma(const string& n = "Форма") : nazvanie(n) {}
    virtual ~Forma() {}

    string getNazvanie() const {
        return nazvanie;
    }

    virtual double perimeter() const = 0;
    virtual double ploshad() const = 0;
    virtual void print() const = 0;
};

// Точка
class Tochka : public Forma {
private:
    double x;
    double y;

public:
    Tochka(double x = 0, double y = 0)
        : Forma("Точка"), x(x), y(y) {
    }

    double getX() const { return x; }
    double getY() const { return y; }

    void setX(double v) { x = v; }
    void setY(double v) { y = v; }

    double distanceTo(const Tochka& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }

    double perimeter() const override {
        return 0;
    }

    double ploshad() const override {
        return 0;
    }

    void print() const override {
        cout << "Точка (" << x << ", " << y << ")" << endl;
    }
};

// Многоугольник
class Mnogougolnik : public Forma {
protected:
    Tochka vershiny[MAX];
    int count;

    void proverka() const {
        if (count < 3) {
            throw runtime_error("Мало вершин");
        }
    }

public:
    Mnogougolnik(const string& n = "Многоугольник")
        : Forma(n), count(0) {
    }

    void addVertex(const Tochka& t) {
        if (count >= MAX) {
            throw runtime_error("Слишком много вершин");
        }
        vershiny[count++] = t;
    }

    int getCount() const {
        return count;
    }

    double perimeter() const override {
        proverka();

        double res = 0;

        for (int i = 0; i < count; i++) {
            int next = (i + 1) % count;
            res += vershiny[i].distanceTo(vershiny[next]);
        }

        return res;
    }

    double ploshad() const override {
        proverka();

        double res = 0;

        for (int i = 0; i < count; i++) {
            int next = (i + 1) % count;

            res += vershiny[i].getX() * vershiny[next].getY();
            res -= vershiny[next].getX() * vershiny[i].getY();
        }

        return fabs(res) / 2.0;
    }

    void print() const override {
        cout << nazvanie << endl;

        for (int i = 0; i < count; i++) {
            cout << "Вершина " << i + 1 << ": ("
                << vershiny[i].getX() << ", "
                << vershiny[i].getY() << ")" << endl;
        }
    }
};

// Многогранник (призма)
class Mnogogrannik : public Mnogougolnik {
private:
    double vysota;

public:
    Mnogogrannik(const string& n = "Многогранник")
        : Mnogougolnik(n), vysota(1) {
    }

    void setVysota(double h) {
        if (h <= 0)
            throw runtime_error("Высота должна быть > 0");
        vysota = h;
    }

    double getVysota() const {
        return vysota;
    }

    double perimeter() const override {
        proverka();
        return 2 * Mnogougolnik::perimeter() + count * vysota;
    }

    double ploshad() const override {
        proverka();
        return 2 * Mnogougolnik::ploshad() +
            Mnogougolnik::perimeter() * vysota;
    }

    double obem() const {
        proverka();
        return Mnogougolnik::ploshad() * vysota;
    }

    void print() const override {
        cout << nazvanie << endl;


        for (int i = 0; i < count; i++) {
            cout << "Основание " << i + 1 << ": ("
                << vershiny[i].getX() << ", "
                << vershiny[i].getY() << ")" << endl;
        }

        cout << "Высота: " << vysota << endl;
    }
};

// Демонстрация динамического связывания
void show(Forma* f) {
    cout << "\n-------------------\n";
    cout << f->getNazvanie() << endl;

    f->print();

    cout << "Периметр: " << f->perimeter() << endl;
    cout << "Площадь: " << f->ploshad() << endl;

    Mnogogrannik* m = dynamic_cast<Mnogogrannik*>(f);

    if (m != nullptr) {
        cout << "Объем: " << m->obem() << endl;
    }
}

int main() {

    setlocale(LC_ALL, "Russian");

    try {

        Tochka t1(0, 0);
        Tochka t2(4, 0);
        Tochka t3(4, 3);
        Tochka t4(0, 3);

        cout << "Расстояние между точками: "
            << t1.distanceTo(t2) << endl;

        // Треугольник
        Mnogougolnik tri("Треугольник");

        tri.addVertex(t1);
        tri.addVertex(t2);
        tri.addVertex(t3);

        // Прямоугольник
        Mnogougolnik rect("Прямоугольник");

        rect.addVertex(t1);
        rect.addVertex(t2);
        rect.addVertex(t3);
        rect.addVertex(t4);

        // Призма
        Mnogogrannik prizma("Призма");

        prizma.addVertex(t1);
        prizma.addVertex(t2);
        prizma.addVertex(t3);
        prizma.addVertex(t4);

        prizma.setVysota(5);

        // динамическое связывание
        Forma* figury[4];

        figury[0] = &t1;
        figury[1] = &tri;
        figury[2] = &rect;
        figury[3] = &prizma;

        cout << "\nДИНАМИЧЕСКОЕ СВЯЗЫВАНИЕ\n";

        for (int i = 0; i < 4; i++) {
            show(figury[i]);
        }

    }
    catch (exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }

    return 0;
}
