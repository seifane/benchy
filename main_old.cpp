#include <iostream>
#include <windows.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <tchar.h>
#include <vector>

BOOL CALLBACK enumWindowsCallback(HWND hwnd, LPARAM lParam){
    const DWORD TITLE_SIZE = 1024;
    WCHAR windowTitle[TITLE_SIZE];

    GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

    int length = ::GetWindowTextLength(hwnd);
    std::wstring title(&windowTitle[0]);
    if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager") {
        return TRUE;
    }

    // Retrieve the pointer passed into this callback, and re-'type' it.
    // The only way for a C API to pass arbitrary data is by means of a void*.
    std::vector<std::string>& titles =
            *reinterpret_cast<std::vector<std::string>*>(lParam);
    titles.push_back(std::string(title.begin(), title.end()));

    return TRUE;
}

std::string getWindowFromList(std::vector<std::string> list, std::string like) {
    for (const auto &item: list) {
        if (item.find(like) != std::string::npos) {
            return item;
        }
    }
    return "";
}

std::vector<std::string> getOpenWindows() {
    std::vector<std::string> titles;
    EnumWindows(enumWindowsCallback, reinterpret_cast<LPARAM>(&titles));
    // At this point, titles if fully populated and could be displayed, e.g.:
    for ( const auto& title : titles )
        std::cout << "Title: " << title << std::endl;
    return titles;
}

HWND findWindow(LPARAM windowTitle) //TCHAR windowTitle[ 2048 ] = "Untitled - Notepad";
{ // Should iterate through all windows and find a window matching title without the need for an exact match.
    HWND notepadHandle = FindWindow(0, LPCTSTR(windowTitle));
    TCHAR title[250];
    GetWindowText(notepadHandle,title,250);
    std::cout << "TITLE:" << title << std::endl;
    return notepadHandle;
}

void ClickButton(HWND hwnd, LPARAM buttonText)
{
    auto ButtonHandle = FindWindowEx(hwnd, 0, "Button", reinterpret_cast<LPCSTR>(buttonText));
    PostMessage(ButtonHandle,BM_CLICK,0,0);
/*    char windowText[2048];
    GetWindowText(hwnd, windowText, 2048);
    std::cout << "WINDOWTEXT:" << windowText << std::endl;
    if(_tcsstr(windowText, LPCTSTR(buttonText)) != NULL)
    {
        SendMessage(hwnd, BM_CLICK, 0, 0);
        std::cout << "BUTTONSELECTED:" << windowText << std::endl;
    }
    return true;*/
}

VOID startup(LPCTSTR lpApplicationName)
{
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // start the program up
    CreateProcess( lpApplicationName,   // the path
                   nullptr,        // Command line
                   nullptr,           // Process handle not inheritable
                   nullptr,           // Thread handle not inheritable
                   FALSE,          // Set handle inheritance to FALSE
                   0,              // No creation flags
                   nullptr,           // Use parent's environment block
                   nullptr,           // Use parent's starting directory
                   &si,            // Pointer to STARTUPINFO structure
                   &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    std::chrono::milliseconds timespan(5000);
    std::this_thread::sleep_for(timespan);
    // Close process and thread handles.

    auto windows = getOpenWindows();
    auto win = getWindowFromList(windows, "FurMark");
    HWND saveAsDialog = findWindow((LPARAM) win.c_str());
    ClickButton(saveAsDialog, (LPARAM)std::string("GPU stress test").c_str());

    TerminateProcess(pi.hProcess, 0);

    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    startup(std::string(R"(C:\Program Files (x86)\Geeks3D\Benchmarks\FurMark\FurMark.exe)").c_str());
    std::cout << "end" << std::endl;
    return 0;
}
