#ifndef LAB3_APP_HPP
#define LAB3_APP_HPP

namespace NonlinearSolver {

enum class MenuOption {
    ITERATION_METHOD = 1,
    BISECTION_METHOD,
    NEWTON_METHOD,
    ADDITIONAL_TASK,
    EXIT = 0
};

bool shouldContinue();
void showMenu();
void runProgram();
void iterationMethod();
void bisectionMethod();
void newtonMethod();
void additionalTask();

}

#endif
