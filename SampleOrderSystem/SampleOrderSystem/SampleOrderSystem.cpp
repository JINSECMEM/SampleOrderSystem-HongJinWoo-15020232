#include <windows.h>
#include "Controllers/AppController.h"

int main() {
    // QuickEdit 모드 비활성화 - 콘솔 클릭 시 I/O가 멈추는 현상 방지
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(hInput, &consoleMode);
    consoleMode &= ~ENABLE_QUICK_EDIT_MODE;
    consoleMode |= ENABLE_EXTENDED_FLAGS;
    SetConsoleMode(hInput, consoleMode);

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    FlushConsoleInputBuffer(hInput);

    AppController app;
    app.Run();

    return 0;
}
