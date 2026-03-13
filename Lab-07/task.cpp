#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

using namespace std;

// Абстрактный базовый класс
class Forma {
protected:
    string nazvanie;
public:
    Forma(const string& n = "Форма") : nazvanie(n) {}
    virtual ~Forma() {}

    virtual double perimeter() const = 0;
    virtual double ploshad() const = 0;
    virtual void print() const = 0;

    string getNazvanie() const { return nazvanie; }
};

// Класс Точка
class Tochka : public Forma {
private:
    double x, y;
public:
    Tochka(double x = 0, double y = 0) : Forma("Точка"), x(x), y(y) {}

    double getX() const { return x; }
    double getY() const { return y; }
    void setX(double val) { x = val; }
    void setY(double val) { y = val; }

    double distanceTo(const Tochka& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }

    double perimeter() const override { return 0; }
    double ploshad() const override { return 0; }

    void print() const override {
        cout << "Точка (" << x << ", " << y << ")";
    }
};

// Класс Многоугольник
class Mnogougolnik : public Forma {
protected:
    vector<Tochka> vershiny;

    void proverka() const {
        if (vershiny.size() < 3) {
            throw runtime_error("Мало вершин");
        }
    }

public:
    Mnogougolnik(const string& n = "Многоугольник") : Forma(n) {}

    Mnogougolnik(const vector<Tochka>& points, const string& n = "Многоугольник")
        : Forma(n), vershiny(points) {
        proverka();
    }

    void addVertex(const Tochka& t) {
        vershiny.push_back(t);
    }

    double perimeter() const override {
        if (vershiny.size() < 2) return 0;
        double res = 0;
        for (size_t i = 0; i < vershiny.size(); i++) {
            size_t next = (i + 1) % vershiny.size();
            res += vershiny[i].distanceTo(vershiny[next]);
        }
        return res;
    }

    double ploshad() const override {
        proverka();
        double res = 0;
        int n = vershiny.size();
        for (int i = 0; i < n; i++) {
            int next = (i + 1) % n;
            res += vershiny[i].getX() * vershiny[next].getY();
            res -= vershiny[next].getX() * vershiny[i].getY();
        }
        return abs(res) / 2.0;
    }

    void print() const override {
        cout << nazvanie << " с " << vershiny.size() << " вершинами:" << endl;
        for (const auto& v : vershiny) {
            cout << "  ";
            v.print();
            cout << endl;
        }
    }
};

// Класс Многогранник
class Mnogogrannik : public Mnogougolnik {
private:
    double vysota;

public:
    Mnogogrannik(const string& n = "Многогранник") : Mnogougolnik(n), vysota(0) {}

    Mnogogrannik(const vector<Tochka>& basePoints, double h, const string& n = "Многогранник")
        : Mnogougolnik(basePoints, n), vysota(h) {
        if (h <= 0) {
            throw runtime_error("Высота должна быть положительной");
        }
    }

    void setVysota(double h) {
        if (h <= 0) {
            throw runtime_error("Высота должна быть положительной");
        }
        vysota = h;
    }

    double getVysota() const { return vysota; }

    double perimeter() const override {
        return Mnogougolnik::perimeter();
    }

    double ploshad() const override {
        double osn = Mnogougolnik::ploshad();
        double bok = perimeter() * vysota;
        return 2 * osn + bok;
    }

    double obem() const {
        return Mnogougolnik::ploshad() * vysota;
    }

    void print() const override {
        cout << nazvanie << " (3D)" << endl;
        cout << "Основание: ";
        Mnogougolnik::print();
        cout << "Высота: " << vysota << endl;
        cout << "Объем: " << obem() << endl;
    }
};

// Демонстрация динамического связывания
void show(Forma* f) {
    cout << "\n- " << f->getNazvanie() << " -" << endl;
    f->print();
    cout << "Периметр: " << f->perimeter() << endl;
    cout << "Площадь: " << f->ploshad() << endl;
}

int main() {
    setlocale(LC_ALL, "RUS");
    try {
        Tochka t1(0, 0);
        Tochka t2(4, 0);
        Tochka t3(4, 3);
        Tochka t4(0, 3);

        cout << "\n- Точки -" << endl;
        t1.print(); cout << endl;
        t2.print(); cout << endl;
        cout << "Расстояние: " << t1.distanceTo(t2) << endl;

        vector<Tochka> tri = { t1, t2, t3 };
        vector<Tochka> quad = { t1, t2, t3, t4 };

        Mnogougolnik treugolnik(tri, "Треугольник");
        Mnogougolnik pryamougolnik(quad, "Прямоугольник");
        Mnogogrannik prizma1(quad, 5, "Призма");
        Mnogogrannik prizma2(tri, 7, "Треугольная призма");

        cout << "\n- ДИНАМИЧЕСКОЕ СВЯЗЫВАНИЕ -" << endl;

        vector<Forma*> figury;
        figury.push_back(&t1);
        figury.push_back(&treugolnik);
        figury.push_back(&pryamougolnik);
        figury.push_back(&prizma1);
        figury.push_back(&prizma2);

        for (size_t i = 0; i < figury.size(); i++) {
            show(figury[i]);
        }

    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}