#include "unity.h"

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

void ReadVersion(FILE *fp, char *buf) {
    int currentBufSize = 0;
    bool versionStarted = false;
    while (1) {
        const int c = fgetc(fp);
        if (c == '\n' || feof(fp))
            break;

        if (versionStarted)
            buf[currentBufSize++] = c;

        if (c == ' ')
            versionStarted = true;
    }
    buf[currentBufSize++] = '\0';
}

void LaunchUnity(char unityPath[MAX_PATH], char projectPath[MAX_PATH]) {
    char unityCall[MAX_PATH];
    sprintf(unityCall, "\"%s\" --projectPath \"%s\"", unityPath, projectPath);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, unityCall, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%lu).\n", GetLastError());
        exit(3);
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}