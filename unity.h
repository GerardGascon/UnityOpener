#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <windows.h>

void ReadVersion(FILE *fp, char *buf);
void LaunchUnity(char unityPath[MAX_PATH], char projectPath[MAX_PATH]);

#endif //UNITY_H
