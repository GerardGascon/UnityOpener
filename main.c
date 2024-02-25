#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void ReadVersion(FILE *fp, char *buf);

bool DirectoryExists(const LPCTSTR szPath) {
    const DWORD dwAttrib = GetFileAttributes(szPath);

    return dwAttrib != INVALID_FILE_ATTRIBUTES &&
           dwAttrib & FILE_ATTRIBUTE_ARCHIVE;
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

int main(int argc, char **argv) {
    if(argc == 1) {
        perror("No path passed as argument");
        return 4;
    }

    char projectVersionPath[MAX_PATH];
    sprintf(projectVersionPath, "%s\\ProjectSettings\\ProjectVersion.txt", argv[1]);
    FILE *fp = fopen(projectVersionPath, "r");
    if (fp == NULL) {
        perror("Version file open failed");
        return 1;
    }

    char unityVersion[64];
    ReadVersion(fp, unityVersion);

    fclose(fp);

    const char *editorsPath = "C:\\Program Files\\Unity\\Hub\\Editor";
    char unityPath[MAX_PATH];
    sprintf(unityPath, "%s\\%s\\%s\\%s", editorsPath, unityVersion, "Editor", "Unity.exe");

    if (!DirectoryExists(unityPath)) {
        char err[128];
        sprintf(err, "Editor with version %s not found", unityVersion);
        perror(err);
        return 2;
    }

    LaunchUnity(unityPath, argv[1]);
    return 0;
}

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
