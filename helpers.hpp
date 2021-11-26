//
// Created by TieMajor on 26/11/2021.
//

#ifndef BENCHER_HELPERS_HPP
#define BENCHER_HELPERS_HPP

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

namespace benchy {
    namespace utils {
        struct ChildWinData {
            std::string windowName;
            std::string className;
            std::string text;
            std::string caption;
        };

        HWND findWindow(const std::string &windowTitle)
        {
            HWND window = FindWindow(0, LPCTSTR(windowTitle.c_str()));
            TCHAR title[250];
            GetWindowText(window, title, 250);
            return window;
        }

        std::string getWindowName(HWND win) {
            TCHAR buff[1024];
            GetWindowText(win, buff, 1024);
            return buff;
        }

        std::string getClassName(HWND hwnd) {
            char buff[512];
            GetClassName(hwnd, buff, 512);
            return buff;
        }

        std::string getWindowText(HWND hwnd) {
            TCHAR buff[1024];
            GetWindowText(hwnd, buff, 1024);
            return buff;
        }

        std::string getWindowCaption(HWND hwnd) {
            TCHAR buff[2048];
            SendMessage(hwnd,WM_GETTEXT,sizeof(buff),(LPARAM)buff);
            return buff;
        }


        BOOL CALLBACK enumWindowsChildCallback(HWND hWnd, LPARAM lParam)
        {
            std::string name = getWindowName(hWnd);
            std::string className = getClassName(hWnd);
            if (className == "Edit") {
                std::string text = getWindowText(hWnd);
                std::string caption = getWindowCaption(hWnd);
                std::vector<std::shared_ptr<ChildWinData>>& childs =
                        *reinterpret_cast< std::vector<std::shared_ptr<ChildWinData>>*>(lParam);
                auto data = std::make_shared<ChildWinData>();
                data->windowName = name;
                data->className = className;
                data->text = text;
                data->caption = caption;
                childs.push_back(data);
            }
            return TRUE;
        }

        std::vector<std::shared_ptr<ChildWinData>> getChildWindows(HWND parentWin) {
            std::vector<std::shared_ptr<ChildWinData>> childs;
            EnumChildWindows(parentWin, enumWindowsChildCallback, (LPARAM)&childs);
            return childs;
        }

        BOOL CALLBACK enumWindowsCallback(HWND hwnd, LPARAM lParam){
            const DWORD TITLE_SIZE = 1024;
            WCHAR windowTitle[TITLE_SIZE];

            GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

            int length = ::GetWindowTextLength(hwnd);
            std::wstring title(&windowTitle[0]);
            if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager") {
                return true;
            }

            std::vector<std::string>& titles = *reinterpret_cast<std::vector<std::string>*>(lParam);
            titles.push_back(std::string(title.begin(), title.end()));
            return true;
        }

        std::vector<std::string> getOpenWindows() {
            std::vector<std::string> titles;
            EnumWindows(enumWindowsCallback, reinterpret_cast<LPARAM>(&titles));
            return titles;
        }

        std::string getWindowFromList(const std::vector<std::string> &list, const std::string &like, HWND except = NULL) {
            for (const auto &item: list) {
                if (item.find(like) != std::string::npos) {
                    auto handle = findWindow(item);
                    if ((except != NULL && handle != except) || except == NULL) {
                        return item;
                    }
                }
            }
            return "";
        }

        void clickButton(HWND hwnd, const std::string &buttonClass, const std::string &buttonText) {
            auto ButtonHandle = FindWindowEx(hwnd, 0, buttonClass.c_str(), buttonText.c_str());
            PostMessage(ButtonHandle, BM_CLICK, 0, 0);
        }

        void startApp(const std::string &appPath, STARTUPINFO &si, PROCESS_INFORMATION &pi) {
            // set the size of the structures
            ZeroMemory( &si, sizeof(si) );
            si.cb = sizeof(si);
            ZeroMemory( &pi, sizeof(pi) );

            // start the program up
            CreateProcess((LPCSTR)appPath.c_str(),   // the path
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
        }
    }
}

#endif //BENCHER_HELPERS_HPP
