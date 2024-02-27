
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <limits>
#include <filesystem>

namespace fs = std::filesystem;
struct Inventory {
    std::string NameOfGood;
    std::string CategoryOfGood;
    int QuantityOfGood;
    float priceOfGood;
    int ChoiseToChange;
    const std::string databasePath = "C:/Users/Admin/source/repos/ConsoleApplication32/ConsoleApplication32/DataBase/";

    Inventory() : QuantityOfGood(0), priceOfGood(0), ChoiseToChange(0) {}

    std::string getValidStringInput(const std::string& prompt) {
        std::string userInput;
        do {
            std::cout << prompt;
            std::cin >> userInput;
            if (userInput.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos) {
                std::cerr << "Ошибка! Введите корректные данные.\n";
            }
            else {
                break;
            }
        } while (true);
        return userInput;
    }

    double getValidNumberInput(const std::string& prompt) {
        double userInput;
        std::string inputStr;

        do {
            std::cout << prompt;
            std::cin >> inputStr;

            try {
                size_t pos;
                userInput = std::stod(inputStr, &pos);

                // Проверка, что введен разделитель - точка, а не запятая
                if (pos != std::string::npos && inputStr[pos] == ',') {
                    throw std::invalid_argument("Invalid character ',' in input");
                }

                break;
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Ошибка! " << e.what() << "\n";
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Ошибка! " << e.what() << "\n";
            }
            catch (...) {
                std::cerr << "Ошибка! Введите числовое значение.\n";
            }

            std::cout << "----------------------------------------------------" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } while (true);

        return userInput;
    }

    int getValidIntegerInput(const std::string& prompt) {
        double userInput;
        std::string inputStr;

        do {
            std::cout << prompt;
            std::cin >> inputStr;

            try {
                size_t pos;
                userInput = std::stod(inputStr, &pos);

                // Проверка, что вся строка была использована (нет лишних символов)
                if (pos != inputStr.length()) {
                    std::cerr << "Ошибка! Введите целочисленное значение.\n";
                    continue;
                }

                // Проверка, что введено целое число (без дробной части)
                if (userInput != static_cast<int>(userInput)) {
                    std::cerr << "Ошибка! Введите целочисленное значение.\n";
                    continue;
                }
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Ошибка! Введите числовое значение.\n";
                continue;
            }
            catch (const std::out_of_range&) {
                std::cerr << "Ошибка! Введено слишком большое или слишком маленькое число.\n";
                continue;
            }

            break;

            std::cout << "----------------------------------------------------" << std::endl;
        } while (true);

        return static_cast<int>(userInput);
    }

    bool AddData(bool itemAdded = false) {
        std::string NameOfGood;
        std::string CategoryOfGood;
        std::string directoryName;
        std::string filePath;
        std::cout << "----------------------------------------------------" << std::endl;

        while (true) {
            int choise = getValidIntegerInput("Вы уверены, что хотите добавить товар?\n1. Добавить товар\n2. Вернуться к выбору\n Ваш выбор: ");
            switch (choise) {
            case 1:
                NameOfGood = getValidStringInput("Введите название товара: ");
                CategoryOfGood = getValidStringInput("Введите категорию товара: ");
                directoryName = databasePath + CategoryOfGood;

                if (fs::exists(directoryName) && fs::is_directory(directoryName)) {
                    std::string filePath = directoryName + "/" + NameOfGood + ".txt";

                    if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
                        std::cerr << "Ошибка! Товар с таким названием уже существует в указанной категории.\n";
                        return false;
                    }

                    int QuantityOfGood = getValidIntegerInput("Введите количество товара: ");
                    if (QuantityOfGood < 0) {
                        std::cerr << "Ошибка! Введите корректные данные о количестве товара.\n";
                        return false;
                    }

                    int priceOfGood = getValidNumberInput("Введите цену товара: ");
                    if (priceOfGood < 0) {
                        std::cerr << "Ошибка! Введите корректные данные о цене товара.\n";
                        return false;
                    }

                    std::ofstream file(filePath);

                    if (!file.is_open()) {
                        std::cerr << "Ошибка! Файл не был открыт!\n";
                    }
                    else {
                        file << "Название товара: " << NameOfGood << "\n";
                        file << "Категория товара: " << CategoryOfGood << "\n";
                        file << "Количество товара: " << QuantityOfGood << "\n";
                        file << "Цена товара: " << priceOfGood << "\n";

                        std::time_t currentTime;
                        std::time(&currentTime);
                        char timeBuffer[26];
                        ctime_s(timeBuffer, sizeof(timeBuffer), &currentTime);
                        file << "Дата и время изменения товара: " << timeBuffer;

                        std::cout << "Данные успешно записаны в файл.\n";
                        return true;
                    }
                }
                else {
                    try {
                        fs::create_directory(directoryName);
                        std::cout << "Категория сохранена. " << std::endl;

                        int QuantityOfGood = getValidIntegerInput("Введите количество товара: ");
                        if (QuantityOfGood < 0) {
                            std::cerr << "Ошибка! Введите корректные данные о количестве товара.\n";
                            return false;
                        }

                        int priceOfGood = getValidNumberInput("Введите цену товара: ");
                        if (priceOfGood < 0) {
                            std::cerr << "Ошибка! Введите корректные данные о цене товара.\n";
                            return false;
                        }

                        std::string filePath = directoryName + "/" + NameOfGood + ".txt";
                        std::ofstream file(filePath);

                        if (!file.is_open()) {
                            std::cerr << "Ошибка! Файл не был открыт! \n";
                        }
                        else {
                            file << "Название товара: " << NameOfGood << "\n";
                            file << "Категория товара: " << CategoryOfGood << "\n";
                            file << "Количество товара: " << QuantityOfGood << "\n";
                            file << "Цена товара: " << priceOfGood << "\n";

                            std::time_t currentTime;
                            std::time(&currentTime);
                            char timeBuffer[26];
                            ctime_s(timeBuffer, sizeof(timeBuffer), &currentTime);
                            file << "Дата и время изменения товара: " << timeBuffer;

                            std::cout << "Данные успешно записаны в файл.\n";
                            return true;
                        }
                    }
                    catch (const std::filesystem::filesystem_error& e) {
                        std::cerr << "Категория не была сохранена. " << e.what() << "\n";
                        return false;
                    }
                }
                break;

            case 2:
                return false;
                break;

            default:
                std::cout << "Некорректный ввод." << std::endl;
                std::cout << "----------------------------------------------------" << std::endl;
            }
        }
    }


    void DeleteData() {
        if (fs::is_empty(databasePath)) {
            std::cout << "Нечего удалять.";
            return;
        }
        else {
            std::string directoryName;
            std::string filePath;

            do {
                std::cout << "Существующие категории: \n";
                for (const auto& entry : fs::directory_iterator(databasePath)) {
                    if (fs::is_directory(entry.path())) {
                        std::cout << entry.path().filename() << "\n";
                    }
                }
                std::cout << "----------------------------------------------------" << std::endl;

                ChoiseToChange = getValidNumberInput("Выберите действие:\n1.Удалить товар\n2.Удалить категорию\n0.Отмена\nВаш выбор: ");
                if (ChoiseToChange != 1 && ChoiseToChange != 2 && ChoiseToChange != 0) {
                    std::cerr << "Введите корректный выбор!" << std::endl;
                    std::cout << "----------------------------------------------------" << std::endl;
                }
            } while (ChoiseToChange != 1 && ChoiseToChange != 2 && ChoiseToChange != 0);
            std::cout << "----------------------------------------------------" << std::endl;

            switch (ChoiseToChange) {
            case 1:
                do {
                    std::cout << "Введите категорию товара, который хотите удалить: ";
                    std::cin >> CategoryOfGood;
                    directoryName = databasePath + CategoryOfGood;

                    if (fs::exists(directoryName) && fs::is_directory(directoryName)) {
                        if (fs::is_empty(directoryName) || std::distance(fs::directory_iterator(directoryName), fs::directory_iterator()) == 0) {
                            std::cout << "Выбранная категория товаров не существует или пуста." << std::endl;
                        }
                        else {
                            std::cout << "Товары в выбранной категории:\n";
                            for (const auto& entry : fs::directory_iterator(directoryName)) {
                                if (fs::is_regular_file(entry.path())) {
                                    std::cout << entry.path().filename() << "\n";
                                }
                            }
                            std::cout << "----------------------------------------------------" << std::endl;
                            break;
                        }
                    }
                    else {
                        std::cerr << "Ошибка! Неверно введена категория или категория не существует.\n";
                    }
                } while (true);

                do {
                    std::cout << "Введите название товара, который вы хотите удалить: ";
                    std::cin >> NameOfGood;

                    filePath = directoryName + "/" + NameOfGood + ".txt";

                    if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
                        fs::remove(filePath);
                        std::cout << "Товар успешно удален.\n";

                        // Проверяем, стала ли категория пустой после удаления файла
                        if (fs::is_empty(directoryName) || std::distance(fs::directory_iterator(directoryName), fs::directory_iterator()) == 0) {
                            fs::remove(directoryName);
                            std::cout << "Категория успешно удалена, так как она стала пустой.\n";
                        }
                        break;
                    }
                    else {
                        std::cout << "Такого товара не существует.\n";
                    }
                } while (true);
                break;
            case 2:
                do {
                    std::cout << "Введите категорию товара, которую хотите удалить: ";
                    std::cin >> CategoryOfGood;
                    directoryName = databasePath + CategoryOfGood;

                    if (fs::exists(directoryName) && fs::is_directory(directoryName)) {
                        break;
                    }
                    else {
                        std::cerr << "Ошибка! Неверно введена категория или категория не существует.\n";
                    }
                } while (true);

                try {
                    for (const auto& entry : fs::directory_iterator(directoryName)) {
                        fs::remove_all(entry.path());
                    }
                    fs::remove(directoryName);

                    std::cout << "Категория успешно удалена." << std::endl;
                }
                catch (const std::filesystem::filesystem_error& e) {
                    std::cerr << "Ошибка при удалении категории: \n" << e.what() << std::endl;
                    return;
                }
                std::cout << "----------------------------------------------------" << std::endl;
                break;
            case 0:
                return;
                break;
            }
        }
    }


    void DisplayData() {
        std::string directoryName;
        int choise;
        std::string fileName;
        std::string input;
        std::string Data;
        bool continueChoosingCategory = true;  // Флаг для определения, нужно ли продолжать выбор категории
        std::cout << "----------------------------------------------------" << std::endl;
        while (true) {
            int choise = getValidIntegerInput("Вы уверены, чо хотите просмотреть товар?\n1. Просмотреть товар\n2. Вернуться к выбору\n Ваш выбор: ");
            switch (choise) {
            case 1:
                if (fs::is_empty(databasePath)) {
                    std::cout << "Нечего просматривать.";
                    return;
                }
                do {
                    // Выводим существующие категории
                    std::cout << "Существующие категории: \n";
                    for (const auto& entry : fs::directory_iterator(databasePath)) {
                        if (fs::is_directory(entry.path())) {
                            std::cout << entry.path().filename() << "\n";
                        }
                    }
                    std::cout << "----------------------------------------------------" << std::endl;

                    // Запрашиваем ввод категории товаров
                    std::cout << "Введите категорию товара, которую хотите просмотреть: " << std::endl;
                    std::cin >> CategoryOfGood;

                    directoryName = databasePath + CategoryOfGood;

                    try {
                        // Проверяем существование и является ли введенная строка директорией
                        if (fs::exists(directoryName) && fs::is_directory(directoryName)) {
                            // Проверяем, не пуста ли категория товаров
                            if (fs::is_empty(directoryName) || std::distance(fs::directory_iterator(directoryName), fs::directory_iterator()) == 0) {
                                std::cout << "Выбранная категория товаров пуста." << std::endl;
                                continueChoosingCategory = true;  // Продолжаем выбор категории
                            }
                            else {
                                // Выводим список товаров в выбранной категории
                                std::cout << "Товары в выбранной категории:\n";
                                for (const auto& entry : fs::directory_iterator(directoryName)) {
                                    if (fs::is_regular_file(entry.path())) {
                                        std::cout << entry.path().filename() << "\n";
                                    }
                                }
                                std::cout << "----------------------------------------------------" << std::endl;

                                continueChoosingCategory = false;  // Переходим к следующему этапу
                            }
                        }
                        else {
                            // Выводим ошибку, если категория не существует
                            std::cerr << "Ошибка! Неверно введена категория или категория не существует.\n";
                            std::cout << "Попробуйте ввести категорию еще раз." << std::endl;
                            continueChoosingCategory = true;  // Продолжаем выбор категории
                        }
                    }
                    catch (const std::filesystem::filesystem_error& e) {
                        // Выводим ошибку, если произошла ошибка при обращении к файловой системе
                        std::cerr << "Ошибка! Не удалось открыть категорию. " << e.what() << "\n";
                        return;
                    }
                } while (continueChoosingCategory);
                do {
                    std::cout << "Введите название товара, который хотите просмотреть: " << std::endl;
                    std::cin >> NameOfGood;
                    fileName = directoryName + "/" + NameOfGood + ".txt";
                    std::cout << "----------------------------------------------------" << std::endl;
                    if (fs::exists(fileName) && fs::is_regular_file(fileName)) {
                        std::ifstream file(fileName);
                        if (!file.is_open()) {
                            std::cout << "Ошибка в открытии файла" << std::endl;
                            std::cout << "----------------------------------------------------" << std::endl;
                            continue;  // Продолжаем ввод товара
                        }
                        else {
                            std::cout << "Информация о товаре " << NameOfGood << ": " << std::endl;
                            std::string fileLine;
                            while (getline(file, fileLine)) {
                                std::cout << fileLine << std::endl;
                            }
                            std::cout << "----------------------------------------------------" << std::endl;
                            break;  // Выходим из цикла, если товар успешно найден и выведен
                        }
                    }
                    else {
                        std::cout << "Файл не существует или не является обычным файлом." << std::endl;
                    }
                } while (true);
                break;
            case 2:
                return;
                break;
            default:
                std::cout << "Некорректный ввод." << std::endl;
                std::cout << "----------------------------------------------------" << std::endl;
            }
        }
    }




    void ChangeData() {
        std::string newNameOfGood;
        std::string newCategoryOfGood;
        int newpriceOfGood;
        int newQuantityOfGood;
        std::string directoryName;
        std::string filePath;

        if (fs::is_empty(databasePath)) {
            std::cout << "Нечего изменять.";
            return;
        }
        else {
            std::cout << "----------------------------------------------------" << std::endl;
            while (true) {
                int choise = getValidIntegerInput("Вы уверены, чо хотите изменить товар?\n1. Изменить товар\n2. Вернуться к выбору\n Ваш выбор: ");
                switch (choise) {
                case 1:



                    do {
                        std::cout << "Существующие категории: \n";
                        for (const auto& entry : fs::directory_iterator(databasePath)) {
                            if (fs::is_directory(entry.path())) {
                                std::cout << entry.path().filename() << "\n";
                            }
                        }
                        std::cout << "----------------------------------------------------" << std::endl;

                        CategoryOfGood = getValidStringInput("Введите категорию товара, который хотите изменить: ");
                        directoryName = databasePath + CategoryOfGood;

                        if (fs::exists(directoryName) && fs::is_directory(directoryName)) {
                            break;
                        }
                        else {
                            std::cerr << "Ошибка! Такой категории товара не существует.\n";
                        }
                    } while (true);

                    try {
                        if (fs::is_empty(directoryName) || std::distance(fs::directory_iterator(directoryName), fs::directory_iterator()) == 0) {
                            std::cout << "Выбранная категория товаров не существует или пуста." << std::endl;
                            return;
                        }
                        else {
                            std::cout << "Товары в выбранной категории:\n";
                            for (const auto& entry : fs::directory_iterator(directoryName)) {
                                if (fs::is_regular_file(entry.path())) {
                                    std::cout << entry.path().filename() << "\n";
                                }
                            }
                            std::cout << "----------------------------------------------------" << std::endl;
                        }

                        do {
                            NameOfGood = getValidStringInput("Введите название товара, который вы хотите изменить : ");
                            filePath = directoryName + "/" + NameOfGood + ".txt";

                            if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
                                break;
                            }
                            else {
                                std::cerr << "Ошибка! Такого товара не существует.\n";
                            }
                        } while (true);

                        std::string directoryPath = databasePath + CategoryOfGood;
                        std::string FilePath = directoryPath + "/" + NameOfGood + ".txt";

                        newNameOfGood = getValidStringInput("Введите новое название товара: ");
                        newCategoryOfGood = getValidStringInput("Введите новую категорию товара: ");
                        newQuantityOfGood = getValidIntegerInput("Введите новое количество товара: ");
                        newpriceOfGood = getValidNumberInput("Введите новую цену товара: ");

                        std::string newdirectoryPath = databasePath + newCategoryOfGood;
                        std::string newFilePath = newdirectoryPath + "/" + newNameOfGood + ".txt";

                        if (fs::exists(directoryPath) && fs::is_directory(directoryPath)) {
                            if (fs::exists(FilePath) && fs::is_regular_file(FilePath)) {
                                if (fs::exists(newdirectoryPath) && fs::is_directory(newdirectoryPath)) {
                                    std::ofstream file(newFilePath);

                                    if (!file.is_open()) {
                                        std::cerr << "Ошибка! Файл не был открыт!\n";
                                    }
                                    else {
                                        file << "Название товара: " << newNameOfGood << "\n";
                                        file << "Категория товара: " << newCategoryOfGood << "\n";
                                        file << "Количество товара: " << newQuantityOfGood << "\n";
                                        file << "Цена товара: " << newpriceOfGood << "\n";

                                        if (newQuantityOfGood < 0) {
                                            std::cout << "Введите корректные данные о количестве товара: " << std::endl;
                                            std::cin >> newQuantityOfGood;
                                        }
                                        else if (newpriceOfGood < 0) {
                                            std::cout << "Введите корректные данные о цене товара: " << std::endl;
                                            std::cin >> newpriceOfGood;
                                        }

                                        std::time_t currentTime;
                                        std::time(&currentTime);
                                        char timeBuffer[26];
                                        ctime_s(timeBuffer, sizeof(timeBuffer), &currentTime);
                                        file << "Дата и время изменения товара: " << timeBuffer;

                                        std::cout << "Данные успешно записаны в файл.\n";

                                        fs::remove(FilePath);

                                        if (fs::is_empty(directoryPath)) {
                                            fs::remove(directoryPath);
                                        }
                                    }
                                }
                            }
                            else {
                                std::cerr << "Такого товара не существует.";
                            }
                        }
                        else {
                            std::cerr << "Такой категории товара не существует";
                        }
                    }
                    catch (const std::filesystem::filesystem_error& e) {
                        std::cerr << "Ошибка: " << e.what();
                    }
                    break;
                case 2:
                    return;
                    break;
                default:
                    std::cout << "Некорректный ввод." << std::endl;
                    std::cout << "----------------------------------------------------" << std::endl;
                }
                std::cout << "----------------------------------------------------";

            }
        }
    }
};

int getValidNumberMainInput(const std::string& prompt) {
    double userInput;
    do {
        std::cout << prompt;
        std::cin >> userInput;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Ошибка! Введите числовое значение.\n";
            std::cout << "----------------------------------------------------" << std::endl;
        }
        else {
            break;
        }
    } while (true);
    return userInput;
}

int main() {
    int choice;
    setlocale(LC_ALL, "ru");
    Inventory inventory;

    const std::string DatabasePath = "C:/Users/Admin/source/repos/ConsoleApplication32/ConsoleApplication32/DataBase";

    std::cout << ". . : БАЗА ДАННЫХ ТОВАРОВ : . ." << std::endl;

    if (fs::is_empty(DatabasePath)) {
        bool itemAdded = false;
        do {
            choice = getValidNumberMainInput("\nвыберите действие:\n 1. Добавить товар \n 0. Завершение программы \n Ваш выбор: ");

            switch (choice) {
            case 1:
                itemAdded = inventory.AddData();
                if (itemAdded) {
                    std::cout << "Товар успешно добавлен.\n";
                    break;
                }
                std::cout << "Товар не был добавлен. Попробуйте еще раз.\n";
                break;
            case 0:
                std::cout << "Программа завершена.";
                return 0;
            default:
                std::cout << "некорректный ввод";
                break;
            }

            std::cout << "________________________________________________________________" << std::endl;
        } while (!itemAdded);
    }
    else {
        std::cout << "Существующие категории товаров:\n";

        for (const auto& entry : fs::directory_iterator(DatabasePath)) {
            if (fs::is_directory(entry.path())) {
                std::cout << "            " << entry.path().filename() << "\n";
            }
        }

        std::cout << "_______________________________" << std::endl;
        std::cout << "-------------------------------" << std::endl;
    }

    do {
        choice = getValidNumberMainInput("\nвыберите действие:\n 1. Добавить товар\n 2. Просмотреть категории и товары\n 3. Удалить товар или категорию\n 4. Изменить товар \n 0. Завершение программы \n Ваш выбор: ");

        switch (choice) {
        case 1:
            inventory.AddData();
            break;
        case 2:
            inventory.DisplayData();
            break;
        case 3:
            inventory.DeleteData();
            break;
        case 4:
            inventory.ChangeData();
            break;
        case 0:
            std::cout << "Выход из программы. . .";
            break;
        default:
            std::cout << "некорректный ввод" << std::endl;
            std::cout << "________________________________________________________________" << std::endl;
            break;
        }
    } while (choice != 0);

    return 0;
}