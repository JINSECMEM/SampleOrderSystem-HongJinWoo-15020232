#include <windows.h>
#include "Controllers/AppController.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    AppController app;
    app.Run();

    return 0;
}
