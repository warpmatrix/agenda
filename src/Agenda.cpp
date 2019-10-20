#include "AgendaUI.hpp"
#include <signal.h>

AgendaUI ui;

void setSig(int sig) {
    ui.quitAgenda();
    exit(0);
}
int main() {
    signal(SIGINT, setSig);
    ui.startAgenda();
    return 0;
}