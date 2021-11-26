//
// Created by TieMajor on 26/11/2021.
//

#include <processthreadsapi.h>
#include <chrono>
#include <thread>
#include "FurMarkBench.hpp"
#include "helpers.hpp"

benchy::FurMarkBench::FurMarkBench() {

}

benchy::FurMarkBench::~FurMarkBench() {

}

std::string benchy::FurMarkBench::getName() {
    return "FurMark Bench";
}

std::map<std::string, std::string> benchy::FurMarkBench::doBenchmark() {
    std::map<std::string, std::string> results;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    benchy::utils::startApp(R"(C:\Program Files (x86)\Geeks3D\Benchmarks\FurMark\FurMark.exe)", si, pi);

    std::chrono::milliseconds timespan(5000);
    std::this_thread::sleep_for(timespan);

    auto windows = benchy::utils::getOpenWindows();
    auto win = benchy::utils::getWindowFromList(windows, "FurMark");
    auto winHandle = benchy::utils::findWindow(win);
    benchy::utils::getChildWindows(winHandle);
    benchy::utils::clickButton(winHandle, "Button", "Preset:1080 (FHD)");
    std::chrono::seconds waitBench(70);
    std::this_thread::sleep_for(waitBench);

    auto winResults = benchy::utils::getWindowFromList(benchy::utils::getOpenWindows(), "FurMark", winHandle);
    if (!winResults.empty()) {
        auto winResultHandle = benchy::utils::findWindow(winResults);
        auto childs = benchy::utils::getChildWindows(winResultHandle);
        for (const auto &child : childs) {
            if (child->caption.starts_with("SCORE")) {
                results["score"] = child->caption;
            }
        }
    }

    TerminateProcess(pi.hProcess, 0);

    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

    return results;
}
