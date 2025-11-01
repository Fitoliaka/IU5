#include "lab3_app.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

namespace NonlinearSolver {

const int MAXITER = 100000;
const double MINEPS = 1e-15;
const int MINPRECISION = 1;
const int MAXPRECISION = 15;
const double DEFAULTCOEF = 1.0;
const int DEFAULT_DIGITS = 6;
const double HALF_DIVISOR = 2.0;

[[nodiscard]] double func(double x, double coef) {
  return x - coef * std::cos(x);
}

[[nodiscard]] double dfunc(double x, double coef) {
  return 1.0 + coef * std::sin(x);
}

[[nodiscard]] double gfunc(double x, double coef) {
  return coef * std::cos(x);

[[nodiscard]] double funcTask4(double x) {
  return std::tan(x / 2.0) - x;
}

[[nodiscard]] double dfuncTask4(double x) {
  return (1.0 / (2.0 * std::cos(x / 2.0) * std::cos(x / 2.0))) - 1.0;
}

[[nodiscard]] double gfuncTask4(double x) {
  return std::tan(x / 2.0);
}

[[nodiscard]] int countDigits(double eps) {
  if (eps <= 0.0) {
    return DEFAULT_DIGITS;
  }

  double logeps = std::log10(eps);
  int digits = static_cast<int>(std::ceil(-logeps));

  if (digits < MINPRECISION) {
    digits = MINPRECISION;
  }
  if (digits > MAXPRECISION) {
    digits = MAXPRECISION;
  }

  return digits;
}

void printResult(double root, int iter, double eps) {
  int prec = countDigits(eps);

  std::cout << std::fixed << std::setprecision(prec);
  std::cout << "Найденный корень: " << root << std::endl;
  std::cout << "Количество итераций: " << iter << std::endl;
  std::cout << "Точность: " << eps << std::endl;
  std::cout << "Значение функции: f(" << root << ") = " << func(root, 1.0)
            << std::endl;
  std::cout << "----------------------------------------" << std::endl;

  std::cout << std::defaultfloat;
}

void printResultTask4(double root, int iter, double eps,
                      const std::string& method) {
  int prec = countDigits(eps);

  std::cout << std::fixed << std::setprecision(prec);
  std::cout << "Метод: " << method << std::endl;
  std::cout << "Найденный корень: " << root << std::endl;
  std::cout << "Количество итераций: " << iter << std::endl;
  std::cout << "Точность: " << eps << std::endl;
  std::cout << "Значение функции: f(" << root << ") = " << funcTask4(root)
            << std::endl;
  std::cout << "----------------------------------------" << std::endl;

  std::cout << std::defaultfloat;
}

[[nodiscard]] double getEps() {
  double eps = 0.001;
  std::cout << "Введите точность (например 0.001 или 1e-6): ";
  std::cin >> eps;
  return eps;
}

[[nodiscard]] double getCoef() {
  double coef = 1.0;
  std::cout << "Введите коэффициент при cos(x): ";
  std::cin >> coef;
  return coef;
}

bool shouldContinue() {
  char choice = 'n';
  std::cout << "Продолжить работу? (y/n): ";
  std::cin >> choice;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return choice == 'y';
}

void showMenu() {
  std::cout << "\n=== РЕШЕНИЕ НЕЛИНЕЙНЫХ УРАВНЕНИЙ ===" << std::endl;
  std::cout << "1. Метод простых итераций" << std::endl;
  std::cout << "2. Метод половинного деления" << std::endl;
  std::cout << "3. Метод Ньютона" << std::endl;
  std::cout << "4. Дополнительное задание (tg(x/2) - x = 0)" << std::endl;
  std::cout << "0. Выход" << std::endl;
  std::cout << "Выберите метод: ";
}

void iterationMethod() {
  std::cout << "\n=== МЕТОД ПРОСТЫХ ИТЕРАЦИЙ ===" << std::endl;
  std::cout << "Уравнение: x - k*cos(x) = 0" << std::endl;

  double eps = getEps();
  double coef = getCoef();

  double x0 = 0.0;
  std::cout << "Введите начальное приближение: ";
  std::cin >> x0;

  int iter = 0;
  double x = x0;
  double xold = x0;

  do {
    xold = x;
    x = gfunc(xold, coef);
    iter++;

    if (iter > MAXITER) {
      std::cout << "Слишком много итераций!" << std::endl;
      return;
    }
  } while (std::abs(x - xold) > eps);

  printResult(x, iter, eps);
}

void bisectionMethod() {
  std::cout << "\n=== МЕТОД ПОЛОВИННОГО ДЕЛЕНИЯ ===" << std::endl;
  std::cout << "Уравнение: x - k*cos(x) = 0" << std::endl;

  double eps = getEps();
  double coef = getCoef();

  double a = 0.0, b = 0.0;
  std::cout << "Введите левый конец интервала: ";
  std::cin >> a;
  std::cout << "Введите правый конец интервала: ";
  std::cin >> b;

  double fa = func(a, coef);
  double fb = func(b, coef);

  if (fa * fb > 0.0) {
    std::cout << "Ошибка: f(a) и f(b) должны быть разных знаков!" << std::endl;
    std::cout << "f(" << a << ") = " << fa << std::endl;
    std::cout << "f(" << b << ") = " << fb << std::endl;
    return;
  }

  int iter = 0;
  double root = (a + b) / HALF_DIVISOR;

  while ((b - a) > eps && iter < MAXITER) {
    root = (a + b) / HALF_DIVISOR;
    iter++;

    if (std::abs(func(root, coef)) < eps) {
      break;
    }

    if (func(a, coef) * func(root, coef) < 0.0) {
      b = root;
    } else {
      a = root;
    }
  }

  printResult(root, iter, eps);
}

void newtonMethod() {
  std::cout << "\n=== МЕТОД НЬЮТОНА ===" << std::endl;
  std::cout << "Уравнение: x - k*cos(x) = 0" << std::endl;

  double eps = getEps();
  double coef = getCoef();

  double x0 = 0.0;
  std::cout << "Введите начальное приближение: ";
  std::cin >> x0;

  int iter = 0;
  double x = x0;
  double xold = x0;

  do {
    xold = x;
    double fx = func(xold, coef);
    double dfx = dfunc(xold, coef);

    if (std::abs(dfx) < MINEPS) {
      std::cout << "Ошибка: производная слишком мала!" << std::endl;
      return;
    }

    x = xold - fx / dfx;
    iter++;

    if (iter > MAXITER) {
      std::cout << "Слишком много итераций!" << std::endl;
      return;
    }
  } while (std::abs(x - xold) > eps);

  printResult(x, iter, eps);
}

void additionalTask() {
  std::cout << "\n=== ДОПОЛНИТЕЛЬНОЕ ЗАДАНИЕ ===" << std::endl;
  std::cout << "Уравнение: tg(x/2) - x = 0" << std::endl;
  std::cout << "Начальные приближения: -2.3, 0, 2.3" << std::endl;

  double eps = getEps();

  int choice = 0;
  std::cout << "\nВыберите метод для дополнительного задания:" << std::endl;
  std::cout << "1. Метод итераций" << std::endl;
  std::cout << "2. Метод касательных (Ньютона)" << std::endl;
  std::cout << "Выберите метод: ";
  std::cin >> choice;

  if (choice == 1) {
    std::cout << "\n=== МЕТОД ИТЕРАЦИЙ (tg(x/2) - x = 0) ===" << std::endl;

    double x0 = 0.0;
    std::cout << "Введите начальное приближение (-2.3, 0, 2.3): ";
    std::cin >> x0;

    int iter = 0;
    double x = x0;
    double xold = x0;

    do {
      xold = x;
      x = gfuncTask4(xold);
      iter++;

      if (iter > MAXITER) {
        std::cout << "Слишком много итераций!" << std::endl;
        return;
      }
    } while (std::abs(x - xold) > eps);

    printResultTask4(x, iter, eps, "Метод итераций");

  } else if (choice == 2) {
    std::cout << "\n=== МЕТОД КАСАТЕЛЬНЫХ (tg(x/2) - x = 0) ===" << std::endl;

    double x0 = 0.0;
    std::cout << "Введите начальное приближение (-2.3, 0, 2.3): ";
    std::cin >> x0;

    int iter = 0;
    double x = x0;
    double xold = x0;

    do {
      xold = x;
      double fx = funcTask4(xold);
      double dfx = dfuncTask4(xold);

      if (std::abs(dfx) < MINEPS) {
        std::cout << "Ошибка: производная слишком мала!" << std::endl;
        return;
      }

      x = xold - fx / dfx;
      iter++;

      if (iter > MAXITER) {
        std::cout << "Слишком много итераций!" << std::endl;
        return;
      }
    } while (std::abs(x - xold) > eps);

    printResultTask4(x, iter, eps, "Метод касательных");

  } else {
    std::cout << "Неверный выбор метода!" << std::endl;
  }
}

void runProgram() {
  std::cout << "=== ПРОГРАММА ДЛЯ РЕШЕНИЯ УРАВНЕНИЙ ===" << std::endl;

  do {
    showMenu();

    int choice = 0;
    std::cin >> choice;

    MenuOption option = static_cast<MenuOption>(choice);

    switch (option) {
    case MenuOption::ITERATION_METHOD:
      iterationMethod();
      break;
    case MenuOption::BISECTION_METHOD:
      bisectionMethod();
      break;
    case MenuOption::NEWTON_METHOD:
      newtonMethod();
      break;
    case MenuOption::ADDITIONAL_TASK:
      additionalTask();
      break;
    case MenuOption::EXIT:
      std::cout << "Выход из программы" << std::endl;
      return;
    default:
      std::cout << "Неверный выбор!" << std::endl;
      break;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  } while (shouldContinue());

  std::cout << "Программа завершена" << std::endl;
}

} // namespace NonlinearSolver
