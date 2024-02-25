#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "unity.h"

bool DirectoryExists(const LPCTSTR szPath) {
    const DWORD dwAttrib = GetFileAttributes(szPath);
    return dwAttrib != INVALID_FILE_ATTRIBUTES && dwAttrib & FILE_ATTRIBUTE_ARCHIVE;
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
