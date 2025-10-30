#include <iostream>
#include <cmath>
#include <iomanip>
#include "lab3_app.hpp"

namespace lab_3 {

namespace {
    const int maxIterations = 100000;
    const double derivativeEpsilon = 1e-12;
    const double defaultCoefficient = 1.0;
    const int precisionValue = 10;

    double targetFunction(double x, double coefficient = defaultCoefficient) {
        return x - coefficient * std::cos(x);
    }

    double targetFunctionDerivative(double x, double coefficient = defaultCoefficient) {
        return 1 + coefficient * std::sin(x);
    }

    double iterationTransform(double x, double coefficient = defaultCoefficient) {
        return coefficient * std::cos(x);
    }

    void printResult(double root, int iterations, const std::string& method, double coefficient) {
        std::cout << "\n" << method << " метод (коэффициент = " << coefficient << "):" << std::endl;
        std::cout << "Корень: " << std::fixed << std::setprecision(precisionValue) << root << std::endl;
        std::cout << "Количество итераций: " << iterations << std::endl;
        std::cout << "Проверка (f(x) = x - " << coefficient << "*cos(x)): "
                  << targetFunction(root, coefficient) << std::endl;
    }

    void getUserParameters(double& coefficient, double& epsilon) {
        std::cout << "Введите коэффициент при cos(x): ";
        std::cin >> coefficient;
        std::cout << "Введите точность epsilon (например, 1e-6): ";
        std::cin >> epsilon;
    }
}

bool shouldContinue() {
    char answer;
    std::cout << "\nПродолжить работу? (y/n): ";
    std::cin >> answer;
    std::cin.ignore(100, '\n');
    return (answer == 'y' || answer == 'Y');
}

MenuOption displayMenu() {
    int choice;
    std::cout << "\n=== Лабораторная работа 3 ===" << std::endl;
    std::cout << "Уравнение: x - k*cos(x) = 0" << std::endl;
    std::cout << static_cast<int>(MenuOption::BISECTION_METHOD) << " - Метод половинного деления" << std::endl;
    std::cout << static_cast<int>(MenuOption::NEWTON_METHOD) << " - Метод Ньютона" << std::endl;
    std::cout << static_cast<int>(MenuOption::ITERATION_METHOD) << " - Метод простой итерации" << std::endl;
    std::cout << static_cast<int>(MenuOption::COMBINED_METHOD) << " - Комбинированный метод" << std::endl;
    std::cout << static_cast<int>(MenuOption::EXIT) << " - Выход" << std::endl;
    std::cout << "Выберите опцию: ";
    std::cin >> choice;
    std::cin.ignore(100, '\n');
    return static_cast<MenuOption>(choice);
}

void runApplication() {
    bool running = true;

    while (running) {
        const MenuOption choice = displayMenu();

        if (choice == MenuOption::EXIT) {
            running = false;
        } else {
            executeTask(choice);
            if (choice != MenuOption::EXIT) {
                running = shouldContinue();
            }
        }
    }

    std::cout << "Программа завершена." << std::endl;
}

void executeTask(MenuOption option) {
    switch (option) {
        case MenuOption::BISECTION_METHOD:
            bisectionMethod();
            break;
        case MenuOption::NEWTON_METHOD:
            newtonMethod();
            break;
        case MenuOption::ITERATION_METHOD:
            iterationMethod();
            break;
        case MenuOption::COMBINED_METHOD:
            combinedMethod();
            break;
        default:
            std::cout << "Неверный выбор!" << std::endl;
    }
}

void bisectionMethod() {
    double a, b, coefficient, epsilon;

    std::cout << "\n=== Метод половинного деления ===" << std::endl;
    std::cout << "Введите левую границу a: ";
    std::cin >> a;
    std::cout << "Введите правую границу b: ";
    std::cin >> b;
    getUserParameters(coefficient, epsilon);

    double fa = targetFunction(a, coefficient);
    double fb = targetFunction(b, coefficient);

    if (fa * fb >= 0) {
        std::cout << "Ошибка: функция должна иметь разные знаки на концах отрезка!" << std::endl;
        std::cout << "f(a) = " << fa << std::endl;
        std::cout << "f(b) = " << fb << std::endl;
        return;
    }

    double root = a;
    int iterations = 0;

    while ((b - a) >= epsilon && iterations < maxIterations) {
        root = (a + b) / 2;
        double froot = targetFunction(root, coefficient);

        if (std::abs(froot) < epsilon) {
            break;
        } else if (froot * fa < 0) {
            b = root;
            fb = froot;
        } else {
            a = root;
            fa = froot;
        }
        iterations++;
    }

    if (iterations >= maxIterations) {
        std::cout << "Достигнуто максимальное количество итераций!" << std::endl;
    }

    printResult(root, iterations, "Половинного деления", coefficient);
}

void newtonMethod() {
    double x0, coefficient, epsilon;

    std::cout << "\n=== Метод Ньютона ===" << std::endl;
    std::cout << "Введите начальное приближение x0: ";
    std::cin >> x0;
    getUserParameters(coefficient, epsilon);

    double x = x0;
    double xprev;
    int iterations = 0;

    do {
        xprev = x;
        double fx = targetFunction(xprev, coefficient);
        double dfx = targetFunctionDerivative(xprev, coefficient);

        if (std::abs(dfx) < derivativeEpsilon) {
            std::cout << "Ошибка: производная близка к нулю!" << std::endl;
            return;
        }

        x = xprev - fx / dfx;
        iterations++;

    } while (std::abs(x - xprev) >= epsilon && iterations < maxIterations);

    if (iterations >= maxIterations) {
        std::cout << "Достигнуто максимальное количество итераций!" << std::endl;
    }

    printResult(x, iterations, "Ньютона", coefficient);
}

void iterationMethod() {
    double x0, coefficient, epsilon;

    std::cout << "\n=== Метод простой итерации ===" << std::endl;
    std::cout << "Введите начальное приближение x0: ";
    std::cin >> x0;
    getUserParameters(coefficient, epsilon);

    double x = x0;
    double xprev;
    int iterations = 0;

    do {
        xprev = x;
        x = iterationTransform(xprev, coefficient);
        iterations++;

    } while (std::abs(x - xprev) >= epsilon && iterations < maxIterations);

    if (iterations >= maxIterations) {
        std::cout << "Достигнуто максимальное количество итераций!" << std::endl;
    }

    printResult(x, iterations, "Простой итерации", coefficient);
}

void combinedMethod() {
    double a, b, coefficient, epsilon;

    std::cout << "\n=== Комбинированный метод (хорд и Ньютона) ===" << std::endl;
    std::cout << "Уравнение: x - k*cos(x) = 0" << std::endl;
    std::cout << "Введите левую границу a: ";
    std::cin >> a;
    std::cout << "Введите правую границу b: ";
    std::cin >> b;
    getUserParameters(coefficient, epsilon);

    double fa = targetFunction(a, coefficient);
    double fb = targetFunction(b, coefficient);

    if (fa * fb >= 0) {
        std::cout << "Ошибка: функция должна иметь разные знаки на концах отрезка!" << std::endl;
        std::cout << "f(a) = " << fa << std::endl;
        std::cout << "f(b) = " << fb << std::endl;
        return;
    }

    double xchord, xnewton, root;
    int iterations = 0;

    std::cout << "\nИтерационный процесс:" << std::endl;
    std::cout << std::setw(3) << "i" << std::setw(12) << "a" << std::setw(12) << "b"
              << std::setw(15) << "xchord" << std::setw(15) << "xnewton" << std::endl;

    while ((b - a) >= epsilon && iterations < maxIterations) {
        xchord = a - fa * (b - a) / (fb - fa);

        double x0newton;
        if (std::abs(targetFunctionDerivative(a, coefficient)) > derivativeEpsilon) {
            x0newton = a;
        } else {
            x0newton = b;
        }
        xnewton = x0newton - targetFunction(x0newton, coefficient) / targetFunctionDerivative(x0newton, coefficient);

        std::cout << std::setw(3) << iterations << std::fixed << std::setprecision(6)
                  << std::setw(12) << a << std::setw(12) << b
                  << std::setw(15) << xchord << std::setw(15) << xnewton << std::endl;

        if (fa * targetFunction(xchord, coefficient) < 0) {
            b = xchord;
            fb = targetFunction(b, coefficient);
        } else {
            a = xnewton;
            fa = targetFunction(a, coefficient);
        }

        root = (a + b) / 2;
        iterations++;

        if (std::abs(targetFunction(root, coefficient)) < epsilon) {
            break;
        }
    }

    if (iterations >= maxIterations) {
        std::cout << "Достигнуто максимальное количество итераций!" << std::endl;
    }

    printResult(root, iterations, "Комбинированный", coefficient);
    std::cout << "Финальный отрезок: [" << a << ", " << b << "]" << std::endl;
    std::cout << "Длина отрезка: " << (b - a) << std::endl;
}

}
