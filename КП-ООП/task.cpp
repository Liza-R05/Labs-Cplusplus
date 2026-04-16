#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

// Товар
class Product {
private:
    string article;   // артикул товара
    string name;      // наименование товара
    int quantity;     // количество на складе
    double price;     // цена за единицу

public:
    // Конструкторы
    Product() : quantity(0), price(0.0) {}

    Product(const string& article, const string& name, int quantity, double price)
        : article(article), name(name), quantity(quantity), price(price) {
    }

    // Геттеры
    string getArticle() const { return article; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }

    // Сеттеры
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setPrice(double newPrice) { price = newPrice; }

    // Общая стоимость товара
    double getTotalValue() const {
        return quantity * price;
    }

    // Вывод информации о товаре
    void print() const {
        cout << "Артикул: " << article << endl;
        cout << "Наименование: " << name << endl;
        cout << "Количество: " << quantity << endl;
        cout << "Цена за ед.: " << price << " руб." << endl;
        cout << "Общая стоимость: " << getTotalValue() << " руб." << endl;
    }

    // Преобразование в строку для сохранения в файл
    string toString() const {
        return article + ";" + name + ";" + to_string(quantity) + ";" + to_string(price);
    }

    // Создание объекта Product из строки файла
    static Product fromString(const string& line) {
        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);
        size_t pos3 = line.find(';', pos2 + 1);

        string art = line.substr(0, pos1);
        string nam = line.substr(pos1 + 1, pos2 - pos1 - 1);
        int quant = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        double pr = stod(line.substr(pos3 + 1));

        return Product(art, nam, quant, pr);
    }
};

// Склад
class Warehouse {
private:
    vector<Product> products;

    // Поиск индекса товара по артикулу
    int findIndexByArticle(const string& article) const {
        for (size_t i = 0; i < products.size(); ++i) {
            if (products[i].getArticle() == article) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

public:
    // Добавление товара
    bool addProduct(const Product& product) {
        if (findIndexByArticle(product.getArticle()) != -1) {
            cout << "Ошибка: товар с артикулом " << product.getArticle()
                << " уже существует!" << endl;
            return false;
        }
        products.push_back(product);
        cout << "Товар \"" << product.getName() << "\" успешно добавлен." << endl;
        return true;
    }

    // Удаление товара
    bool removeProduct(const string& article) {
        int index = findIndexByArticle(article);
        if (index == -1) {
            cout << "Ошибка: товар с артикулом " << article << " не найден!" << endl;
            return false;
        }
        cout << "Товар \"" << products[index].getName() << "\" удалён." << endl;
        products.erase(products.begin() + index);
        return true;
    }

    // Приход товара (увеличение количества)
    bool addStock(const string& article, int amount) {
        if (amount <= 0) {
            cout << "Ошибка: количество для прихода должно быть положительным!" << endl;
            return false;
        }
        int index = findIndexByArticle(article);
        if (index == -1) {
            cout << "Ошибка: товар с артикулом " << article << " не найден!" << endl;
            return false;
        }
        int newQuantity = products[index].getQuantity() + amount;
        products[index].setQuantity(newQuantity);
        cout << "Приход оформлен. Новое количество: " << newQuantity << endl;
        return true;
    }

    // Расход товара (уменьшение количества с контролем остатка)
    bool removeStock(const string& article, int amount) {
        if (amount <= 0) {
            cout << "Ошибка: количество для расхода должно быть положительным!" << endl;
            return false;
        }
        int index = findIndexByArticle(article);
        if (index == -1) {
            cout << "Ошибка: товар с артикулом " << article << " не найден!" << endl;
            return false;
        }
        int currentQuantity = products[index].getQuantity();
        if (amount > currentQuantity) {
            cout << "Ошибка: недостаточно товара на складе! Доступно: "
                << currentQuantity << endl;
            return false;
        }
        int newQuantity = currentQuantity - amount;
        products[index].setQuantity(newQuantity);
        cout << "Расход оформлен. Остаток: " << newQuantity << endl;
        return true;
    }

    // Поиск товара по артикулу
    Product* findProduct(const string& article) {
        int index = findIndexByArticle(article);
        if (index == -1) {
            return nullptr;
        }
        return &products[index];
    }

    // Поиск товаров по наименованию (частичное совпадение)
    vector<Product> findProductsByName(const string& name) const {
        vector<Product> result;
        for (const auto& product : products) {
            if (product.getName().find(name) != string::npos) {
                result.push_back(product);
            }
        }
        return result;
    }

    // Вывод всех товаров
    void listAllProducts() const {
        if (products.empty()) {
            cout << "Склад пуст." << endl;
            return;
        }
        cout << "\n--- Список всех товаров ---" << endl;
        for (const auto& product : products) {
            product.print();
            cout << "------------------------" << endl;
        }
    }

    // Формирование отчёта об остатках
    void printReport() const {
        if (products.empty()) {
            cout << "Склад пуст. Отчёт не может быть сформирован." << endl;
            return;
        }
        cout << "\n++++++++++ ОТЧЁТ ОБ ОСТАТКАХ ++++++++++" << endl;
        cout << "Всего наименований товаров: " << products.size() << endl;
        cout << "Общая стоимость всех товаров: " << getTotalWarehouseValue() << " руб." << endl;
        cout << "\nДетализация:" << endl;
        cout << "----------------------------------------" << endl;
        for (const auto& product : products) {
            cout << "Артикул: " << product.getArticle()
                << " | Наименование: " << product.getName()
                << " | Количество: " << product.getQuantity()
                << " | Общая стоимость: " << product.getTotalValue() << " руб." << endl;
        }
        cout << "----------------------------------------" << endl;
    }

    // Общая стоимость всех товаров на складе
    double getTotalWarehouseValue() const {
        double total = 0;
        for (const auto& product : products) {
            total += product.getTotalValue();
        }
        return total;
    }

    // Получение всех товаров (для сохранения)
    const vector<Product>& getAllProducts() const {
        return products;
    }

    // Очистка склада
    void clear() {
        products.clear();
    }

    // Добавление товара из данных (для загрузки)
    void addProductFromData(const Product& product) {
        products.push_back(product);
    }

    // Количество товаров
    int getProductCount() const {
        return static_cast<int>(products.size());
    }
};

// Работа с файлами
class FileManager {
private:
    string filename;

public:
    FileManager(const string& fname = "warehouse_data.txt") : filename(fname) {}

    // Сохранение данных в файл
    bool saveToFile(const Warehouse& warehouse) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Ошибка: не удалось открыть файл для записи!" << endl;
            return false;
        }

        const vector<Product>& products = warehouse.getAllProducts();
        for (const auto& product : products) {
            file << product.toString() << endl;
        }

        file.close();
        cout << "Данные сохранены в файл " << filename << endl;
        return true;
    }

    // Загрузка данных из файла
    bool loadFromFile(Warehouse& warehouse) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Файл " << filename << " не найден. Будет создан новый склад." << endl;
            return false;
        }

        warehouse.clear();
        string line;
        int lineCount = 0;

        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                Product product = Product::fromString(line);
                warehouse.addProductFromData(product);
                lineCount++;
            }
            catch (const exception& e) {
                cout << "Ошибка при чтении строки: " << line << endl;
            }
        }

        file.close();
        if (lineCount > 0) {
            cout << "Загружено " << lineCount << " товаров из файла " << filename << endl;
        }
        return lineCount > 0;
    }
};

// Консольный интерфейс
class ConsoleInterface {
public:
    // Очистка потока ввода
    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Вывод главного меню
    void showMainMenu() {
        cout << "\n++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "+        СКЛАДСКОЕ ХОЗЯЙСТВО           +" << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "1. Добавить товар" << endl;
        cout << "2. Удалить товар" << endl;
        cout << "3. Оформить приход товара" << endl;
        cout << "4. Оформить расход товара" << endl;
        cout << "5. Найти товар по артикулу" << endl;
        cout << "6. Найти товары по наименованию" << endl;
        cout << "7. Показать все товары" << endl;
        cout << "8. Сформировать отчёт об остатках" << endl;
        cout << "9. Выход" << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "Выберите пункт меню (1-9): ";
    }

    // Ввод артикула
    string inputArticle(const string& prompt = "Введите артикул товара: ") {
        string article;
        cout << prompt;
        getline(cin, article);
        return article;
    }

    // Ввод наименования
    string inputName(const string& prompt = "Введите наименование товара: ") {
        string name;
        cout << prompt;
        getline(cin, name);
        return name;
    }

    // Ввод количества
    int inputQuantity(const string& prompt = "Введите количество: ") {
        int quantity;
        cout << prompt;
        cin >> quantity;
        clearInput();
        return quantity;
    }

    // Ввод цены
    double inputPrice(const string& prompt = "Введите цену за единицу: ") {
        double price;
        cout << prompt;
        cin >> price;
        clearInput();
        return price;
    }

    // Ввод целого числа с проверкой
    int inputInt(const string& prompt) {
        int value;
        cout << prompt;
        cin >> value;
        clearInput();
        return value;
    }

    // Пауза перед продолжением
    void waitForEnter() {
        cout << "\nНажмите Enter для продолжения...";
        cin.get();
    }
};

// Главный класс приложения
class Application {
private:
    Warehouse warehouse;
    FileManager fileManager;
    ConsoleInterface console;
    bool running;

    // Обработка добавления товара
    void handleAddProduct() {
        cout << "\n--- ДОБАВЛЕНИЕ ТОВАРА ---" << endl;
        string article = console.inputArticle();
        string name = console.inputName();
        int quantity = console.inputQuantity();
        double price = console.inputPrice();

        Product newProduct(article, name, quantity, price);
        warehouse.addProduct(newProduct);
    }

    // Обработка удаления товара
    void handleRemoveProduct() {
        cout << "\n--- УДАЛЕНИЕ ТОВАРА ---" << endl;
        string article = console.inputArticle();
        warehouse.removeProduct(article);
    }

    // Обработка прихода товара
    void handleAddStock() {
        cout << "\n--- ПРИХОД ТОВАРА ---" << endl;
        string article = console.inputArticle();
        int amount = console.inputQuantity("Введите количество для прихода: ");
        warehouse.addStock(article, amount);
    }

    // Обработка расхода товара
    void handleRemoveStock() {
        cout << "\n--- РАСХОД ТОВАРА ---" << endl;
        string article = console.inputArticle();
        int amount = console.inputQuantity("Введите количество для расхода: ");
        warehouse.removeStock(article, amount);
    }

    // Обработка поиска по артикулу
    void handleFindByArticle() {
        cout << "\n--- ПОИСК ТОВАРА ПО АРТИКУЛУ ---" << endl;
        string article = console.inputArticle();
        Product* product = warehouse.findProduct(article);
        if (product == nullptr) {
            cout << "Товар с артикулом " << article << " не найден!" << endl;
        }
        else {
            cout << "\nИнформация о товаре:" << endl;
            cout << "------------------------" << endl;
            product->print();
        }
    }

    // Обработка поиска по наименованию
    void handleFindByName() {
        cout << "\n--- ПОИСК ТОВАРОВ ПО НАИМЕНОВАНИЮ ---" << endl;
        string name = console.inputName("Введите наименование (или его часть): ");
        vector<Product> found = warehouse.findProductsByName(name);
        if (found.empty()) {
            cout << "Товары, содержащие \"" << name << "\", не найдены!" << endl;
        }
        else {
            cout << "\nНайдено товаров: " << found.size() << endl;
            cout << "------------------------" << endl;
            for (const auto& product : found) {
                product.print();
                cout << "------------------------" << endl;
            }
        }
    }

    // Обработка вывода всех товаров
    void handleListAll() {
        warehouse.listAllProducts();
    }

    // Обработка формирования отчёта
    void handlePrintReport() {
        warehouse.printReport();
    }

public:
    Application() : running(true) {}

    // Запуск приложения
    void run() {
        // Загружаем данные из файла
        cout << "Загрузка данных..." << endl;
        fileManager.loadFromFile(warehouse);

        // Основной цикл программы
        while (running) {
            console.showMainMenu();
            int choice = console.inputInt("");

            switch (choice) {
            case 1:
                handleAddProduct();
                break;
            case 2:
                handleRemoveProduct();
                break;
            case 3:
                handleAddStock();
                break;
            case 4:
                handleRemoveStock();
                break;
            case 5:
                handleFindByArticle();
                break;
            case 6:
                handleFindByName();
                break;
            case 7:
                handleListAll();
                break;
            case 8:
                handlePrintReport();
                break;
            case 9:
                cout << "\nЗавершение работы..." << endl;
                fileManager.saveToFile(warehouse);
                running = false;
                break;
            default:
                cout << "Ошибка: неверный пункт меню. Выберите 1-9." << endl;
                break;
            }

            if (running) {
                console.waitForEnter();
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "+    ДОБРО ПОЖАЛОВАТЬ В ПРОГРАММУ      +" << endl;
    cout << "+    СКЛАДСКОЕ ХОЗЯЙСТВО               +" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Разработчик: Рассоха Е.Д." << endl;
    cout << "Группа: бИД-252" << endl;

    Application app;
    app.run();

    cout << "\nПрограмма завершена. До свидания!" << endl;
    return 0;
}
