#ifndef __ENTERPOINT_H
#define __ENTERPOINT_H

void InitItunes(HMODULE hModule);

void PatchItunes(int lpAddress);
void restorePatch();
int getHookPoint1(int lpAddr);///�ڵ���CALL DWORD PTR DS:[EAX+5C]��������򲹶���λ��
int getHookPoint2(char* hModule);

#endif