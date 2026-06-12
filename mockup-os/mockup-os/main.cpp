#include "task_manager.h"

int main() {

    //dummy data
    std::vector<Process> processes = {
        {1240,  "C+G",
         "C:\\Program Files\\WindowsApps\\Microsoft.WindowsTerminal_1.24.11321.0_x64__8wekyb3d8bbwe\\WindowsTerminal.exe",
         "N/A"},
        {2344,  "C+G",
         "D:\\Steam\\bin\\cef\\cef.win64\\steamwebhelper.exe",
         "N/A"},
        {9944,  "C+G",
         "C:\\Windows\\explorer.exe",
         "N/A"},
        {17828, "C+G",
         "C:\\Program Files\\WindowsApps\\AppleInc.iCloud_15.8.118.0_x64__nzyj5cx40ttqa\\iCloud\\iCloudHome.exe",
         "N/A"},
        {20816, "C+G",
         "C:\\Users\\User\\AppData\\Roaming\\Spotify\\Spotify.exe",
         "N/A"},
    };

    printProcesses(processes);

    return 0;
}