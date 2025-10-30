#ifndef LAB3_APP_HPP
#define LAB3_APP_HPP

namespace lab_3 {

enum class MenuOption {
    EXIT = 0,
    BISECTION_METHOD = 1,
    NEWTON_METHOD = 2,
    ITERATION_METHOD = 3,
    COMBINED_METHOD = 4
};

bool shouldContinue();
MenuOption displayMenu();
void runApplication();
void executeTask(MenuOption option);

void bisectionMethod();
void newtonMethod();
void iterationMethod();
void combinedMethod();

}

#endif
