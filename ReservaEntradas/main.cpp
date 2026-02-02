#include "lib/mvc/Controller.h"
#include "lib/mvc/ConsoleView.h"

int main() {
    ConsoleView view;
    Controller controller(&view);
    controller.run();
    return 0;
}
