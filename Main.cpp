#include <Windows.h>
#include<iostream>
#include<stdlib.h>
#include<tchar.h>
#include<intrin.h>
#include<vector>
#include<thread>
#include<algorithm>
#include<string>
#include<ctime>
#include<TlHelp32.h>
#include "Header.h"

#define m_iDefaultFOV 0x332C

void polymorphic()
{
	std::srand(std::time(0));

	int count = 0;
	for (count; count < 10; count++)
	{
		int index = rand() % (2 - 0 + 1) + 0;



		switch (index)
		{

		case 0:

			__asm __volatile
			{
				mov eax, 0
				add bp, ax
				mov eax, 0
				mov ebx, 0
				mov ax, bp
				mov bl, 7
				mul cx
				_emit 0x90


			}



		case 1:

			_asm
			{
				add al, al
				_emit 0x34
				_emit 0x35
				_emit 0x90
				_emit 0x36
				_emit 0x37
				_emit 0x90
				_emit 0x90

			}

		case 2:
			__asm __volatile
			{
			add    eax, 0;
			 dec    ecx;
			 add    eax, 0;
			 inc    ecx;
			 inc    edi;
			 _emit 0xb8
			 dec    ecx
			 inc    ecx;
			 dec    ecx;
			 inc    ecx;
			}


		}

	}
}
auto TitleGen = [](int num) {
	std::string nameoftitle;
	for (int i = 0; i < num; i++)
		nameoftitle += rand() % 255 + 1;
	return nameoftitle;
};
const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN); const int xhairx = SCREEN_WIDTH / 2;
const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN); const int xhairy = SCREEN_HEIGHT / 2;

void setcolor(unsigned short color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}
bool togglebunny = false;
bool togglewallhack = false;
bool toggletriggerbot = false;
bool toggleaimbot = false;
bool toggleradarhack = false;
bool toggleanti = false;
bool togglefov = false;

using namespace std;
vector<string> serial;
void loadserial() {
	serial.push_back("4050364792");
	serial.push_back("1548865274");
	serial.push_back("HWIDS");
}

uintptr_t modulebase;
DWORD proc;
HWND hwnd;
HANDLE hprocess;
HDC hdc;
int closest;
uintptr_t buffer;
uintptr_t GetModuleBaseAddress(const char* modName) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!strcmp(modEntry.szModule, modName)) {
					CloseHandle(hSnap);
					return (uintptr_t)modEntry.modBaseAddr;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
}
template<typename T> T RPM(SIZE_T address) {
	T buffer;
	ReadProcessMemory(hprocess, (LPCVOID)address, &buffer, sizeof(T), NULL);
	return buffer;
}
template<typename T> void WPM(SIZE_T address, T buffer) {
	WriteProcessMemory(hprocess, (LPVOID)address, &buffer, sizeof(T), NULL);
}
class Vector3 {
public:
	float x, y, z;
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};
struct glowStructEnemy {
	float red = 1.f;
	float green = 0.f;
	float blue = 0.f;
	float alpha = 1.f;
	uint8_t padding[8];
	float unknown = 1.f;
	uint8_t padding2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = false;
	BYTE fullBloom = false;
}glowEnm;
struct glowStructLocal {
	float red = 0.f;
	float green = 70.f;
	float blue = 230.f;
	float alpha = 1.f;
	uint8_t padding[8];
	float unknown = 1;
	uint8_t padding2[4];
	BYTE renderOccluded = 1;
	BYTE renderUnoccluded = 0;
	BYTE fullBloom = 0;
}glowLocal;
uintptr_t getLocalPlayer() {
	return RPM<uintptr_t>(modulebase + dwLocalPlayer);
}
uintptr_t getPlayer(int index) {
	return RPM< uintptr_t>(modulebase + dwEntityList + index * 0x10);
}
int GetPlayerHealth(uintptr_t player) {
	return RPM<int>(player + m_iHealth);
}
int getTeam(uintptr_t player) {
	return RPM<int>(player + m_iTeamNum);
}
int getCrosshairID(uintptr_t player) {
	return RPM<int>(player + m_iCrosshairId);
}
Vector3 PlayerLocation(uintptr_t player) {
	return RPM<Vector3>(player + m_vecOrigin);
}
bool DormantCheck(uintptr_t player) {
	return RPM<int>(player + m_bDormant);
}
Vector3 get_head(uintptr_t player) {
	struct boneMatrix_t {
		byte pad3[12];
		float x;
		byte pad1[12];
		float y;
		byte pad2[12];
		float z;
	};
	uintptr_t boneBase = RPM<uintptr_t>(player + m_dwBoneMatrix);
	boneMatrix_t boneMatrix = RPM<boneMatrix_t>(boneBase + (sizeof(boneMatrix) * 8 /*8 is the boneid for head*/));
	return Vector3(boneMatrix.x, boneMatrix.y, boneMatrix.z);
}
struct view_matrix_t {
	float matrix[16];
} vm;
struct Vector3 WorldToScreen(const struct Vector3 pos, struct view_matrix_t matrix) {
	struct Vector3 out;
	float _x = matrix.matrix[0] * pos.x + matrix.matrix[1] * pos.y + matrix.matrix[2] * pos.z + matrix.matrix[3];
	float _y = matrix.matrix[4] * pos.x + matrix.matrix[5] * pos.y + matrix.matrix[6] * pos.z + matrix.matrix[7];
	out.z = matrix.matrix[12] * pos.x + matrix.matrix[13] * pos.y + matrix.matrix[14] * pos.z + matrix.matrix[15];

	_x *= 1.f / out.z;
	_y *= 1.f / out.z;

	out.x = SCREEN_WIDTH * .5f;
	out.y = SCREEN_HEIGHT * .5f;

	out.x += 0.5f * _x * SCREEN_WIDTH + 0.5f;
	out.y -= 0.5f * _y * SCREEN_HEIGHT + 0.5f;

	return out;
}


float pythag(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int FindClosestEnemy() {
	float Finish;
	int ClosestEntity = 1;
	Vector3 Calc = { 0, 0, 0 };
	float Closest = FLT_MAX;
	int localTeam = getTeam(getLocalPlayer());
	for (int i = 1; i < 64; i++) {
		DWORD Entity = getPlayer(i);
		int EnmTeam = getTeam(Entity); if (EnmTeam == localTeam) continue;
		int EnmHealth = GetPlayerHealth(Entity); if (EnmHealth < 1 || EnmHealth > 100) continue;
		int Dormant = DormantCheck(Entity); if (Dormant) continue;
		Vector3 headBone = WorldToScreen(get_head(Entity), vm);
		Finish = pythag(headBone.x, headBone.y, xhairx, xhairy);
		if (Finish < Closest) {
			Closest = Finish;
			ClosestEntity = i;
		}
	}

	return ClosestEntity;
}

//void DrawLine(float StartX, float StartY, float EndX, float EndY) { 
//	int a, b = 0;
//	HPEN hOPen;
//	HPEN hNPen = CreatePen(PS_SOLID, 2, 0x0000FF /*red*/);
//	hOPen = (HPEN)SelectObject(hdc, hNPen);
//	MoveToEx(hdc, StartX, StartY, NULL); //start of line
//	a = LineTo(hdc, EndX, EndY); //end of line
//	DeleteObject(SelectObject(hdc, hOPen));
//}


void bunny() {
	while (true) {
		Sleep(5);
		if (!togglebunny)
			continue;
		uintptr_t localPlayer = RPM<uintptr_t>(modulebase + dwEntityList);
		int flags = RPM<int>(localPlayer + m_fFlags);
		if (flags & 1) {
			buffer = 5;
		}
		else {
			buffer = 4;
		}

		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			WPM(modulebase + dwForceJump, buffer);
		}
	}
}
void anti() {
	while (true) {
		Sleep(5);
		if (!toggleanti)
			continue;

		DWORD zero = GetModuleBaseAddress("client.dll");

		DWORD two = RPM<DWORD>(zero + dwLocalPlayer);
		/* If flashed, set flash to 0.4f */
		if (true)
		{
			if ((RPM<float>(two + m_flFlashMaxAlpha)) > 0.0f)
			{
				WPM<float>(two + m_flFlashMaxAlpha, 0.4f);
			}
		}
		else if (RPM<float>(two + m_flFlashMaxAlpha) == 0.0f)
		{
			WPM<float>(two + m_flFlashMaxAlpha, 255.0f);
		};
	}
}
void aim() {
	while (true) {
		if (!toggleaimbot)
			continue;
		closest = FindClosestEnemy();

		vm = RPM<view_matrix_t>(modulebase + dwViewMatrix);
		Vector3 closestw2shead = WorldToScreen(get_head(getPlayer(closest)), vm);
		//DrawLine(xhairx, xhairy, closestw2shead.x, closestw2shead.y); //optinal for debugging
		if (GetAsyncKeyState(VK_MENU /*alt key*/) && closestw2shead.z >= 0.001f /*onscreen check*/)
			SetCursorPos(closestw2shead.x, closestw2shead.y);

	}
}
void radar() {
	while (true) {
		Sleep(5);
		if (!toggleradarhack)
			continue;
		hprocess = OpenProcess(PROCESS_ALL_ACCESS, NULL, proc);
		while (true) {
			for (int i = 1; i < 64; i++) {
				DWORD dwCurrentEntity = RPM<DWORD>(modulebase + dwEntityList + i * 0x10);
				if (dwCurrentEntity) {
					WPM<bool>(dwCurrentEntity + m_bSpotted, true);
				}
			}
			Sleep(50);
		}
	}
}
void wall() {
	while (true) {
		Sleep(5);
		if (!togglewallhack)
			continue;
		uintptr_t dwGlowManager = RPM<uintptr_t>(modulebase + dwGlowObjectManager);
		int LocalTeam = RPM<int>(getLocalPlayer() + m_iTeamNum);
		for (int i = 1; i < 32; i++) {
			uintptr_t dwEntity = RPM<uintptr_t>(modulebase + dwEntityList + i * 0x10);
			int iGlowIndx = RPM<int>(dwEntity + m_iGlowIndex);
			int EnmHealth = RPM<int>(dwEntity + m_iHealth); if (EnmHealth < 1 || EnmHealth > 100) continue;
			int Dormant = RPM<int>(dwEntity + m_bDormant); if (Dormant) continue;
			int EntityTeam = RPM<int>(dwEntity + m_iTeamNum);

			if (LocalTeam == EntityTeam)
			{
				WPM<glowStructLocal>(dwGlowManager + (iGlowIndx * 0x38) + 0x4, glowLocal);
			}
			else if (LocalTeam != EntityTeam)
			{
				WPM<glowStructEnemy>(dwGlowManager + (iGlowIndx * 0x38) + 0x4, glowEnm);
			}
		}
	}
}
void trigger() {
	while (true) {
		Sleep(5);
		if (!toggletriggerbot)
			continue;
		int CrosshairID = getCrosshairID(getLocalPlayer());
		int CrosshairTeam = getTeam(getPlayer(CrosshairID - 1));
		int LocalTeam = getTeam(getLocalPlayer());
		if (CrosshairID > 0 && CrosshairID < 32 && LocalTeam != CrosshairTeam)
		{
			if (GetAsyncKeyState(VK_MENU /*alt key*/))
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
			}
		}
	}
}
int fov = 90;
void fover() {
	while (true) {
		Sleep(5);
		if (!togglefov)
			continue;
		uintptr_t localPlayer = RPM<uintptr_t>(modulebase + dwEntityList);
		int iFOV = RPM<int>(localPlayer + m_iDefaultFOV);


		if (GetAsyncKeyState(0x6D /*F7*/) & 1)
		{
			//minus
			fov = fov - 1;
			WPM<int>(localPlayer + m_iDefaultFOV, fov);
		}

		if (GetAsyncKeyState(0x6B /*F8*/) & 1)
		{
			//add
			fov = fov + 1;
			WPM<int>(localPlayer + m_iDefaultFOV, fov);
		}
		if (GetAsyncKeyState(0x23) & 1) {
			fov = 90;
			WPM<int>(localPlayer + m_iDefaultFOV, fov);

		}
	}
}
int main(int argc, char* argv[]) {
	loadserial();
	setcolor(10);
	std::cout << "Checking Poustideslist....... \n";
	Sleep(420);
	TCHAR volumeName[MAX_PATH + 1] = { 0 }; // info harddriver
	TCHAR fileSystemName[MAX_PATH + 1] = { 0 };
	DWORD serialNumber = 0; //HEHRERERERERERERERE
	DWORD maxComponentLen = 0;
	DWORD fileSystemFlags = 0;
	if (GetVolumeInformation(_T("C:\\"), volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName)));
	while (true) {
		if (find(serial.begin(), serial.end(), to_string(serialNumber)) != serial.end()) {

			std::cout << "Eisia mesa\n";
			Sleep(2000);
			system("cls");

			SetConsoleTitleA(TitleGen(rand() % 100 + 30).c_str());
			HWND hconsole = GetConsoleWindow();
			SetWindowPos(hconsole, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			std::cout << "Done Spawing Threads.....\n";
			std::cout << "Searching CSGO window....\n";
			Sleep(200);
			hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
			GetWindowThreadProcessId(hwnd, &proc);
			std::cout << "Cs:Go Process is now runing\n";
			modulebase = GetModuleBaseAddress("client.dll");
			hprocess = OpenProcess(PROCESS_ALL_ACCESS, NULL, proc);
			hdc = GetDC(hwnd);
			std::thread t1(bunny);
			std::thread t2(aim);
			std::thread t3(wall);
			std::thread t4(radar);
			std::thread t5(trigger);
			std::thread t6(anti);
			std::thread t7(fover);
			Sleep(1000);
			setcolor(5);
			std::cout << "Meraki [0%]....\n";
			Sleep(100);
			std::cout << "Ferma [25%]\n";
			Sleep(200);
			std::cout << "Psofa [50%]\n";
			Sleep(200);
			std::cout << "Mounaki [75%]\n";
			Sleep(200);
			std::cout << "MAGAS [100%]\n";
			Sleep(200);
			std::cout << "KAVLADA DONE\n";
			Sleep(500);
			system("Color 4");
			std::cout << R"(
						 .``-+-``````````````````````````````````````..oyo+++
						.```/sy-``````````````````````````..``````````..syo++
			`          .````+sys.`              `````````-/:```````````..sy++
			-         `.```.ooy+                     ```-y:+`````````````-ys+
			:-        .`````:+s`.`.......````````       +s++````````````` /ys
			::`      ```.```..`.``````````````````````  /+s-:.``````````. `sh
			:/:`     .`...```....`````````````````````.`-+--``.``````````  -h
			-//-`   ```-```````:.``````````````````````.`.``   ..`````````  o
			`///- ``-`````````-:````````````````````````.```..` ..````````  -
			 .///.``.````.```-..``````````````````````````````..`..```````   
			  :-`..-:```..``.` -`-`````-```````..``````-````.````.-.`````.   
			 `.`---.-```-``.`  -./`````:```````:-``````::```-`````..`````.   
			 -`:/.`.-```-`..`  --/````./```````/:``````-/.``-`.``````````-   
			 :.+-``..```-.. `----:.```-/`.````./:``````.-:``-..`````````..   
			 ://``...```..    .::::```-/`-````-:-..```.../-`.:.`````-```-.   
			 :+:``.-````--/o+:-::-/```--`..```/.:-:``--```:``/`````.-``.-.   
			 -/:```:```.-`.:/oys///::`-....``.+:/::`.-:   .-`:`````-..`-..   
			 `-:```:```.`     ```-.::.-`..`.`-:-://-/:/::-.-.-`````/:-...`   
			-`-..``:```.`            `` ```.```-:/++oosoo+:`--````...-``-  `:
			hs:-```:```.`             `.`   `               -.````- `.``-`:sd
			mdhy.``-```.`            `/:.                   .````..`.```/smmd
			NNmdo``:```..            `...                   .````-.`````hmmyo
			NNNNm/`-```.:            ``                    `.````-`````.yysoo
			NNNNNd--.``.+.            `                    .````..`````-ooooo
			NNNNNNh.-``.++.       `````        `..       `./````-``````-+oooo
			NNNNNNN+-...+oo:``   `o-``````````.`+o      `:/:````-`````.-+oooo
			NNNNNNNm+...+oso+-``` `.````  ```.`.-.    `.///:````-`````-`+oooo
			NNNNNNNNy.../oss+++:.`````````````     ``-/////-```..````..`ooooo
			NNNNNNNNN/..:os++++++/.``````````````.-:+/++//+.```-`````-``yhyoo
			mNNNNNNNNh..-ss--/+++++/-````````.-:+o+++++++++````-````..`.yhhhy
			dNNNNNNNNN/..oss``-+++++++--:::/++oo+++++++++oo.``..````-``.hhhhh
			dmNNNNNNNNh..+sds.``:+++++++++ooooo+++++++++od/.....```-.``:hhhhh
			dmNNNNNNNNm-.:yddy.``./++++++++++++++++++++odm-........-...+hhhhh
			Na pas na botla ekatsa mexri tis 4 na sou ta ftia3w 
			-Philiboss12342069
			)";
			Sleep(1000);
			system("cls");


			setcolor(5);

			std::cout << "__________                ___________              ____  __.           .__              .___       \n";
			std::cout << "\____    /___________  ___\__    ___/_  _  ______ |    |/ _|____ ___  _|  | _____     __| _/____   \n";
			std::cout << "  /     // __ \_  __ \/  _ \|    |  \ \/ \/ /  _ \|      < \__  \\  \/ /  | \__  \   / __ |\__  \  \n";
			std::cout << " /     /\  ___/|  | \(  <_> )    |   \     (  <_> )    |  \ / __ \\   /|  |__/ __ \_/ /_/ | / __ \_\n";
			std::cout << "/_______ \___  >__|   \____/|____|    \/\_/ \____/|____|__ (____  /\_/ |____(____  /\____ |(____  /\n";
			std::cout << "        \/   \/                                           \/    \/               \/      \/     \/ \n";

			setcolor(12);
			std::cout << "\n";
			std::cout << "Welcome to 02 Kavlada\n";
			setcolor(15);
			std::cout << "------------------------------------------------------------------------------------------------------------------------\n";
			setcolor(12);
			std::cout << "<=====POUSTIES=====>\n";
			std::cout << "  FOV CHANGER | F4\n";
			std::cout << "  ANTI FLASH: | F5\n";
			std::cout << "  TriggerBot: | F6\n";
			std::cout << "  Aimbot:     | F7\n";
			std::cout << "  Wallhack:   | F8\n";
			std::cout << "  Radar:      | F9\n";
			std::cout << "  Bunnyhop:   | F10\n";
			std::cout << "  Exit:       | F12\n";
			std::cout << "\n";
			std::cout << "|02 philiboss 02|\n";
			while (true) {
				if (GetAsyncKeyState(VK_F4)) {

					togglefov = !togglefov;
					Sleep(200);
					if (togglefov) {
						std::cout << "FOV CHANGER: NAI\n";
						std::cout << "+ & - TO CHANGE FOV\n";
					}
					else {
						std::cout << "FOV CHANGER: OXI\n";
					}
				}
				if (GetAsyncKeyState(VK_F5)) {

					toggleanti = !toggleanti;
					Sleep(200);
					if (toggleanti) {
						std::cout << "ANTI FLASH : NAI\n";
					}
					else {
						std::cout << "ANTI FLASH : OXI\n";
					}
				}
				if (GetAsyncKeyState(VK_F8)) {
					togglewallhack = !togglewallhack;
					Sleep(200);
					if (togglewallhack) {
						std::cout << "WALLHACK : NAI\n";
					}
					else {
						std::cout << "WALLHACK : OXI\n";
					}
				}
				if (GetAsyncKeyState(VK_F6)) {
					toggletriggerbot = !toggletriggerbot;
					Sleep(200);
					if (toggletriggerbot) {
						std::cout << "TRIGGERBOT : NAI\n";
					}
					else {

						std::cout << "TRIGGERBOT : OXI\n";
					}

				}

				if (GetAsyncKeyState(VK_F7)) {
					toggleaimbot = !toggleaimbot;
					Sleep(200);
					if (toggleaimbot) {
						std::cout << "AIMBOT : NAI\n";

					}
					else {
						std::cout << "AIMBOT : OXI\n";
					}
				}
				if (GetAsyncKeyState(VK_F9)) {

					toggleradarhack = !toggleradarhack;
					Sleep(200);
					if (toggleradarhack) {
						std::cout << "RADAR : NAI\n";
					}
					else {
						std::cout << "RADAR : OXI\n";
					}
				}
				if (GetAsyncKeyState(VK_F10)) {

					togglebunny = !togglebunny;
					Sleep(200);
					if (togglebunny) {
						std::cout << "BUNNYHOP : NAI\n";
					}
					else {
						std::cout << "BUNNYHOP : OXI\n";
					}
				}
				if (GetAsyncKeyState(VK_F12)) {
					std::cout << "Closing...\n";
					Sleep(200);
					system("Color 4");
					std::cout << R"(
						 .``-+-``````````````````````````````````````..oyo+++
						.```/sy-``````````````````````````..``````````..syo++
			`          .````+sys.`              `````````-/:```````````..sy++
			-         `.```.ooy+                     ```-y:+`````````````-ys+
			:-        .`````:+s`.`.......````````       +s++````````````` /ys
			::`      ```.```..`.``````````````````````  /+s-:.``````````. `sh
			:/:`     .`...```....`````````````````````.`-+--``.``````````  -h
			-//-`   ```-```````:.``````````````````````.`.``   ..`````````  o
			`///- ``-`````````-:````````````````````````.```..` ..````````  -
			 .///.``.````.```-..``````````````````````````````..`..```````   
			  :-`..-:```..``.` -`-`````-```````..``````-````.````.-.`````.   
			 `.`---.-```-``.`  -./`````:```````:-``````::```-`````..`````.   
			 -`:/.`.-```-`..`  --/````./```````/:``````-/.``-`.``````````-   
			 :.+-``..```-.. `----:.```-/`.````./:``````.-:``-..`````````..   
			 ://``...```..    .::::```-/`-````-:-..```.../-`.:.`````-```-.   
			 :+:``.-````--/o+:-::-/```--`..```/.:-:``--```:``/`````.-``.-.   
			 -/:```:```.-`.:/oys///::`-....``.+:/::`.-:   .-`:`````-..`-..   
			 `-:```:```.`     ```-.::.-`..`.`-:-://-/:/::-.-.-`````/:-...`   
			-`-..``:```.`            `` ```.```-:/++oosoo+:`--````...-``-  `:
			hs:-```:```.`             `.`   `               -.````- `.``-`:sd
			mdhy.``-```.`            `/:.                   .````..`.```/smmd
			NNmdo``:```..            `...                   .````-.`````hmmyo
			NNNNm/`-```.:            ``                    `.````-`````.yysoo
			NNNNNd--.``.+.            `                    .````..`````-ooooo
			NNNNNNh.-``.++.       `````        `..       `./````-``````-+oooo
			NNNNNNN+-...+oo:``   `o-``````````.`+o      `:/:````-`````.-+oooo
			NNNNNNNm+...+oso+-``` `.````  ```.`.-.    `.///:````-`````-`+oooo
			NNNNNNNNy.../oss+++:.`````````````     ``-/////-```..````..`ooooo
			NNNNNNNNN/..:os++++++/.``````````````.-:+/++//+.```-`````-``yhyoo
			mNNNNNNNNh..-ss--/+++++/-````````.-:+o+++++++++````-````..`.yhhhy
			dNNNNNNNNN/..oss``-+++++++--:::/++oo+++++++++oo.``..````-``.hhhhh
			dmNNNNNNNNh..+sds.``:+++++++++ooooo+++++++++od/.....```-.``:hhhhh
			dmNNNNNNNNm-.:yddy.``./++++++++++++++++++++odm-........-...+hhhhh
			Na pas na gamithis ekatsa mexri tis 4 na sou ta ftia3w 
			-Philiboss12342069
			)";
					Sleep(300);
					system("cls");
					exit(0);
					system("cls");
				}
			}
			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
		}
		else {
			setcolor(12);
			std::cout << "Den eisai sto poustides list ::>_<::\n";
			std::cout << " Your serial is:";
			std::cout << serialNumber << std::endl;
			Sleep(15000);
			exit(0);
		}
	}

}
























#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class mdkuycg {
public:
	bool cuifzdnzssfuyu;
	bool yvcfglwyzljt;
	int mqovqkcagtvxyn;
	bool ztbyprsvrlnmqv;
	double endgqenjkwaoo;
	mdkuycg();
	string mlnptdyyzmaafxgwodlfqilj(bool ueohhyj, double ohvvrfnnkkrueeo, int rvqklynxqezcdij, double olrlsahtqcslf, double irxvntsagxpyh, bool vqkbwfotagugbq, double ipugcr, double sbvfztncsa, int esjyxbvdvcrf);
	string njpgnjeoamiofuzqomrnfa(bool zwaitx, int nwrqjdvnyg, double azfyms, bool zepbdzzdinuk, double xszjm, int tmjmvspledi, string qfsaxpuf);
	string xzmhzethjxixq(string xaqhhwkhxry, int apqvkbuprrddc, string yupuqugiydkxxzj, int ouoxcegf, string dpxqzgdhqp, int wdqjhlydqtakuf, string xndwrlqv, bool ymteebg, double nqzseykldslnf);
	bool fhogaqcaiotnjyqcjadonvfc(string fponnxtsdeluu, int hhqywbkjvizg, bool oyatkivdl, string tcwduvsw, int gdtejsgosyyrpr, string hdsog, bool exhzryvekkbpr);
	int futsiwbtcvusqkqmtfiztv(double qdeqtls, double chjxsajaah, string gomgfuyjghhtycp, string kvmkgceokh, string wzocpwamytfvn);

protected:
	bool xhwzckk;

	double ayszrpuscspa(bool lfadradpdqegst, bool eovmqi, double edcyfgdggwjve, int kaamlruiqvacqwt);
	int wwobihwvjfg();
	bool plajtvffykklpuhvsiqnk(double wvuhbwr, bool glfopfer, string njsroxzyhedp);
	bool wbbhgzuetvlcshdcgpnvioimo(string smkxpoglpvp, double zthbcqbx, string fjnjkpfhdpnjp);
	string etwekrfmzzwnxyyhjfmot(string qqxkuympfe, double dqwznebw, string cptohxwjlze, bool jtozkftzydk, double rvxyuyqdgrnfd, bool bzamywfmwvdeqy, double uttgyuehgmtakzh, bool ovsejrumxv, double nziqmzmjw);
	int vketcdhyvkvlh(string ppjdpz, string fpwwk, bool plsxglk, string zpozazcoxbbvtsj, bool ymgeqcj, double qxqhreo, bool qiqdkqpt, bool tvltvx);
	void ryuaticownvnwdnajjzkjkkps(int wyajq, int evgokcs, bool enspbrsraaf, bool zytrovhmsy, bool tmuzloavs, bool xouqvz, double wfixkfwifm, double ziipwv);
	void umyrlrkixbyfjhisuo(string slmgvdpftuhtm, double frhxqpsbs, string oaamet);
	string wbiqbyjkkpenxibuqlkrhgpn(double gvcinhmv, double rroceluispjmvv, int xiljrzhxck);

private:
	int qguvihcjeodwlh;
	bool wnzlwnn;
	int qkfnrhuc;
	double snhrjpqqnbbec;
	double mqmjm;

	int pdvihwqoiluutnzy();
	void kavqeyqoyxy(int lvlmborljz, bool hvwjc, string vcsmgqavcnc, bool lbgvrhv, int rtjpfsa, int qthmhmygllneqd, string srivhntcyusjz, int mtkyldsxdthnmt, int nmvegsmfv, double maokm);
	double ufpwinvioqqih(bool nwxannicalregau, string ldvjwyqyrvyf);

};


int mdkuycg::pdvihwqoiluutnzy() {
	string uldqjurvj = "lavorhwyocffzcgnylxptsnujgvqgcvityjwsfsiifygjrjeaktsqaeafcxvugnbzllv";
	bool hilkeyytrddrktd = true;
	string akmtxcply = "auzulollsoxhlhigbwrzfquitxcdokscqgpeoasgdlraueinxmkqkbmbrsnnnufutefygwhhccrxjqexueomhrqk";
	int czvvuca = 2434;
	bool urcvyugdxwu = true;
	int gbnwbhtdwchs = 2346;
	if (string("auzulollsoxhlhigbwrzfquitxcdokscqgpeoasgdlraueinxmkqkbmbrsnnnufutefygwhhccrxjqexueomhrqk") != string("auzulollsoxhlhigbwrzfquitxcdokscqgpeoasgdlraueinxmkqkbmbrsnnnufutefygwhhccrxjqexueomhrqk")) {
		int znfntkze;
		for (znfntkze = 31; znfntkze > 0; znfntkze--) {
			continue;
		}
	}
	return 67516;
}

void mdkuycg::kavqeyqoyxy(int lvlmborljz, bool hvwjc, string vcsmgqavcnc, bool lbgvrhv, int rtjpfsa, int qthmhmygllneqd, string srivhntcyusjz, int mtkyldsxdthnmt, int nmvegsmfv, double maokm) {
	int vcmud = 2834;
	if (2834 == 2834) {
		int ag;
		for (ag = 45; ag > 0; ag--) {
			continue;
		}
	}
	if (2834 == 2834) {
		int eepkk;
		for (eepkk = 98; eepkk > 0; eepkk--) {
			continue;
		}
	}

}

double mdkuycg::ufpwinvioqqih(bool nwxannicalregau, string ldvjwyqyrvyf) {
	int krqjrdxdhve = 6378;
	double xhpethbbxqreb = 24124;
	bool oajyhtd = true;
	double dwowmb = 24303;
	double ppdvvj = 81061;
	if (true == true) {
		int oytbntsaa;
		for (oytbntsaa = 2; oytbntsaa > 0; oytbntsaa--) {
			continue;
		}
	}
	return 93895;
}

double mdkuycg::ayszrpuscspa(bool lfadradpdqegst, bool eovmqi, double edcyfgdggwjve, int kaamlruiqvacqwt) {
	bool zkohkhltrnxxjdt = false;
	if (false == false) {
		int uvhxhtvngb;
		for (uvhxhtvngb = 47; uvhxhtvngb > 0; uvhxhtvngb--) {
			continue;
		}
	}
	if (false == false) {
		int fmwxrlwmbo;
		for (fmwxrlwmbo = 54; fmwxrlwmbo > 0; fmwxrlwmbo--) {
			continue;
		}
	}
	if (false == false) {
		int gglplnbwt;
		for (gglplnbwt = 19; gglplnbwt > 0; gglplnbwt--) {
			continue;
		}
	}
	if (false == false) {
		int uqqorbm;
		for (uqqorbm = 45; uqqorbm > 0; uqqorbm--) {
			continue;
		}
	}
	if (false != false) {
		int dbhbbur;
		for (dbhbbur = 69; dbhbbur > 0; dbhbbur--) {
			continue;
		}
	}
	return 12124;
}

int mdkuycg::wwobihwvjfg() {
	string urovgeiaeqp = "gmyduhvuxqqi";
	double mgvvgehkethm = 18774;
	bool fweoob = false;
	double oszqhyirocytfcs = 15542;
	string mxhktrtb = "srynqicnhxvvhdasainu";
	string txfhstnzwtb = "gcrkeqqstcnprixvtkncmpttcdvreufjwmdvwvbsfclxsfnnywirqcwkwublbezrmazffwlrogrkafkufbfzwneriysvevdz";
	int urnicm = 891;
	string shmxy = "vuuubghimhapgarwwhhdszcdoahdegjdhwlluidtauncxdjgootpgnsuhdfncsywmbuawvkxoodflfjvqpgzrseknuyszzmhu";
	if (string("gcrkeqqstcnprixvtkncmpttcdvreufjwmdvwvbsfclxsfnnywirqcwkwublbezrmazffwlrogrkafkufbfzwneriysvevdz") != string("gcrkeqqstcnprixvtkncmpttcdvreufjwmdvwvbsfclxsfnnywirqcwkwublbezrmazffwlrogrkafkufbfzwneriysvevdz")) {
		int gbjwrovhb;
		for (gbjwrovhb = 28; gbjwrovhb > 0; gbjwrovhb--) {
			continue;
		}
	}
	if (string("vuuubghimhapgarwwhhdszcdoahdegjdhwlluidtauncxdjgootpgnsuhdfncsywmbuawvkxoodflfjvqpgzrseknuyszzmhu") == string("vuuubghimhapgarwwhhdszcdoahdegjdhwlluidtauncxdjgootpgnsuhdfncsywmbuawvkxoodflfjvqpgzrseknuyszzmhu")) {
		int zczg;
		for (zczg = 58; zczg > 0; zczg--) {
			continue;
		}
	}
	return 8106;
}

bool mdkuycg::plajtvffykklpuhvsiqnk(double wvuhbwr, bool glfopfer, string njsroxzyhedp) {
	bool yxnybyfc = false;
	bool atcjfzbw = true;
	double yjekpszycxqhjpz = 34094;
	bool hkahugr = false;
	bool jejpmdlzihrbzz = false;
	double ggdqcusrnpldzqn = 17895;
	bool hqgqmeuyxq = false;
	bool znrcnllxmsiklw = false;
	int yrybwsw = 758;
	if (34094 != 34094) {
		int br;
		for (br = 39; br > 0; br--) {
			continue;
		}
	}
	if (34094 == 34094) {
		int zohsio;
		for (zohsio = 22; zohsio > 0; zohsio--) {
			continue;
		}
	}
	if (false != false) {
		int wor;
		for (wor = 76; wor > 0; wor--) {
			continue;
		}
	}
	return false;
}

bool mdkuycg::wbbhgzuetvlcshdcgpnvioimo(string smkxpoglpvp, double zthbcqbx, string fjnjkpfhdpnjp) {
	bool cbmta = false;
	string sqdmiaxseah = "pessklldfyiwikyduwxaxzyrcdzsmzmrtpenoiowzrgxpcjdyzmmfjvzfenttikortudhwjybztwthcuol";
	bool bztjzrwmd = true;
	string ydytaub = "qdkvlwrngmgtcpivwiihrguxcnc";
	bool rhorntdwzwa = true;
	bool xshmvgqyqr = true;
	bool dglalosd = true;
	bool lidirodmv = true;
	bool etzzqqryvlbvim = true;
	return true;
}

string mdkuycg::etwekrfmzzwnxyyhjfmot(string qqxkuympfe, double dqwznebw, string cptohxwjlze, bool jtozkftzydk, double rvxyuyqdgrnfd, bool bzamywfmwvdeqy, double uttgyuehgmtakzh, bool ovsejrumxv, double nziqmzmjw) {
	int zxplrorjuuwjq = 2424;
	bool wynmnlmezfdbmk = false;
	double tvwnuu = 7912;
	string kkhmqxkloij = "uwohtsqdwanpysopqypevubcngeqgbocftjiakraaggrkeomommetlxyxrecgjlxipoekhlgthrqpbs";
	return string("xyuauvfnkygjjudh");
}

int mdkuycg::vketcdhyvkvlh(string ppjdpz, string fpwwk, bool plsxglk, string zpozazcoxbbvtsj, bool ymgeqcj, double qxqhreo, bool qiqdkqpt, bool tvltvx) {
	double pzggnynljioa = 6764;
	string umomrrgkmw = "neaqwhgfwtrwkngcnbyzdpbodromcpt";
	double nbtbxsbkguizpg = 33264;
	double zhxjfrnpkiczaz = 460;
	int btatexuxye = 2856;
	bool bewjteav = false;
	bool nqrddjkmjukb = false;
	string ywxjqjhtccoln = "ijqkeaebaaepw";
	bool uvsdxaa = false;
	if (string("neaqwhgfwtrwkngcnbyzdpbodromcpt") == string("neaqwhgfwtrwkngcnbyzdpbodromcpt")) {
		int pidlfqlcje;
		for (pidlfqlcje = 61; pidlfqlcje > 0; pidlfqlcje--) {
			continue;
		}
	}
	if (2856 == 2856) {
		int focbckk;
		for (focbckk = 50; focbckk > 0; focbckk--) {
			continue;
		}
	}
	return 69899;
}

void mdkuycg::ryuaticownvnwdnajjzkjkkps(int wyajq, int evgokcs, bool enspbrsraaf, bool zytrovhmsy, bool tmuzloavs, bool xouqvz, double wfixkfwifm, double ziipwv) {

}

void mdkuycg::umyrlrkixbyfjhisuo(string slmgvdpftuhtm, double frhxqpsbs, string oaamet) {
	bool dalxcqjc = true;
	double gwsamrwbqaezqxn = 76481;
	string rsbrbyjxyjqw = "pxilqzhncllduiloyreisocefqkkzwyzyyqcmnipxfvuwbktnrssccp";
	string fknptoxlnmtcs = "rqahxxqouutredrdjfetutpfrjmvqkfoghgxqbntppqujosl";
	int lffevjfs = 7356;
	bool ytknhpfrsrwnfr = true;
	bool mycxvhygvewo = true;
	string vxcogdraxirk = "sgsccxkxzhkaqcswqrhkgdpdwmuhlsxztymsqshfkynecljt";
	string umlrn = "anzecfaxifpsgqnzbhkgixxisjfgmjvcnqu";
	if (true != true) {
		int acehyamphz;
		for (acehyamphz = 90; acehyamphz > 0; acehyamphz--) {
			continue;
		}
	}
	if (true == true) {
		int iemfyia;
		for (iemfyia = 86; iemfyia > 0; iemfyia--) {
			continue;
		}
	}
	if (76481 == 76481) {
		int jrnqr;
		for (jrnqr = 87; jrnqr > 0; jrnqr--) {
			continue;
		}
	}
	if (string("sgsccxkxzhkaqcswqrhkgdpdwmuhlsxztymsqshfkynecljt") == string("sgsccxkxzhkaqcswqrhkgdpdwmuhlsxztymsqshfkynecljt")) {
		int fq;
		for (fq = 34; fq > 0; fq--) {
			continue;
		}
	}

}

string mdkuycg::wbiqbyjkkpenxibuqlkrhgpn(double gvcinhmv, double rroceluispjmvv, int xiljrzhxck) {
	bool dbbasxjnxlf = false;
	double jhzuqilnjyell = 23042;
	bool vnbnqfwoom = true;
	double gcclhbtqrwyb = 48464;
	bool nikhbthooaxkg = true;
	return string("kisnkhm");
}

string mdkuycg::mlnptdyyzmaafxgwodlfqilj(bool ueohhyj, double ohvvrfnnkkrueeo, int rvqklynxqezcdij, double olrlsahtqcslf, double irxvntsagxpyh, bool vqkbwfotagugbq, double ipugcr, double sbvfztncsa, int esjyxbvdvcrf) {
	double geeiumd = 40586;
	double zkdhzialxgtk = 47588;
	string hbhqptgltzpsfzk = "sgdqbrhbmerjdagstdabkgwdxjiwegmzawihizvqfbolzngedogpiecxegjgyanqaekgrjveqiqftnvngumhnrfjhayp";
	string lfudu = "u";
	string udtpj = "mjqsfngwkdzvexfklhfefidqgxgsnubqjnkhtdxxowopcsgtbcxasrbnzuhvttfwmfmahdchknhvyfnwmsheadnelpyysoiqbq";
	int qhbaftxcx = 392;
	if (string("u") == string("u")) {
		int ol;
		for (ol = 57; ol > 0; ol--) {
			continue;
		}
	}
	if (string("u") == string("u")) {
		int dnf;
		for (dnf = 33; dnf > 0; dnf--) {
			continue;
		}
	}
	if (string("u") != string("u")) {
		int euzw;
		for (euzw = 58; euzw > 0; euzw--) {
			continue;
		}
	}
	if (47588 != 47588) {
		int zw;
		for (zw = 15; zw > 0; zw--) {
			continue;
		}
	}
	if (string("mjqsfngwkdzvexfklhfefidqgxgsnubqjnkhtdxxowopcsgtbcxasrbnzuhvttfwmfmahdchknhvyfnwmsheadnelpyysoiqbq") == string("mjqsfngwkdzvexfklhfefidqgxgsnubqjnkhtdxxowopcsgtbcxasrbnzuhvttfwmfmahdchknhvyfnwmsheadnelpyysoiqbq")) {
		int ty;
		for (ty = 79; ty > 0; ty--) {
			continue;
		}
	}
	return string("fiharscmwrzcijoejgu");
}

string mdkuycg::njpgnjeoamiofuzqomrnfa(bool zwaitx, int nwrqjdvnyg, double azfyms, bool zepbdzzdinuk, double xszjm, int tmjmvspledi, string qfsaxpuf) {
	string ltlfrv = "mcspuhlpviuhjgkxxatsrhxwsxfdqrdyypkxcbzjqelyipfjdnn";
	return string("lpglzfqhufor");
}

string mdkuycg::xzmhzethjxixq(string xaqhhwkhxry, int apqvkbuprrddc, string yupuqugiydkxxzj, int ouoxcegf, string dpxqzgdhqp, int wdqjhlydqtakuf, string xndwrlqv, bool ymteebg, double nqzseykldslnf) {
	int muiirmmbs = 3795;
	int pyhaoiz = 5401;
	bool exdubcrlf = true;
	string rqwyfpbzwb = "flaryontmaesddobicsfdclebdugeefqvckiizmbbngsbuxpvwgkizlujnif";
	double dtinfitfns = 12601;
	string ddsags = "fmumzezjtkljouccrdnsgbxhznoolbsroimcfnfgiznnvkulfvkvtjwukedxjkvogrzrbojgpxigvsgw";
	bool sbvsvsghcex = false;
	string meyel = "yadjsastqrtjwjcegdlshrbsfxrlkkazqmpgzrymhfqknvenwdowxuyppthrelqrlbnytxeuxxjwzuytbdwfwfsmnmugtf";
	if (true != true) {
		int dske;
		for (dske = 3; dske > 0; dske--) {
			continue;
		}
	}
	if (string("flaryontmaesddobicsfdclebdugeefqvckiizmbbngsbuxpvwgkizlujnif") == string("flaryontmaesddobicsfdclebdugeefqvckiizmbbngsbuxpvwgkizlujnif")) {
		int uranqte;
		for (uranqte = 92; uranqte > 0; uranqte--) {
			continue;
		}
	}
	if (string("fmumzezjtkljouccrdnsgbxhznoolbsroimcfnfgiznnvkulfvkvtjwukedxjkvogrzrbojgpxigvsgw") != string("fmumzezjtkljouccrdnsgbxhznoolbsroimcfnfgiznnvkulfvkvtjwukedxjkvogrzrbojgpxigvsgw")) {
		int ufhlrdtx;
		for (ufhlrdtx = 56; ufhlrdtx > 0; ufhlrdtx--) {
			continue;
		}
	}
	if (12601 != 12601) {
		int wreyg;
		for (wreyg = 91; wreyg > 0; wreyg--) {
			continue;
		}
	}
	return string("xgrvmfs");
}

bool mdkuycg::fhogaqcaiotnjyqcjadonvfc(string fponnxtsdeluu, int hhqywbkjvizg, bool oyatkivdl, string tcwduvsw, int gdtejsgosyyrpr, string hdsog, bool exhzryvekkbpr) {
	string andyhvlnphlofdj = "msguksgdsnszhtepcjterbzyjddslhszuqoberwsmwhzupnslyvnkmwmxhosdadfpclbfhquixdhzqwicxirdeytuhvy";
	double wwqeomeuzoks = 14512;
	double rwzppxjovin = 36196;
	int mhhuqssefmkz = 6620;
	string kbkgcbfk = "iqbyddsvpfmwwbylghpmqackyeiwsxxkosfurhecvfuxaviuufieacslezgmzjdznyqbjixadxpizyqzihbulrydvj";
	bool wlwxqiyntrxdzww = true;
	double zuutq = 61863;
	int fdwgorigxjgjtb = 7507;
	int bnmhp = 5609;
	double ilwyeoejhddcpc = 9133;
	if (61863 != 61863) {
		int vrdfaauxf;
		for (vrdfaauxf = 42; vrdfaauxf > 0; vrdfaauxf--) {
			continue;
		}
	}
	if (61863 != 61863) {
		int rzksb;
		for (rzksb = 99; rzksb > 0; rzksb--) {
			continue;
		}
	}
	if (5609 == 5609) {
		int qk;
		for (qk = 18; qk > 0; qk--) {
			continue;
		}
	}
	if (36196 != 36196) {
		int shjbwxp;
		for (shjbwxp = 90; shjbwxp > 0; shjbwxp--) {
			continue;
		}
	}
	return false;
}

int mdkuycg::futsiwbtcvusqkqmtfiztv(double qdeqtls, double chjxsajaah, string gomgfuyjghhtycp, string kvmkgceokh, string wzocpwamytfvn) {
	double zzdoz = 30641;
	int hneqdy = 4725;
	double chcey = 9713;
	bool ravtavfxrd = true;
	if (true != true) {
		int swzkriyoju;
		for (swzkriyoju = 24; swzkriyoju > 0; swzkriyoju--) {
			continue;
		}
	}
	if (4725 != 4725) {
		int rvhtuqic;
		for (rvhtuqic = 65; rvhtuqic > 0; rvhtuqic--) {
			continue;
		}
	}
	if (true != true) {
		int dq;
		for (dq = 39; dq > 0; dq--) {
			continue;
		}
	}
	if (9713 != 9713) {
		int ysxjvwfxhm;
		for (ysxjvwfxhm = 11; ysxjvwfxhm > 0; ysxjvwfxhm--) {
			continue;
		}
	}
	if (true == true) {
		int icgo;
		for (icgo = 14; icgo > 0; icgo--) {
			continue;
		}
	}
	return 23617;
}

mdkuycg::mdkuycg() {
	this->mlnptdyyzmaafxgwodlfqilj(true, 16326, 6275, 13263, 51236, true, 43640, 40825, 893);
	this->njpgnjeoamiofuzqomrnfa(false, 861, 43905, false, 35779, 1040, string("onvlqupalaadrbdfneviqbcskybkzqdgruuvzbopkdooucqyxyiakdmctfpnt"));
	this->xzmhzethjxixq(string("qtpevsaltvceozkqsosdpgpwmwlqrnskksjbwzfxknrxfkifyeappztsv"), 4363, string("pi"), 2114, string("dpuxftnwvhkfdeqwwscdhueezxoumkviakubxtwqpjvskmnofhslbbwkyxvqtzlunhvpyvclfgnxtwpouobzwqmmeebt"), 3069, string("vfpxjbbalpyasnrelnzlpnkbomhldgjhyevfovonnbgavztjueadelsisdfkqxgzxuxpovyv"), true, 6939);
	this->fhogaqcaiotnjyqcjadonvfc(string("reiaflmrqjayrlbhupzfzgstdkovepwztfuhotigyigzviqfwliuedsbqxrg"), 2478, false, string("ixjqnletmlbppdrhglxwirqubrydmz"), 2955, string("cnwblkphhpquathinwkzbaqocpvvulnc"), true);
	this->futsiwbtcvusqkqmtfiztv(3672, 58602, string("bpufzdgdqdxwueouxwhaowosc"), string("eefxsomzcpalphxjcgsrklruumsguvswafm"), string("smuzgvfqrssgdkyivwozutkcpzgmbepmoachgfejwjeb"));
	this->ayszrpuscspa(false, false, 8975, 6240);
	this->wwobihwvjfg();
	this->plajtvffykklpuhvsiqnk(8916, false, string("tlyufxohuytotthxqhyirueuleputcyveiozxfviijyigfmxouybtycavy"));
	this->wbbhgzuetvlcshdcgpnvioimo(string("bdblbipxcuzbzjsjgjrktjnphnlybpiy"), 1640, string("vdwctdbzsfoyhanziqpunamckppkzjidpprwuxeslurljxsishavzexvzgpunelsbtalzuxezmruvypjyioazfzhhn"));
	this->etwekrfmzzwnxyyhjfmot(string("bikddajrouxjnmvrzgkzpludjg"), 12615, string("yjbcjxwdnttcexmnraadychjczhrhothgdulldotgoshtcrgklyekrnevevznqnwkshirdq"), true, 23119, true, 18310, false, 3950);
	this->vketcdhyvkvlh(string("pqywcsrdblezlnbwizytfalekealoqw"), string("gatghagwwqhdbwqofnztcpevkuqtjnzwoujwpgbslfjzedmshaowkkutmvuristvrbsrazqdmlnvarxfpsptmxelbsw"), true, string("blbugowosegdqynvrbhrdqbxudrnqxrznaoipbgemnznsvececkxzjnxo"), false, 694, true, false);
	this->ryuaticownvnwdnajjzkjkkps(59, 2060, true, true, true, false, 21994, 23171);
	this->umyrlrkixbyfjhisuo(string("ijpzcyqhubfcwhaustkdpwqnfftuhgbtmbfauoaxlrmzpecywbwubmyeuipzvwotzucmflwehvtutmntsirbcp"), 23296, string("tbihvgfolrbxxlmayomkkigzsjknochgjjdazgwxlota"));
	this->wbiqbyjkkpenxibuqlkrhgpn(7025, 52026, 2673);
	this->pdvihwqoiluutnzy();
	this->kavqeyqoyxy(3572, true, string("mhrpirbrgaasbmmgryufpkzytuj"), true, 3074, 4871, string("ckmmfsiioziyysqscfocgqbtolmqcizkzpcxxsksxbjkuqlnpwoqnwgjteqwzvmkbyhjiesyulbgqiwjhpblhb"), 5004, 6294, 17398);
	this->ufpwinvioqqih(false, string("yjjnebqigctedgrcknnqjqfzxaylnrkqbgarapfzlhfevjmnswjxtptufkutw"));
}






























































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class qqupxbv {
public:
	string yojaswdhwsk;
	int prlxyt;
	int qgjcafzlvcz;
	double vdnbooa;
	double dqesyteuo;
	qqupxbv();
	int unhixzrcwalovxk(int vkplqhw, double efucrcnskydpgtt, double bwwcadpdsfocmsq, bool etgjz, string fdimequfqmehegf, int vrmforltfzam);
	bool dglfdzaoqciqbulm(int yusybpqmuvx);
	int ziovjtmdtccpb(bool ofsaojbcckxrf, string kqgxywzm, string siyyeimfmhyzldk, int iitps, string fqlzoqy, string lyaoahnwloy);
	int kkwoostyouuynhfqp(bool boafc, int ufzpuwcvtiogl, double odsyomkuxppzi, string hscqpx);
	string rrufzyqqooe(string yiyvunz, int bcxnqdlxon, double kxtgthcjevwvjsy, int zrvpugmldzd, bool nrnkkulwmatgo, bool dvibekvnflvhy, string hjcbgybljhltvks, string tchzkttsgywcoa, string kbeovryt, string khsgbmollxz);

protected:
	bool vptoepy;
	bool sdskb;

	bool dfiaoiipitdnopprtwvakrom(bool drqchyenh, int fqdljkxpwmqph, int pffxbhppmfmfy, bool zamuz, bool yzdqygtakrczk, string qlwnsblv, int xsmrtdjcdtt, string rqcmomolzmzuaa, bool kztplolhqrwlq, bool ngmlkbsttfvuyyh);
	int nzqzkdcuvxnuwsqyzgztmdy(double natnbhm, int sjgllccwejo, string khicfravtzi, double bwlktmodagdredz, int avsmlimpdklptxo, int qbtpzuttdjbp, string twapmtfwr, bool pbptoetwawnqar, double wtjpyiqagvkjdzf);
	double mhophbuqzexvt(bool qmanjmdcc);
	void ngolfpjcztwwvndbbysstnz(string dgnpsx, int ddlfp);
	int prqqcpcrekzyoeskauor(bool gmtie, int liemcjoadzvfbuj, string pqnoosdqqrnx, int izwslquqvzlgis);

private:
	bool lzyosjxqtrqno;
	double pjrxijcxmjvtw;
	double seqtegyez;

	void ntcyntofcnz(int vakjsqiothjl, string scliljwta, bool gxznocf, string fxndfkrdfsi, int mhoxadgbffxaegz, string dkxdyciodwweel, int tsbaftyoif, string qjaia);
	double epondntekfrrk(bool cnelcblixphyvti, bool pozvztixipay, string egeemkctjfeb, string ddemjuy, bool kekfvh, double pphqihqnt, bool zjnjgds);
	bool bbbiexbcailulsbluneccoqof(bool pfqhtzmd, int pzleghno, bool ryloefxa, bool wyblkgbtyusqul, double cnfowlou, int ejixsurfplbit, string alrbxot, int vyzngmnqbvqrnhw, double npgqztv);

};


void qqupxbv::ntcyntofcnz(int vakjsqiothjl, string scliljwta, bool gxznocf, string fxndfkrdfsi, int mhoxadgbffxaegz, string dkxdyciodwweel, int tsbaftyoif, string qjaia) {
	string jkkucoasf = "wpsk";

}

double qqupxbv::epondntekfrrk(bool cnelcblixphyvti, bool pozvztixipay, string egeemkctjfeb, string ddemjuy, bool kekfvh, double pphqihqnt, bool zjnjgds) {
	bool tzotaayrsi = true;
	bool cxapvdxpotp = true;
	string hyidjkhxeelst = "cdvmb";
	int jdjvncwfodc = 5331;
	double xhfymqz = 40826;
	int zaxnk = 271;
	string qarbeuswpuvk = "safgkmuimpgencjhtwqshfloghihewsozmeqorztyobtgwwylrbyfismsmlnnestgvpfjpiqtilfqnakvjsotgkuktwikm";
	string saeiqj = "crgtmzpadqbdpmizkvxofyyuiypclxdmqbnjlyhqlcdzgcfeayajpgcgylpghuiaaccsoteazlhzeznleqtplkraeptyp";
	string rsdoiuhfnkcm = "uujhsmvxpkdpsyjuxdnaijzsnopmzwwtgjecdoothcgd";
	bool vnkogeoldmnp = true;
	if (string("cdvmb") == string("cdvmb")) {
		int ahfewkydaf;
		for (ahfewkydaf = 55; ahfewkydaf > 0; ahfewkydaf--) {
			continue;
		}
	}
	if (string("uujhsmvxpkdpsyjuxdnaijzsnopmzwwtgjecdoothcgd") != string("uujhsmvxpkdpsyjuxdnaijzsnopmzwwtgjecdoothcgd")) {
		int tbiq;
		for (tbiq = 6; tbiq > 0; tbiq--) {
			continue;
		}
	}
	return 15799;
}

bool qqupxbv::bbbiexbcailulsbluneccoqof(bool pfqhtzmd, int pzleghno, bool ryloefxa, bool wyblkgbtyusqul, double cnfowlou, int ejixsurfplbit, string alrbxot, int vyzngmnqbvqrnhw, double npgqztv) {
	bool xybwmphboaxzhi = true;
	int bepdpxjodhpkkj = 2543;
	string gumcgzbfm = "qidumknxhvpsfpptqzja";
	bool xirckxemcahx = false;
	string gnmzmyu = "uecoknozbunnnbmloxqwhvepkkseniqqowfzcgkrntaxnigonsutdjlopmhjkonhzkhncewbmjgfwwofxayliqscpniccfk";
	if (string("qidumknxhvpsfpptqzja") != string("qidumknxhvpsfpptqzja")) {
		int zgcncrd;
		for (zgcncrd = 11; zgcncrd > 0; zgcncrd--) {
			continue;
		}
	}
	if (true != true) {
		int uiuzm;
		for (uiuzm = 62; uiuzm > 0; uiuzm--) {
			continue;
		}
	}
	if (false != false) {
		int txtsk;
		for (txtsk = 2; txtsk > 0; txtsk--) {
			continue;
		}
	}
	if (string("uecoknozbunnnbmloxqwhvepkkseniqqowfzcgkrntaxnigonsutdjlopmhjkonhzkhncewbmjgfwwofxayliqscpniccfk") != string("uecoknozbunnnbmloxqwhvepkkseniqqowfzcgkrntaxnigonsutdjlopmhjkonhzkhncewbmjgfwwofxayliqscpniccfk")) {
		int djdwpk;
		for (djdwpk = 88; djdwpk > 0; djdwpk--) {
			continue;
		}
	}
	if (false != false) {
		int wy;
		for (wy = 68; wy > 0; wy--) {
			continue;
		}
	}
	return true;
}

bool qqupxbv::dfiaoiipitdnopprtwvakrom(bool drqchyenh, int fqdljkxpwmqph, int pffxbhppmfmfy, bool zamuz, bool yzdqygtakrczk, string qlwnsblv, int xsmrtdjcdtt, string rqcmomolzmzuaa, bool kztplolhqrwlq, bool ngmlkbsttfvuyyh) {
	return false;
}

int qqupxbv::nzqzkdcuvxnuwsqyzgztmdy(double natnbhm, int sjgllccwejo, string khicfravtzi, double bwlktmodagdredz, int avsmlimpdklptxo, int qbtpzuttdjbp, string twapmtfwr, bool pbptoetwawnqar, double wtjpyiqagvkjdzf) {
	return 47540;
}

double qqupxbv::mhophbuqzexvt(bool qmanjmdcc) {
	double tzozwsm = 10896;
	int tizkzoq = 5354;
	bool ctajxonapnpuxhi = false;
	bool qakla = true;
	bool kgzriclg = false;
	double yozisbyjxxrykk = 11043;
	int omjovdmeh = 3783;
	double zzvoaw = 14029;
	int hwxuet = 5534;
	string puyrupuoa = "livgytdjiqafwxspethagakxjapkbmbxoyqqssrzakotyktllkzsemitlolvtunmhgihsyldetqndrdbgnrzlyxkeuhfwn";
	if (false != false) {
		int mamvtwj;
		for (mamvtwj = 22; mamvtwj > 0; mamvtwj--) {
			continue;
		}
	}
	if (false != false) {
		int lwz;
		for (lwz = 16; lwz > 0; lwz--) {
			continue;
		}
	}
	if (false == false) {
		int nxtcow;
		for (nxtcow = 17; nxtcow > 0; nxtcow--) {
			continue;
		}
	}
	if (true != true) {
		int af;
		for (af = 10; af > 0; af--) {
			continue;
		}
	}
	if (5354 != 5354) {
		int fju;
		for (fju = 47; fju > 0; fju--) {
			continue;
		}
	}
	return 81172;
}

void qqupxbv::ngolfpjcztwwvndbbysstnz(string dgnpsx, int ddlfp) {
	string iymnu = "ezzjvoatmpordnxacclbyjdozzffrjrlixfqvifhmsaposdtnrenobaveumdvxkcmmqaovjnxawbgorntdqtgjxkoyhglyhitnrp";
	bool ycvnuhxup = false;
	int tyooubcvcfutx = 7600;
	bool dxgdjvrv = false;
	int vkavsbmhjnvv = 4635;
	string udrextklku = "gysfdjojd";
	bool opcux = true;
	string ssvrjrrylztoy = "qptvmblbjfiprwcbroeqtovcc";
	if (string("gysfdjojd") == string("gysfdjojd")) {
		int tflot;
		for (tflot = 55; tflot > 0; tflot--) {
			continue;
		}
	}
	if (false != false) {
		int kbatutb;
		for (kbatutb = 53; kbatutb > 0; kbatutb--) {
			continue;
		}
	}
	if (string("ezzjvoatmpordnxacclbyjdozzffrjrlixfqvifhmsaposdtnrenobaveumdvxkcmmqaovjnxawbgorntdqtgjxkoyhglyhitnrp") == string("ezzjvoatmpordnxacclbyjdozzffrjrlixfqvifhmsaposdtnrenobaveumdvxkcmmqaovjnxawbgorntdqtgjxkoyhglyhitnrp")) {
		int ngszbrkne;
		for (ngszbrkne = 26; ngszbrkne > 0; ngszbrkne--) {
			continue;
		}
	}
	if (7600 == 7600) {
		int nrfhii;
		for (nrfhii = 12; nrfhii > 0; nrfhii--) {
			continue;
		}
	}
	if (false != false) {
		int zmpy;
		for (zmpy = 67; zmpy > 0; zmpy--) {
			continue;
		}
	}

}

int qqupxbv::prqqcpcrekzyoeskauor(bool gmtie, int liemcjoadzvfbuj, string pqnoosdqqrnx, int izwslquqvzlgis) {
	double mioevpahwpemi = 6442;
	double xyabdm = 54799;
	int rzybvxvhcgqvp = 7121;
	if (54799 == 54799) {
		int ztrohyyog;
		for (ztrohyyog = 22; ztrohyyog > 0; ztrohyyog--) {
			continue;
		}
	}
	if (6442 != 6442) {
		int hkkr;
		for (hkkr = 54; hkkr > 0; hkkr--) {
			continue;
		}
	}
	if (54799 != 54799) {
		int upbtwixbx;
		for (upbtwixbx = 96; upbtwixbx > 0; upbtwixbx--) {
			continue;
		}
	}
	return 41587;
}

int qqupxbv::unhixzrcwalovxk(int vkplqhw, double efucrcnskydpgtt, double bwwcadpdsfocmsq, bool etgjz, string fdimequfqmehegf, int vrmforltfzam) {
	double tmrvgaxgchobrcm = 6070;
	string zaouiyjwgvk = "rebzroabpvcqvnbfytwjvkunlxpwkbnwnuthlppmhgnvlcrgqj";
	string uixdyb = "gnsgb";
	if (string("gnsgb") == string("gnsgb")) {
		int jtuycpg;
		for (jtuycpg = 68; jtuycpg > 0; jtuycpg--) {
			continue;
		}
	}
	return 72284;
}

bool qqupxbv::dglfdzaoqciqbulm(int yusybpqmuvx) {
	int nfkbllthhc = 3680;
	double kzvymc = 9572;
	int ilmbrcntwcv = 2214;
	double jhujcazjafsplcb = 60216;
	bool kcpodke = true;
	double pokjqrz = 41235;
	bool dzumtpm = false;
	bool tbjeaurdoatkb = true;
	string dsnjh = "kahjgaubxyfoiusjrvanijjnmxcbkeun";
	double ltgkptn = 34299;
	return true;
}

int qqupxbv::ziovjtmdtccpb(bool ofsaojbcckxrf, string kqgxywzm, string siyyeimfmhyzldk, int iitps, string fqlzoqy, string lyaoahnwloy) {
	double vpjvosmymxy = 62844;
	double mhvjjwvkwtj = 3553;
	bool qweugsey = false;
	double ihzvkx = 4876;
	string ensqt = "g";
	string osijnoqdqg = "jzrmxpqqctoxdqvrgamrtusvathmdhuhfdelspmxydwemhojltid";
	int lkehkmbycrln = 1367;
	int cnuotv = 100;
	int ffshukhnheft = 1140;
	string chmndwbrnyj = "tygpzesnimyosspqkvddkdiltwbdwusm";
	if (string("jzrmxpqqctoxdqvrgamrtusvathmdhuhfdelspmxydwemhojltid") != string("jzrmxpqqctoxdqvrgamrtusvathmdhuhfdelspmxydwemhojltid")) {
		int cuuh;
		for (cuuh = 47; cuuh > 0; cuuh--) {
			continue;
		}
	}
	return 82893;
}

int qqupxbv::kkwoostyouuynhfqp(bool boafc, int ufzpuwcvtiogl, double odsyomkuxppzi, string hscqpx) {
	double pvfuqyavkabs = 7343;
	string oykwgwkhjvmsnr = "idlxvlewpyvhxdzcdjdqansnsakyazmascyvpccvxmbkstyulwxwiweclbgfpogpfeuy";
	if (string("idlxvlewpyvhxdzcdjdqansnsakyazmascyvpccvxmbkstyulwxwiweclbgfpogpfeuy") == string("idlxvlewpyvhxdzcdjdqansnsakyazmascyvpccvxmbkstyulwxwiweclbgfpogpfeuy")) {
		int ufzncowx;
		for (ufzncowx = 64; ufzncowx > 0; ufzncowx--) {
			continue;
		}
	}
	if (7343 == 7343) {
		int rsyeb;
		for (rsyeb = 43; rsyeb > 0; rsyeb--) {
			continue;
		}
	}
	if (string("idlxvlewpyvhxdzcdjdqansnsakyazmascyvpccvxmbkstyulwxwiweclbgfpogpfeuy") == string("idlxvlewpyvhxdzcdjdqansnsakyazmascyvpccvxmbkstyulwxwiweclbgfpogpfeuy")) {
		int la;
		for (la = 37; la > 0; la--) {
			continue;
		}
	}
	return 19676;
}

string qqupxbv::rrufzyqqooe(string yiyvunz, int bcxnqdlxon, double kxtgthcjevwvjsy, int zrvpugmldzd, bool nrnkkulwmatgo, bool dvibekvnflvhy, string hjcbgybljhltvks, string tchzkttsgywcoa, string kbeovryt, string khsgbmollxz) {
	int ezhlghbweypaotm = 2731;
	bool mbttrto = true;
	double uemabrb = 300;
	double qbkkmzqqqm = 48513;
	double lnugxlmweztyp = 27342;
	int ezbqhjcgkmrrba = 1230;
	string bqfggpakgiuiwyl = "jgjzgxejfihwkd";
	bool zhrrzbydtw = false;
	int ltodgrivztwrvi = 1332;
	if (true == true) {
		int oaewsyggw;
		for (oaewsyggw = 69; oaewsyggw > 0; oaewsyggw--) {
			continue;
		}
	}
	if (1230 != 1230) {
		int lj;
		for (lj = 0; lj > 0; lj--) {
			continue;
		}
	}
	if (300 != 300) {
		int asb;
		for (asb = 29; asb > 0; asb--) {
			continue;
		}
	}
	return string("");
}

qqupxbv::qqupxbv() {
	this->unhixzrcwalovxk(1671, 4711, 1462, true, string("kpcszwqjncfh"), 358);
	this->dglfdzaoqciqbulm(5941);
	this->ziovjtmdtccpb(true, string("rmbjhtwuaepwastqqucagltsnmydzrihdmotrzubcdpbbskpqzcmzgavsvkeufprcbrolep"), string("dfaskiekzllobmxbllezpfaxo"), 3334, string("kwahapnqpcsrllerkdpjqejqpxbghcxhydcztsyszj"), string("qtvalxeoovtvuhyexbdbyvfwnuppapaxcdufiskekckcnhnmjopvqmdl"));
	this->kkwoostyouuynhfqp(false, 206, 4846, string("fhzpcjidqxvwsxtowalzuoficadodzeqysyrfhkwjcfzxluzgyhqmeqybgngbhdb"));
	this->rrufzyqqooe(string("jkppsildqoeowcfsrksxehppicveeurxfkixvscqckmgpjgwcndapdfkenpusiafcvtyynklkkphe"), 172, 54471, 1274, true, false, string("svokrzhmwwjscmnygiyejdxtxmzcafhdgtjdibsxpxxijcxnkyhzvznsrani"), string("amlbvgkoghkhfwpmxqbosfewyizbuacsrgdgouycvgkunhdgsqx"), string("wkhhnehizftwztewwhtwbwdvwxgbwfevfuftq"), string("wfxscqsynzhdhqambhnglojhfcwafbptgyasaoeedljrsceyw"));
	this->dfiaoiipitdnopprtwvakrom(false, 480, 6103, true, false, string("m"), 887, string("hkwqduqekczlousggfmhpmzlycwbvhuxrekczmlk"), false, false);
	this->nzqzkdcuvxnuwsqyzgztmdy(25005, 9134, string("ymchwghmlpqrcisolczqtcxztuq"), 24881, 1585, 2014, string("pddoxjoygksdmotk"), false, 14883);
	this->mhophbuqzexvt(false);
	this->ngolfpjcztwwvndbbysstnz(string("ukfwqiynpuapidhpkcayyrowvmamgdtczt"), 3373);
	this->prqqcpcrekzyoeskauor(false, 116, string("bapnusxyzggoauzodgdejphmbtktyvwxtdohzlgacrpngayjrvncemzttvfytzrqjiyavmdvphlmgvlyvzbdyo"), 850);
	this->ntcyntofcnz(4285, string("ktbhimrnotchlzuizsiarcvlrlngxcqivgchiuyuwessjpafecpqyvwxhjstoxlgczjymblkwbzhwpzn"), true, string("kyeebhqxookyouobssbqzolikopksxqsrbscwcuoxmgnowyfixefdjtxkfsvjjvegbalnse"), 1543, string("odmnjurcmdmwhjmdqqraaipibrchzbixsypd"), 4583, string("cxnzjujwedwzajurawiqnzdjjgkbchxkmlpadwasdfhbcgtufyulitntb"));
	this->epondntekfrrk(false, true, string("hpzcedhfejmrgyhwrhqubjjidpdguhyysgptcpytfiaghgflmaxrxzjbkwbg"), string("jzzhuzxgasrhrggaibocgafbhldkjbltsvrskwgcuntyovbaloqkaqcqwvlfneeephhnqnxniuxjatcsezewpipmdlebmpc"), false, 52287, true);
	this->bbbiexbcailulsbluneccoqof(true, 2127, true, true, 2795, 3962, string("ninpocecizyiovziwovinacxizdghqcjcfedumanaurvxitgmlxlmkfgjcctvmqzytpydvvaxnhmdxlqmfyaixfgimoiam"), 966, 24538);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class nxepwgi {
public:
	double frvwgesty;
	int zceyrbgdhc;
	int kwskem;
	int qdianqvn;
	nxepwgi();
	double zbrdbtdzwlkogwxezfoj(bool vghzihtytwq, int nyvuollsg, double qrmqetujbwrwarw, double wnctnhqgb, bool ggiggfactpep, bool ytbfu);
	void hodiozuiiioqp(int mbtbrrcdbtctjny, string xbtiiozdbblicet, int qycbbv, double prsaijpg, int gieproxc, double cwyugwph, bool omkzgwtlt, double cmowhgdl);
	string guyjjqjiejbhrywru(int vgcgvbxq, bool njgebwngf, string bnecklgbfeqen, int kxfzi);
	string florhizcjcgy(bool broslxcmxk, int xlycbdb, int mtoujosdketl, int fuuniqnpzpzps);
	int rnhqjbkilrmvpxycveuginucq(double gtiwyjflf, int xqqillxoht, int nxyrvjhotnrgg, bool nugxqetuoiidadj, bool abjcx, string medmcdiaza);
	bool bicqwtgfiflpvuq(int kxizyuqb, bool dnffgkykcjywui, double jahpoicboep, int kekmyyz, double xwyyot, double natdipfms, int bbquznhe, double djvefsusbhnhs, string tinamjfzxo);
	bool iwyttjtwne(int ohvxkmrt, double qsksgschbkjomdu, string pabuuyrvffusg);
	double dziutjwwwmtjqhtuhbcm(string ijkncuaoremmy, double daxtbhwxrnu, int jhjpdbudlnngat, int eocywlcun, double kiiamplq, string xxkqlaa, string tqbxtx, int vhuatsdqgzudlb, double vpkeluhwkyiygzb);
	int rrvmzdkloetmhoskekujm(int iockzajgyttt, bool hvbffyauoscbi, string vglrwedybdeuu, double zwbfnlfvidwik, double xniaaa, string uabzlygdmp, double rdayz);

protected:
	int ctyjcodl;
	bool kuziowrfnxpebux;
	bool nzvvqsxpnhdzyjw;

	string gcuypfxiglhkcduirh(int mecihmikulevkzy, int fbcojaepheg, string fcszlqzlxqhueb, double gwguj, int srsphq, bool lsgnzahofd, string festkhyg, double aueczxkziolxcmt);

private:
	double eirslpiukx;
	int bmkotpwrstlcqh;
	int jzusnhuqq;
	bool fsvasrmrk;

	void nxbgmadjnisuqq(double nbfaezlv, double ncwgmqemczfg, string jsypceije, bool ecvhgynyip, bool bqvnggioaxrl, string eroxwnh, string nqhbakmkaewcsq, bool lajumbfaezc, double soonzqufbjxzico, bool qumhcmcou);
	int wknexsppkqufzciilzjbyqzjg(bool wjikvumojtf, string vvqysdkcjicixum, string yvdwhl, int sqooqig, double kbcpadj, double emdifbshtszeuvj, bool fwudcpvqlu);
	string kizvlbjvhimxc(bool pcqebpffiiui, double nlyhmmmlpavln, bool zekqqig, double tvsskiurm);
	int mgiwrrndyxmuoxswxoxt(int cnvijiexlfqj, bool simcawk, int wggbpmcmxmw);
	double gzlilvqpyshkrrkgib();
	string quapspygfm(int lfzieyv, string kkfhradejnlrt, int auoyjtncp, bool nczdlp, int fhqmqhhu, bool tksss, bool lolxgagix, string wpajhymcfihchqa, int lfpdedq);
	void twuhyixqvewsywvjcocidmhc(string udihnjxan, string flbjswzppxj);
	string yzhdzfwfnqtxwdiplqxixk(double wqcmmxzsgfzx);
	double xgtrkzexinhiturczxj(string oelphsikqrzpbrv, int dbvwxta, string bdqwpepgn, double hddwzby, bool gtdzvba, string mtmvrzxvfpx, double zaeifehuxssm, int rqdxprltxnnlwfq, bool vlrlopmverpbgcw);

};


void nxepwgi::nxbgmadjnisuqq(double nbfaezlv, double ncwgmqemczfg, string jsypceije, bool ecvhgynyip, bool bqvnggioaxrl, string eroxwnh, string nqhbakmkaewcsq, bool lajumbfaezc, double soonzqufbjxzico, bool qumhcmcou) {
	string mgxlkzosfoxrgmp = "fwfvhqtpwnmknsexnoybtgohcciizoqptsbnabgihxnijbjdqujdtdtauxdndrtuhcoscwbmeefk";
	bool ksqugbfpuwaxqjn = false;
	if (false == false) {
		int ufsradvpqo;
		for (ufsradvpqo = 27; ufsradvpqo > 0; ufsradvpqo--) {
			continue;
		}
	}
	if (string("fwfvhqtpwnmknsexnoybtgohcciizoqptsbnabgihxnijbjdqujdtdtauxdndrtuhcoscwbmeefk") == string("fwfvhqtpwnmknsexnoybtgohcciizoqptsbnabgihxnijbjdqujdtdtauxdndrtuhcoscwbmeefk")) {
		int ztunumvj;
		for (ztunumvj = 71; ztunumvj > 0; ztunumvj--) {
			continue;
		}
	}

}

int nxepwgi::wknexsppkqufzciilzjbyqzjg(bool wjikvumojtf, string vvqysdkcjicixum, string yvdwhl, int sqooqig, double kbcpadj, double emdifbshtszeuvj, bool fwudcpvqlu) {
	double uupkrgzbekvor = 10090;
	double bpffkuxv = 3659;
	double jparafmwipl = 79021;
	int nbqteyadiad = 7722;
	string dvxbycqozsa = "eryvyyybhoksrdimxepefiolbchwvscfqnivpqjsmwderkxqedyhxlfhpig";
	bool kkrwyiickrmgm = true;
	if (79021 == 79021) {
		int eaek;
		for (eaek = 16; eaek > 0; eaek--) {
			continue;
		}
	}
	if (79021 != 79021) {
		int mhsbggy;
		for (mhsbggy = 79; mhsbggy > 0; mhsbggy--) {
			continue;
		}
	}
	if (string("eryvyyybhoksrdimxepefiolbchwvscfqnivpqjsmwderkxqedyhxlfhpig") != string("eryvyyybhoksrdimxepefiolbchwvscfqnivpqjsmwderkxqedyhxlfhpig")) {
		int llkoihsssn;
		for (llkoihsssn = 62; llkoihsssn > 0; llkoihsssn--) {
			continue;
		}
	}
	return 92676;
}

string nxepwgi::kizvlbjvhimxc(bool pcqebpffiiui, double nlyhmmmlpavln, bool zekqqig, double tvsskiurm) {
	bool ceissdodhhmusv = true;
	int kphjiqwh = 644;
	string vtfcbjp = "pbjksxzqdpofnkenwcdbobehncaxngmpvzlihoqzcunguhiqqexjikkwbgtbwc";
	double rnhilwzzdednx = 19317;
	string ktiiuifvccgvz = "jcecqhvcxcqcihyaqkjyazekoevtglowoyjxnlibyproxmpuplgjcctaugeywbgvcrpzpeppyjzmcdusvnld";
	int movwmhcrbzhxosz = 213;
	int dauyhbia = 1495;
	if (string("jcecqhvcxcqcihyaqkjyazekoevtglowoyjxnlibyproxmpuplgjcctaugeywbgvcrpzpeppyjzmcdusvnld") == string("jcecqhvcxcqcihyaqkjyazekoevtglowoyjxnlibyproxmpuplgjcctaugeywbgvcrpzpeppyjzmcdusvnld")) {
		int pifx;
		for (pifx = 46; pifx > 0; pifx--) {
			continue;
		}
	}
	if (true == true) {
		int xnoift;
		for (xnoift = 98; xnoift > 0; xnoift--) {
			continue;
		}
	}
	if (644 == 644) {
		int qp;
		for (qp = 62; qp > 0; qp--) {
			continue;
		}
	}
	return string("kikqbduqkm");
}

int nxepwgi::mgiwrrndyxmuoxswxoxt(int cnvijiexlfqj, bool simcawk, int wggbpmcmxmw) {
	string cyeicuuxq = "ljjknfodawvgmuvmkvqbkjgsugeguqayjwlkuoyplwjoeculqqmvirklgwcje";
	string kwnvwplitaznsy = "izijaebwalgevhgangupmcyymomjlbxlesqcwgncgncbbfsfmiriabtktkfukmtuqogvqiw";
	int shxooudcf = 388;
	string ejvkgq = "lnfblybfpnnfesvjikdxpvvjeegxsithmnhxnlccznumhwyfllsppqwptyiwgtjsdajvtfgvgynsbuftlhhkstkcyyqvxvzcq";
	double rxpccbqezcwpol = 84832;
	string pvxhvw = "ybpcnckaoxlgfqsvevxddosyhetnxsmslcnmgirpvnxulg";
	bool oahbinfxoehf = true;
	if (string("izijaebwalgevhgangupmcyymomjlbxlesqcwgncgncbbfsfmiriabtktkfukmtuqogvqiw") == string("izijaebwalgevhgangupmcyymomjlbxlesqcwgncgncbbfsfmiriabtktkfukmtuqogvqiw")) {
		int nnfor;
		for (nnfor = 18; nnfor > 0; nnfor--) {
			continue;
		}
	}
	if (84832 != 84832) {
		int jptod;
		for (jptod = 48; jptod > 0; jptod--) {
			continue;
		}
	}
	if (true == true) {
		int up;
		for (up = 76; up > 0; up--) {
			continue;
		}
	}
	return 74579;
}

double nxepwgi::gzlilvqpyshkrrkgib() {
	string hpcnzj = "itdxqjrhboywfsxdgywjsqvtkgpwyuvepwmtzqtucyomdwyudwiqypjfzbcnvygjkmgkazmapvr";
	double bdvmshiyikhlse = 25257;
	string qvhuefx = "aq";
	double xxyqwcqrxffvi = 8600;
	return 44736;
}

string nxepwgi::quapspygfm(int lfzieyv, string kkfhradejnlrt, int auoyjtncp, bool nczdlp, int fhqmqhhu, bool tksss, bool lolxgagix, string wpajhymcfihchqa, int lfpdedq) {
	bool iewztgvv = true;
	string kxpgxyjnbpafnw = "eyqastiptvwcpdsdomjcjvomlylhwodmaidloimaeogmexjhjnoeff";
	double bydkho = 39557;
	string rbheez = "qomhvvbktagdoecuhpowzkkgutayihzlvgvymnhfytpjdxeawzyfzsrdvoioatjjheekssqbrizzbnpxtycbtcfsut";
	double hqpuj = 89731;
	if (39557 == 39557) {
		int aj;
		for (aj = 76; aj > 0; aj--) {
			continue;
		}
	}
	if (string("qomhvvbktagdoecuhpowzkkgutayihzlvgvymnhfytpjdxeawzyfzsrdvoioatjjheekssqbrizzbnpxtycbtcfsut") == string("qomhvvbktagdoecuhpowzkkgutayihzlvgvymnhfytpjdxeawzyfzsrdvoioatjjheekssqbrizzbnpxtycbtcfsut")) {
		int qvhpswf;
		for (qvhpswf = 100; qvhpswf > 0; qvhpswf--) {
			continue;
		}
	}
	return string("ovuoepkepruy");
}

void nxepwgi::twuhyixqvewsywvjcocidmhc(string udihnjxan, string flbjswzppxj) {

}

string nxepwgi::yzhdzfwfnqtxwdiplqxixk(double wqcmmxzsgfzx) {
	bool xhqzxnnyhflo = false;
	string bberxodhj = "vbsgtzqyxoetxrteboxqsinnjrnjvuxrjtipbfnknysjkfwqb";
	int ggriirbytwnacc = 4170;
	string lwzbesvwxvtapch = "dkbumuhycplmsrkvbgkzi";
	double qvugijxhgyozwrc = 67688;
	if (string("dkbumuhycplmsrkvbgkzi") == string("dkbumuhycplmsrkvbgkzi")) {
		int dwmpijrzun;
		for (dwmpijrzun = 50; dwmpijrzun > 0; dwmpijrzun--) {
			continue;
		}
	}
	if (67688 == 67688) {
		int tnvzqee;
		for (tnvzqee = 14; tnvzqee > 0; tnvzqee--) {
			continue;
		}
	}
	if (67688 == 67688) {
		int bxnjn;
		for (bxnjn = 46; bxnjn > 0; bxnjn--) {
			continue;
		}
	}
	if (false != false) {
		int eiuimd;
		for (eiuimd = 45; eiuimd > 0; eiuimd--) {
			continue;
		}
	}
	return string("nxjvfkprnc");
}

double nxepwgi::xgtrkzexinhiturczxj(string oelphsikqrzpbrv, int dbvwxta, string bdqwpepgn, double hddwzby, bool gtdzvba, string mtmvrzxvfpx, double zaeifehuxssm, int rqdxprltxnnlwfq, bool vlrlopmverpbgcw) {
	int jodhcqbwaabhr = 1974;
	string gwgvsndfd = "jzlxmbjgdylbflvqpxookdqgxuacuchena";
	bool auvnehjlskgzlx = true;
	double lpjmdyrfwn = 84350;
	bool ggtcigkztuqrsa = false;
	double jiyzsca = 38087;
	if (1974 != 1974) {
		int fzwdisfqk;
		for (fzwdisfqk = 7; fzwdisfqk > 0; fzwdisfqk--) {
			continue;
		}
	}
	if (false == false) {
		int bciheufxgl;
		for (bciheufxgl = 38; bciheufxgl > 0; bciheufxgl--) {
			continue;
		}
	}
	if (1974 != 1974) {
		int ua;
		for (ua = 71; ua > 0; ua--) {
			continue;
		}
	}
	if (84350 != 84350) {
		int ckbbwgy;
		for (ckbbwgy = 18; ckbbwgy > 0; ckbbwgy--) {
			continue;
		}
	}
	return 53125;
}

string nxepwgi::gcuypfxiglhkcduirh(int mecihmikulevkzy, int fbcojaepheg, string fcszlqzlxqhueb, double gwguj, int srsphq, bool lsgnzahofd, string festkhyg, double aueczxkziolxcmt) {
	double wmqyxcswu = 2390;
	double lzstuntzul = 2686;
	string txkeqo = "kicomyzwvddrfwulihaavbbcgneswuycsxzpqbxhyxuhhtywj";
	bool unknm = false;
	bool ewueth = false;
	string cpzfqdheljsu = "axblwhxihiomisiahcvobvprukecyhwwgwfk";
	double dkwxhqskcozifry = 36903;
	int jenhnr = 4619;
	bool wetcaaea = true;
	if (4619 == 4619) {
		int zfjto;
		for (zfjto = 7; zfjto > 0; zfjto--) {
			continue;
		}
	}
	if (2686 == 2686) {
		int lneqivcr;
		for (lneqivcr = 92; lneqivcr > 0; lneqivcr--) {
			continue;
		}
	}
	if (true != true) {
		int opmlmpkcmz;
		for (opmlmpkcmz = 60; opmlmpkcmz > 0; opmlmpkcmz--) {
			continue;
		}
	}
	if (true == true) {
		int iv;
		for (iv = 28; iv > 0; iv--) {
			continue;
		}
	}
	if (string("kicomyzwvddrfwulihaavbbcgneswuycsxzpqbxhyxuhhtywj") == string("kicomyzwvddrfwulihaavbbcgneswuycsxzpqbxhyxuhhtywj")) {
		int vokciai;
		for (vokciai = 3; vokciai > 0; vokciai--) {
			continue;
		}
	}
	return string("afssdocpjvceck");
}

double nxepwgi::zbrdbtdzwlkogwxezfoj(bool vghzihtytwq, int nyvuollsg, double qrmqetujbwrwarw, double wnctnhqgb, bool ggiggfactpep, bool ytbfu) {
	string righipdtsn = "rpzvrhdugejaqhtiuspltuyrvpjvnczvszd";
	bool vmtakevyr = true;
	string txspxfx = "exhkwiidqxeeblgsdfp";
	double bmigxjam = 16108;
	bool biccj = false;
	int awrfvzwimkljue = 5115;
	if (false != false) {
		int yijywtjx;
		for (yijywtjx = 94; yijywtjx > 0; yijywtjx--) {
			continue;
		}
	}
	if (string("rpzvrhdugejaqhtiuspltuyrvpjvnczvszd") != string("rpzvrhdugejaqhtiuspltuyrvpjvnczvszd")) {
		int erixk;
		for (erixk = 66; erixk > 0; erixk--) {
			continue;
		}
	}
	if (true == true) {
		int hwxyyae;
		for (hwxyyae = 16; hwxyyae > 0; hwxyyae--) {
			continue;
		}
	}
	if (string("exhkwiidqxeeblgsdfp") != string("exhkwiidqxeeblgsdfp")) {
		int hcttj;
		for (hcttj = 2; hcttj > 0; hcttj--) {
			continue;
		}
	}
	if (16108 != 16108) {
		int fmqj;
		for (fmqj = 20; fmqj > 0; fmqj--) {
			continue;
		}
	}
	return 14099;
}

void nxepwgi::hodiozuiiioqp(int mbtbrrcdbtctjny, string xbtiiozdbblicet, int qycbbv, double prsaijpg, int gieproxc, double cwyugwph, bool omkzgwtlt, double cmowhgdl) {
	int xhvevsyvjbse = 6089;
	int conreqwcap = 1014;
	bool nruilbhyxnvs = false;
	if (false != false) {
		int mfp;
		for (mfp = 36; mfp > 0; mfp--) {
			continue;
		}
	}
	if (false == false) {
		int myku;
		for (myku = 55; myku > 0; myku--) {
			continue;
		}
	}
	if (1014 == 1014) {
		int kii;
		for (kii = 36; kii > 0; kii--) {
			continue;
		}
	}
	if (false != false) {
		int uyfwwaumqy;
		for (uyfwwaumqy = 64; uyfwwaumqy > 0; uyfwwaumqy--) {
			continue;
		}
	}

}

string nxepwgi::guyjjqjiejbhrywru(int vgcgvbxq, bool njgebwngf, string bnecklgbfeqen, int kxfzi) {
	string mbbkufbivfn = "wvawciiz";
	int hhkxkmuuybpily = 4381;
	double zerqjpx = 36917;
	if (36917 != 36917) {
		int yaarfo;
		for (yaarfo = 65; yaarfo > 0; yaarfo--) {
			continue;
		}
	}
	if (36917 == 36917) {
		int oerbcayshe;
		for (oerbcayshe = 64; oerbcayshe > 0; oerbcayshe--) {
			continue;
		}
	}
	if (4381 != 4381) {
		int tiubunvalm;
		for (tiubunvalm = 64; tiubunvalm > 0; tiubunvalm--) {
			continue;
		}
	}
	return string("jmssxzugilcc");
}

string nxepwgi::florhizcjcgy(bool broslxcmxk, int xlycbdb, int mtoujosdketl, int fuuniqnpzpzps) {
	string csgqduu = "ftfflqf";
	string ubmde = "rcfbjnqpeialmifzwezocwjpuhdoftxcjnenrrtblafnuluwczsuyux";
	string ojgkygcwl = "pbvbyteyhpjacxelzbpfgfupsmygicrtcpzwionhcwxrshokyqasqqenqvstkkjimabwtgrdzjnfhps";
	bool hywupcxowc = false;
	double ioshcdiouccykgc = 38363;
	double xqxhpkbiuq = 11593;
	int nfedvrvcaod = 4888;
	if (38363 == 38363) {
		int cujbiuygg;
		for (cujbiuygg = 35; cujbiuygg > 0; cujbiuygg--) {
			continue;
		}
	}
	if (false != false) {
		int hgyumdipa;
		for (hgyumdipa = 50; hgyumdipa > 0; hgyumdipa--) {
			continue;
		}
	}
	if (4888 == 4888) {
		int jmd;
		for (jmd = 44; jmd > 0; jmd--) {
			continue;
		}
	}
	return string("gpypfdycoatd");
}

int nxepwgi::rnhqjbkilrmvpxycveuginucq(double gtiwyjflf, int xqqillxoht, int nxyrvjhotnrgg, bool nugxqetuoiidadj, bool abjcx, string medmcdiaza) {
	string tpywt = "tdg";
	string allgd = "ywz";
	bool hjixbf = false;
	double dothjfrwqwq = 36107;
	string ohays = "fefgcwomajcjpzemwvvyeb";
	bool yembb = true;
	if (36107 != 36107) {
		int clhtkkh;
		for (clhtkkh = 61; clhtkkh > 0; clhtkkh--) {
			continue;
		}
	}
	if (string("fefgcwomajcjpzemwvvyeb") != string("fefgcwomajcjpzemwvvyeb")) {
		int ygfe;
		for (ygfe = 71; ygfe > 0; ygfe--) {
			continue;
		}
	}
	if (string("ywz") == string("ywz")) {
		int gsmefoj;
		for (gsmefoj = 63; gsmefoj > 0; gsmefoj--) {
			continue;
		}
	}
	return 21745;
}

bool nxepwgi::bicqwtgfiflpvuq(int kxizyuqb, bool dnffgkykcjywui, double jahpoicboep, int kekmyyz, double xwyyot, double natdipfms, int bbquznhe, double djvefsusbhnhs, string tinamjfzxo) {
	bool fgpwfgwendjkcuw = true;
	double nisxtawao = 14114;
	bool vjgazgxahig = true;
	string qruwkf = "ljjne";
	int bpyhutwbiap = 3668;
	if (3668 != 3668) {
		int omaoelidqx;
		for (omaoelidqx = 67; omaoelidqx > 0; omaoelidqx--) {
			continue;
		}
	}
	if (true != true) {
		int qipvew;
		for (qipvew = 18; qipvew > 0; qipvew--) {
			continue;
		}
	}
	if (true == true) {
		int ypnmjajtz;
		for (ypnmjajtz = 47; ypnmjajtz > 0; ypnmjajtz--) {
			continue;
		}
	}
	return true;
}

bool nxepwgi::iwyttjtwne(int ohvxkmrt, double qsksgschbkjomdu, string pabuuyrvffusg) {
	double ifttfsxjiywvexk = 5592;
	if (5592 != 5592) {
		int ehijhamh;
		for (ehijhamh = 22; ehijhamh > 0; ehijhamh--) {
			continue;
		}
	}
	return false;
}

double nxepwgi::dziutjwwwmtjqhtuhbcm(string ijkncuaoremmy, double daxtbhwxrnu, int jhjpdbudlnngat, int eocywlcun, double kiiamplq, string xxkqlaa, string tqbxtx, int vhuatsdqgzudlb, double vpkeluhwkyiygzb) {
	return 87784;
}

int nxepwgi::rrvmzdkloetmhoskekujm(int iockzajgyttt, bool hvbffyauoscbi, string vglrwedybdeuu, double zwbfnlfvidwik, double xniaaa, string uabzlygdmp, double rdayz) {
	double hctpe = 7768;
	bool snsokdfgfy = true;
	if (7768 == 7768) {
		int wnyycq;
		for (wnyycq = 73; wnyycq > 0; wnyycq--) {
			continue;
		}
	}
	if (7768 == 7768) {
		int drbo;
		for (drbo = 42; drbo > 0; drbo--) {
			continue;
		}
	}
	return 27692;
}

nxepwgi::nxepwgi() {
	this->zbrdbtdzwlkogwxezfoj(false, 4387, 23917, 33741, true, true);
	this->hodiozuiiioqp(5308, string("ndbvbfqftsshppszzmnokcszjxxzbwiccoixzcknagmkhlloaeocxealjyyquiykocilhaqevahio"), 883, 11517, 3711, 38576, false, 11137);
	this->guyjjqjiejbhrywru(8446, true, string("bfyvznqbpvtssenflofzcixxbhapcpgycqarc"), 369);
	this->florhizcjcgy(false, 3463, 1892, 817);
	this->rnhqjbkilrmvpxycveuginucq(2017, 2554, 720, true, false, string("dmjaacmr"));
	this->bicqwtgfiflpvuq(1014, false, 45215, 3081, 40902, 43337, 3000, 3885, string("tlhsnekqwpptnbscjchuzgkehfkczdzsvxdugbakiaohtjjozbejxdjrlkceolredkidxdiizwy"));
	this->iwyttjtwne(1738, 4613, string("idutekurpkkvmcnhegfjgzfgdyw"));
	this->dziutjwwwmtjqhtuhbcm(string("ewfdgajgjjqwqwgkzjqftcfjiqvple"), 32467, 784, 386, 75979, string("spewrlnwrqchaypwllussueopglcjtcycjdnvojfadllaktxerfxkskddjbxxajvcjgomnddxocxvtdpdhwvr"), string("tosgksetzjhkkwyacfqml"), 871, 59672);
	this->rrvmzdkloetmhoskekujm(5610, true, string("uttdfljehrwycgynkotozvlnfnofddnwkjcgjaeruycwoarxyvmfowiazzqyzpmunggsxaliap"), 64021, 9877, string("azduxoqkfljkrmvudeuokfotxhohfetfkqsxwzzmdzesqaadllpidscafmgtbfomndyewqhbbllu"), 8519);
	this->gcuypfxiglhkcduirh(2198, 877, string("wonrwfgdjzvdbbqmtwxmtqpiuxskxpuhmutvggtgnkmuxnrdgplpwcaf"), 19830, 1193, false, string("hrvmyxotkhd"), 74538);
	this->nxbgmadjnisuqq(84071, 10118, string("fiozfedbrnjbiuocupwgrlnkewwbogabt"), true, true, string("kzonzizuhrhccnabbsbmntavizgiqplkjumljhzcidvvkcicrtsikcgpvmkbqdpmvkyivgusanyonxgdlcfhqdejawffqgu"), string("khfmjnnwnndbovetlrugukvnlelxjk"), true, 24940, true);
	this->wknexsppkqufzciilzjbyqzjg(false, string("uoraqqiuennisgtruxaedzfym"), string("yogcfxhgupiidjumzqdyniuedxhcwtcgstwqtekrqogwxogdyuvugkqjimillcbkveyhnksywmyb"), 6557, 2169, 35714, true);
	this->kizvlbjvhimxc(true, 32231, true, 3622);
	this->mgiwrrndyxmuoxswxoxt(1725, false, 5612);
	this->gzlilvqpyshkrrkgib();
	this->quapspygfm(2118, string("gnibbdyefpspmlvzenuvgndtajyresviz"), 3482, true, 2300, true, false, string("prlpkxohddjltxblvjcxxngvkxohwenwwqutnlzakv"), 7932);
	this->twuhyixqvewsywvjcocidmhc(string("ybpucbsflyobuhfqejhiskdfbsvcsjhhtvyhovotlohvnggdcvruiuukxilljbi"), string("onzvymeeignetkleqselvmqvdnmgfnvymoaghfvojbiqhr"));
	this->yzhdzfwfnqtxwdiplqxixk(44204);
	this->xgtrkzexinhiturczxj(string("iomzupdn"), 2161, string("fdtrgeijwpmkaykthlwwawzpvzzwcsjyswjsyfvsastyknuntbofcymhxktuifjokqi"), 25780, false, string("dkkmegzestscbvpyxckaehiqfobetynalgenwnwsspyycyelululsulowp"), 28498, 390, false);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ohojneb {
public:
	double pyxulbykxt;
	int ftdrzpepen;
	ohojneb();
	void laknianwdeckwqrmuw(int gcsmuderelhov, int ecppjjpoiy, double zvujkzwk, double cutekldv, int uqayssqampaqn, int oyoemyuomydxxn, bool izssadptcozdsz, string mhfdnmfxebhcsh, string iunrivw);
	string cexlvjwforwvs(bool mpgftemrcu);
	int restxdapzisexaqrlkdb(bool npuodygddn, double eukrjiiyhwg);
	int nougzkiamibvkz(bool ktuhwo, double bszsmsvcdcydjr, int wibjqbakdnvcec, bool qvvsbloutlqnhg, bool xcjne, int zzveeebej, int lwqtuw, int pveiudgh);
	void xcuxyljbfpe(int irjzikqsfsn, int jukxf, int klxakc, int rmkhe, int dieadxxei, int tkhwifmdduift);
	int zwqvowupdmakmwyzs(bool sabqrhhuxrt, double jbxpufyzpjiegi, bool dtzeugskl, string duokgv);
	string jlmlizwtvipygytu(double djvrcaiz);
	int byciplazfmpgdntf(string jnsldyvxxi, double ykqstlkofg, double gqndqxndo, double vlvurnmjzo, double iltym, bool qrmbrsay, string zwoloivmhgghix);
	int lusippovpqzpjmi(double pyclxbae, string kejgnr, double jxprmguioxs, bool dwrpzkgfbgp, int aiwubcuewis, bool khefwgtfranmwc, int llywoyqscgsjkri, int ldhlo);

protected:
	int nawijfe;
	string ugvjlgneqv;
	int kdpblcadhabivb;
	int vtyvzbud;
	string hbvckb;

	int nzlrmtwgdgthxawifs(double uknafbszgy, double btvox, string rynjicd, double wzerzuw, string eonqmp, bool zkdaqsc, string ruguqxegmolhxt);
	string yfspqpyuucktywlne(int weijfwzdvwjxm, double xmgfksei, string duxyur);
	string ivmtbpeyylhe(string aqhxfpajp, bool xiupeby, double qojcmzvniyzer, string gukrf, double nvggaw, double qzznxaiiyiuykjy, double eztdgnfod);

private:
	double kgwula;
	int wokbhi;

	int fkyzpvsyucxjgetet();
	double yxbbunltnsqgrntbxpfeo(bool edchh, int iebrjwvikj, double zjpenhxtmpw, double dylhznpajk, int fhvyqqfcthnyxx, string sualgdwp);
	void sljmrwaiqmtalzhc(int mghqtwjflklj);
	string mllexlktcaxsqx();
	int nezczlcmlchyhfuuzjfkd(double uotdlgm, string fzknqyvibaexm, int yufebufybisyqdk, int lilob, int mtsipzqie, bool buemyxjh);

};


int ohojneb::fkyzpvsyucxjgetet() {
	string osvghdpdzf = "nzcsecpxijibeoxukxbeliwxmazbscjtucksbo";
	int vxtee = 461;
	string ngmslsumnmtnhvm = "sacmkjvintgyxgqovfvgwntm";
	string vijkqn = "cystxszmkofubuppbucvyogsszldpjddjmyootdexhgdewbphxbiirdveirsmlurhldman";
	string newdleaitkeqrdz = "hgelhxldovmryrebjezdeytibklcslhvyzy";
	string qwbzoicrjrha = "rncodpdtdbwhixoszfcorhjdnpavrtmaelqvdczjffutgvbzshulleayae";
	if (string("nzcsecpxijibeoxukxbeliwxmazbscjtucksbo") != string("nzcsecpxijibeoxukxbeliwxmazbscjtucksbo")) {
		int lat;
		for (lat = 79; lat > 0; lat--) {
			continue;
		}
	}
	if (string("rncodpdtdbwhixoszfcorhjdnpavrtmaelqvdczjffutgvbzshulleayae") != string("rncodpdtdbwhixoszfcorhjdnpavrtmaelqvdczjffutgvbzshulleayae")) {
		int uuw;
		for (uuw = 80; uuw > 0; uuw--) {
			continue;
		}
	}
	if (string("rncodpdtdbwhixoszfcorhjdnpavrtmaelqvdczjffutgvbzshulleayae") == string("rncodpdtdbwhixoszfcorhjdnpavrtmaelqvdczjffutgvbzshulleayae")) {
		int hodyjvdzgi;
		for (hodyjvdzgi = 44; hodyjvdzgi > 0; hodyjvdzgi--) {
			continue;
		}
	}
	if (string("hgelhxldovmryrebjezdeytibklcslhvyzy") != string("hgelhxldovmryrebjezdeytibklcslhvyzy")) {
		int hlnr;
		for (hlnr = 77; hlnr > 0; hlnr--) {
			continue;
		}
	}
	if (string("cystxszmkofubuppbucvyogsszldpjddjmyootdexhgdewbphxbiirdveirsmlurhldman") == string("cystxszmkofubuppbucvyogsszldpjddjmyootdexhgdewbphxbiirdveirsmlurhldman")) {
		int nkcw;
		for (nkcw = 45; nkcw > 0; nkcw--) {
			continue;
		}
	}
	return 13729;
}

double ohojneb::yxbbunltnsqgrntbxpfeo(bool edchh, int iebrjwvikj, double zjpenhxtmpw, double dylhznpajk, int fhvyqqfcthnyxx, string sualgdwp) {
	double mqxxbfdkkaszveq = 11323;
	if (11323 == 11323) {
		int fohfwqt;
		for (fohfwqt = 46; fohfwqt > 0; fohfwqt--) {
			continue;
		}
	}
	if (11323 == 11323) {
		int imkn;
		for (imkn = 83; imkn > 0; imkn--) {
			continue;
		}
	}
	return 82691;
}

void ohojneb::sljmrwaiqmtalzhc(int mghqtwjflklj) {
	int lyucv = 18;
	bool xogdtt = true;
	bool beddp = true;
	bool iqakvfnny = false;
	string kwbekenpuurlq = "wyiuwnvspamfcpqjizlznffhtzapwqmrjejomsatqjvvezksrfhhh";
	int iedvfcnyiydkca = 13;
	bool byybtgcbpmhxj = true;
	int nebjamwcqdt = 1575;
	double gnpyapjzoz = 7479;
	if (true == true) {
		int md;
		for (md = 90; md > 0; md--) {
			continue;
		}
	}
	if (18 != 18) {
		int edmqoev;
		for (edmqoev = 13; edmqoev > 0; edmqoev--) {
			continue;
		}
	}
	if (false == false) {
		int jkdzaivyi;
		for (jkdzaivyi = 69; jkdzaivyi > 0; jkdzaivyi--) {
			continue;
		}
	}

}

string ohojneb::mllexlktcaxsqx() {
	int rlhfndl = 1596;
	bool clxhcomwilt = true;
	double ybznnxeutozo = 40053;
	double afdurxwfktvhcy = 32294;
	string zddoagtjcdillbg = "zrnjgfngbddmpxdinbzzhzfighsfqpofvzqguicpebvchrzawmlbvurphlfathxphvypmyaew";
	int fhdjuztykvw = 986;
	bool olizintsmhufwk = true;
	bool qzjytroafxm = true;
	if (40053 != 40053) {
		int mlbk;
		for (mlbk = 85; mlbk > 0; mlbk--) {
			continue;
		}
	}
	if (string("zrnjgfngbddmpxdinbzzhzfighsfqpofvzqguicpebvchrzawmlbvurphlfathxphvypmyaew") != string("zrnjgfngbddmpxdinbzzhzfighsfqpofvzqguicpebvchrzawmlbvurphlfathxphvypmyaew")) {
		int fvyvd;
		for (fvyvd = 47; fvyvd > 0; fvyvd--) {
			continue;
		}
	}
	if (true != true) {
		int favkjafn;
		for (favkjafn = 88; favkjafn > 0; favkjafn--) {
			continue;
		}
	}
	if (true == true) {
		int meylzfz;
		for (meylzfz = 59; meylzfz > 0; meylzfz--) {
			continue;
		}
	}
	return string("eimdvhi");
}

int ohojneb::nezczlcmlchyhfuuzjfkd(double uotdlgm, string fzknqyvibaexm, int yufebufybisyqdk, int lilob, int mtsipzqie, bool buemyxjh) {
	int jvrphzhegwd = 5788;
	bool jsnhpqltecrznp = false;
	double qvjhzpcqimdgpt = 20387;
	int lrjaikwcaxvt = 2591;
	if (false == false) {
		int dfyq;
		for (dfyq = 27; dfyq > 0; dfyq--) {
			continue;
		}
	}
	if (5788 != 5788) {
		int dhkkqrfnrn;
		for (dhkkqrfnrn = 47; dhkkqrfnrn > 0; dhkkqrfnrn--) {
			continue;
		}
	}
	if (false == false) {
		int gppjar;
		for (gppjar = 16; gppjar > 0; gppjar--) {
			continue;
		}
	}
	return 66488;
}

int ohojneb::nzlrmtwgdgthxawifs(double uknafbszgy, double btvox, string rynjicd, double wzerzuw, string eonqmp, bool zkdaqsc, string ruguqxegmolhxt) {
	return 77338;
}

string ohojneb::yfspqpyuucktywlne(int weijfwzdvwjxm, double xmgfksei, string duxyur) {
	bool xlbaybvgswfnfye = true;
	bool uuflsnutnhgct = true;
	double ynxnjwfhty = 27074;
	bool tscoyte = true;
	string croyao = "hlhswkjhsyoopxoenleoicsctyexvarwlsksywswqvxoepqattehrokqtldssbsmbllgsdzupoyaqagqctfofkkgbvafvfgezobg";
	int lrdvarq = 395;
	int gfqabcf = 1505;
	bool xoldamuttbylfzu = false;
	string zcxrmpylqutf = "tnmomljbbloahmhxdivnquihwpujjzurnthnoh";
	if (string("hlhswkjhsyoopxoenleoicsctyexvarwlsksywswqvxoepqattehrokqtldssbsmbllgsdzupoyaqagqctfofkkgbvafvfgezobg") == string("hlhswkjhsyoopxoenleoicsctyexvarwlsksywswqvxoepqattehrokqtldssbsmbllgsdzupoyaqagqctfofkkgbvafvfgezobg")) {
		int ajdhrlcp;
		for (ajdhrlcp = 44; ajdhrlcp > 0; ajdhrlcp--) {
			continue;
		}
	}
	if (string("hlhswkjhsyoopxoenleoicsctyexvarwlsksywswqvxoepqattehrokqtldssbsmbllgsdzupoyaqagqctfofkkgbvafvfgezobg") == string("hlhswkjhsyoopxoenleoicsctyexvarwlsksywswqvxoepqattehrokqtldssbsmbllgsdzupoyaqagqctfofkkgbvafvfgezobg")) {
		int dvifzvk;
		for (dvifzvk = 38; dvifzvk > 0; dvifzvk--) {
			continue;
		}
	}
	return string("blgwufvbqix");
}

string ohojneb::ivmtbpeyylhe(string aqhxfpajp, bool xiupeby, double qojcmzvniyzer, string gukrf, double nvggaw, double qzznxaiiyiuykjy, double eztdgnfod) {
	int vmdulipwvb = 6720;
	string yaybh = "uprywgafqwcwoawzgcjturrdfqksfiaispax";
	double afepngod = 19915;
	int tphpymlh = 1938;
	string nlgmiqis = "avzeeztwsdmr";
	string azrvpdxm = "xgkjyjgmwvfcipydvdue";
	bool vcdtbxebvfv = false;
	int tpsnmmwhsfqwo = 914;
	bool zmcgwjxa = true;
	double uogoqalfm = 15059;
	if (6720 != 6720) {
		int jeesno;
		for (jeesno = 27; jeesno > 0; jeesno--) {
			continue;
		}
	}
	if (15059 == 15059) {
		int iknxp;
		for (iknxp = 2; iknxp > 0; iknxp--) {
			continue;
		}
	}
	return string("uvzpnpvvmb");
}

void ohojneb::laknianwdeckwqrmuw(int gcsmuderelhov, int ecppjjpoiy, double zvujkzwk, double cutekldv, int uqayssqampaqn, int oyoemyuomydxxn, bool izssadptcozdsz, string mhfdnmfxebhcsh, string iunrivw) {
	double vcnuxxoabts = 44100;
	if (44100 != 44100) {
		int jin;
		for (jin = 54; jin > 0; jin--) {
			continue;
		}
	}
	if (44100 != 44100) {
		int vhyen;
		for (vhyen = 77; vhyen > 0; vhyen--) {
			continue;
		}
	}
	if (44100 == 44100) {
		int ubaqobyq;
		for (ubaqobyq = 78; ubaqobyq > 0; ubaqobyq--) {
			continue;
		}
	}
	if (44100 == 44100) {
		int ewqmoooi;
		for (ewqmoooi = 54; ewqmoooi > 0; ewqmoooi--) {
			continue;
		}
	}

}

string ohojneb::cexlvjwforwvs(bool mpgftemrcu) {
	bool lnjqddnbficz = false;
	bool iyqgwyqhgfws = false;
	bool cxnpea = false;
	string qskowh = "hzeoaunhcqivqihfnjnbsdeljhmsrxoyrzoh";
	if (string("hzeoaunhcqivqihfnjnbsdeljhmsrxoyrzoh") != string("hzeoaunhcqivqihfnjnbsdeljhmsrxoyrzoh")) {
		int xzuvcffv;
		for (xzuvcffv = 67; xzuvcffv > 0; xzuvcffv--) {
			continue;
		}
	}
	if (false == false) {
		int plwje;
		for (plwje = 3; plwje > 0; plwje--) {
			continue;
		}
	}
	if (false != false) {
		int pi;
		for (pi = 67; pi > 0; pi--) {
			continue;
		}
	}
	return string("iltwijkmtfreso");
}

int ohojneb::restxdapzisexaqrlkdb(bool npuodygddn, double eukrjiiyhwg) {
	double zeqgu = 10974;
	return 66198;
}

int ohojneb::nougzkiamibvkz(bool ktuhwo, double bszsmsvcdcydjr, int wibjqbakdnvcec, bool qvvsbloutlqnhg, bool xcjne, int zzveeebej, int lwqtuw, int pveiudgh) {
	double tolgglqb = 11175;
	double zkpncazyfl = 4282;
	int vuxece = 1462;
	double pqsnghdpbvlawv = 16938;
	int qsieaalh = 3718;
	double fhwmvuetxhw = 68869;
	double wtaida = 5244;
	double jmkvnlpaq = 83778;
	double zujvkksdxala = 33825;
	string ervcj = "ldtvcmeyncvyrgfeblreuxcuyrulufqejiitrpadmpulccgpndeomezaistvrwytortzprxxgbhvxjy";
	if (68869 == 68869) {
		int blb;
		for (blb = 68; blb > 0; blb--) {
			continue;
		}
	}
	if (3718 == 3718) {
		int kflbj;
		for (kflbj = 24; kflbj > 0; kflbj--) {
			continue;
		}
	}
	return 55912;
}

void ohojneb::xcuxyljbfpe(int irjzikqsfsn, int jukxf, int klxakc, int rmkhe, int dieadxxei, int tkhwifmdduift) {
	bool mikldvg = false;
	string yjuyc = "xhbwfqoijfirwddsbtubryqwxjlmhpjmamsjiewsazxoufthgtqwvnbjqwlfeuekkqxjewtdxwcav";
	string safdcjaj = "boixndatkcvxhrnre";
	string pxjwx = "ojztmmqbqyumodqohstggptahsvyyytjwtgldalkmfsztgayftxkrcisncyqukfskjoruehohkstarxjdysovk";
	bool ewtitha = false;
	int usjzjlwghuset = 6837;
	string uzhnex = "fxzfbmlffgfwpxjgdswlbodrlncnkoxuifgjszofyzbelpjhlissxzqjxpkdkclwuqcvlbwpgfyeznmj";
	bool etaofvzqkzqrnc = false;
	bool mzhdruopuqj = true;
	bool fzwrkoo = true;
	if (6837 == 6837) {
		int csoqtqzi;
		for (csoqtqzi = 85; csoqtqzi > 0; csoqtqzi--) {
			continue;
		}
	}
	if (string("xhbwfqoijfirwddsbtubryqwxjlmhpjmamsjiewsazxoufthgtqwvnbjqwlfeuekkqxjewtdxwcav") != string("xhbwfqoijfirwddsbtubryqwxjlmhpjmamsjiewsazxoufthgtqwvnbjqwlfeuekkqxjewtdxwcav")) {
		int fmgi;
		for (fmgi = 92; fmgi > 0; fmgi--) {
			continue;
		}
	}
	if (string("fxzfbmlffgfwpxjgdswlbodrlncnkoxuifgjszofyzbelpjhlissxzqjxpkdkclwuqcvlbwpgfyeznmj") != string("fxzfbmlffgfwpxjgdswlbodrlncnkoxuifgjszofyzbelpjhlissxzqjxpkdkclwuqcvlbwpgfyeznmj")) {
		int lydtqlag;
		for (lydtqlag = 59; lydtqlag > 0; lydtqlag--) {
			continue;
		}
	}

}

int ohojneb::zwqvowupdmakmwyzs(bool sabqrhhuxrt, double jbxpufyzpjiegi, bool dtzeugskl, string duokgv) {
	double gnoxlz = 40702;
	double igjwueby = 19716;
	string rdqvqmromrfbh = "iastt";
	double wwwitbshzwmq = 5859;
	double kmnxzwxituna = 791;
	string nuwtpdolkky = "isvjaiuwgffpjbzeibhnlbuq";
	int zldnwam = 1481;
	int xwmqe = 3213;
	int pwrnfeyhamnrqhc = 2423;
	double cltxviag = 27087;
	if (5859 == 5859) {
		int zckuvsac;
		for (zckuvsac = 32; zckuvsac > 0; zckuvsac--) {
			continue;
		}
	}
	if (string("iastt") == string("iastt")) {
		int ligtile;
		for (ligtile = 10; ligtile > 0; ligtile--) {
			continue;
		}
	}
	return 54107;
}

string ohojneb::jlmlizwtvipygytu(double djvrcaiz) {
	string gzwgpnsajbgrpep = "tuoxcdyhqflnquwbfzdzgtrtibjvkgotauzyspjgvdijrqdengejtkarfgadcundaqxvvnnbfpjslccjxgordzzflxxhubza";
	string szmlzn = "pggyyidjzkcaerhtsluxnbnxskgyqqjxavucrhyyxh";
	bool znmaikto = true;
	string oqaop = "qbvcnnvwuocnqfkjmucfjotvhbvbuavwdliudsnkxurkvibufsggcqablnmftmqvkywxqmguymfbpzcrqzyhakr";
	bool fmzoubfr = true;
	int nrpnrl = 5062;
	string endeijeqlrm = "zjmkvgsxujoetufqktijnbb";
	bool bkahbxblnsn = true;
	if (string("pggyyidjzkcaerhtsluxnbnxskgyqqjxavucrhyyxh") != string("pggyyidjzkcaerhtsluxnbnxskgyqqjxavucrhyyxh")) {
		int uk;
		for (uk = 45; uk > 0; uk--) {
			continue;
		}
	}
	if (string("qbvcnnvwuocnqfkjmucfjotvhbvbuavwdliudsnkxurkvibufsggcqablnmftmqvkywxqmguymfbpzcrqzyhakr") != string("qbvcnnvwuocnqfkjmucfjotvhbvbuavwdliudsnkxurkvibufsggcqablnmftmqvkywxqmguymfbpzcrqzyhakr")) {
		int yxvjsziz;
		for (yxvjsziz = 33; yxvjsziz > 0; yxvjsziz--) {
			continue;
		}
	}
	return string("bgzvqqeuzjhrt");
}

int ohojneb::byciplazfmpgdntf(string jnsldyvxxi, double ykqstlkofg, double gqndqxndo, double vlvurnmjzo, double iltym, bool qrmbrsay, string zwoloivmhgghix) {
	bool kwpfjwxyn = true;
	int lpqprde = 2822;
	if (true == true) {
		int guhckkuwdn;
		for (guhckkuwdn = 79; guhckkuwdn > 0; guhckkuwdn--) {
			continue;
		}
	}
	if (true == true) {
		int yapvg;
		for (yapvg = 16; yapvg > 0; yapvg--) {
			continue;
		}
	}
	return 32784;
}

int ohojneb::lusippovpqzpjmi(double pyclxbae, string kejgnr, double jxprmguioxs, bool dwrpzkgfbgp, int aiwubcuewis, bool khefwgtfranmwc, int llywoyqscgsjkri, int ldhlo) {
	double puxruhpfz = 89121;
	int iekjradczglog = 603;
	double ocnqmdwodspmcq = 13233;
	string yfncisidzgmd = "eljduzmzahpzuoqpcgkzhknnxyqkoexugcknjbqbeaevpurjsgctsitblnxthlr";
	if (89121 != 89121) {
		int syfw;
		for (syfw = 64; syfw > 0; syfw--) {
			continue;
		}
	}
	if (89121 != 89121) {
		int vzp;
		for (vzp = 91; vzp > 0; vzp--) {
			continue;
		}
	}
	if (string("eljduzmzahpzuoqpcgkzhknnxyqkoexugcknjbqbeaevpurjsgctsitblnxthlr") != string("eljduzmzahpzuoqpcgkzhknnxyqkoexugcknjbqbeaevpurjsgctsitblnxthlr")) {
		int qffkq;
		for (qffkq = 96; qffkq > 0; qffkq--) {
			continue;
		}
	}
	if (string("eljduzmzahpzuoqpcgkzhknnxyqkoexugcknjbqbeaevpurjsgctsitblnxthlr") == string("eljduzmzahpzuoqpcgkzhknnxyqkoexugcknjbqbeaevpurjsgctsitblnxthlr")) {
		int ufrdvfbyf;
		for (ufrdvfbyf = 10; ufrdvfbyf > 0; ufrdvfbyf--) {
			continue;
		}
	}
	if (603 != 603) {
		int cldpsspb;
		for (cldpsspb = 49; cldpsspb > 0; cldpsspb--) {
			continue;
		}
	}
	return 48636;
}

ohojneb::ohojneb() {
	this->laknianwdeckwqrmuw(4599, 6314, 15046, 50360, 3913, 5689, true, string("rabuexsvxiasoarxxuhsqmvxdtordhdcbsvqvbrufgnvkutasrqhkwqcpbyvimqtyir"), string("eypznkosavcxiomvwgtqedpxtlaumhtepfk"));
	this->cexlvjwforwvs(true);
	this->restxdapzisexaqrlkdb(false, 25878);
	this->nougzkiamibvkz(true, 3760, 169, true, true, 1053, 632, 120);
	this->xcuxyljbfpe(3758, 1717, 4498, 600, 4561, 2719);
	this->zwqvowupdmakmwyzs(false, 805, false, string("flggclchescyjxuqunkqfihdvefvpzfjsnmzmsesgsqcilkwt"));
	this->jlmlizwtvipygytu(14434);
	this->byciplazfmpgdntf(string("duygpvqkhzipazuhesfvrvilphmhqhydpkgkydn"), 76399, 29293, 57415, 13805, true, string("ftdosgwohskohmppoxaoygxstvaokyzurpfwcceajqovkelibvthyuntdnq"));
	this->lusippovpqzpjmi(11930, string("xcveedqilncoafvioxeaqcalphixcqj"), 17729, false, 907, true, 3454, 2550);
	this->nzlrmtwgdgthxawifs(53331, 30409, string("sxviszxgeuwgfdlwgbuzu"), 17168, string("kxxxibkmxuftmdwsoftbgokqfkstiftmarnhxqafzdxyfapdkyqcjylkclipjs"), false, string("qrapaerwbtisbecfitjhfzywbqikcjgthgtzwsqbaaelhjrmk"));
	this->yfspqpyuucktywlne(2220, 7236, string("dijamzbmoxuy"));
	this->ivmtbpeyylhe(string("pbrztziwdoctyvgavbxhpwzclwvtjrgqziiwjeculrmgoqvzkihzmjfktaxksngazjhhnddeyibjfeayoeyhogdetwetys"), true, 3490, string("vkqxcdxqormbuasslbgywcklkdsho"), 89139, 66834, 8236);
	this->fkyzpvsyucxjgetet();
	this->yxbbunltnsqgrntbxpfeo(true, 3523, 29925, 22283, 404, string("zlufxbtkhzbrmlhfmbyvawjyztjimthdbojxfrrrxehrgkp"));
	this->sljmrwaiqmtalzhc(177);
	this->mllexlktcaxsqx();
	this->nezczlcmlchyhfuuzjfkd(48899, string("pgdxsnisjccvqcftpautaybojuaughfdtiqxfevqpikgalvdvvovtybecukysatsiqcboepkihakdvnkbtnzn"), 2713, 1714, 3551, true);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class pfnmptr {
public:
	bool xuuqlgckqpyi;
	int iztzgkxpc;
	string ebexit;
	int irjmzjxbtvc;
	int ayqzrkzpsejmrx;
	pfnmptr();
	bool ivkffpguquvtca(int wyaprxqht, string pnqrdrhvkwadux, double gxibsvm, string robyjvzmxatlod, int taoclh, string aawnbroqrvsrf, double lsjiobg, int fcfxyf, double vbwhuadgv, double yxkijqhasnbpuvy);

protected:
	int lsqeelyszrzylsi;
	string kskqkj;
	int rxfogspqt;
	string jsiqjotmclcqlaf;

	void xxveqkkemnjnln(bool wndwjcy);
	int yzzvyjqbfvmkmlsztrsasa(bool kjeybeikhitlfoj, string zoolfivwnjyp);
	bool ujqhsqjvdnvocpmi(bool cpjkqrrlfqpf, int hhibkzd, double oddggnjul, int kkcny);
	double vbvbamulgwikvmvmmmz(bool osnpmnnjfjtyc, double wbbharg, string aodkewrgbdmtm, string dzbepenskkf, bool ffukktafg, bool vujfq, string sekiihpu, int qwdwbubrbp, string lwooscvso);
	int bpxdukiygknlyirvt(int ftapida);
	string qakcnwadyptlbyxnyzwiz(bool nswlqbxfdpdy, string rtzloazqcebm, int pznhtd, int ubdjh, int egcqnoaltamtbdo, int mikxiz, int nntbyuvozjjfqrh, int zebonbugpq);
	int qrhauwqqlzodzkdsijpvzcx(double olwrggfbblprw, bool blpobgppq, int zpyuqpyq, double idfoswzjqa, bool tstvvqcfye, double txkuenmygvzjlf, string rzycysna, double okhhqmtduicmmy);
	bool xuekvanlsveaqmbvhaw(int eeefspnze, string khdhesbptpaunvy, bool eyzcpfvnc);
	int htbvgifxjnihgrmlsqatxpmp(bool zwpqw, bool rqtvgczgdycuk, string zjyjzmwqkdbdhry, bool wlwkpqlia, string zaiygs, bool jcljltfhzdsuick, int azigszbfvofwee, int hyhzbzcllfs, int pjhokhjs, string dlfaohwtfver);
	int wwdwpqkovqeybijqekmvin(int wxobgkuagc, bool buptntfosmm, bool xqamfdzfgbvqy, int ubkphllls, string ajtajw, int czgmsmlwlyzmhd, double iiuogkuldhtt, int nlnufzqpalh, int ejcyg, int rnwqgnftaca);

private:
	string xgxtpcgw;
	int lbxhspfuwguo;

	bool yoplnainhkz(string cbrvfqowwl, double qzddtktkstqgjwz, int lrmkwb, bool dhdcuhy, double nxknbrq, string qihmqxwobzh);
	bool bxschrfyqzqrtvttwv(string exexlprplpjptz, bool csepcs, double xvfocjcla, string ixpgum, int hmazjikmsf, string qxobpkp, bool slylvzjhfanhs);
	string cubboyqjkue(double qrqwciv, bool cxjgddslgevnzbr, double xbfvuf, int issysvh, bool ckslm, int mnngpkeyilkbb, double aqjpt, bool jjndcqu, int dvfhamdrdjlnnc, double kymdkditshohgl);
	string vutgotcsvmbvizuvlrjx(bool ejxanvacuu, double cfsbtmht, bool xynmlgz, int ealcyifxme, bool bqrmpthxzdomx, double uzliwa, string lucjkdbrad, string fzqeylsrrexm, string lcgwegdka);
	int crakoerpltq();
	int goswisehvy();
	double inshsyxwddhrc(string qmggzqfopevof, string sgbavqguq, bool rvhgiwpyku, bool rwgenqm, double klzswyzsvsqzjgo, int rngbacm, int movwlnao, double ottctytdjztbved, string smyznvfcpjl, string opxjwwpdrn);
	int kywhozngmiphdhtnvjxk(string hcjvqyftvmmef, bool jgaiebguguuzz);
	string thdmcsummaokqltxhj(string kmceblolg, string otiyhsxp, string hjeywrcynbyylr, bool hxyqpcoz, bool czafhvwrwfr);
	string lvruwqdyztnxrnrscynpfrb(double ilbifvdrkxqe, int uswotte);

};


bool pfnmptr::yoplnainhkz(string cbrvfqowwl, double qzddtktkstqgjwz, int lrmkwb, bool dhdcuhy, double nxknbrq, string qihmqxwobzh) {
	return true;
}

bool pfnmptr::bxschrfyqzqrtvttwv(string exexlprplpjptz, bool csepcs, double xvfocjcla, string ixpgum, int hmazjikmsf, string qxobpkp, bool slylvzjhfanhs) {
	return true;
}

string pfnmptr::cubboyqjkue(double qrqwciv, bool cxjgddslgevnzbr, double xbfvuf, int issysvh, bool ckslm, int mnngpkeyilkbb, double aqjpt, bool jjndcqu, int dvfhamdrdjlnnc, double kymdkditshohgl) {
	return string("totbqlzzkmakzskraexw");
}

string pfnmptr::vutgotcsvmbvizuvlrjx(bool ejxanvacuu, double cfsbtmht, bool xynmlgz, int ealcyifxme, bool bqrmpthxzdomx, double uzliwa, string lucjkdbrad, string fzqeylsrrexm, string lcgwegdka) {
	double frvqijl = 14892;
	int mydhd = 1799;
	double tczfybogwwu = 10030;
	bool umizmmgarf = false;
	int owfpp = 3164;
	int mxkljzqxpmmla = 2151;
	string fttkgkjgbeam = "wwmtzqlkbiumwawuhinzvjncgtfxvstwqoauvgiz";
	int httfpkqyu = 1800;
	bool tsgadqpdsuviy = true;
	if (string("wwmtzqlkbiumwawuhinzvjncgtfxvstwqoauvgiz") != string("wwmtzqlkbiumwawuhinzvjncgtfxvstwqoauvgiz")) {
		int kicwwbz;
		for (kicwwbz = 13; kicwwbz > 0; kicwwbz--) {
			continue;
		}
	}
	return string("euvzoanbuahraa");
}

int pfnmptr::crakoerpltq() {
	bool qaqiyyqrqt = true;
	bool tzzedgjqquf = true;
	bool rcyjheqdo = false;
	double cvzplbhsbqhq = 32531;
	double nyzlzmgekeic = 878;
	bool yiqaemkuscq = false;
	if (false != false) {
		int rhph;
		for (rhph = 76; rhph > 0; rhph--) {
			continue;
		}
	}
	return 51995;
}

int pfnmptr::goswisehvy() {
	string uyswr = "";
	string qtgrtebhfqpefhs = "rzgujsmfltobfwekftsqfnzhhurhmjecazamucrqhavdmaoawfhj";
	double vffmvm = 26469;
	double lraidhfrwi = 22513;
	double inqbns = 66143;
	double vortcb = 56564;
	double kvaukcqlaic = 21209;
	string lhtnvxonqino = "rlehxgvhmmcgpocblnulozxyahmacccjxdmer";
	int faqcvwttbn = 82;
	if (56564 == 56564) {
		int zcfzh;
		for (zcfzh = 41; zcfzh > 0; zcfzh--) {
			continue;
		}
	}
	if (66143 != 66143) {
		int holxvd;
		for (holxvd = 91; holxvd > 0; holxvd--) {
			continue;
		}
	}
	if (string("") == string("")) {
		int smbsjlkhrh;
		for (smbsjlkhrh = 26; smbsjlkhrh > 0; smbsjlkhrh--) {
			continue;
		}
	}
	return 31974;
}

double pfnmptr::inshsyxwddhrc(string qmggzqfopevof, string sgbavqguq, bool rvhgiwpyku, bool rwgenqm, double klzswyzsvsqzjgo, int rngbacm, int movwlnao, double ottctytdjztbved, string smyznvfcpjl, string opxjwwpdrn) {
	bool wcbppmfs = false;
	if (false == false) {
		int rtntkw;
		for (rtntkw = 76; rtntkw > 0; rtntkw--) {
			continue;
		}
	}
	if (false != false) {
		int aubrtu;
		for (aubrtu = 35; aubrtu > 0; aubrtu--) {
			continue;
		}
	}
	if (false == false) {
		int vgtouspju;
		for (vgtouspju = 77; vgtouspju > 0; vgtouspju--) {
			continue;
		}
	}
	if (false != false) {
		int fvxgeg;
		for (fvxgeg = 62; fvxgeg > 0; fvxgeg--) {
			continue;
		}
	}
	if (false != false) {
		int jnjazvkf;
		for (jnjazvkf = 38; jnjazvkf > 0; jnjazvkf--) {
			continue;
		}
	}
	return 72109;
}

int pfnmptr::kywhozngmiphdhtnvjxk(string hcjvqyftvmmef, bool jgaiebguguuzz) {
	int nzvdtzdkjykm = 547;
	string trczqhcbkllwxaw = "qnuxklldtwakhrgvkdtfxwdfncwegolmyruwgivfdcoyezdvyqdvadlyqevkcblhdulduzkyasyhqvspxzha";
	string squptyysawk = "tdrsftptkwcystojotwclmqtmuaobenlssxhwntkmmccmfmuyazdrsytuzskemzksehddwdpqbgawurlafhqry";
	string pmriyeoeo = "rmtermvghdawdwjqjzn";
	int mdrwtqqrdp = 461;
	double rendqwfyhznrnju = 74702;
	bool kcplkkbkeqpnvou = false;
	string xztolmm = "bqlqrbflleupkwqduqhgobeifryofjchjpqgzdkeplcastdsdbjkewnscjqwqcljkphwnusmvu";
	string ylivpceaptzho = "yzq";
	string mrsgdk = "fncdbijhkmwbmnutnucfpksnljjbemglcbyvgfwldryhgowhng";
	if (547 == 547) {
		int asuwc;
		for (asuwc = 92; asuwc > 0; asuwc--) {
			continue;
		}
	}
	if (74702 != 74702) {
		int fhyaqvfg;
		for (fhyaqvfg = 15; fhyaqvfg > 0; fhyaqvfg--) {
			continue;
		}
	}
	if (string("fncdbijhkmwbmnutnucfpksnljjbemglcbyvgfwldryhgowhng") == string("fncdbijhkmwbmnutnucfpksnljjbemglcbyvgfwldryhgowhng")) {
		int hvjebcc;
		for (hvjebcc = 60; hvjebcc > 0; hvjebcc--) {
			continue;
		}
	}
	if (string("yzq") != string("yzq")) {
		int jjrn;
		for (jjrn = 30; jjrn > 0; jjrn--) {
			continue;
		}
	}
	if (string("tdrsftptkwcystojotwclmqtmuaobenlssxhwntkmmccmfmuyazdrsytuzskemzksehddwdpqbgawurlafhqry") != string("tdrsftptkwcystojotwclmqtmuaobenlssxhwntkmmccmfmuyazdrsytuzskemzksehddwdpqbgawurlafhqry")) {
		int viuwv;
		for (viuwv = 8; viuwv > 0; viuwv--) {
			continue;
		}
	}
	return 93000;
}

string pfnmptr::thdmcsummaokqltxhj(string kmceblolg, string otiyhsxp, string hjeywrcynbyylr, bool hxyqpcoz, bool czafhvwrwfr) {
	bool xdljulqvxrlcnd = true;
	bool ipujty = true;
	int ojiqiptpktgfni = 1082;
	bool ktvoobwpdkpq = true;
	double mxpmjypqpye = 13551;
	int dolaueiqzjiap = 122;
	return string("shgibaulfkgbcxnttuh");
}

string pfnmptr::lvruwqdyztnxrnrscynpfrb(double ilbifvdrkxqe, int uswotte) {
	double egelugzcuuht = 10477;
	bool sjshncmahzw = true;
	bool nfnifh = false;
	double vhppekikhe = 10469;
	bool lazfvoqaagb = true;
	double pvnshtvim = 4360;
	int cjhtjaxegvp = 2155;
	string fhgqmjcnwqod = "postventcxvvpqqazsnadfceyqcnnoksjrazpuatwlqhwjxgizgwvoubwqismbcstessvrnrrbdvjmj";
	bool omczwgyaurkdhaz = true;
	int uudvvrtjqc = 1866;
	if (false == false) {
		int vgxxx;
		for (vgxxx = 6; vgxxx > 0; vgxxx--) {
			continue;
		}
	}
	if (true == true) {
		int lzndadjb;
		for (lzndadjb = 42; lzndadjb > 0; lzndadjb--) {
			continue;
		}
	}
	if (10477 != 10477) {
		int ei;
		for (ei = 8; ei > 0; ei--) {
			continue;
		}
	}
	if (1866 == 1866) {
		int gtvxlif;
		for (gtvxlif = 37; gtvxlif > 0; gtvxlif--) {
			continue;
		}
	}
	if (true != true) {
		int ijibe;
		for (ijibe = 86; ijibe > 0; ijibe--) {
			continue;
		}
	}
	return string("krlxanjdfnrerobbqxr");
}

void pfnmptr::xxveqkkemnjnln(bool wndwjcy) {
	string mfyecokrlia = "hylwyorhpaopaegpovhegflmdexaqqiaukiwtivdjgdwlfkrjlchufyuzizziq";
	bool wjvrrj = true;
	string zxaevr = "wsslfibauwdrrveuglwqoqhcvxuoj";
	string cgvtulajzduwq = "qchhmzwcvsgdwdosrwlgjlhxxohihbdurghlyadcahyquafknj";
	string orferhdkp = "pwpzxicfr";
	string nhxgwmwbmw = "zsoxhxcyeywsampisnjflmdlivhtbdspwdnldzoqtsmlyrxlhvyrfdvrzsdmcqubu";
	if (string("pwpzxicfr") == string("pwpzxicfr")) {
		int kgterv;
		for (kgterv = 85; kgterv > 0; kgterv--) {
			continue;
		}
	}
	if (true != true) {
		int qmkntgyo;
		for (qmkntgyo = 76; qmkntgyo > 0; qmkntgyo--) {
			continue;
		}
	}
	if (true == true) {
		int lonclewpcw;
		for (lonclewpcw = 8; lonclewpcw > 0; lonclewpcw--) {
			continue;
		}
	}

}

int pfnmptr::yzzvyjqbfvmkmlsztrsasa(bool kjeybeikhitlfoj, string zoolfivwnjyp) {
	bool mjdakjfmnpla = true;
	bool xedsd = false;
	bool aprmqzigzj = true;
	string zndyxggwq = "cfvpucz";
	int eqcbxnqfpllzr = 5467;
	int jkaywpl = 1286;
	int gnupushqudeyv = 4175;
	int odoyd = 5874;
	string qmpmgyvtcxq = "mjdthqskkuyuarcqqk";
	string pqwkuwl = "trlsofdcou";
	if (1286 == 1286) {
		int laxt;
		for (laxt = 59; laxt > 0; laxt--) {
			continue;
		}
	}
	if (true != true) {
		int iqxly;
		for (iqxly = 58; iqxly > 0; iqxly--) {
			continue;
		}
	}
	if (string("mjdthqskkuyuarcqqk") != string("mjdthqskkuyuarcqqk")) {
		int tzxplca;
		for (tzxplca = 28; tzxplca > 0; tzxplca--) {
			continue;
		}
	}
	if (true == true) {
		int niwk;
		for (niwk = 61; niwk > 0; niwk--) {
			continue;
		}
	}
	if (4175 != 4175) {
		int kam;
		for (kam = 83; kam > 0; kam--) {
			continue;
		}
	}
	return 1690;
}

bool pfnmptr::ujqhsqjvdnvocpmi(bool cpjkqrrlfqpf, int hhibkzd, double oddggnjul, int kkcny) {
	string ttcnpheairxsk = "iazhtzigipbugochbplvzpeamfgdovjhn";
	bool rmlqh = true;
	string udrbokj = "jeawmmuydfcwgv";
	string vufccookktj = "dnmxnflslbtjeoacjqoxgdvvschlfugvekycznfpavfdayksioovdqbdcdrnmicnelkekdeswxvinogmvjgcjecylku";
	double fyhdn = 21861;
	double nihuoziav = 7364;
	double fzpyq = 45126;
	bool fkllj = false;
	if (45126 == 45126) {
		int ad;
		for (ad = 42; ad > 0; ad--) {
			continue;
		}
	}
	if (true != true) {
		int udux;
		for (udux = 86; udux > 0; udux--) {
			continue;
		}
	}
	return false;
}

double pfnmptr::vbvbamulgwikvmvmmmz(bool osnpmnnjfjtyc, double wbbharg, string aodkewrgbdmtm, string dzbepenskkf, bool ffukktafg, bool vujfq, string sekiihpu, int qwdwbubrbp, string lwooscvso) {
	bool sxvbdzwgou = true;
	double xjlfk = 31615;
	bool kxueimfsygihea = true;
	bool eoewqxmgh = true;
	double bvditedy = 9293;
	string vnnbyijmkrxen = "alkrupobtrgrjjkqrrjluvlyrbxkpagsebhswwpyhnhxywxa";
	int kohpn = 1784;
	double yrkts = 4821;
	int gbwxqrka = 1611;
	if (31615 == 31615) {
		int utjhsiuwlt;
		for (utjhsiuwlt = 66; utjhsiuwlt > 0; utjhsiuwlt--) {
			continue;
		}
	}
	return 68649;
}

int pfnmptr::bpxdukiygknlyirvt(int ftapida) {
	bool zmsohpmorgalogb = true;
	string mwxqvvz = "rovqqpozfltmsqwrptvsbatwbanfxavmiupnxbmkienljlrldzspakgzpjzmfzm";
	double ijrea = 12531;
	double uciwfelrhftpf = 48136;
	bool qhmby = true;
	bool bexpd = true;
	double wsrfjiwq = 13912;
	string ddjwwzsmkt = "oldnkqvpy";
	return 48272;
}

string pfnmptr::qakcnwadyptlbyxnyzwiz(bool nswlqbxfdpdy, string rtzloazqcebm, int pznhtd, int ubdjh, int egcqnoaltamtbdo, int mikxiz, int nntbyuvozjjfqrh, int zebonbugpq) {
	int frfuu = 2645;
	int gprmmujrcvm = 971;
	bool mfrlvlcszknj = false;
	bool uorotnqzxhfm = false;
	string nbyxjrj = "hwqyuawvxsjlhqksauwgrgvhigi";
	bool qnylhqkiblbz = false;
	string wiovokovbghjgp = "vedbfhevdijczwzvkslgbexjqzkdsxzmvtfxdnophyoccohioilhbxtsahijyvctqrgjegdmgjeekoyfoeiikdr";
	bool kzufgjhwihhon = true;
	string svyyvnwtuluz = "sxtmkjsmbganztjdyqnerjoshgklonzvzmmlizbqnfeaaqnc";
	if (false == false) {
		int mjwtx;
		for (mjwtx = 29; mjwtx > 0; mjwtx--) {
			continue;
		}
	}
	if (string("sxtmkjsmbganztjdyqnerjoshgklonzvzmmlizbqnfeaaqnc") != string("sxtmkjsmbganztjdyqnerjoshgklonzvzmmlizbqnfeaaqnc")) {
		int ufncyxecb;
		for (ufncyxecb = 28; ufncyxecb > 0; ufncyxecb--) {
			continue;
		}
	}
	if (false != false) {
		int nipip;
		for (nipip = 80; nipip > 0; nipip--) {
			continue;
		}
	}
	if (true != true) {
		int ncviup;
		for (ncviup = 5; ncviup > 0; ncviup--) {
			continue;
		}
	}
	return string("pedrhiwagxorf");
}

int pfnmptr::qrhauwqqlzodzkdsijpvzcx(double olwrggfbblprw, bool blpobgppq, int zpyuqpyq, double idfoswzjqa, bool tstvvqcfye, double txkuenmygvzjlf, string rzycysna, double okhhqmtduicmmy) {
	string yjcnw = "dblnzqtlyvgfuoxefhkovtihwxuibqnmocudvasfmaltvrtrxiw";
	string eglphnpylfql = "djbfbmhqfmworondwicuplelppqozmesfsfnvskywzavvelhawcvmavedp";
	int dohmpxdxfuvw = 1788;
	int qybeklcw = 2217;
	bool qinzvev = false;
	bool wptsyvdzreea = false;
	if (false == false) {
		int hn;
		for (hn = 100; hn > 0; hn--) {
			continue;
		}
	}
	if (1788 == 1788) {
		int gyqj;
		for (gyqj = 51; gyqj > 0; gyqj--) {
			continue;
		}
	}
	return 3166;
}

bool pfnmptr::xuekvanlsveaqmbvhaw(int eeefspnze, string khdhesbptpaunvy, bool eyzcpfvnc) {
	string afakmhtcumzi = "tikhbomnesniumejijltwbhraoeleretjiyqfdweypmqmeubrqhkqlhyyexpgjuzlxszxpzdubtdhwabahvrbpnubvdcxhzuf";
	if (string("tikhbomnesniumejijltwbhraoeleretjiyqfdweypmqmeubrqhkqlhyyexpgjuzlxszxpzdubtdhwabahvrbpnubvdcxhzuf") != string("tikhbomnesniumejijltwbhraoeleretjiyqfdweypmqmeubrqhkqlhyyexpgjuzlxszxpzdubtdhwabahvrbpnubvdcxhzuf")) {
		int ootxnttcjv;
		for (ootxnttcjv = 21; ootxnttcjv > 0; ootxnttcjv--) {
			continue;
		}
	}
	if (string("tikhbomnesniumejijltwbhraoeleretjiyqfdweypmqmeubrqhkqlhyyexpgjuzlxszxpzdubtdhwabahvrbpnubvdcxhzuf") == string("tikhbomnesniumejijltwbhraoeleretjiyqfdweypmqmeubrqhkqlhyyexpgjuzlxszxpzdubtdhwabahvrbpnubvdcxhzuf")) {
		int mtdrd;
		for (mtdrd = 63; mtdrd > 0; mtdrd--) {
			continue;
		}
	}
	if (string("tikhbomnesniumejijltwbhraoeleretjiyqfdweypmqmeubrqhkqlhyyexpgjuzlxszxpzdubtdhwabahvrbpnubvdcxhzuf") != string("tikhbomnesniumejijltwbhraoeleretjiyqfdweypmqmeubrqhkqlhyyexpgjuzlxszxpzdubtdhwabahvrbpnubvdcxhzuf")) {
		int mvyjlwodv;
		for (mvyjlwodv = 16; mvyjlwodv > 0; mvyjlwodv--) {
			continue;
		}
	}
	if (string("tikhbomnesniumejijltwbhraoeleretjiyqfdweypmqmeubrqhkqlhyyexpgjuzlxszxpzdubtdhwabahvrbpnubvdcxhzuf") == string("tikhbomnesniumejijltwbhraoeleretjiyqfdweypmqmeubrqhkqlhyyexpgjuzlxszxpzdubtdhwabahvrbpnubvdcxhzuf")) {
		int cnqzjdjiko;
		for (cnqzjdjiko = 10; cnqzjdjiko > 0; cnqzjdjiko--) {
			continue;
		}
	}
	if (string("tikhbomnesniumejijltwbhraoeleretjiyqfdweypmqmeubrqhkqlhyyexpgjuzlxszxpzdubtdhwabahvrbpnubvdcxhzuf") == string("tikhbomnesniumejijltwbhraoeleretjiyqfdweypmqmeubrqhkqlhyyexpgjuzlxszxpzdubtdhwabahvrbpnubvdcxhzuf")) {
		int iuziip;
		for (iuziip = 19; iuziip > 0; iuziip--) {
			continue;
		}
	}
	return false;
}

int pfnmptr::htbvgifxjnihgrmlsqatxpmp(bool zwpqw, bool rqtvgczgdycuk, string zjyjzmwqkdbdhry, bool wlwkpqlia, string zaiygs, bool jcljltfhzdsuick, int azigszbfvofwee, int hyhzbzcllfs, int pjhokhjs, string dlfaohwtfver) {
	double pzbxdlpgscxfdy = 11042;
	bool hefrd = false;
	bool bqcypelpx = true;
	int ikgic = 1987;
	bool czuyloqqmqn = false;
	return 25119;
}

int pfnmptr::wwdwpqkovqeybijqekmvin(int wxobgkuagc, bool buptntfosmm, bool xqamfdzfgbvqy, int ubkphllls, string ajtajw, int czgmsmlwlyzmhd, double iiuogkuldhtt, int nlnufzqpalh, int ejcyg, int rnwqgnftaca) {
	string qobtzgs = "aqtnmylowqswxotsmmxn";
	int hbpamma = 1740;
	string prmchl = "brllnenbwbjktomecldvjjghfxptahyukkqwwnfsdlkgupklratkdxanrlpifvjpcffmvxzyrqnnzmcedaqa";
	bool slcjn = false;
	bool zklprztkwql = true;
	bool xhnlc = true;
	bool lcoghnrpyovbtvn = false;
	if (false != false) {
		int li;
		for (li = 29; li > 0; li--) {
			continue;
		}
	}
	if (true == true) {
		int gidwq;
		for (gidwq = 83; gidwq > 0; gidwq--) {
			continue;
		}
	}
	if (false == false) {
		int wyhlv;
		for (wyhlv = 36; wyhlv > 0; wyhlv--) {
			continue;
		}
	}
	if (string("brllnenbwbjktomecldvjjghfxptahyukkqwwnfsdlkgupklratkdxanrlpifvjpcffmvxzyrqnnzmcedaqa") != string("brllnenbwbjktomecldvjjghfxptahyukkqwwnfsdlkgupklratkdxanrlpifvjpcffmvxzyrqnnzmcedaqa")) {
		int bthmp;
		for (bthmp = 88; bthmp > 0; bthmp--) {
			continue;
		}
	}
	return 12936;
}

bool pfnmptr::ivkffpguquvtca(int wyaprxqht, string pnqrdrhvkwadux, double gxibsvm, string robyjvzmxatlod, int taoclh, string aawnbroqrvsrf, double lsjiobg, int fcfxyf, double vbwhuadgv, double yxkijqhasnbpuvy) {
	bool vzhjwsqv = true;
	string kxwdbjs = "fvczgnyyxxcfoegpnunebgdfouspwvvbg";
	double auylb = 3296;
	int huxvopf = 5048;
	bool knymtsvvvqh = true;
	int gtawzax = 695;
	double xsaxxpffobfrtt = 18004;
	int vcnfzkqla = 7477;
	bool dkmfwpjkwindpzd = false;
	int fvssnykrncm = 1807;
	if (1807 == 1807) {
		int aprp;
		for (aprp = 54; aprp > 0; aprp--) {
			continue;
		}
	}
	return true;
}

pfnmptr::pfnmptr() {
	this->ivkffpguquvtca(985, string("np"), 10786, string("izdotmhtrfhsyeyqvkfplxzucuqdlv"), 70, string("piejxnazkjtzpkbmehnltmzteubkzivesddbhxwmfjkpsmupyrnkcqnrxpkih"), 56799, 697, 12080, 15193);
	this->xxveqkkemnjnln(false);
	this->yzzvyjqbfvmkmlsztrsasa(false, string("feszqkylxcembktxdpdmliisdqprkkrspogforwrzwwuutbnucvupamjaoaqenzgsmlafkvyweycciizqfqiiagui"));
	this->ujqhsqjvdnvocpmi(true, 2214, 17578, 1305);
	this->vbvbamulgwikvmvmmmz(false, 56725, string("ciodzsjjgclpmdlgkhvnwhbqbwfgncfzzdit"), string("tgbjquabybjcnsygxwddzwglzmvhvufnbzksddtswmibxxrjaulatakadisxwddsinwzszzttbucawdcsibehnxjs"), true, true, string("apmyeanicwnhfpsenjhorkfnppvxbcesngfzbrdgehfbdwbczmdoqaegizhzsxccusksjsgmzbcdt"), 469, string("sxokttcbtuokvcofjmmltkxhohawp"));
	this->bpxdukiygknlyirvt(8029);
	this->qakcnwadyptlbyxnyzwiz(true, string("wexpenmykwttzg"), 5590, 2120, 3266, 5021, 4975, 1106);
	this->qrhauwqqlzodzkdsijpvzcx(7290, true, 1206, 26, true, 14722, string("lhrjtt"), 42066);
	this->xuekvanlsveaqmbvhaw(223, string("lnanieoataupyluzslablpybogdibpysnfmctlmxeqzfwfryugzhynhxxkmvkllevagablxbysagzde"), true);
	this->htbvgifxjnihgrmlsqatxpmp(true, true, string("jybmneolcrktuthsdwyutglixuiyfqaptqmoifbdlepzmytuidqcoiqvytweaweocdkrssvlykkqxyuhked"), false, string("dxuqtgeznwmpydldiqrjbnldizhwfcqxoodydcdvdwmtdxjvidnhmnsmhmalewshtofboldwwwwmiuffzmmeteshkdwl"), false, 91, 2106, 2634, string("metgqrqqruwitdleceqjvhjjwizdwiykmhqbejcdtxcnmwpdmufcgdssufkjqgsdcoke"));
	this->wwdwpqkovqeybijqekmvin(3840, false, false, 1516, string("twavjvadxzkephvvenlxqfmocodmferumap"), 451, 25283, 3591, 4652, 1140);
	this->yoplnainhkz(string("awwbcobaecqjmlifacqbgwxtozsqzyedupqcsezerhfrdsetwnzwppwbrbvzpxiqrcowivqdolpbmhofthezbvzmgrctvosxgbw"), 25149, 3734, false, 27300, string("hxhbqvxjpqfdwxcivesgcrnioxaejqptwgdghffxwzcxlcgihikhtkizfgsxoekkkuqsahkpnxrweofqwmcsacyqxupfma"));
	this->bxschrfyqzqrtvttwv(string("dwuyumiyllygjiikwwerled"), false, 52665, string("sgxrtbwjjthbmoyiwtdjaxgwfuvs"), 1593, string("ossuvfkbkmegdsyqbdodquwcpshcvpzkfnndkhenfhlcapwhkgyfqogzoxbnyygzoixsddhw"), true);
	this->cubboyqjkue(5304, false, 18665, 230, true, 838, 35670, true, 3020, 3554);
	this->vutgotcsvmbvizuvlrjx(true, 33155, true, 8435, false, 15627, string("nhbojzehnynbkmkrmkpfbkfzroubtedrfujzpmfdeyaywcmmwbywtszaoidbitbt"), string("muokeduhitcxzwnvrp"), string("pfyyrpahjfhzbnttjzfyqwtxymqfcklkzjujonabtmsvzzcfizyzuezoqcuzofsadiyawhragkuaorn"));
	this->crakoerpltq();
	this->goswisehvy();
	this->inshsyxwddhrc(string("mrnkxzlrlfweqzcyyxkkvaruxcdwihcncvawvlxrzkuzzqrocmuwvmuj"), string("dbodzz"), true, true, 11767, 992, 678, 57758, string("aksuobwopopbxtkrptgypuwszliiuqhferbwwf"), string("kxdogazotdclkvqc"));
	this->kywhozngmiphdhtnvjxk(string("csktbutooilzjhlounojuybwnnnwvplwboyvypvx"), false);
	this->thdmcsummaokqltxhj(string("qxqvjyhcdshpkzwrkrwvmpvqqoajlimyejfbbkeqenxkwqpkwxebsaonttfrocohhqv"), string("omyfyhbawdwlsfygiujhpmakzwpygmnsasjnjealivvtxxhkitraejkreqkxnxefiunulwwrbqbksogthyvlatlapoprf"), string("xpazyiyablheytuynngksvenyqxaddtohbvaego"), false, false);
	this->lvruwqdyztnxrnrscynpfrb(33013, 2431);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class kkzobdr {
public:
	int ohmdokeq;
	int imvadfosxmhaot;
	kkzobdr();
	bool okyfykzfjbdyfwzj(string iwtkysbq, double xqpriyvld, bool fwdqieyarbz, double dyahzh);

protected:
	bool hdbuf;
	int ldkiahxghcyl;

	int eaaymlmkmpblzlitlrtqqogt(double auklokcoxa, int fvuiqyoy, double wrkxrrwc, double usyxlqwx, bool hbapj, int wkexe, bool znosupvytzy, double lmuzlhutp, int ciwlkug, string ofkiult);

private:
	double xrezeevevwn;
	int aeblxixwoafd;
	string ejizrp;

	string forlemjzyvygquxwfeyhvpaoq(int ennqkmxaqr, string rtfwurmy, bool gndzw, double tdkitcbpyvq, int nnbsplsimlbx, int spzdu, double railrhxcqiv, string hdqrbnikwju, double ocpafbcslasywvf);
	string sdurquyjileod(bool ugrrvt, string ulzerkmhlys, double acqml, string lNAIicmhrrvsy, int qtzbxaktoz, double lcexqsay, string otcza);
	double tylorfrsar(string pgruby, double vtovhguca, bool jmvjt, string vziejfcfj, double mriqi);
	int srlppzvvclhirzz(int bqotokbqclzml, int ijnbon, double rpmsehezxpafbk);
	string fbcpjnlclyafskgzolhevovs(string yispgbvn, bool pcofhuatojjk, double czcmmuhfjvbis);
	string zgcfzqjnfzuinmz(bool rhble, string keumabuvw);
	string fsfcyevrraxtnpchmctiqbyik(double olsqxgrcnk, bool eqmiexhdx, string hgizzogys, double zabjenq, string ypcpee, int xlpojttpizkweyf);
	double egssfkosuzsvzianns(string pjwveovp, int ojfkbjqvfkdc, double qozgpwabixpxz, string dwujawxbrjm, bool dujghbs);
	void edzlxtlxuocugxva(bool evrvr, string bavovzo, bool qcicewfqoi, double jqytpe, bool opazhqjmhh, int nxollhsey, double ykvnejfwl, double gdaidh, string rinwqbykxngz);

};


string kkzobdr::forlemjzyvygquxwfeyhvpaoq(int ennqkmxaqr, string rtfwurmy, bool gndzw, double tdkitcbpyvq, int nnbsplsimlbx, int spzdu, double railrhxcqiv, string hdqrbnikwju, double ocpafbcslasywvf) {
	double wiehhginxnpx = 45606;
	bool zqavqqk = true;
	bool rylciplpngkzu = false;
	string wtjfbxqj = "goxnsxgfmwecjkftuusccyxlewpkbpooayfkoxkd";
	if (string("goxnsxgfmwecjkftuusccyxlewpkbpooayfkoxkd") != string("goxnsxgfmwecjkftuusccyxlewpkbpooayfkoxkd")) {
		int od;
		for (od = 60; od > 0; od--) {
			continue;
		}
	}
	if (string("goxnsxgfmwecjkftuusccyxlewpkbpooayfkoxkd") == string("goxnsxgfmwecjkftuusccyxlewpkbpooayfkoxkd")) {
		int xdamhp;
		for (xdamhp = 31; xdamhp > 0; xdamhp--) {
			continue;
		}
	}
	if (45606 == 45606) {
		int spomugdq;
		for (spomugdq = 94; spomugdq > 0; spomugdq--) {
			continue;
		}
	}
	return string("piacfbbvsrhdx");
}

string kkzobdr::sdurquyjileod(bool ugrrvt, string ulzerkmhlys, double acqml, string lNAIicmhrrvsy, int qtzbxaktoz, double lcexqsay, string otcza) {
	int tmshublxim = 3778;
	double drglwrpfsthgrl = 70210;
	string wwkjoh = "srgmftbsgcqhqtaednjqdcmcquijmervegpsejdgoqnclcjdhlq";
	int yegrrqcmdkln = 4651;
	double vxbarrfviev = 14493;
	string upapzzxzklzmies = "j";
	string tzhvezyrgiur = "izouztyoqts";
	int dxrrwbg = 2602;
	double szfilqramvqfpmt = 39226;
	bool khgtkzzrpkzaln = true;
	if (3778 == 3778) {
		int pw;
		for (pw = 33; pw > 0; pw--) {
			continue;
		}
	}
	if (4651 != 4651) {
		int dwdyibo;
		for (dwdyibo = 60; dwdyibo > 0; dwdyibo--) {
			continue;
		}
	}
	if (70210 != 70210) {
		int ngbteu;
		for (ngbteu = 32; ngbteu > 0; ngbteu--) {
			continue;
		}
	}
	if (true == true) {
		int hu;
		for (hu = 16; hu > 0; hu--) {
			continue;
		}
	}
	return string("fyvcvt");
}

double kkzobdr::tylorfrsar(string pgruby, double vtovhguca, bool jmvjt, string vziejfcfj, double mriqi) {
	int zsyqfsfqwpgzdb = 4517;
	int fsrcnxqzy = 9558;
	bool budtatqnfeo = true;
	return 98232;
}

int kkzobdr::srlppzvvclhirzz(int bqotokbqclzml, int ijnbon, double rpmsehezxpafbk) {
	int tiimxttpo = 3079;
	int rnfxett = 4539;
	string bbdvcwimysawzp = "yimzayvqauyptunpfyknastzzxaiqteyfsmbvkakomvfwzwywlsqaxlqsaqfmsziaimcaymttpfmszpvivcpvtqudgnoknagje";
	if (3079 == 3079) {
		int cmio;
		for (cmio = 12; cmio > 0; cmio--) {
			continue;
		}
	}
	if (string("yimzayvqauyptunpfyknastzzxaiqteyfsmbvkakomvfwzwywlsqaxlqsaqfmsziaimcaymttpfmszpvivcpvtqudgnoknagje") != string("yimzayvqauyptunpfyknastzzxaiqteyfsmbvkakomvfwzwywlsqaxlqsaqfmsziaimcaymttpfmszpvivcpvtqudgnoknagje")) {
		int ihi;
		for (ihi = 83; ihi > 0; ihi--) {
			continue;
		}
	}
	if (3079 == 3079) {
		int npb;
		for (npb = 52; npb > 0; npb--) {
			continue;
		}
	}
	if (4539 == 4539) {
		int ntbles;
		for (ntbles = 36; ntbles > 0; ntbles--) {
			continue;
		}
	}
	if (3079 != 3079) {
		int dkl;
		for (dkl = 5; dkl > 0; dkl--) {
			continue;
		}
	}
	return 51487;
}

string kkzobdr::fbcpjnlclyafskgzolhevovs(string yispgbvn, bool pcofhuatojjk, double czcmmuhfjvbis) {
	double pwyqasrfec = 37848;
	int dlkneagcmtkrz = 7021;
	double rpxsuqin = 3915;
	double nejsbmq = 26055;
	string horadfnxl = "najgxdsfpdksmtckuoyrtyynrvxjzkdupajscyizlroip";
	bool vwtguksic = true;
	if (37848 != 37848) {
		int dkjrl;
		for (dkjrl = 47; dkjrl > 0; dkjrl--) {
			continue;
		}
	}
	if (string("najgxdsfpdksmtckuoyrtyynrvxjzkdupajscyizlroip") != string("najgxdsfpdksmtckuoyrtyynrvxjzkdupajscyizlroip")) {
		int opc;
		for (opc = 4; opc > 0; opc--) {
			continue;
		}
	}
	if (37848 != 37848) {
		int rwqc;
		for (rwqc = 49; rwqc > 0; rwqc--) {
			continue;
		}
	}
	if (true == true) {
		int owkfrpqy;
		for (owkfrpqy = 71; owkfrpqy > 0; owkfrpqy--) {
			continue;
		}
	}
	if (3915 != 3915) {
		int yf;
		for (yf = 14; yf > 0; yf--) {
			continue;
		}
	}
	return string("zvjq");
}

string kkzobdr::zgcfzqjnfzuinmz(bool rhble, string keumabuvw) {
	int oyudcdryekesomv = 2703;
	double uvrsmbhbdqxzxyq = 1072;
	bool zxoerdgrfydx = true;
	if (1072 != 1072) {
		int xlgpd;
		for (xlgpd = 86; xlgpd > 0; xlgpd--) {
			continue;
		}
	}
	if (true == true) {
		int avzdwmavs;
		for (avzdwmavs = 28; avzdwmavs > 0; avzdwmavs--) {
			continue;
		}
	}
	if (2703 != 2703) {
		int hpr;
		for (hpr = 73; hpr > 0; hpr--) {
			continue;
		}
	}
	return string("mforzqldqmzvt");
}

string kkzobdr::fsfcyevrraxtnpchmctiqbyik(double olsqxgrcnk, bool eqmiexhdx, string hgizzogys, double zabjenq, string ypcpee, int xlpojttpizkweyf) {
	return string("uqb");
}

double kkzobdr::egssfkosuzsvzianns(string pjwveovp, int ojfkbjqvfkdc, double qozgpwabixpxz, string dwujawxbrjm, bool dujghbs) {
	return 45982;
}

void kkzobdr::edzlxtlxuocugxva(bool evrvr, string bavovzo, bool qcicewfqoi, double jqytpe, bool opazhqjmhh, int nxollhsey, double ykvnejfwl, double gdaidh, string rinwqbykxngz) {
	string aeoqaurjfmwgj = "mnovzisdvksagzkkzhrvarmgfplfekpfektcnhyjiepksxcchchimyzwpvazwpow";
	if (string("mnovzisdvksagzkkzhrvarmgfplfekpfektcnhyjiepksxcchchimyzwpvazwpow") == string("mnovzisdvksagzkkzhrvarmgfplfekpfektcnhyjiepksxcchchimyzwpvazwpow")) {
		int bileawdj;
		for (bileawdj = 60; bileawdj > 0; bileawdj--) {
			continue;
		}
	}
	if (string("mnovzisdvksagzkkzhrvarmgfplfekpfektcnhyjiepksxcchchimyzwpvazwpow") != string("mnovzisdvksagzkkzhrvarmgfplfekpfektcnhyjiepksxcchchimyzwpvazwpow")) {
		int vyiwosgnux;
		for (vyiwosgnux = 23; vyiwosgnux > 0; vyiwosgnux--) {
			continue;
		}
	}
	if (string("mnovzisdvksagzkkzhrvarmgfplfekpfektcnhyjiepksxcchchimyzwpvazwpow") == string("mnovzisdvksagzkkzhrvarmgfplfekpfektcnhyjiepksxcchchimyzwpvazwpow")) {
		int wjhk;
		for (wjhk = 25; wjhk > 0; wjhk--) {
			continue;
		}
	}
	if (string("mnovzisdvksagzkkzhrvarmgfplfekpfektcnhyjiepksxcchchimyzwpvazwpow") != string("mnovzisdvksagzkkzhrvarmgfplfekpfektcnhyjiepksxcchchimyzwpvazwpow")) {
		int abso;
		for (abso = 51; abso > 0; abso--) {
			continue;
		}
	}

}

int kkzobdr::eaaymlmkmpblzlitlrtqqogt(double auklokcoxa, int fvuiqyoy, double wrkxrrwc, double usyxlqwx, bool hbapj, int wkexe, bool znosupvytzy, double lmuzlhutp, int ciwlkug, string ofkiult) {
	double uzqqcgzgjdzcsrl = 39579;
	int vxeby = 934;
	bool ajktdclegui = false;
	string eafgi = "xyyssgdewwhgykugqwqmkgzdcmjmieebalbeuddggdbsurjxozzqkq";
	double jgapdirwgir = 54037;
	int nhgtooqmllhxaj = 1773;
	string hzxpb = "nttyfdtxyqkyrosxeswgizxmrdojkpabfwuhzltiadfcidnjxdfpprnqvqpt";
	double zjddcj = 18091;
	if (18091 != 18091) {
		int cq;
		for (cq = 78; cq > 0; cq--) {
			continue;
		}
	}
	return 41847;
}

bool kkzobdr::okyfykzfjbdyfwzj(string iwtkysbq, double xqpriyvld, bool fwdqieyarbz, double dyahzh) {
	int xeamrqsgvjh = 1334;
	string tfhdh = "efebujffhfvpsgyfphtgtsqegwymoggflyzbixdlogndmvrnwoettnxneyfjyivesqhpl";
	string sizgmhueiv = "rvaoeynvgbjpesosrspimxzeeylgowjylkminzgfosdjkccyehznbeigwqtqxwjoisouinmcmx";
	string dcfosytuizx = "ziexjlubgfeovynbupzoapxkxzhmwfvtalmubyplm";
	double tvhudyotno = 4883;
	double wwvmhyujgeu = 77588;
	bool qdongzivqten = false;
	string nmwpod = "zunaaovveefzjglqvelxqnlsfzpwkltbnotljcnprqakuwlmzsllogsadpxxt";
	string zljnzfjeekg = "qyxgfgbusuavnrkpkejvzofuudasvkpynnyxesatllxnggeamowsvoytvrueby";
	double ohyvxify = 2287;
	if (string("zunaaovveefzjglqvelxqnlsfzpwkltbnotljcnprqakuwlmzsllogsadpxxt") == string("zunaaovveefzjglqvelxqnlsfzpwkltbnotljcnprqakuwlmzsllogsadpxxt")) {
		int tznn;
		for (tznn = 87; tznn > 0; tznn--) {
			continue;
		}
	}
	if (string("ziexjlubgfeovynbupzoapxkxzhmwfvtalmubyplm") == string("ziexjlubgfeovynbupzoapxkxzhmwfvtalmubyplm")) {
		int mh;
		for (mh = 51; mh > 0; mh--) {
			continue;
		}
	}
	if (77588 != 77588) {
		int yjfvoqwr;
		for (yjfvoqwr = 96; yjfvoqwr > 0; yjfvoqwr--) {
			continue;
		}
	}
	if (false != false) {
		int nxaflwwxft;
		for (nxaflwwxft = 43; nxaflwwxft > 0; nxaflwwxft--) {
			continue;
		}
	}
	if (string("rvaoeynvgbjpesosrspimxzeeylgowjylkminzgfosdjkccyehznbeigwqtqxwjoisouinmcmx") == string("rvaoeynvgbjpesosrspimxzeeylgowjylkminzgfosdjkccyehznbeigwqtqxwjoisouinmcmx")) {
		int qcayye;
		for (qcayye = 27; qcayye > 0; qcayye--) {
			continue;
		}
	}
	return false;
}

kkzobdr::kkzobdr() {
	this->okyfykzfjbdyfwzj(string("bgscammtxosvcjhakzemrlnbed"), 43892, true, 3650);
	this->eaaymlmkmpblzlitlrtqqogt(74682, 4526, 3477, 9306, false, 4329, true, 27927, 1510, string("velkghxpbfnktsrkzgrqribipxgzhojkdsbteqsbwjjvsmolkghhuajtuwtog"));
	this->forlemjzyvygquxwfeyhvpaoq(2030, string("gwfihdvqazdssmwrtdhctqmlzkgkpakgkzirkmgxzsjzbbi"), true, 40820, 2262, 717, 40914, string("czfacvkgtntqaofbdhufdkdvplzagzedxrphfvomuxsoydebamlszgrxzlrkbsqgntpp"), 28680);
	this->sdurquyjileod(true, string("lgeunhoatojtmlpngttfdyzqrnezikhaujfhyxwlonkovzzolsasrbrabfadtakmufe"), 26982, string("zxuwbcwuuvrdvrzvvhzkrwmbysfnldnphagatmrdaykjwiioghekgkioeutrvewshrewjsidhtn"), 364, 10817, string("cndddfwkfcqhqpssmuhiotvdmnuzbiddkxoyttkfewxksncpgctrstqxgachqqjffiufyjeqnpwncskz"));
	this->tylorfrsar(string("dfdctlguwesesubfvnmgulygnqqhcbkmaczbarmxtfdwilxedgbdnjskiuljeigthaooxsyijirexrf"), 70612, false, string("wsvrbkldqiljr"), 86879);
	this->srlppzvvclhirzz(157, 6707, 4159);
	this->fbcpjnlclyafskgzolhevovs(string("fgmtmabphisyzywimxubpimpyvycjskooysxbvnoqdsazmzrchmkeyqpqpadpkgmmkjrbyrn"), false, 36848);
	this->zgcfzqjnfzuinmz(true, string("iotjqunoblgqnzcgovqilnvtpjnawonaldmfaafwkwshrzlvaobpviwfow"));
	this->fsfcyevrraxtnpchmctiqbyik(78035, false, string("kgysx"), 19577, string("ussnmamkehksdmfbhbjezzanxvmjzrhcjmnpguxxjrdmqkbfaexjyifhlqaqkieoykhffkqtqazuptyk"), 3593);
	this->egssfkosuzsvzianns(string("vehvpodwpahsrifvgzebpouryrjsuiltllmujfgzlrmwhearaolexbositegkblzoxpzteajwqvgz"), 5864, 53487, string("bey"), true);
	this->edzlxtlxuocugxva(true, string("jibkbxtwkjpcevbcklpvnpvxcdbtrglwomvyfwzgrxvtdetneuvtjqovyrobhfikbkxomofijb"), false, 3870, true, 1740, 19348, 17998, string("yoyrtszneyrdjahuaxqlrnbvhzlqxsvirpbfmgcxsaajkeviwhmorwtrwxyejbraozyborfarunkfnpubixrcycunnuevkb"));
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class dfgbnwq {
public:
	int ueywlqae;
	double hqrgo;
	string oxmmiwgnemuwhlq;
	bool nngrpqtxbdkhb;
	int gznhvwdf;
	dfgbnwq();
	bool hfxyhgiadgnn(bool vfpyo, string bbnacszub);
	string zqooatqrphpzbqdfj(int gdaygpuh, bool tnelazsvcuawc, double qugtbdiakh, int pbrvpttbj);
	bool nqkkykvuga(double nkclgcne, bool rxdrzc);
	void cmexbeptjoiqvtfqlyjwyj(int eqiwa, int uwscvjgivvysf, bool iafrqgnyio, double dubcdrwcllkahb, bool wbqzthj);

protected:
	string uemukvuxfwoue;

	void lquclsffhgdqtugvnenyleid(bool asbwlzwklwggbyb, int oabrqwr, string pxxiwvuljbz, double pmihq, double lmfkzmcwbwp);
	void ctkbowawphjslgka(double hwavfuwcvlddv, bool omxtqahpyexk);
	bool xmipbwbhdy(double zqnxcxhzlfugrvg, double xkzzswq, double gbggrnnciv, bool ximhb, string nyhbuejnvu, string pvuej, int kfuqbmzc);
	bool hayzcshlfrtoxlmodfl(int vhxxpflvpklns, bool upstucbxdxlzh, string vqvqtdlzzpi);
	void wlyaxsvtocdggymsiozacnu(int fafntelqqqkohbw, double yjamljzhrxyrgq, double lirslmp, bool frnopdfhzhd);
	double ulrtxmwhsmdbdaqjtd(bool savbbcdnti, bool ggpnzujmpnqtnm, int xgdyuttezkd, string bdkouqqtp);
	double ivmekptcupwswcqrsijav(double maimspopa, string fvxrrdtemk, string ffvblbsi, double afyarghpse, int zjkszwaytdxehvy, bool kcgdgicxodcirh, int zrzrak, bool iokoabu, double casltzqwif);

private:
	bool opspci;
	string lnmjoshmonouzc;
	int jsadpmpudo;

	double qltwwpahlaimsyyzeqkloea();
	bool jkuuczvrsmabvysijynisyjg(double wngpk, double wosndnndxkuo);

};


double dfgbnwq::qltwwpahlaimsyyzeqkloea() {
	bool crdxlynkdcarr = true;
	string gcokrukpmwin = "vwspkrddgeqdtwglsuyyfumotyojqniyydchtousguqjeualewpvcorckdjvraunhcihbxcmtiatczmxhiqpboihvr";
	int chgtke = 1540;
	bool gurva = true;
	if (true == true) {
		int ul;
		for (ul = 44; ul > 0; ul--) {
			continue;
		}
	}
	return 1652;
}

bool dfgbnwq::jkuuczvrsmabvysijynisyjg(double wngpk, double wosndnndxkuo) {
	string tiimhmlen = "iuajbdidwknljafpbavnpytdjmmrypjyxprxvigbgvzgzmgqauyuoppezhm";
	string nghnezlcxp = "dgqykonrvzptghsbenziohufpejodiiuol";
	if (string("iuajbdidwknljafpbavnpytdjmmrypjyxprxvigbgvzgzmgqauyuoppezhm") != string("iuajbdidwknljafpbavnpytdjmmrypjyxprxvigbgvzgzmgqauyuoppezhm")) {
		int yb;
		for (yb = 31; yb > 0; yb--) {
			continue;
		}
	}
	if (string("iuajbdidwknljafpbavnpytdjmmrypjyxprxvigbgvzgzmgqauyuoppezhm") == string("iuajbdidwknljafpbavnpytdjmmrypjyxprxvigbgvzgzmgqauyuoppezhm")) {
		int dnowutvcwp;
		for (dnowutvcwp = 14; dnowutvcwp > 0; dnowutvcwp--) {
			continue;
		}
	}
	if (string("iuajbdidwknljafpbavnpytdjmmrypjyxprxvigbgvzgzmgqauyuoppezhm") != string("iuajbdidwknljafpbavnpytdjmmrypjyxprxvigbgvzgzmgqauyuoppezhm")) {
		int kgjqzpdg;
		for (kgjqzpdg = 100; kgjqzpdg > 0; kgjqzpdg--) {
			continue;
		}
	}
	return true;
}

void dfgbnwq::lquclsffhgdqtugvnenyleid(bool asbwlzwklwggbyb, int oabrqwr, string pxxiwvuljbz, double pmihq, double lmfkzmcwbwp) {
	int biynrg = 1205;
	int shwewhfxjyojoqy = 4155;
	if (4155 == 4155) {
		int ngejp;
		for (ngejp = 76; ngejp > 0; ngejp--) {
			continue;
		}
	}

}

void dfgbnwq::ctkbowawphjslgka(double hwavfuwcvlddv, bool omxtqahpyexk) {

}

bool dfgbnwq::xmipbwbhdy(double zqnxcxhzlfugrvg, double xkzzswq, double gbggrnnciv, bool ximhb, string nyhbuejnvu, string pvuej, int kfuqbmzc) {
	bool ewttlglptcpdce = false;
	double gufknwodqbqg = 11522;
	int mcnaiymoxsq = 4879;
	string qvsdft = "gqojouesanvopw";
	bool boxeuwrne = true;
	int wehzwilrpve = 5508;
	int rmrhkr = 985;
	bool prsbiqkysofmn = false;
	bool pklzro = false;
	double oaysvii = 45670;
	if (true == true) {
		int nlp;
		for (nlp = 6; nlp > 0; nlp--) {
			continue;
		}
	}
	if (false != false) {
		int tp;
		for (tp = 7; tp > 0; tp--) {
			continue;
		}
	}
	return false;
}

bool dfgbnwq::hayzcshlfrtoxlmodfl(int vhxxpflvpklns, bool upstucbxdxlzh, string vqvqtdlzzpi) {
	string wklwta = "lrbmewr";
	if (string("lrbmewr") == string("lrbmewr")) {
		int bjamjmgmr;
		for (bjamjmgmr = 60; bjamjmgmr > 0; bjamjmgmr--) {
			continue;
		}
	}
	if (string("lrbmewr") == string("lrbmewr")) {
		int yooi;
		for (yooi = 98; yooi > 0; yooi--) {
			continue;
		}
	}
	if (string("lrbmewr") == string("lrbmewr")) {
		int dm;
		for (dm = 30; dm > 0; dm--) {
			continue;
		}
	}
	return true;
}

void dfgbnwq::wlyaxsvtocdggymsiozacnu(int fafntelqqqkohbw, double yjamljzhrxyrgq, double lirslmp, bool frnopdfhzhd) {

}

double dfgbnwq::ulrtxmwhsmdbdaqjtd(bool savbbcdnti, bool ggpnzujmpnqtnm, int xgdyuttezkd, string bdkouqqtp) {
	string mcwqenszotbtjvi = "hp";
	bool dvrfsfykkmmyspm = true;
	if (true == true) {
		int ajwv;
		for (ajwv = 55; ajwv > 0; ajwv--) {
			continue;
		}
	}
	if (string("hp") == string("hp")) {
		int rchtu;
		for (rchtu = 77; rchtu > 0; rchtu--) {
			continue;
		}
	}
	if (true == true) {
		int jsxo;
		for (jsxo = 38; jsxo > 0; jsxo--) {
			continue;
		}
	}
	if (string("hp") == string("hp")) {
		int gpefwsw;
		for (gpefwsw = 18; gpefwsw > 0; gpefwsw--) {
			continue;
		}
	}
	return 74477;
}

double dfgbnwq::ivmekptcupwswcqrsijav(double maimspopa, string fvxrrdtemk, string ffvblbsi, double afyarghpse, int zjkszwaytdxehvy, bool kcgdgicxodcirh, int zrzrak, bool iokoabu, double casltzqwif) {
	int lobhndvi = 733;
	int qvurhvsj = 3338;
	string ohpjepajged = "orzvhjzlnixomzjlpktirwnyqbyqctgx";
	bool lkqxmdirqbbn = true;
	double goikqez = 28257;
	double tvlfqohyymro = 2435;
	double sffvmqtij = 2737;
	int ynfuvuohlnkfde = 2560;
	if (2737 == 2737) {
		int adonmsejw;
		for (adonmsejw = 34; adonmsejw > 0; adonmsejw--) {
			continue;
		}
	}
	if (string("orzvhjzlnixomzjlpktirwnyqbyqctgx") == string("orzvhjzlnixomzjlpktirwnyqbyqctgx")) {
		int ecqwisjwnp;
		for (ecqwisjwnp = 14; ecqwisjwnp > 0; ecqwisjwnp--) {
			continue;
		}
	}
	if (3338 != 3338) {
		int utipqdgolb;
		for (utipqdgolb = 43; utipqdgolb > 0; utipqdgolb--) {
			continue;
		}
	}
	if (2435 != 2435) {
		int ttaanuccfe;
		for (ttaanuccfe = 22; ttaanuccfe > 0; ttaanuccfe--) {
			continue;
		}
	}
	if (true == true) {
		int nrrbvqn;
		for (nrrbvqn = 0; nrrbvqn > 0; nrrbvqn--) {
			continue;
		}
	}
	return 45313;
}

bool dfgbnwq::hfxyhgiadgnn(bool vfpyo, string bbnacszub) {
	string aoezazrbu = "dawxddszgeotkbvkqtirly";
	double hifykxoyzlxg = 21817;
	string upmkqbjvcqylbhv = "wlugvcwuhkdabjineb";
	int gzgevuwsrdezzn = 3595;
	string maobyipgqukpjn = "htyyaaqblctvdeqejennqpkpjqdfolq";
	double sesbrqhfefmbp = 23908;
	string fucgpkonctdkrkc = "yskkdpcpopvjgeqsvktyichndnnjgkarpgtdmcmakghuirpwawwpglcstzreoqsybmshrmrxgzbhhrvnzvqaatnm";
	if (string("dawxddszgeotkbvkqtirly") != string("dawxddszgeotkbvkqtirly")) {
		int ejdotlkkn;
		for (ejdotlkkn = 90; ejdotlkkn > 0; ejdotlkkn--) {
			continue;
		}
	}
	if (string("wlugvcwuhkdabjineb") != string("wlugvcwuhkdabjineb")) {
		int xdzk;
		for (xdzk = 89; xdzk > 0; xdzk--) {
			continue;
		}
	}
	if (string("dawxddszgeotkbvkqtirly") == string("dawxddszgeotkbvkqtirly")) {
		int bdyxtavg;
		for (bdyxtavg = 50; bdyxtavg > 0; bdyxtavg--) {
			continue;
		}
	}
	if (21817 == 21817) {
		int xs;
		for (xs = 68; xs > 0; xs--) {
			continue;
		}
	}
	return true;
}

string dfgbnwq::zqooatqrphpzbqdfj(int gdaygpuh, bool tnelazsvcuawc, double qugtbdiakh, int pbrvpttbj) {
	string hgcec = "opxbjtjqmqxzuvurlffluhtaehmjbpelzbocukdzkpetzgguuvmwddxyfrgchscksodpqbh";
	bool vrvggdjwrrpji = true;
	bool hzleqiwl = true;
	string ksujohqy = "vlywhdmxnsglyqwzatpeprefthwsafm";
	if (string("vlywhdmxnsglyqwzatpeprefthwsafm") == string("vlywhdmxnsglyqwzatpeprefthwsafm")) {
		int ovt;
		for (ovt = 19; ovt > 0; ovt--) {
			continue;
		}
	}
	if (string("opxbjtjqmqxzuvurlffluhtaehmjbpelzbocukdzkpetzgguuvmwddxyfrgchscksodpqbh") == string("opxbjtjqmqxzuvurlffluhtaehmjbpelzbocukdzkpetzgguuvmwddxyfrgchscksodpqbh")) {
		int nkpajb;
		for (nkpajb = 93; nkpajb > 0; nkpajb--) {
			continue;
		}
	}
	return string("tfvkglu");
}

bool dfgbnwq::nqkkykvuga(double nkclgcne, bool rxdrzc) {
	int pfwjzmr = 1960;
	string wkmjditxovelu = "hidgvnzjexctrydnuenzmyzwlunxvmnpfmlukjxblfiwrscnsujyfjjgdrbquqjg";
	bool iiahgsizkyaa = true;
	int ccqplblcpv = 2275;
	double axidruoc = 52556;
	if (string("hidgvnzjexctrydnuenzmyzwlunxvmnpfmlukjxblfiwrscnsujyfjjgdrbquqjg") == string("hidgvnzjexctrydnuenzmyzwlunxvmnpfmlukjxblfiwrscnsujyfjjgdrbquqjg")) {
		int wx;
		for (wx = 31; wx > 0; wx--) {
			continue;
		}
	}
	return true;
}

void dfgbnwq::cmexbeptjoiqvtfqlyjwyj(int eqiwa, int uwscvjgivvysf, bool iafrqgnyio, double dubcdrwcllkahb, bool wbqzthj) {
	bool xkzdrfndkovfrk = true;
	if (true == true) {
		int mjrcrhnah;
		for (mjrcrhnah = 62; mjrcrhnah > 0; mjrcrhnah--) {
			continue;
		}
	}
	if (true == true) {
		int mea;
		for (mea = 98; mea > 0; mea--) {
			continue;
		}
	}

}

dfgbnwq::dfgbnwq() {
	this->hfxyhgiadgnn(true, string("oyllusxqfwjtzcrsngxxiwpqobbfdmdclvypclswmftorhlig"));
	this->zqooatqrphpzbqdfj(4947, true, 19721, 5230);
	this->nqkkykvuga(25371, false);
	this->cmexbeptjoiqvtfqlyjwyj(5654, 5292, false, 23686, true);
	this->lquclsffhgdqtugvnenyleid(false, 7640, string("adjomiqvcuzqhxzeuitcmfp"), 766, 12070);
	this->ctkbowawphjslgka(7730, true);
	this->xmipbwbhdy(32420, 5543, 34773, true, string("jrwshbsrdgujutnbhucgba"), string("hlorinskefdqdkounwpixtpzoqwlxrozqprhvetooywkplkxzktbc"), 902);
	this->hayzcshlfrtoxlmodfl(70, true, string("gtmcxsfigxexwdstpucdxxkjufkyndndgywik"));
	this->wlyaxsvtocdggymsiozacnu(4173, 16485, 43541, true);
	this->ulrtxmwhsmdbdaqjtd(false, false, 1818, string("uypwsnzbqcudvxrbbubatruepjxdyjttgfmnchoqqxkqvsyhbktowzcaavxztfolyi"));
	this->ivmekptcupwswcqrsijav(22342, string("wxwdqsbgitaorhyroeezeyxdvusnevwggsjyigwsiaamlyehoobjsvfvjnilcmstqmcwxbfmplaeimudaluaksqydwwyysomw"), string("msdomrblzhcpkhrptsteszpujmloiuirnoafddkcjtqcxylxwbjiafokegbqs"), 11948, 2508, true, 5504, false, 9294);
	this->qltwwpahlaimsyyzeqkloea();
	this->jkuuczvrsmabvysijynisyjg(1733, 5445);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class iepruxi {
public:
	string aformpl;
	string fqyhaykzdbtryfn;
	iepruxi();
	void lrdznefhrlqt();
	string xhfvdvrkyocnzrygpagdtiv(bool bgjhjbdnvodzgzt, int rxapgcmtj, string yfsiyn);
	int zweutcnkbcvcvushdsdiyim(int jpfzalgvzgg);
	bool jxsqiwntpv(string wnrndfjz, int nwyad, bool mhaddoj);
	double okeckhrfdaeuchytvw(int wvmhzcxnhgfgecn, bool agxighzsuynxox, double bkxcfivxv, int fnsskgyptcidog, string cykimicdlpi, string zdhdd, bool qlatvfl, double uhieonq, bool tecoe, bool qfmupawxsdwz);
	bool vnvzpbyrxwjmnsh(double kbeaddahc, double gduzb, int qlhmlvjog, double edewqsplwj, bool cexjaxrztzv, string wbruvacqmox, int qdubndoxuzko, double wairarkfzxorfe, string hzyhgauehxkfr);

protected:
	string achglzkhdcng;

	int upqjzwliiqusittzdzievtamf(bool xtzjswx, string gisoprzujtjc, string piwgzph, int usdqoagtr, double wcppkzcvvyc, string gupknrijcr, string nkgbeubefuy, string axjuwg, string nhsqydwsbqsfca);
	string uxswhywhbn(int ouwjk, bool zthdwpth, bool rxwfksewginrjsx, bool ashekbif, bool jreibnkzqik, double yvcilt, double qcljlefpistv, bool bkeutdpprwexohh, bool bwrrwjjjezuqtav);
	void oebfylyfwheapjjqpqm(double tyxpqfeqoxlq, bool ztxdzvbu, int rvcvzjqebvlidj, bool jeewwttkinqfyh, double rrxaoixarp, string kjbafra);
	int okntwyfftcbiwgvou(bool ysvghsdyozetuo);
	string dymjlgynevyqnmkfrt(bool yamigcn, string crfjseafzlkb, double wxyeoxlwc, string nqxztcqyj);
	void nvcrnfybdcgegldzpazaq(double huuqbyurbfzek);
	string knayozkhfirxghxbtomrb(double uaarppcexweby, double ojromksylzgimv, int fmxidg, string qnqninoqptmuzfh, bool wmeaapepefreyi, double nefgnwdquy);
	double bxdsqvqrkcpv(int gltfyy, double sommlgdhkmujsks, string mkyunjepdme, double zhmgrc);
	void wkcnidikuaqhkqotcgtn(int uzwddlrjscgqpox, bool iqtfejwk, bool lmroiy);

private:
	string iprxnepm;
	bool soyayxafzn;
	int wrobjmwezjoboy;
	int jnsvzbxpam;
	int iahidmsf;

	void swbubisyhtoav(double jratvbers, int grlnvqsbzv, bool zmnutm);
	string fpmnahbxphweelepp(int jqpmpivzsqmtyi, int aitiybqcgdqjikv);
	int mcicophfctftwfoc(double lotvexiepiswndm, bool ctajejdizslcdwy, bool jbrajidoc, int ezynlzctzaybc, string mxfzajc);
	double zfjzwdigkcolnkpwai(bool gblpgt, double dgfoi, bool jecxmgnl, double ixpob, double nsmftrtrnltewb, string rcurhkyaoog);

};


void iepruxi::swbubisyhtoav(double jratvbers, int grlnvqsbzv, bool zmnutm) {
	int qlytdmdixoguslg = 4228;
	bool rnpnb = false;
	double csgerfbef = 12306;
	if (12306 == 12306) {
		int mmme;
		for (mmme = 74; mmme > 0; mmme--) {
			continue;
		}
	}
	if (4228 == 4228) {
		int otkl;
		for (otkl = 99; otkl > 0; otkl--) {
			continue;
		}
	}
	if (4228 == 4228) {
		int qrjmrfdup;
		for (qrjmrfdup = 31; qrjmrfdup > 0; qrjmrfdup--) {
			continue;
		}
	}

}

string iepruxi::fpmnahbxphweelepp(int jqpmpivzsqmtyi, int aitiybqcgdqjikv) {
	double rguomtaqkp = 50447;
	string kmipemgrhxooyqf = "gxzfbcseagmlieddyjsbavgrmtznygasqmdvtweyxbbuffvvhshmrvhy";
	double hpzryqzizdl = 45800;
	double wtvwoi = 3061;
	string vlshirqvwjp = "hfmxz";
	string ncorlnlooqywu = "tsmjnpacassbvgphyxejmhyzmpwjumdghiuirxrwqfvxxxljbxwdgfmqgcrlsepkqfptzahkxszqyfgfrnqhlhyconxhvw";
	int dhkweafkvyl = 6278;
	int tmjimjggwkouxt = 5087;
	bool xjnytiasyccqlr = true;
	double vtjkigrmley = 13345;
	if (13345 != 13345) {
		int xzb;
		for (xzb = 82; xzb > 0; xzb--) {
			continue;
		}
	}
	if (string("hfmxz") != string("hfmxz")) {
		int oomvly;
		for (oomvly = 27; oomvly > 0; oomvly--) {
			continue;
		}
	}
	if (45800 != 45800) {
		int aap;
		for (aap = 29; aap > 0; aap--) {
			continue;
		}
	}
	return string("ffys");
}

int iepruxi::mcicophfctftwfoc(double lotvexiepiswndm, bool ctajejdizslcdwy, bool jbrajidoc, int ezynlzctzaybc, string mxfzajc) {
	int gjkjcmau = 2496;
	string eazfja = "cgmpbnumxzfahrswjcbjaniozjpgasinykyghlnwgsepzfhymntowrxkvywn";
	if (string("cgmpbnumxzfahrswjcbjaniozjpgasinykyghlnwgsepzfhymntowrxkvywn") == string("cgmpbnumxzfahrswjcbjaniozjpgasinykyghlnwgsepzfhymntowrxkvywn")) {
		int poxtdfcb;
		for (poxtdfcb = 32; poxtdfcb > 0; poxtdfcb--) {
			continue;
		}
	}
	if (string("cgmpbnumxzfahrswjcbjaniozjpgasinykyghlnwgsepzfhymntowrxkvywn") == string("cgmpbnumxzfahrswjcbjaniozjpgasinykyghlnwgsepzfhymntowrxkvywn")) {
		int uwlbud;
		for (uwlbud = 24; uwlbud > 0; uwlbud--) {
			continue;
		}
	}
	if (2496 != 2496) {
		int qtuowpfv;
		for (qtuowpfv = 73; qtuowpfv > 0; qtuowpfv--) {
			continue;
		}
	}
	if (string("cgmpbnumxzfahrswjcbjaniozjpgasinykyghlnwgsepzfhymntowrxkvywn") == string("cgmpbnumxzfahrswjcbjaniozjpgasinykyghlnwgsepzfhymntowrxkvywn")) {
		int yjglaxgj;
		for (yjglaxgj = 65; yjglaxgj > 0; yjglaxgj--) {
			continue;
		}
	}
	if (2496 == 2496) {
		int hhpk;
		for (hhpk = 43; hhpk > 0; hhpk--) {
			continue;
		}
	}
	return 98896;
}

double iepruxi::zfjzwdigkcolnkpwai(bool gblpgt, double dgfoi, bool jecxmgnl, double ixpob, double nsmftrtrnltewb, string rcurhkyaoog) {
	bool arumiahubgcu = false;
	bool jbkouckzewpqoi = false;
	double osrwh = 29996;
	string brbjmfli = "idvadgimjqolmsoewlgzlighaprjgipnfuunlflsifzzyykcgzlkjwmnswzujopopyiwwig";
	string bolvenpgkhqm = "iynchnqpjulfcyhqxtowncwnxnkjohojyuszdgikfwbqbmtfxyvdsxjeomjlkhftxpmnkigmjkkktao";
	bool ryzibcqdo = false;
	int bfxeyqdwhaltyt = 1178;
	double awbkozsemx = 12287;
	if (1178 != 1178) {
		int yy;
		for (yy = 31; yy > 0; yy--) {
			continue;
		}
	}
	return 46869;
}

int iepruxi::upqjzwliiqusittzdzievtamf(bool xtzjswx, string gisoprzujtjc, string piwgzph, int usdqoagtr, double wcppkzcvvyc, string gupknrijcr, string nkgbeubefuy, string axjuwg, string nhsqydwsbqsfca) {
	bool vljigtlp = true;
	if (true == true) {
		int bdjptn;
		for (bdjptn = 41; bdjptn > 0; bdjptn--) {
			continue;
		}
	}
	if (true == true) {
		int liyizqq;
		for (liyizqq = 69; liyizqq > 0; liyizqq--) {
			continue;
		}
	}
	return 34454;
}

string iepruxi::uxswhywhbn(int ouwjk, bool zthdwpth, bool rxwfksewginrjsx, bool ashekbif, bool jreibnkzqik, double yvcilt, double qcljlefpistv, bool bkeutdpprwexohh, bool bwrrwjjjezuqtav) {
	int wqdstt = 6536;
	if (6536 != 6536) {
		int pwzslof;
		for (pwzslof = 37; pwzslof > 0; pwzslof--) {
			continue;
		}
	}
	if (6536 != 6536) {
		int dnj;
		for (dnj = 97; dnj > 0; dnj--) {
			continue;
		}
	}
	return string("gsaaakkarfmbv");
}

void iepruxi::oebfylyfwheapjjqpqm(double tyxpqfeqoxlq, bool ztxdzvbu, int rvcvzjqebvlidj, bool jeewwttkinqfyh, double rrxaoixarp, string kjbafra) {
	int rdhdqqwi = 5579;
	string hlonmzoapfjelg = "uvhabxwikuycpftwisbeibptwecyxlbipdeoxfknnbpltpospplwqzbstbdsarhptgkdo";
	double aujnbmhud = 22857;
	int uplpquzhvmdm = 5336;
	if (5336 != 5336) {
		int xaeoggdaa;
		for (xaeoggdaa = 98; xaeoggdaa > 0; xaeoggdaa--) {
			continue;
		}
	}
	if (5336 == 5336) {
		int aonfdlzu;
		for (aonfdlzu = 89; aonfdlzu > 0; aonfdlzu--) {
			continue;
		}
	}
	if (5336 == 5336) {
		int gr;
		for (gr = 67; gr > 0; gr--) {
			continue;
		}
	}
	if (22857 == 22857) {
		int puied;
		for (puied = 32; puied > 0; puied--) {
			continue;
		}
	}
	if (5579 != 5579) {
		int nqjyf;
		for (nqjyf = 85; nqjyf > 0; nqjyf--) {
			continue;
		}
	}

}

int iepruxi::okntwyfftcbiwgvou(bool ysvghsdyozetuo) {
	string rzctsxwaznzbm = "ihhvf";
	double qzlexnlgo = 26751;
	bool zifmtikhuyet = true;
	bool otoapf = false;
	if (false != false) {
		int zn;
		for (zn = 7; zn > 0; zn--) {
			continue;
		}
	}
	if (26751 == 26751) {
		int lvb;
		for (lvb = 15; lvb > 0; lvb--) {
			continue;
		}
	}
	if (false != false) {
		int zjym;
		for (zjym = 74; zjym > 0; zjym--) {
			continue;
		}
	}
	if (string("ihhvf") == string("ihhvf")) {
		int hmumesq;
		for (hmumesq = 48; hmumesq > 0; hmumesq--) {
			continue;
		}
	}
	if (26751 == 26751) {
		int bocdjyzc;
		for (bocdjyzc = 40; bocdjyzc > 0; bocdjyzc--) {
			continue;
		}
	}
	return 8935;
}

string iepruxi::dymjlgynevyqnmkfrt(bool yamigcn, string crfjseafzlkb, double wxyeoxlwc, string nqxztcqyj) {
	double aqbgogek = 52988;
	int xbpdtr = 2978;
	int ekzmfrepuneo = 1699;
	string tbshfbqoaoeea = "vzpxqkknglahgrouynzuxmsnzivfttjeydmghwenvmdegygwfyhoblieanvalxkbidtxgcobdjhwqqvrtgutin";
	int kyhsuy = 1496;
	double firlktqspqpvh = 62709;
	bool lehcx = false;
	double boatcgty = 15536;
	if (false == false) {
		int gztahm;
		for (gztahm = 59; gztahm > 0; gztahm--) {
			continue;
		}
	}
	if (2978 != 2978) {
		int bmiwuc;
		for (bmiwuc = 83; bmiwuc > 0; bmiwuc--) {
			continue;
		}
	}
	if (false == false) {
		int asyete;
		for (asyete = 62; asyete > 0; asyete--) {
			continue;
		}
	}
	return string("gdviidyzajxczzahbmed");
}

void iepruxi::nvcrnfybdcgegldzpazaq(double huuqbyurbfzek) {
	double xsfxlysqifbjd = 61552;
	string yoweibnkeqpm = "tzjmsjeueavepnfwtaorurfoaotshnptcgzhljacuregtojyzvgzjtiz";
	int hhbgrawhmonp = 3985;
	double sjruxooqsdxqb = 44328;
	bool kmaqwfxbycgum = true;
	int nhlejyjn = 780;
	if (true != true) {
		int daxehyhf;
		for (daxehyhf = 71; daxehyhf > 0; daxehyhf--) {
			continue;
		}
	}
	if (string("tzjmsjeueavepnfwtaorurfoaotshnptcgzhljacuregtojyzvgzjtiz") == string("tzjmsjeueavepnfwtaorurfoaotshnptcgzhljacuregtojyzvgzjtiz")) {
		int tall;
		for (tall = 16; tall > 0; tall--) {
			continue;
		}
	}
	if (string("tzjmsjeueavepnfwtaorurfoaotshnptcgzhljacuregtojyzvgzjtiz") != string("tzjmsjeueavepnfwtaorurfoaotshnptcgzhljacuregtojyzvgzjtiz")) {
		int funlydc;
		for (funlydc = 97; funlydc > 0; funlydc--) {
			continue;
		}
	}
	if (61552 != 61552) {
		int biyko;
		for (biyko = 5; biyko > 0; biyko--) {
			continue;
		}
	}

}

string iepruxi::knayozkhfirxghxbtomrb(double uaarppcexweby, double ojromksylzgimv, int fmxidg, string qnqninoqptmuzfh, bool wmeaapepefreyi, double nefgnwdquy) {
	bool ezsyxiungi = true;
	double zlzgkvelhjt = 81340;
	int fcvkieddwwsbw = 2730;
	int ywdpiqn = 5248;
	string rdixvvsw = "mzsvnvmbxeneeyrvisktqjwfeohslfuvejqigg";
	string cguio = "fjdmdmxzxshitfjzoxbxunzbrkcefofuygpcfptjacjoovrahyjbqjthegcngteiwdckvkqsyenlfglmlrz";
	string gezcjv = "grefgeqdiyscvdnxsezmwasjwobxnqhjapdyqdtsmxxhnxuenhxogeyptsvhfocemwhkrq";
	double cmnfawrqskkof = 45138;
	double bjivvweys = 77219;
	int qootypgmqqtdkuf = 1217;
	if (string("grefgeqdiyscvdnxsezmwasjwobxnqhjapdyqdtsmxxhnxuenhxogeyptsvhfocemwhkrq") == string("grefgeqdiyscvdnxsezmwasjwobxnqhjapdyqdtsmxxhnxuenhxogeyptsvhfocemwhkrq")) {
		int tfrz;
		for (tfrz = 45; tfrz > 0; tfrz--) {
			continue;
		}
	}
	if (true == true) {
		int uglizhcfs;
		for (uglizhcfs = 10; uglizhcfs > 0; uglizhcfs--) {
			continue;
		}
	}
	return string("jrnlz");
}

double iepruxi::bxdsqvqrkcpv(int gltfyy, double sommlgdhkmujsks, string mkyunjepdme, double zhmgrc) {
	return 53579;
}

void iepruxi::wkcnidikuaqhkqotcgtn(int uzwddlrjscgqpox, bool iqtfejwk, bool lmroiy) {
	bool vmqgqozev = false;
	double kzlectrsc = 11405;
	int pnimyatg = 1930;
	double aweshaqs = 15984;
	bool bogvabkfvrjtbg = false;
	double qwtydjfls = 11148;
	int oyfhebwa = 830;
	bool rcdquyv = true;
	string ebhggrmok = "jkpfudqfdjfrgdrkrdobdumvowssedpehuiycahaozp";
	if (11405 != 11405) {
		int wpwlflgly;
		for (wpwlflgly = 56; wpwlflgly > 0; wpwlflgly--) {
			continue;
		}
	}
	if (830 != 830) {
		int in;
		for (in = 87; in > 0; in--) {
			continue;
		}
	}
	if (1930 != 1930) {
		int nkpbeoiemu;
		for (nkpbeoiemu = 1; nkpbeoiemu > 0; nkpbeoiemu--) {
			continue;
		}
	}
	if (15984 != 15984) {
		int jzw;
		for (jzw = 95; jzw > 0; jzw--) {
			continue;
		}
	}

}

void iepruxi::lrdznefhrlqt() {
	string rjiyrjm = "le";
	if (string("le") == string("le")) {
		int uvxatamux;
		for (uvxatamux = 85; uvxatamux > 0; uvxatamux--) {
			continue;
		}
	}
	if (string("le") != string("le")) {
		int bwxbayij;
		for (bwxbayij = 79; bwxbayij > 0; bwxbayij--) {
			continue;
		}
	}
	if (string("le") != string("le")) {
		int vs;
		for (vs = 78; vs > 0; vs--) {
			continue;
		}
	}

}

string iepruxi::xhfvdvrkyocnzrygpagdtiv(bool bgjhjbdnvodzgzt, int rxapgcmtj, string yfsiyn) {
	string crdrygeelzudr = "gpoxdmfytbuimbioarszzujqeamflfgyudgiccuces";
	int qgogpsm = 931;
	bool iemwtyctdpetfa = false;
	if (false != false) {
		int rfcbjehv;
		for (rfcbjehv = 25; rfcbjehv > 0; rfcbjehv--) {
			continue;
		}
	}
	if (false != false) {
		int etm;
		for (etm = 20; etm > 0; etm--) {
			continue;
		}
	}
	if (string("gpoxdmfytbuimbioarszzujqeamflfgyudgiccuces") == string("gpoxdmfytbuimbioarszzujqeamflfgyudgiccuces")) {
		int ow;
		for (ow = 0; ow > 0; ow--) {
			continue;
		}
	}
	if (931 != 931) {
		int gbbf;
		for (gbbf = 62; gbbf > 0; gbbf--) {
			continue;
		}
	}
	if (false != false) {
		int kktfqrqd;
		for (kktfqrqd = 13; kktfqrqd > 0; kktfqrqd--) {
			continue;
		}
	}
	return string("tuxbmeurdfmz");
}

int iepruxi::zweutcnkbcvcvushdsdiyim(int jpfzalgvzgg) {
	string pdcecowhewtqkh = "pmr";
	if (string("pmr") == string("pmr")) {
		int rbajl;
		for (rbajl = 31; rbajl > 0; rbajl--) {
			continue;
		}
	}
	return 49916;
}

bool iepruxi::jxsqiwntpv(string wnrndfjz, int nwyad, bool mhaddoj) {
	string ojxeer = "bdikpq";
	double cwrbqmyqzwk = 35810;
	bool jhyjy = false;
	string qrcsa = "pviryzwhbeqdxkqtyqcyjitpqocxswcfqddmhtvouucqbceibqhfksicjgozccbrkpoqnbhmlnaly";
	bool tsxkwz = true;
	string msidks = "iwehrmzwzujnppoawycvkasmcnhphhfoaomfdpydmsyxilcfkeoxzmcue";
	double aoildyalotpetn = 17769;
	string ubzmvoxxtbfkpf = "thywxrlpuetxqnmgnfikbjjxubigikoninnubybgbryvzajrdimbhhawkxndr";
	if (17769 != 17769) {
		int ke;
		for (ke = 49; ke > 0; ke--) {
			continue;
		}
	}
	if (17769 != 17769) {
		int vg;
		for (vg = 22; vg > 0; vg--) {
			continue;
		}
	}
	if (17769 == 17769) {
		int jzavjmrnh;
		for (jzavjmrnh = 73; jzavjmrnh > 0; jzavjmrnh--) {
			continue;
		}
	}
	if (false != false) {
		int jqd;
		for (jqd = 39; jqd > 0; jqd--) {
			continue;
		}
	}
	return false;
}

double iepruxi::okeckhrfdaeuchytvw(int wvmhzcxnhgfgecn, bool agxighzsuynxox, double bkxcfivxv, int fnsskgyptcidog, string cykimicdlpi, string zdhdd, bool qlatvfl, double uhieonq, bool tecoe, bool qfmupawxsdwz) {
	int zkzbageqsfygap = 251;
	if (251 != 251) {
		int uflptymh;
		for (uflptymh = 55; uflptymh > 0; uflptymh--) {
			continue;
		}
	}
	if (251 == 251) {
		int fo;
		for (fo = 43; fo > 0; fo--) {
			continue;
		}
	}
	if (251 == 251) {
		int giyuagyel;
		for (giyuagyel = 93; giyuagyel > 0; giyuagyel--) {
			continue;
		}
	}
	if (251 == 251) {
		int cjdnrastjj;
		for (cjdnrastjj = 6; cjdnrastjj > 0; cjdnrastjj--) {
			continue;
		}
	}
	if (251 == 251) {
		int cyuemma;
		for (cyuemma = 35; cyuemma > 0; cyuemma--) {
			continue;
		}
	}
	return 33344;
}

bool iepruxi::vnvzpbyrxwjmnsh(double kbeaddahc, double gduzb, int qlhmlvjog, double edewqsplwj, bool cexjaxrztzv, string wbruvacqmox, int qdubndoxuzko, double wairarkfzxorfe, string hzyhgauehxkfr) {
	double wcdyemsmtn = 16162;
	bool zfdxr = false;
	double seetbrsd = 34944;
	bool inndzgmgdhrawwy = true;
	if (34944 == 34944) {
		int dqg;
		for (dqg = 94; dqg > 0; dqg--) {
			continue;
		}
	}
	return false;
}

iepruxi::iepruxi() {
	this->lrdznefhrlqt();
	this->xhfvdvrkyocnzrygpagdtiv(false, 1104, string("swrvjzvhmqupdemswwwdqcdcyuphrymyuzmqkdvxvyqjxlekxwmtcsgdglqpdxbkjyqyysowdvodlkrqskvsyhqvpuh"));
	this->zweutcnkbcvcvushdsdiyim(5414);
	this->jxsqiwntpv(string("bqdkpieb"), 6566, true);
	this->okeckhrfdaeuchytvw(149, true, 30328, 499, string("dnadqlzwnihgxdkhvogtijjwdbserxzrxewbsgbndgcyswkumjsmioztdrfupmjabmlupazrbcr"), string("kwtpueyrojygfado"), true, 6377, true, false);
	this->vnvzpbyrxwjmnsh(48799, 2987, 8453, 27217, true, string("gtnuzq"), 134, 46236, string("zemlqbsrk"));
	this->upqjzwliiqusittzdzievtamf(false, string("plbwmtbavyqfsaddpqojsreqsbfvhkabyvxiajxnvxdkyyefzaxkvnqghwyyhhsrnrvetfzquqgdki"), string("vlrppgfczhklxzgehbxreryinrmccczgcmigikmmvtkgkrclrafuzbggfziusyt"), 2293, 87909, string("frghcabbywpmzganaufhsebczzlhdzwaopulknxinwbmhm"), string("wzesjwgpnbujteluqmkzeargiointyjilidjiqjhkhixttzjnipqvwfvssbfugamdgaslztfvzgjaugzswthdahnywjvtctsacxe"), string("ezosrepkieqpcjhpjbckgpjadutz"), string("etivkniajvyrjnv"));
	this->uxswhywhbn(413, true, true, true, false, 51019, 71667, false, false);
	this->oebfylyfwheapjjqpqm(77932, true, 1934, false, 13217, string("hzripwlzkwdapizlmrhgutwmbrhnqyvgcqcpvhrxvswnzmpr"));
	this->okntwyfftcbiwgvou(true);
	this->dymjlgynevyqnmkfrt(false, string("irchfswpblqhshbytrmrsafnxjvdszrurphgcapijwpnnmqbbhbwk"), 1875, string("wpcuuqfdncnizhvpgshaatxnfrgkagizlewtlhfazzycogvwnuqjujtyrwwkdeaefn"));
	this->nvcrnfybdcgegldzpazaq(7823);
	this->knayozkhfirxghxbtomrb(5040, 12428, 4236, string("mlyca"), false, 59892);
	this->bxdsqvqrkcpv(3700, 3081, string("xbjlhwenosxzjnbsephgzbblmmqyqxqoxdmxlswsdtpdohbulnxtnckpswbnkpclwjbdzcat"), 5448);
	this->wkcnidikuaqhkqotcgtn(6089, true, true);
	this->swbubisyhtoav(9574, 3833, false);
	this->fpmnahbxphweelepp(3042, 872);
	this->mcicophfctftwfoc(15967, false, false, 5824, string("ceuenhqedpkeo"));
	this->zfjzwdigkcolnkpwai(false, 36690, false, 34917, 53408, string("xwpnpdmqaoowngqzcoiqaxebtmmcsrhdodagjtijxjutdafdpputotdwtiu"));
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class sphdndj {
public:
	double rswlcp;
	bool eezaharvpeuqsw;
	int bnlnjowdnpoj;
	int kwztz;
	string jnqlarofidhcxvj;
	sphdndj();
	double wxmyrtgwdzgn(double yqrankhhfugtj, string epubp, double qwllxottor, double kjbiwjqdsc, int ozsqvdcwqrrw);
	void dlkgbbsiljvcs(string nexcmgrzrwt, bool ihikkxrfy, string aqknndpfdsl, double mnpizmpiwhnhi, string jplmfkp, bool kboqkcmfekbq, double nimiektjx, double fmatnkuclhogmnv, string rgzxjxru);
	void athdbsqpdtpuhknnb(bool xibcvwi, int kkdzvivdpjzg, bool izpfedlsqhm, string rgfaofjdihik, bool vsknhvtezxe);
	int yhdugkfsiedumeel(bool tgsvvwppy, double edusojmgzjyb, int nacfxkd, int bkqirh, int outyfbwjukj, int qspvahkruujrcw, bool zvdgleunriajzvy);
	bool hxjsuwwloegwipeqxtvjdbik(bool nrzlxwarwsdx, string ztmfxncfshiersm, int ilecbvsur, int woqwxmwpizl, bool oxswrctf);
	void lsizkvobtxsnxwjkszzzsaxt(int jetpsdkayy, int qejbnmqtuuir, double jnltzxufj, bool rqrcj, double ppysupwwcgdvj, double lklfptldxk, double gvpslkl);
	void ufqrualxhm(double ewvlbanhezi, string zfltlmeevw, int arfrlp, int jhkfhs, string fcyoyfhe, bool dpxnvplhlbrtrgj, string sjfqawxwmbvui, string jayimjkjjz);
	string bkeudcwemv(string byqubvstwrm, bool eezqiyonchdxot, bool lxkoycqzmv, double mxesuvqq, double mswejricgtky, bool wsmezeaavrsv, int frggefyehjbgbvl, double ktkeddbvmy, string sfvfck, string nhgcmnib);
	double tgkeywqgyphqubqbzh(double gvwykpkg, int pobriugrrpp, string dtius, bool lwfbzom, int izikstkw, int jqaprid, string qrdvbzbbixokx);
	double lhamrxzwxxepvgdxukaw(double jcmye, int qsmirxapsrwzafb, double cilcpxlljso, string smlzyvhwszob, bool ychbtwp, int xpplvfuilccgnku, bool ljnzcvyqnpyn, double ckoaqcmkq, int pkvet, string mglxogw);

protected:
	int qskgucnmywon;
	bool jxxff;
	double fgsxmvs;
	bool vapvnocddlxgpxk;

	bool eanbspvphwjjlpncqjfetu(bool yphoiozh);
	int sfrizcwsaljdao(int awhvmiyqljdmaip, int wqmlevzigglasz, bool jeasjjzy, double xhklwxlhsovk, double tyjafbbnfiadwx, int pcwkie, int bavbc, bool feqiuorn);
	int cjifabamstf(string hkudkjkeaun, string bjckn, bool tiluywmzzejnijq, int rvclwhrkwdutxzq, string dyvftea, double vheqrdocmqqbdd);
	double cxxgjywitvjikudbksyvngdi(int kgkodzjwgt, string bdzagpntvzokjho, string gsealuaw, int hitjbyf, bool qdntwtxuhpuva, int udixhajbywczmn, double fkolm, string yjbul, bool oyzvigvvgd, string zrqzvjbd);
	string nrzpqnmadiirfqjlpooql(bool izefkq, bool iwdxeoueaa, bool bjxovgolhcntqy, int qmigoiww, string aozvtaesg, string ueaajhmuxodtlxt, int cobaxrh, bool fhjdvofobent, string zuujjhfpxhcotm, int iofjbbmk);

private:
	double aetgnzio;
	double jtscopdag;
	double jzhokvtfwfhfb;
	double qjnvezubdmfzlgh;
	double vbfyzqokpfwcuvx;

	double bozazshxkzlonatjswsjpnvva(string yrhhofjcoxpqe, double pgeblhhhqhaah, bool ujkxugajmtk, bool glphsledk, bool bxgdsfeerpk, double xgdxxgi, double xzdszmqv);
	void bmnupzayxtnzuvptxmoaz(int aixbxvpihynbgf, int czanvbrhhkph, int ajxgprjmrlx, string ewapqpzobc, bool kdzjcguhbeszm);
	string azocletmqvwnuirvmj(string aacfozjv, int akhpk, int fmvlczhv, bool atwopxwljudultt);
	string gajycmutxpfeectqgd(bool lehktbf, string qtopnql, int fhchgfgoiwyhog, double caenzuoklwmhd);

};


double sphdndj::bozazshxkzlonatjswsjpnvva(string yrhhofjcoxpqe, double pgeblhhhqhaah, bool ujkxugajmtk, bool glphsledk, bool bxgdsfeerpk, double xgdxxgi, double xzdszmqv) {
	return 38615;
}

void sphdndj::bmnupzayxtnzuvptxmoaz(int aixbxvpihynbgf, int czanvbrhhkph, int ajxgprjmrlx, string ewapqpzobc, bool kdzjcguhbeszm) {

}

string sphdndj::azocletmqvwnuirvmj(string aacfozjv, int akhpk, int fmvlczhv, bool atwopxwljudultt) {
	return string("psrqzmnl");
}

string sphdndj::gajycmutxpfeectqgd(bool lehktbf, string qtopnql, int fhchgfgoiwyhog, double caenzuoklwmhd) {
	int kuxbo = 1062;
	bool csdnori = false;
	string rbwqkudtbajhcwx = "kuqbqvpsptnqnazkriqbcznkkrqgesffvjgyzdnkniuiegfiwfeaxcbgbs";
	string eyjkpk = "zxihmiulyv";
	int clymbmjwftwiqqi = 89;
	int zelfvuopinud = 392;
	double znhabkdidv = 35895;
	double mmtove = 22713;
	bool apuvqgumlcya = true;
	double vamoxqekj = 12676;
	if (12676 == 12676) {
		int lcelri;
		for (lcelri = 35; lcelri > 0; lcelri--) {
			continue;
		}
	}
	if (true == true) {
		int tyrsikebrp;
		for (tyrsikebrp = 10; tyrsikebrp > 0; tyrsikebrp--) {
			continue;
		}
	}
	if (12676 != 12676) {
		int eakvenb;
		for (eakvenb = 71; eakvenb > 0; eakvenb--) {
			continue;
		}
	}
	return string("qtatmwuhgevufnyi");
}

bool sphdndj::eanbspvphwjjlpncqjfetu(bool yphoiozh) {
	int lvyqbbcoqaxtmn = 2751;
	string aczearpqyqdbugv = "jwmqmfxupdearjvzwhfnlelguuatffvbnzxmedkepyoypbxintxfzuranogtfyvqagyeotxignzargjxmjxl";
	if (2751 == 2751) {
		int tgft;
		for (tgft = 92; tgft > 0; tgft--) {
			continue;
		}
	}
	if (2751 != 2751) {
		int ttg;
		for (ttg = 67; ttg > 0; ttg--) {
			continue;
		}
	}
	if (string("jwmqmfxupdearjvzwhfnlelguuatffvbnzxmedkepyoypbxintxfzuranogtfyvqagyeotxignzargjxmjxl") != string("jwmqmfxupdearjvzwhfnlelguuatffvbnzxmedkepyoypbxintxfzuranogtfyvqagyeotxignzargjxmjxl")) {
		int zjwwdxhsb;
		for (zjwwdxhsb = 41; zjwwdxhsb > 0; zjwwdxhsb--) {
			continue;
		}
	}
	if (2751 != 2751) {
		int vp;
		for (vp = 47; vp > 0; vp--) {
			continue;
		}
	}
	if (string("jwmqmfxupdearjvzwhfnlelguuatffvbnzxmedkepyoypbxintxfzuranogtfyvqagyeotxignzargjxmjxl") == string("jwmqmfxupdearjvzwhfnlelguuatffvbnzxmedkepyoypbxintxfzuranogtfyvqagyeotxignzargjxmjxl")) {
		int kz;
		for (kz = 49; kz > 0; kz--) {
			continue;
		}
	}
	return true;
}

int sphdndj::sfrizcwsaljdao(int awhvmiyqljdmaip, int wqmlevzigglasz, bool jeasjjzy, double xhklwxlhsovk, double tyjafbbnfiadwx, int pcwkie, int bavbc, bool feqiuorn) {
	double umyrdnhkcsweni = 3534;
	double rbfwhfkcxexmym = 26242;
	int dcawx = 8155;
	if (3534 == 3534) {
		int lx;
		for (lx = 31; lx > 0; lx--) {
			continue;
		}
	}
	if (3534 == 3534) {
		int jict;
		for (jict = 77; jict > 0; jict--) {
			continue;
		}
	}
	if (3534 == 3534) {
		int iovty;
		for (iovty = 60; iovty > 0; iovty--) {
			continue;
		}
	}
	if (8155 == 8155) {
		int ppuwjnznqj;
		for (ppuwjnznqj = 25; ppuwjnznqj > 0; ppuwjnznqj--) {
			continue;
		}
	}
	if (26242 == 26242) {
		int ufctakscs;
		for (ufctakscs = 81; ufctakscs > 0; ufctakscs--) {
			continue;
		}
	}
	return 12248;
}

int sphdndj::cjifabamstf(string hkudkjkeaun, string bjckn, bool tiluywmzzejnijq, int rvclwhrkwdutxzq, string dyvftea, double vheqrdocmqqbdd) {
	bool oycjeeaunvus = true;
	int uycumyxhuu = 4066;
	int ehfrvpv = 3739;
	int twvbbpfno = 5380;
	string tvggjwot = "ufhbuxjimjwizzscbybqyayqqliyfahuiklwuymallprjmvytarjppxpmhugdbalczyyxpaqo";
	bool zjajbqecrsikela = true;
	bool lgqjxo = true;
	string mqveoyssmhko = "nivqpxvepuvhyglfvanhxyooiojfuiymmqsolwjagcufghxhcqjejtdedyoulwewfhctdxridndmrhfglfdontorw";
	if (string("nivqpxvepuvhyglfvanhxyooiojfuiymmqsolwjagcufghxhcqjejtdedyoulwewfhctdxridndmrhfglfdontorw") == string("nivqpxvepuvhyglfvanhxyooiojfuiymmqsolwjagcufghxhcqjejtdedyoulwewfhctdxridndmrhfglfdontorw")) {
		int hhtmhi;
		for (hhtmhi = 84; hhtmhi > 0; hhtmhi--) {
			continue;
		}
	}
	return 24216;
}

double sphdndj::cxxgjywitvjikudbksyvngdi(int kgkodzjwgt, string bdzagpntvzokjho, string gsealuaw, int hitjbyf, bool qdntwtxuhpuva, int udixhajbywczmn, double fkolm, string yjbul, bool oyzvigvvgd, string zrqzvjbd) {
	string yhwpklblxgenkxc = "lsnlahuit";
	bool zzhuatzhvx = true;
	if (true != true) {
		int coqt;
		for (coqt = 44; coqt > 0; coqt--) {
			continue;
		}
	}
	if (string("lsnlahuit") == string("lsnlahuit")) {
		int vvaybyqtal;
		for (vvaybyqtal = 29; vvaybyqtal > 0; vvaybyqtal--) {
			continue;
		}
	}
	if (string("lsnlahuit") == string("lsnlahuit")) {
		int bxpnd;
		for (bxpnd = 60; bxpnd > 0; bxpnd--) {
			continue;
		}
	}
	if (true != true) {
		int zcceqnxzrs;
		for (zcceqnxzrs = 37; zcceqnxzrs > 0; zcceqnxzrs--) {
			continue;
		}
	}
	if (true == true) {
		int rue;
		for (rue = 41; rue > 0; rue--) {
			continue;
		}
	}
	return 85127;
}

string sphdndj::nrzpqnmadiirfqjlpooql(bool izefkq, bool iwdxeoueaa, bool bjxovgolhcntqy, int qmigoiww, string aozvtaesg, string ueaajhmuxodtlxt, int cobaxrh, bool fhjdvofobent, string zuujjhfpxhcotm, int iofjbbmk) {
	string avbzvv = "cizatzqsqwnjxiiwqotififdarc";
	if (string("cizatzqsqwnjxiiwqotififdarc") == string("cizatzqsqwnjxiiwqotififdarc")) {
		int blczrvebnh;
		for (blczrvebnh = 54; blczrvebnh > 0; blczrvebnh--) {
			continue;
		}
	}
	if (string("cizatzqsqwnjxiiwqotififdarc") == string("cizatzqsqwnjxiiwqotififdarc")) {
		int aoe;
		for (aoe = 65; aoe > 0; aoe--) {
			continue;
		}
	}
	if (string("cizatzqsqwnjxiiwqotififdarc") != string("cizatzqsqwnjxiiwqotififdarc")) {
		int yzagrdtt;
		for (yzagrdtt = 3; yzagrdtt > 0; yzagrdtt--) {
			continue;
		}
	}
	return string("gkh");
}

double sphdndj::wxmyrtgwdzgn(double yqrankhhfugtj, string epubp, double qwllxottor, double kjbiwjqdsc, int ozsqvdcwqrrw) {
	int qofvduot = 6423;
	int mevxwuz = 127;
	double zfgcrvoofjomgw = 7477;
	string asdtzlqylv = "kabyxmifwtcvekgsluylfqkbgxmupygdydwuy";
	if (7477 == 7477) {
		int cykujfbmc;
		for (cykujfbmc = 8; cykujfbmc > 0; cykujfbmc--) {
			continue;
		}
	}
	if (7477 == 7477) {
		int uzyhcyhkpq;
		for (uzyhcyhkpq = 92; uzyhcyhkpq > 0; uzyhcyhkpq--) {
			continue;
		}
	}
	if (7477 == 7477) {
		int zimvqvzk;
		for (zimvqvzk = 89; zimvqvzk > 0; zimvqvzk--) {
			continue;
		}
	}
	if (string("kabyxmifwtcvekgsluylfqkbgxmupygdydwuy") == string("kabyxmifwtcvekgsluylfqkbgxmupygdydwuy")) {
		int jzjgtfd;
		for (jzjgtfd = 42; jzjgtfd > 0; jzjgtfd--) {
			continue;
		}
	}
	return 61189;
}

void sphdndj::dlkgbbsiljvcs(string nexcmgrzrwt, bool ihikkxrfy, string aqknndpfdsl, double mnpizmpiwhnhi, string jplmfkp, bool kboqkcmfekbq, double nimiektjx, double fmatnkuclhogmnv, string rgzxjxru) {
	bool pwmtxyg = false;
	string huqgyyuxy = "xopditowvhhkyjjppiqoteghceczniehghbguqffjsqlhjmpvzwclsmfafdqvpdhlnhldeojfuuspjach";
	string umiow = "veozpvgpsfaviqdwmdwpjslgomrjcvzjahsajwhhyzrocdrrwnmznqyh";
	if (string("veozpvgpsfaviqdwmdwpjslgomrjcvzjahsajwhhyzrocdrrwnmznqyh") == string("veozpvgpsfaviqdwmdwpjslgomrjcvzjahsajwhhyzrocdrrwnmznqyh")) {
		int pxkhbbcnxn;
		for (pxkhbbcnxn = 3; pxkhbbcnxn > 0; pxkhbbcnxn--) {
			continue;
		}
	}
	if (string("xopditowvhhkyjjppiqoteghceczniehghbguqffjsqlhjmpvzwclsmfafdqvpdhlnhldeojfuuspjach") != string("xopditowvhhkyjjppiqoteghceczniehghbguqffjsqlhjmpvzwclsmfafdqvpdhlnhldeojfuuspjach")) {
		int sc;
		for (sc = 78; sc > 0; sc--) {
			continue;
		}
	}
	if (false == false) {
		int ixd;
		for (ixd = 12; ixd > 0; ixd--) {
			continue;
		}
	}
	if (string("veozpvgpsfaviqdwmdwpjslgomrjcvzjahsajwhhyzrocdrrwnmznqyh") != string("veozpvgpsfaviqdwmdwpjslgomrjcvzjahsajwhhyzrocdrrwnmznqyh")) {
		int innakcfqs;
		for (innakcfqs = 1; innakcfqs > 0; innakcfqs--) {
			continue;
		}
	}
	if (string("xopditowvhhkyjjppiqoteghceczniehghbguqffjsqlhjmpvzwclsmfafdqvpdhlnhldeojfuuspjach") == string("xopditowvhhkyjjppiqoteghceczniehghbguqffjsqlhjmpvzwclsmfafdqvpdhlnhldeojfuuspjach")) {
		int ejvwzq;
		for (ejvwzq = 58; ejvwzq > 0; ejvwzq--) {
			continue;
		}
	}

}

void sphdndj::athdbsqpdtpuhknnb(bool xibcvwi, int kkdzvivdpjzg, bool izpfedlsqhm, string rgfaofjdihik, bool vsknhvtezxe) {
	bool enlfbboz = false;
	int eutrlu = 979;
	int ebqknrqbykoknzb = 3830;
	if (979 != 979) {
		int iima;
		for (iima = 49; iima > 0; iima--) {
			continue;
		}
	}
	if (false == false) {
		int cl;
		for (cl = 33; cl > 0; cl--) {
			continue;
		}
	}

}

int sphdndj::yhdugkfsiedumeel(bool tgsvvwppy, double edusojmgzjyb, int nacfxkd, int bkqirh, int outyfbwjukj, int qspvahkruujrcw, bool zvdgleunriajzvy) {
	double xzmkqos = 13530;
	string yqeuqmiwdgqjcq = "zharrzkgjadxkawaibfmluaxhyyzkciolhwwuya";
	double rfmffaizq = 75552;
	string etfyvaxikvj = "xaypizgfjxutkjfnqkxwhtpxkzkutc";
	bool vbkmxxbqwomtamk = true;
	string dzskjnao = "fnyyogbhigzs";
	int unkec = 4904;
	bool bawqbtyzp = true;
	double pmnjzxhwdzlfmls = 40589;
	if (75552 != 75552) {
		int tvhgcvgra;
		for (tvhgcvgra = 70; tvhgcvgra > 0; tvhgcvgra--) {
			continue;
		}
	}
	if (string("fnyyogbhigzs") != string("fnyyogbhigzs")) {
		int ynfxigvy;
		for (ynfxigvy = 69; ynfxigvy > 0; ynfxigvy--) {
			continue;
		}
	}
	return 22688;
}

bool sphdndj::hxjsuwwloegwipeqxtvjdbik(bool nrzlxwarwsdx, string ztmfxncfshiersm, int ilecbvsur, int woqwxmwpizl, bool oxswrctf) {
	bool dilcnzrkqakon = false;
	int ryahlqwauz = 1213;
	string ljlfs = "dkcridbxnejyzoklgnqeqtjskmjlaalmzptwdnfsfioosgdgimppgsjtda";
	int vuscpksymwfdqj = 1115;
	string derfdcnwu = "cvfejixtejdfkbjkqdkuoupyxmegpncioglypcxpgkorhaozvtcylvzwogiyzyqqmfcqkompgiwribkzcsesoaadjkurlinero";
	int silwd = 5153;
	if (1213 == 1213) {
		int uoei;
		for (uoei = 83; uoei > 0; uoei--) {
			continue;
		}
	}
	if (string("dkcridbxnejyzoklgnqeqtjskmjlaalmzptwdnfsfioosgdgimppgsjtda") == string("dkcridbxnejyzoklgnqeqtjskmjlaalmzptwdnfsfioosgdgimppgsjtda")) {
		int rtcdeij;
		for (rtcdeij = 21; rtcdeij > 0; rtcdeij--) {
			continue;
		}
	}
	if (string("cvfejixtejdfkbjkqdkuoupyxmegpncioglypcxpgkorhaozvtcylvzwogiyzyqqmfcqkompgiwribkzcsesoaadjkurlinero") != string("cvfejixtejdfkbjkqdkuoupyxmegpncioglypcxpgkorhaozvtcylvzwogiyzyqqmfcqkompgiwribkzcsesoaadjkurlinero")) {
		int rrtqx;
		for (rrtqx = 55; rrtqx > 0; rrtqx--) {
			continue;
		}
	}
	if (string("cvfejixtejdfkbjkqdkuoupyxmegpncioglypcxpgkorhaozvtcylvzwogiyzyqqmfcqkompgiwribkzcsesoaadjkurlinero") == string("cvfejixtejdfkbjkqdkuoupyxmegpncioglypcxpgkorhaozvtcylvzwogiyzyqqmfcqkompgiwribkzcsesoaadjkurlinero")) {
		int kabspqydjn;
		for (kabspqydjn = 54; kabspqydjn > 0; kabspqydjn--) {
			continue;
		}
	}
	return true;
}

void sphdndj::lsizkvobtxsnxwjkszzzsaxt(int jetpsdkayy, int qejbnmqtuuir, double jnltzxufj, bool rqrcj, double ppysupwwcgdvj, double lklfptldxk, double gvpslkl) {
	double wewoocpdbqb = 31127;
	double kmjsqtsehffcbi = 38405;
	string arykcollyexdq = "jwzkiuhkhexyighpftcgdtiholaqciz";
	if (string("jwzkiuhkhexyighpftcgdtiholaqciz") == string("jwzkiuhkhexyighpftcgdtiholaqciz")) {
		int jdq;
		for (jdq = 5; jdq > 0; jdq--) {
			continue;
		}
	}
	if (31127 == 31127) {
		int ccbpiykp;
		for (ccbpiykp = 19; ccbpiykp > 0; ccbpiykp--) {
			continue;
		}
	}
	if (string("jwzkiuhkhexyighpftcgdtiholaqciz") != string("jwzkiuhkhexyighpftcgdtiholaqciz")) {
		int fhdhanqe;
		for (fhdhanqe = 37; fhdhanqe > 0; fhdhanqe--) {
			continue;
		}
	}

}

void sphdndj::ufqrualxhm(double ewvlbanhezi, string zfltlmeevw, int arfrlp, int jhkfhs, string fcyoyfhe, bool dpxnvplhlbrtrgj, string sjfqawxwmbvui, string jayimjkjjz) {
	bool ikkpxhu = false;
	string ortig = "whkfpvplvntbwgjltztqyjzwurwmbsszqulatpmkohqymdhnmaksftejwkjmfjzthqviayfpr";
	bool vaacja = false;
	int svgmeyyf = 1539;
	double bcanl = 47006;
	int hrtkqcqspbm = 1032;
	int oivrbiqpbafdaf = 445;
	double atsksh = 3203;
	string jgbgebnsh = "wtlozbmgqszxyjvnamonujy";

}

string sphdndj::bkeudcwemv(string byqubvstwrm, bool eezqiyonchdxot, bool lxkoycqzmv, double mxesuvqq, double mswejricgtky, bool wsmezeaavrsv, int frggefyehjbgbvl, double ktkeddbvmy, string sfvfck, string nhgcmnib) {
	string zsinrihgaaxegsv = "xyzdlqeo";
	bool yzezrvk = true;
	return string("xivtyxybdawmzdydt");
}

double sphdndj::tgkeywqgyphqubqbzh(double gvwykpkg, int pobriugrrpp, string dtius, bool lwfbzom, int izikstkw, int jqaprid, string qrdvbzbbixokx) {
	string aneofkbgvt = "vvjxqfucfwnrepncmjkkqjphjyecmdkxpwxbovhsoqqzlysnemhtzwismhtampwvryrflitqokahphm";
	int novgzkhssxwz = 1867;
	double hombsximdc = 36238;
	bool bjdntpt = true;
	double enymefjhnsv = 59550;
	bool rnwxxavcpsqme = true;
	string wfgzwmwnkyts = "rmmskeohqaxtxodojumnnhmadnabhghcmkfcuydyxatteiyjlluxesupzdejidxerfqmiudnbyngzgm";
	double sarkldvjpehqmwy = 5567;
	bool jqkmdjf = false;
	bool miymphliikcg = false;
	return 94994;
}

double sphdndj::lhamrxzwxxepvgdxukaw(double jcmye, int qsmirxapsrwzafb, double cilcpxlljso, string smlzyvhwszob, bool ychbtwp, int xpplvfuilccgnku, bool ljnzcvyqnpyn, double ckoaqcmkq, int pkvet, string mglxogw) {
	return 8072;
}

sphdndj::sphdndj() {
	this->wxmyrtgwdzgn(34329, string("dpxxxyuoeaqkvykgsxzlgxbezthabptmkfeljluuyyvtwpbfcrluaqnfsnqdskllidoydrwvbxaypbamfswjeymoggmdds"), 76125, 92181, 460);
	this->dlkgbbsiljvcs(string("vdyfisbtjrmpoxovkwelljelwjuncbdigpgepj"), false, string("apusyiicejukkrffjaklufynpuxkwikldywqwhutygtnqbynixtwsppkddimwraaenxmjyoorrhzxazyk"), 24306, string("sdnlcgxyskvilokvawufepnmauegeynnmtaepjtcvxsplbllvpubjprcyumendkwhpzhrxovbjdcgtoygbcyiffwgizda"), false, 18674, 65259, string("bekouujqnlefnteefmefjflxlthybxmpspvfsxbkjmecdnbnmjjftnwnlibclslu"));
	this->athdbsqpdtpuhknnb(true, 2174, true, string("cbhdeoweegvpouiaytyefirphwnnehsxavtqhxwbnzknwictbgzjdalvkpcohahaoxcuqg"), false);
	this->yhdugkfsiedumeel(true, 28421, 480, 311, 4701, 3003, false);
	this->hxjsuwwloegwipeqxtvjdbik(true, string("lahdklyqxsgblienhmuuaztfhkpnhfzfkphitvn"), 3148, 1499, true);
	this->lsizkvobtxsnxwjkszzzsaxt(3029, 1897, 78013, false, 27422, 3566, 11212);
	this->ufqrualxhm(3505, string("ljeyvregjqkxsvssp"), 2346, 1133, string("fniwwxbonuuhxtsfkvkfzbimxa"), false, string("rgbvwqeyjwnejjbmtfiqotedsqsnwjljgdtxwjkcdhigsvqgygsugzgppflsmeqrqxjslgujqqckiwuvcecjvvzn"), string("vyubpkuustclxhigdmafyhljxaqargftrgwfbaalocjrgittmxubydtfqnrtjofqypvemesmlxqyydlqcbwl"));
	this->bkeudcwemv(string("gyltndqurtzhafydetpzcnnpwzbpcfedrnpadnuqsrrioywtbxalldulhtjhshaicneiivhpnkcjt"), false, false, 8578, 70301, false, 1411, 10433, string("vbghswpyqbjcpgwrpbbleyiijamioiuqijnjd"), string("namkjlyffjozorjiqrzylohshpyaqshpubrhaapyouebfdlfwihudpxmtgvqrnzrpiotcrqntunb"));
	this->tgkeywqgyphqubqbzh(4547, 3880, string("zsrynnfjfplcuailwswwpsvqchaqiiyqhfcnalnasofpbmzzqgpyhcuakzjtqeitzpipkygeezncloixowxn"), true, 5086, 1509, string("gewuoidmto"));
	this->lhamrxzwxxepvgdxukaw(39811, 5500, 20540, string("mkjbnggabjuwk"), false, 887, false, 14930, 15, string("ftodzgfkqhkpecktlkontkvfqkorlfbfrvbrzgbjlmcqtntggxj"));
	this->eanbspvphwjjlpncqjfetu(true);
	this->sfrizcwsaljdao(847, 3595, true, 66465, 55772, 1513, 250, true);
	this->cjifabamstf(string("ehwumqddjtjzfdatlbavqpfuhhzywuqeefoqssdcclbmldyluiphptNAI"), string("eizoknvuopehjjbeomiuzbogegqkxgyqbcxaasaxesffpjidtbdxpgfcc"), false, 2500, string("tiebfblglijhrxsomxhjqyjcqmiwwodpcjkgkbenopxezsjecuvznnbjvgddjxptxromobazhuwtoflrkzz"), 12071);
	this->cxxgjywitvjikudbksyvngdi(5677, string("ujhsnkossyjmgkajrhyfuuewowbidoxmepfwmchwhhrvscgbvdvjcuphkzgifuoasrqlw"), string("nqitjopnmahhazbmojlqkrrqmfjhmzjgilxqsoetzkyrwocrbmrkjovqoctobhxqzcvtpzbltqqykqndsxtbbgqecfsgfzivlxb"), 6114, false, 3516, 57560, string("pecbggujkbtvfwsbovgamcyywkxnnpklitwjsonqfxfbpzvrbuaqdfwilhduzagsldfo"), true, string("mwwidekcnuciglkjpitrzptjrkmfqizxmgdcohvqagbtyzoypuwefufokfkxnuazwiagirglbyglzuhuarpnnf"));
	this->nrzpqnmadiirfqjlpooql(false, true, false, 4, string("tjpwfbhpuvtzrtqhssbqbwbeqenhsajxurxapujtwscuuznzofxoqciivjeshgjmggvjaimwwuvgdgrutbxyyzshetieakkzqv"), string("utfwuttqawfmnmjcncfwjgjxhomzaftddgksmstwfxqacqvidxgksoaw"), 1332, false, string("fcytigluv"), 601);
	this->bozazshxkzlonatjswsjpnvva(string("zdbeqmndfrsmpawsmpvcbmaqhvlgtbgjwyfhqcrrkpvjizxrcmqnjlhyarbpqtdwcjyqazk"), 36812, true, true, false, 44312, 27878);
	this->bmnupzayxtnzuvptxmoaz(5189, 240, 3138, string("rfkegfashtvafhgfkuxoxnzcxkzpnalh"), false);
	this->azocletmqvwnuirvmj(string("mpzxvqb"), 533, 5379, true);
	this->gajycmutxpfeectqgd(true, string("bikdtmaqimqngbshxigrdlgekqwcxqxbzwoiynedhospythyryisesvhsxjinkuqnkxgyehshikgmejreextzjavynfemafcvnhl"), 5108, 17883);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class orivgbl {
public:
	int dijdlmpjpz;
	int nbkolznbv;
	orivgbl();
	string meojimbjgkqorhirpawqfpy(string yvlahlg, bool pnydiyvhtvq, bool sznbzqyrwkc, double ddondyjnifnya);
	string hetkxwgjjbpmki(bool zhvqfbpftt, bool epwsrcfq, int woemfiob);
	bool zvwsjcyelxsiclszzhcrc();

protected:
	int jsyokwgvfmazc;
	double qmwtjmfgh;

	string pepnxmynejbragka();
	string vfzsqljmpti(double cqpslz, int xjascann, int ceznxucjywekvj, bool oskyzuqeqtda, bool aagggjvbra);
	string vxltmdjegepleeqahpgvccmcq(string tykdlmo, string trpiwiojduku, bool lzryvadznuivqwt, int llodqm, bool xrtlkgyf, bool aauphv, int tsjolw, string tvtmgiktpxdja);
	int ooydtdpuccxpqhsdhnx();
	double nswwyfyfeiqvehpojdg(double scmwaqfnakidn, bool lhekwrut, bool vglydejweqt, double eanlygiipdue, double igmrqxemtvew, bool zgkalifqjlnp);
	bool lnacaqytqqspq(bool mbldvkygucjtjhr, double zfaobfndmx, string frynteeaoiqz, string uvinurcqmhpqqxw, string eothkfrrkpsuk, int ruawczmx, bool cuuhgtx, string nkbya, string dajgqjgdrjvbhm, int lacukku);
	void iwyphvqggekst();
	bool twrvgxumxjvceumiquzpla(double xomcajegmfg, int yvtyo, int lklbxrkqvh, string xnhrvaz, string kccplhvxexfz, double lzinqpnfrg);

private:
	bool evlixd;
	double clopnyuxwagmit;
	int tfiwanltlgmw;
	int dagefz;

	void kcaenrimjamrkfj(double togvxm, string hpfvvbcxtjgznz, string mcdvxccqmjs, bool oyewbicz);
	int itfxawhmdqby(string kwghhnk, double oclnc, bool zktymjomhbgjtd, bool bemjtdkjjj);
	void zrifidybgnojntxttvhtic(bool zvfuq, string grjyrfasbdm, bool lxgsnqaoqjxjz);
	void kzglzjskrhfsnlzjaej(string apyiiqqaqtbiwgl);

};


void orivgbl::kcaenrimjamrkfj(double togvxm, string hpfvvbcxtjgznz, string mcdvxccqmjs, bool oyewbicz) {
	int vfqhcavenf = 3454;
	string xbklsfacco = "tooevotxuchnayqdgtfhzfavrbeoaxfupzaqdbjdgzcczmryrnuopxwlmiytlksy";
	string biekhnczincwm = "avlteklvnxlezjxnfjynmdptnhhpkeawdguomwbtndjlitudfx";
	int fkzgwxjyzem = 25;
	string kwormbfalzgqc = "olemhnyjplzktsykxomkpeafkojrxvabzmltigpgahveazinycvnvwgjcuiugikfosfldpvodmjcdnjfsrtjkpoih";
	int oosmykuehqkgk = 245;
	int xsubnwz = 2227;
	int pqurrb = 1271;
	string stoizxlbhvrlk = "xogxihsdwnmbmaijsqmcxusnjnrkjwqwajtkesjdcpbgffrktwezoieioneqdjuamtrcpupzhhg";
	int hpzqn = 7226;

}

int orivgbl::itfxawhmdqby(string kwghhnk, double oclnc, bool zktymjomhbgjtd, bool bemjtdkjjj) {
	int hczzlchakzozj = 738;
	if (738 == 738) {
		int uwgexrssd;
		for (uwgexrssd = 72; uwgexrssd > 0; uwgexrssd--) {
			continue;
		}
	}
	if (738 != 738) {
		int ck;
		for (ck = 74; ck > 0; ck--) {
			continue;
		}
	}
	if (738 != 738) {
		int tbim;
		for (tbim = 19; tbim > 0; tbim--) {
			continue;
		}
	}
	if (738 == 738) {
		int bubwex;
		for (bubwex = 65; bubwex > 0; bubwex--) {
			continue;
		}
	}
	if (738 != 738) {
		int pcxbbcnq;
		for (pcxbbcnq = 91; pcxbbcnq > 0; pcxbbcnq--) {
			continue;
		}
	}
	return 9294;
}

void orivgbl::zrifidybgnojntxttvhtic(bool zvfuq, string grjyrfasbdm, bool lxgsnqaoqjxjz) {
	bool cowftxpwvdfixp = false;
	double mnyizehicpgtey = 11074;

}

void orivgbl::kzglzjskrhfsnlzjaej(string apyiiqqaqtbiwgl) {
	double qepzaupaqpei = 19309;
	string qqtjjlbssbo = "xlyxtcgsstyaydwehhwqiagruthibrpdrseffnqoahqdn";
	double btpbdqnepgczgfh = 21083;
	double foxppzzronfnry = 17908;
	bool jdyffxcgfwthx = true;
	if (21083 != 21083) {
		int qxsfnlzc;
		for (qxsfnlzc = 60; qxsfnlzc > 0; qxsfnlzc--) {
			continue;
		}
	}
	if (17908 != 17908) {
		int utbhgivuz;
		for (utbhgivuz = 19; utbhgivuz > 0; utbhgivuz--) {
			continue;
		}
	}

}

string orivgbl::pepnxmynejbragka() {
	string zgszcymqow = "yxwvtgbutvzhthhjewxrvojtcelmkrcjohyxyxjhenlkggdqdzzxpvyliquvrfjmrhzljzusglewmtntjenjdqxicwpcnoonl";
	double gbhhmihherfkaw = 22527;
	string lkmuznfotgireid = "xrjefokpznzkbjrsdkydttwdistehycncnlctjlgohavvwjeqtkyqmurihbggrjimavhtcnuarslmbegksptmyngznqq";
	double mfoztjr = 248;
	bool qnpqugmdjanoqyv = false;
	double yunck = 33002;
	int ckdhitabdlcz = 619;
	bool gtdlrhadqvbldca = true;
	string pkpurqamqd = "vdqwibvtw";
	if (true != true) {
		int nbsaf;
		for (nbsaf = 24; nbsaf > 0; nbsaf--) {
			continue;
		}
	}
	return string("nfiotuefto");
}

string orivgbl::vfzsqljmpti(double cqpslz, int xjascann, int ceznxucjywekvj, bool oskyzuqeqtda, bool aagggjvbra) {
	bool hzzkio = true;
	int igtamylfmevilhz = 5906;
	bool fmjoaqolq = true;
	string pevqyjkwwdolvli = "urlupdmieootjpzrgbbxiuovxbdakespoinppwqwujptvc";
	int ncycjoyrt = 5813;
	int tbyew = 1646;
	if (5813 != 5813) {
		int ptcy;
		for (ptcy = 41; ptcy > 0; ptcy--) {
			continue;
		}
	}
	if (1646 == 1646) {
		int lgjd;
		for (lgjd = 18; lgjd > 0; lgjd--) {
			continue;
		}
	}
	return string("lstpdyhfahnubo");
}

string orivgbl::vxltmdjegepleeqahpgvccmcq(string tykdlmo, string trpiwiojduku, bool lzryvadznuivqwt, int llodqm, bool xrtlkgyf, bool aauphv, int tsjolw, string tvtmgiktpxdja) {
	double xvtimg = 18183;
	int nterm = 4855;
	string owioj = "idaqbatsxrbqoovzgrxoicmdmwesodhhjsaiylfkugzlqszfwyctivkkpauwvgrxjojbrngrftukdvvqxymxmspfzfreylfv";
	int irulelqkn = 1395;
	double yxyqpc = 10664;
	string dlwxdcoffu = "ghjtahozmrybfahwddcqqhmmvikvrnhtudkghoskeygfgjakkocfzyykdupvjmji";
	bool uijidxyasdaxov = false;
	if (false != false) {
		int oe;
		for (oe = 50; oe > 0; oe--) {
			continue;
		}
	}
	if (false != false) {
		int rjm;
		for (rjm = 95; rjm > 0; rjm--) {
			continue;
		}
	}
	if (false != false) {
		int dghyftbkj;
		for (dghyftbkj = 85; dghyftbkj > 0; dghyftbkj--) {
			continue;
		}
	}
	return string("ddykg");
}

int orivgbl::ooydtdpuccxpqhsdhnx() {
	double ikngsekzva = 31277;
	double tdolgq = 20168;
	double aohnokozwel = 23869;
	double olysk = 14690;
	if (20168 == 20168) {
		int rbz;
		for (rbz = 89; rbz > 0; rbz--) {
			continue;
		}
	}
	if (23869 == 23869) {
		int zj;
		for (zj = 46; zj > 0; zj--) {
			continue;
		}
	}
	if (23869 == 23869) {
		int ff;
		for (ff = 56; ff > 0; ff--) {
			continue;
		}
	}
	if (31277 != 31277) {
		int juiejcamvc;
		for (juiejcamvc = 50; juiejcamvc > 0; juiejcamvc--) {
			continue;
		}
	}
	return 49331;
}

double orivgbl::nswwyfyfeiqvehpojdg(double scmwaqfnakidn, bool lhekwrut, bool vglydejweqt, double eanlygiipdue, double igmrqxemtvew, bool zgkalifqjlnp) {
	int bhdqudqxpi = 3143;
	double mibajz = 9703;
	string xpdyswlm = "fqdjxfkpvefgxdiuiizrmnjgemjniuixljnrcvshhalmmgjjamqavlsaokzlserabzebyexadxgrpnfdwliw";
	int zlvahony = 418;
	bool xzbrhnynj = true;
	int sczyhveuwwmpkuz = 1391;
	double vcpagjllj = 268;
	int mkatz = 592;
	string rxftnsjzzb = "aptuxvkmatwhtrxlyrhrqraidvw";
	if (418 == 418) {
		int bbjglcsm;
		for (bbjglcsm = 74; bbjglcsm > 0; bbjglcsm--) {
			continue;
		}
	}
	if (true != true) {
		int rvce;
		for (rvce = 21; rvce > 0; rvce--) {
			continue;
		}
	}
	return 23063;
}

bool orivgbl::lnacaqytqqspq(bool mbldvkygucjtjhr, double zfaobfndmx, string frynteeaoiqz, string uvinurcqmhpqqxw, string eothkfrrkpsuk, int ruawczmx, bool cuuhgtx, string nkbya, string dajgqjgdrjvbhm, int lacukku) {
	bool fubustspa = true;
	string giiedzi = "onsgp";
	bool apqujjvblkjnexw = false;
	string vfpsleozvhi = "pkybhivnvwxykpqcszopjckwzeickzrabglxxq";
	bool kpiwqtlsbz = false;
	string lwgjeqrqjbr = "nxvpftfbnqbdgvjvnltgxaugtomkdmqawqrnvkvcojmzrznybfxwijyitl";
	if (string("onsgp") != string("onsgp")) {
		int nhzunlr;
		for (nhzunlr = 45; nhzunlr > 0; nhzunlr--) {
			continue;
		}
	}
	if (string("pkybhivnvwxykpqcszopjckwzeickzrabglxxq") != string("pkybhivnvwxykpqcszopjckwzeickzrabglxxq")) {
		int rv;
		for (rv = 94; rv > 0; rv--) {
			continue;
		}
	}
	if (false != false) {
		int czcoqzfcv;
		for (czcoqzfcv = 98; czcoqzfcv > 0; czcoqzfcv--) {
			continue;
		}
	}
	if (string("onsgp") != string("onsgp")) {
		int pzsiyrzc;
		for (pzsiyrzc = 56; pzsiyrzc > 0; pzsiyrzc--) {
			continue;
		}
	}
	if (string("pkybhivnvwxykpqcszopjckwzeickzrabglxxq") != string("pkybhivnvwxykpqcszopjckwzeickzrabglxxq")) {
		int cfktfx;
		for (cfktfx = 92; cfktfx > 0; cfktfx--) {
			continue;
		}
	}
	return true;
}

void orivgbl::iwyphvqggekst() {
	string iocwkeowv = "vrpmvadivhuzcisfyhhyvlzdtdpzfbworbdzpl";
	int fbccbnhtito = 1944;
	double exgmzagzldi = 77846;
	int ttezjrly = 2797;
	if (77846 != 77846) {
		int wj;
		for (wj = 94; wj > 0; wj--) {
			continue;
		}
	}
	if (2797 == 2797) {
		int mvqys;
		for (mvqys = 20; mvqys > 0; mvqys--) {
			continue;
		}
	}

}

bool orivgbl::twrvgxumxjvceumiquzpla(double xomcajegmfg, int yvtyo, int lklbxrkqvh, string xnhrvaz, string kccplhvxexfz, double lzinqpnfrg) {
	return false;
}

string orivgbl::meojimbjgkqorhirpawqfpy(string yvlahlg, bool pnydiyvhtvq, bool sznbzqyrwkc, double ddondyjnifnya) {
	int bnxxz = 1662;
	string qldne = "bucvwcnoynvnihxnpkznftrgxziyoqkpyufumfvctopqjzhbrjgiuaqyfvgenufplunpz";
	string aujhjoejnj = "lqarauabbhvdajomdzcfkeizgjaobuxdrazakapshubzpbnanetdbj";
	if (string("bucvwcnoynvnihxnpkznftrgxziyoqkpyufumfvctopqjzhbrjgiuaqyfvgenufplunpz") != string("bucvwcnoynvnihxnpkznftrgxziyoqkpyufumfvctopqjzhbrjgiuaqyfvgenufplunpz")) {
		int qihthdqcv;
		for (qihthdqcv = 78; qihthdqcv > 0; qihthdqcv--) {
			continue;
		}
	}
	if (string("bucvwcnoynvnihxnpkznftrgxziyoqkpyufumfvctopqjzhbrjgiuaqyfvgenufplunpz") != string("bucvwcnoynvnihxnpkznftrgxziyoqkpyufumfvctopqjzhbrjgiuaqyfvgenufplunpz")) {
		int cc;
		for (cc = 26; cc > 0; cc--) {
			continue;
		}
	}
	return string("brvygixrvmftugw");
}

string orivgbl::hetkxwgjjbpmki(bool zhvqfbpftt, bool epwsrcfq, int woemfiob) {
	int cqlrq = 1015;
	if (1015 != 1015) {
		int fzozpz;
		for (fzozpz = 55; fzozpz > 0; fzozpz--) {
			continue;
		}
	}
	if (1015 != 1015) {
		int fdpqqngxmk;
		for (fdpqqngxmk = 9; fdpqqngxmk > 0; fdpqqngxmk--) {
			continue;
		}
	}
	if (1015 == 1015) {
		int lsaq;
		for (lsaq = 71; lsaq > 0; lsaq--) {
			continue;
		}
	}
	if (1015 == 1015) {
		int ic;
		for (ic = 52; ic > 0; ic--) {
			continue;
		}
	}
	return string("rpnlumxz");
}

bool orivgbl::zvwsjcyelxsiclszzhcrc() {
	bool uarzg = true;
	int udhludfqq = 2274;
	int bxgmzynemtmjzr = 542;
	if (true != true) {
		int xpce;
		for (xpce = 37; xpce > 0; xpce--) {
			continue;
		}
	}
	if (true == true) {
		int msbuycbir;
		for (msbuycbir = 53; msbuycbir > 0; msbuycbir--) {
			continue;
		}
	}
	if (542 == 542) {
		int yjgxof;
		for (yjgxof = 16; yjgxof > 0; yjgxof--) {
			continue;
		}
	}
	if (2274 == 2274) {
		int frdcps;
		for (frdcps = 44; frdcps > 0; frdcps--) {
			continue;
		}
	}
	if (542 == 542) {
		int agzpdb;
		for (agzpdb = 87; agzpdb > 0; agzpdb--) {
			continue;
		}
	}
	return false;
}

orivgbl::orivgbl() {
	this->meojimbjgkqorhirpawqfpy(string("fhyrbebnbtqyaumpfrdukswuflqrbbvshudelivekxdjziriemsyywltddubeqcosxsmckhpjg"), false, false, 51087);
	this->hetkxwgjjbpmki(false, false, 162);
	this->zvwsjcyelxsiclszzhcrc();
	this->pepnxmynejbragka();
	this->vfzsqljmpti(33539, 7120, 998, true, true);
	this->vxltmdjegepleeqahpgvccmcq(string("tfpau"), string("phnqgzdzfqdipzdydlnpfegpwjradwhxcjuceemyggpwgfsyzpxdrqkbmggkfrvpdjg"), false, 1634, true, true, 4501, string("uiqfkgzqplkbvqwvxpsbluckdregtxrqhtuuve"));
	this->ooydtdpuccxpqhsdhnx();
	this->nswwyfyfeiqvehpojdg(16917, true, false, 10316, 22975, false);
	this->lnacaqytqqspq(true, 629, string("ehckiahfgcktzywqqk"), string("cvlqyniekhloepqxklbzaaqxzdktdrnpehwojewm"), string("nrlfghgvsmfjjhwjpuoetrgljhxmcxmo"), 256, true, string("tgewzvmxobknhueuupoxonbymzjjcvjwwsiiwbgiytclrbqfxgjnuuyvjcyxlh"), string("hrfkfybfqgfhhaeuzmasvkifhwoamqocpcftmkwkubfkeiehqhldwkegsrhqajpsg"), 2048);
	this->iwyphvqggekst();
	this->twrvgxumxjvceumiquzpla(3917, 8334, 2633, string("s"), string("qdnvzczwvtkfqflyoslysownc"), 54950);
	this->kcaenrimjamrkfj(21649, string("wcqdwavsesyhjcfxwxssjpnnlhvvwfkepkiehqdxhyhfjkrsvysmaylhnmpgakj"), string("qwhjmhtutiwbmrpznasnhevuwpbhnazhnyqotoktnzlilmhxkvjqdfydfrqgqnxfuxaienzssinqavsznbjezjsunaheumjzza"), true);
	this->itfxawhmdqby(string("zkvtkgsvwcxbeihrdmvokjvzgrgfyzermftaadjghupbrhiyseivngevnsdkfvzxpvckt"), 7348, false, true);
	this->zrifidybgnojntxttvhtic(false, string("ysnnfdzwlyhexpj"), false);
	this->kzglzjskrhfsnlzjaej(string("prcvsekjszeibkwuuugrnppqyheribzazahnpysbNAIrhueufaodsra"));
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class csvxmux {
public:
	int nllmexqvplhpyv;
	csvxmux();
	int wfyfjkcxtipkhsljz(string qewxzahhihzz, string zhmyghbnftmfwxp, int zmxgspnq, string gbfchih, double mbuyyqrnvnkycc, int ljowu);
	double iwzkjpoaoqcacnslipyd(string ypmucdvamngbuz, int rxubnhgcphkf, bool qamqye, bool juojg, bool oultyhxx, double trjmidlivai, double rnkxyettsvnrau, string errzosvjfbofc, int crbqlmi, double iwiya);
	string vxalvcoigd(bool evfkpw, int lykspypdbxong, int wtboi, int yvijdlgeys, int bseikxp, int htxmbemc, double sxswtvatvrfdge);
	bool lpkqagcmzwvlkekamxt(string xepoembspb, string esswazgypshoo, string tosujuhuv, bool ukuyfwp, bool qzbgqqlwc, bool qxxklerivj);
	bool rjvthoeyivexrdpjwqj();

protected:
	int ugzjyudo;
	int fmcuwbu;
	double vtkadjffnwkju;

	void jqohilzvtpsxuja(double obnqwpbwr, int rirsycctmksfbrg, bool fgxlzugxvl, double buaavjtttu, bool kxakupci, string ctdxqz);
	string dyjmazfgphvofwuajl();
	void wnzunwueegrejvasni(int zeobakhdbhuwwit, string pzjrwvpaecr, bool uezttfqq, bool ghoknhimfrgttu, int wbpuwgufmh);
	void kuwqraelcbwgpngrausknwdbi(double yvfudzwtietsdgm);
	string texmxjmqzsbmvfhhh(bool khhdmjszaizsnf, double anwsmyg, double fiwtkdofobac, string jzgyic, int isymvcdbfowsl);
	bool tcbfyznccmokzhdu();
	string jiyjicffpknlsvvzvydnq(bool acgtxsfdzcxtbli);
	string qabycoybzyekrtwh(int eheqcajczvqo);
	int ytijewzdhqc(bool rjtives);

private:
	int ongyitvihycfz;
	string dbkivb;
	int hvzzirhcpkec;
	int xgddytsaz;

	int ncjfmadssuicqqgna(double rmcfykps, bool urbfyqmrp, double xvggizmn, int wlyzmwvzqq, bool voteynltaohr, string wgjwadtfwdfpxir, double bfoiyohzgysw, int mvovtpefqrmkls);
	string wllqrtjyoaxgeevtjjum();
	string pvesfradpf(int zgyplwd, bool enigvfaxvkekie, int hrkudiqlawrn);

};


int csvxmux::ncjfmadssuicqqgna(double rmcfykps, bool urbfyqmrp, double xvggizmn, int wlyzmwvzqq, bool voteynltaohr, string wgjwadtfwdfpxir, double bfoiyohzgysw, int mvovtpefqrmkls) {
	string sixmxswppfs = "xbznysbfbsrtujeefnahvpjnelaclfqnxbxquiwuipannvnuzjkyfh";
	double uqqfws = 16416;
	double okvujzimx = 1410;
	double tpvckrhwhj = 15631;
	int bdjodhgttvkq = 2431;
	string knhxj = "wettlpluzxfrokxupwpvogdcu";
	if (1410 == 1410) {
		int yd;
		for (yd = 47; yd > 0; yd--) {
			continue;
		}
	}
	if (15631 == 15631) {
		int lkhoz;
		for (lkhoz = 76; lkhoz > 0; lkhoz--) {
			continue;
		}
	}
	if (string("xbznysbfbsrtujeefnahvpjnelaclfqnxbxquiwuipannvnuzjkyfh") != string("xbznysbfbsrtujeefnahvpjnelaclfqnxbxquiwuipannvnuzjkyfh")) {
		int oogblogmc;
		for (oogblogmc = 97; oogblogmc > 0; oogblogmc--) {
			continue;
		}
	}
	return 28307;
}

string csvxmux::wllqrtjyoaxgeevtjjum() {
	double glgwxysiigf = 11338;
	double heyoljtm = 51123;
	bool jsmljmgroqzkuc = false;
	string ernvtpnknkugue = "btqrmzxuqczvtkkrwjgchkwazpbfnxugrtjdzsdsaqqrdubmknhlkesbineyebearvnxdjqznoojxm";
	double wprliqavqlap = 9729;
	if (51123 != 51123) {
		int yymzz;
		for (yymzz = 48; yymzz > 0; yymzz--) {
			continue;
		}
	}
	if (11338 == 11338) {
		int fp;
		for (fp = 45; fp > 0; fp--) {
			continue;
		}
	}
	if (9729 != 9729) {
		int mfcb;
		for (mfcb = 61; mfcb > 0; mfcb--) {
			continue;
		}
	}
	return string("bykvzogaiaty");
}

string csvxmux::pvesfradpf(int zgyplwd, bool enigvfaxvkekie, int hrkudiqlawrn) {
	double lyskrlhztaim = 2966;
	double hqgbzbyjuib = 40899;
	string vdzzw = "zk";
	bool txmwkuyka = false;
	bool pdwaccwvtogflyg = true;
	bool jwbrinmkcmjdgg = true;
	int tywcodn = 8166;
	bool xrwrujhshp = true;
	bool hcysarkly = true;
	if (8166 == 8166) {
		int frcduzix;
		for (frcduzix = 94; frcduzix > 0; frcduzix--) {
			continue;
		}
	}
	if (true != true) {
		int fe;
		for (fe = 27; fe > 0; fe--) {
			continue;
		}
	}
	if (true == true) {
		int vfeoxj;
		for (vfeoxj = 83; vfeoxj > 0; vfeoxj--) {
			continue;
		}
	}
	if (true != true) {
		int fazp;
		for (fazp = 7; fazp > 0; fazp--) {
			continue;
		}
	}
	if (false != false) {
		int ntsxiutcae;
		for (ntsxiutcae = 53; ntsxiutcae > 0; ntsxiutcae--) {
			continue;
		}
	}
	return string("ttkjfgkmoltq");
}

void csvxmux::jqohilzvtpsxuja(double obnqwpbwr, int rirsycctmksfbrg, bool fgxlzugxvl, double buaavjtttu, bool kxakupci, string ctdxqz) {
	string dlpfmrbmwsrwh = "dpurkron";
	string lgccfp = "jwzdthzwryqknrzvycedjzeoojgbcowwqamrqqvxgwdgmzkiacobgwnqibawp";
	int diuum = 2234;
	int imqrsohsc = 1822;
	int ipvwvjuwhgcpvb = 1357;
	double xdpvtxjjkw = 4078;
	if (4078 != 4078) {
		int yv;
		for (yv = 6; yv > 0; yv--) {
			continue;
		}
	}
	if (1822 != 1822) {
		int gtpxgehx;
		for (gtpxgehx = 96; gtpxgehx > 0; gtpxgehx--) {
			continue;
		}
	}
	if (string("jwzdthzwryqknrzvycedjzeoojgbcowwqamrqqvxgwdgmzkiacobgwnqibawp") == string("jwzdthzwryqknrzvycedjzeoojgbcowwqamrqqvxgwdgmzkiacobgwnqibawp")) {
		int aikxexmxg;
		for (aikxexmxg = 64; aikxexmxg > 0; aikxexmxg--) {
			continue;
		}
	}
	if (2234 == 2234) {
		int hwfwqfq;
		for (hwfwqfq = 92; hwfwqfq > 0; hwfwqfq--) {
			continue;
		}
	}

}

string csvxmux::dyjmazfgphvofwuajl() {
	int jjpwdx = 4756;
	int aggqpv = 4923;
	bool cfyejepiarcgucr = false;
	double kizboiupdwwgedk = 17211;
	double gxwvrrsllg = 51443;
	int cmpxmepisdhco = 3352;
	if (4923 == 4923) {
		int wwkkehywce;
		for (wwkkehywce = 45; wwkkehywce > 0; wwkkehywce--) {
			continue;
		}
	}
	if (4923 == 4923) {
		int dqgnxkmuwk;
		for (dqgnxkmuwk = 62; dqgnxkmuwk > 0; dqgnxkmuwk--) {
			continue;
		}
	}
	return string("allzhidigq");
}

void csvxmux::wnzunwueegrejvasni(int zeobakhdbhuwwit, string pzjrwvpaecr, bool uezttfqq, bool ghoknhimfrgttu, int wbpuwgufmh) {
	string faucybkvgwuolr = "hlvpfyqidphvpuknawuxphrpyjypjptwyrqvlxgskyakuwrhnd";
	bool mutzqzejsplw = false;
	bool vtoqlvvjsugcai = false;
	double ggigbhleinkyhbc = 29852;
	double hjyiirpl = 10763;
	if (29852 == 29852) {
		int yiu;
		for (yiu = 35; yiu > 0; yiu--) {
			continue;
		}
	}

}

void csvxmux::kuwqraelcbwgpngrausknwdbi(double yvfudzwtietsdgm) {
	bool oyhgrlqabbfld = true;
	double kanhtijduxwd = 35728;
	double wkdlmpln = 1060;
	double llbuzmdpepziykj = 63712;
	bool zlnamteqngk = false;
	string lqqahlcxvexpkv = "izzcoxtufujvvkdxfoafdmymwgttblbvzdbzeedhnwnqwazcyevnjvicywehtytdennrdmo";
	int ikszwkklavpgs = 3417;
	bool rqszkysy = false;
	if (35728 == 35728) {
		int rydqk;
		for (rydqk = 46; rydqk > 0; rydqk--) {
			continue;
		}
	}
	if (true != true) {
		int fnqt;
		for (fnqt = 29; fnqt > 0; fnqt--) {
			continue;
		}
	}
	if (true == true) {
		int hf;
		for (hf = 90; hf > 0; hf--) {
			continue;
		}
	}

}

string csvxmux::texmxjmqzsbmvfhhh(bool khhdmjszaizsnf, double anwsmyg, double fiwtkdofobac, string jzgyic, int isymvcdbfowsl) {
	return string("isnakjmkods");
}

bool csvxmux::tcbfyznccmokzhdu() {
	double ujflyflxhjgksbu = 66341;
	int bsfnpksset = 3046;
	string kfvredqcimtd = "dqddliwylamnzyjxzijtgopxjhckotncqokxjfbumqaupbrvwnfgbzmtoyzlrsqxjljjnctywvtnaowpzeqvbptacjr";
	int bzjpiltv = 969;
	bool qhendmmb = false;
	int sdnny = 7903;
	string uhjlqexvttny = "ntoucbcxhyplfswhpnykkcsbepfizslcvsqzmhnizuvlsfymvlnecyyubnvgykxsetepoylrxzgzmqysonjynwxdm";
	bool xjxbpij = true;
	if (3046 != 3046) {
		int fjqsymxtzy;
		for (fjqsymxtzy = 53; fjqsymxtzy > 0; fjqsymxtzy--) {
			continue;
		}
	}
	if (3046 == 3046) {
		int zfvio;
		for (zfvio = 85; zfvio > 0; zfvio--) {
			continue;
		}
	}
	if (string("ntoucbcxhyplfswhpnykkcsbepfizslcvsqzmhnizuvlsfymvlnecyyubnvgykxsetepoylrxzgzmqysonjynwxdm") == string("ntoucbcxhyplfswhpnykkcsbepfizslcvsqzmhnizuvlsfymvlnecyyubnvgykxsetepoylrxzgzmqysonjynwxdm")) {
		int fz;
		for (fz = 87; fz > 0; fz--) {
			continue;
		}
	}
	if (string("ntoucbcxhyplfswhpnykkcsbepfizslcvsqzmhnizuvlsfymvlnecyyubnvgykxsetepoylrxzgzmqysonjynwxdm") == string("ntoucbcxhyplfswhpnykkcsbepfizslcvsqzmhnizuvlsfymvlnecyyubnvgykxsetepoylrxzgzmqysonjynwxdm")) {
		int nutck;
		for (nutck = 30; nutck > 0; nutck--) {
			continue;
		}
	}
	return false;
}

string csvxmux::jiyjicffpknlsvvzvydnq(bool acgtxsfdzcxtbli) {
	return string("ywaeybaozldgyfhemru");
}

string csvxmux::qabycoybzyekrtwh(int eheqcajczvqo) {
	int uedzdvkzus = 1511;
	bool hcnnmmgj = false;
	int ksnqbxjwd = 148;
	double nbkvnaapakwnub = 15353;
	bool jtjrjfrgp = true;
	string wjleyugzf = "bbpvxtfousrwywhbrvqqomqettgk";
	int lvyxpquyqaphbv = 3169;
	int qkleajlnecvlfyj = 8861;
	int fqlwbpks = 1246;
	double juvecl = 59921;
	if (false != false) {
		int xd;
		for (xd = 69; xd > 0; xd--) {
			continue;
		}
	}
	if (15353 != 15353) {
		int wly;
		for (wly = 20; wly > 0; wly--) {
			continue;
		}
	}
	return string("ugiqslroqile");
}

int csvxmux::ytijewzdhqc(bool rjtives) {
	bool vfudykiasljxwaq = false;
	bool agldiiuovzvx = true;
	double rdzlnepztdbgjq = 2923;
	bool tpjmbmsbqjgo = true;
	bool qkntqz = true;
	double bhjcqs = 34326;
	string bbruau = "bgmloughikfgszq";
	bool hczljguchzm = true;
	return 22341;
}

int csvxmux::wfyfjkcxtipkhsljz(string qewxzahhihzz, string zhmyghbnftmfwxp, int zmxgspnq, string gbfchih, double mbuyyqrnvnkycc, int ljowu) {
	string ryfvg = "fotqxpnqklfnfgdjoaidfvknhmnyllkeqmrobghsafzjnpwvyptdlufsfvxcewmhqfyrdqxnmualaxtkaqdrdgfxcsha";
	int wbwnchmbccbpr = 3255;
	bool vpzudphas = true;
	double tzpzefthrix = 39978;
	bool pgmdbdwuqsjd = false;
	if (true != true) {
		int swgrsnbn;
		for (swgrsnbn = 18; swgrsnbn > 0; swgrsnbn--) {
			continue;
		}
	}
	if (false == false) {
		int toe;
		for (toe = 14; toe > 0; toe--) {
			continue;
		}
	}
	if (3255 == 3255) {
		int dapomzow;
		for (dapomzow = 23; dapomzow > 0; dapomzow--) {
			continue;
		}
	}
	if (false == false) {
		int jenkcedgnv;
		for (jenkcedgnv = 30; jenkcedgnv > 0; jenkcedgnv--) {
			continue;
		}
	}
	if (true == true) {
		int hfp;
		for (hfp = 52; hfp > 0; hfp--) {
			continue;
		}
	}
	return 3576;
}

double csvxmux::iwzkjpoaoqcacnslipyd(string ypmucdvamngbuz, int rxubnhgcphkf, bool qamqye, bool juojg, bool oultyhxx, double trjmidlivai, double rnkxyettsvnrau, string errzosvjfbofc, int crbqlmi, double iwiya) {
	return 88135;
}

string csvxmux::vxalvcoigd(bool evfkpw, int lykspypdbxong, int wtboi, int yvijdlgeys, int bseikxp, int htxmbemc, double sxswtvatvrfdge) {
	bool qlojru = false;
	int mfbdjplvqpzbp = 143;
	string ombwncchectp = "zpkjgqyykmqumzlckxrywqgdwzoggbizkiksxrbjmowlbcfqrfytiaqe";
	string epulpxyblyth = "anzsylpxjynkxftseqstqrcfdjrefupgzrgyxgxbfsgksrcgiemadxqewcptizimiloisosvejycspfeamjpksczoptoj";
	bool xpniwqzwmdm = false;
	string ctdbhaj = "tnqatotdtywrpwgsizvvvzmngodxklbupumlsispurqraiah";
	int cmwutasfteti = 4940;
	string zfvzzfhqn = "nyxwrbzpxvkvsgfymmmkzzysxtyitfvwqgjlcbjhsrvjjsdwzxuecxaqqlfp";
	string otdusimzcx = "wwnpzfaszftlyviwrsuawowfvsxdvkopjdtpplzmhadgkfvsegopyqunflrzyhavoktxizjyknointhanbyctewyirymxzx";
	if (string("nyxwrbzpxvkvsgfymmmkzzysxtyitfvwqgjlcbjhsrvjjsdwzxuecxaqqlfp") != string("nyxwrbzpxvkvsgfymmmkzzysxtyitfvwqgjlcbjhsrvjjsdwzxuecxaqqlfp")) {
		int njn;
		for (njn = 24; njn > 0; njn--) {
			continue;
		}
	}
	if (string("zpkjgqyykmqumzlckxrywqgdwzoggbizkiksxrbjmowlbcfqrfytiaqe") == string("zpkjgqyykmqumzlckxrywqgdwzoggbizkiksxrbjmowlbcfqrfytiaqe")) {
		int fbcrezrhd;
		for (fbcrezrhd = 32; fbcrezrhd > 0; fbcrezrhd--) {
			continue;
		}
	}
	if (4940 == 4940) {
		int rhytbs;
		for (rhytbs = 62; rhytbs > 0; rhytbs--) {
			continue;
		}
	}
	if (string("zpkjgqyykmqumzlckxrywqgdwzoggbizkiksxrbjmowlbcfqrfytiaqe") != string("zpkjgqyykmqumzlckxrywqgdwzoggbizkiksxrbjmowlbcfqrfytiaqe")) {
		int wuvoozbbym;
		for (wuvoozbbym = 2; wuvoozbbym > 0; wuvoozbbym--) {
			continue;
		}
	}
	return string("jlinllopfxxdkz");
}

bool csvxmux::lpkqagcmzwvlkekamxt(string xepoembspb, string esswazgypshoo, string tosujuhuv, bool ukuyfwp, bool qzbgqqlwc, bool qxxklerivj) {
	string kqufiimiwzwgmpg = "duqiawlvbkavygdetezoctl";
	bool mzvjtfmojqt = false;
	string lnmymbokludumgz = "frgllvugpahebupasroyrxwboydzroevajtdaawfkkjzygsntnmdlcdwoweyzfxgubgqwrameez";
	int mytff = 1940;
	bool essbvxwyj = false;
	string odpapfzwu = "lmdfzapzuwyjriqojpmddqnouhjhpsqetbpgekbobzaln";
	bool zfrsfy = false;
	int cvvovl = 3887;
	if (false == false) {
		int qi;
		for (qi = 15; qi > 0; qi--) {
			continue;
		}
	}
	return true;
}

bool csvxmux::rjvthoeyivexrdpjwqj() {
	bool toflmcleg = false;
	int upzvq = 3254;
	int dpgrmskrrqpo = 1957;
	double qcakiupw = 83104;
	double hucqrwroajn = 1230;
	string jhnoiha = "zbovky";
	if (83104 == 83104) {
		int qnksmz;
		for (qnksmz = 95; qnksmz > 0; qnksmz--) {
			continue;
		}
	}
	if (1230 != 1230) {
		int pjku;
		for (pjku = 77; pjku > 0; pjku--) {
			continue;
		}
	}
	return false;
}

csvxmux::csvxmux() {
	this->wfyfjkcxtipkhsljz(string("npiygeyafybyhlhecpnofmsjsqrbfyktnakiuxtovcpsmkipcuflbukkahcy"), string("tohbsuulagqpwdshjqyknfgqnewtgvdxgdcdusgdwhktrqsihfjzlcwqoehmud"), 7186, string("vkjfibyguupqvgokdotkagfiwsyk"), 35111, 2974);
	this->iwzkjpoaoqcacnslipyd(string("hlaivsakwua"), 6830, true, true, true, 23883, 53859, string("nxtczupuxakgwxzrpujvizahpdzyzpublinbqryxvzgknxifkvkofocvzzdbvjbyhbastniedgqzqo"), 1015, 12701);
	this->vxalvcoigd(false, 2189, 1840, 1008, 3116, 204, 9763);
	this->lpkqagcmzwvlkekamxt(string("nqpdotewyjvlexwiwbjfbmxdmkjpjfenwaf"), string("uamesxabwwonegmmejgekztaeslggoyawmg"), string("ygvinodmkipkbivkejbcgeuhsfmblytszmabeqelyxekspysljfmpvrtwfrxkptbwphaaoahwlobz"), true, true, false);
	this->rjvthoeyivexrdpjwqj();
	this->jqohilzvtpsxuja(38753, 882, true, 66254, true, string("aoedqfsntrdklixdcehygpaqohonhapjnypzqchsrqppgirpycyncashqboitekiq"));
	this->dyjmazfgphvofwuajl();
	this->wnzunwueegrejvasni(7684, string("zqiecgrjrpbjsubebmmjrlwcokjeidaabsbqezjklekvvpcdabnwhvhhgfayvamesjqmqszkw"), true, true, 9180);
	this->kuwqraelcbwgpngrausknwdbi(14287);
	this->texmxjmqzsbmvfhhh(true, 36526, 34116, string("fupannjdymmngyffbwabvwxciaiorqhodyqeoqzeqibnpmaaroqiajgfeokzvfmlgous"), 2501);
	this->tcbfyznccmokzhdu();
	this->jiyjicffpknlsvvzvydnq(true);
	this->qabycoybzyekrtwh(6580);
	this->ytijewzdhqc(false);
	this->ncjfmadssuicqqgna(57947, false, 4173, 2194, true, string("nmjxzwxttrpglfhbstywxwpbuvnmzqkaqrhvojzpjbayzbhemeitwvcufpdupvsoheweyonqhaqicvvfealkdsgsdejsooyh"), 70938, 752);
	this->wllqrtjyoaxgeevtjjum();
	this->pvesfradpf(639, false, 4646);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class dtfovwc {
public:
	double mijeljtr;
	double pesvyuxw;
	double wxqelytd;
	dtfovwc();
	int noeuugkmepkicszxd(bool hwgrmqwn, double ymztmyeffst, double jxlobrrd, string ipqsqtuuhviajpd, double kkqzsuuc, string tximqdzhk, double mkiomwqkutotm, double usqbfyrg, bool nhqnsauxths);

protected:
	int cbtagtogfgjdnpn;
	bool ztuteccpkbcxj;
	int awlnyknumuc;
	bool lnrppshalcanlz;
	double lpowmu;

	int nwsadfbwtauayrlfvzuqrlpgs(bool fitymr, string upfsmac, bool dnvekfzv, bool wadmmpja, double tjwpwyfn);
	int izmxnuesihvjam(bool yhegymmj, int awnemdneuygxgjs, string prhlq, double tvpze, int pskvslx, bool nwfxsrojrntrw, bool qrjvgmbxjpoyil, int nxkjtfxiuncdgo, int tsrlrb);
	double kkmqirgxlcmtqlxqli(double dbyfveqebta);
	string mijfclfujbwojnrbpjjvcg(double obstlh);
	bool nbhhwhrnosueakaejnehjrib(string pinlxwptj, double csinrrizmdl, double pslikmouoxxeozy, string pwivdep);

private:
	string fkyswqbal;
	string ojivsq;
	bool rrhtk;

	string alzondwwtjiqiudana();
	int hxrekdftiuwhabwonli(string lkkzcuxqrbvfq, double dzdht, double lgedjcdccnqr, double ouaoxqeeq, string dnywcmf, double qahtdgphmfjeije, bool qbwsbwb, double umigcjsaozmmqnu, double yjwqmr);
	string tmokzjcpxtyvfnebxya(double ltpbsnqhww, int vhzzuvgt, int yakqhbpnilgkjdv, bool ofvpjq);
	bool ecixzvttuxerppyivl(double phdbml, string oynjzbmzjrn, int oiztvsuxexedbi, bool dzdhpqsmwr, double gpzjebcmytzzrsx, int fuhcwcu, double fxqydtojofinhlt, double kahydttfvyeov);
	void whrfcmgpivwzlww(string kbjpv, bool mqyonenhwe, string chfhkii);
	void gwrojrqxctuo();

};


string dtfovwc::alzondwwtjiqiudana() {
	int orfduyjrpihpf = 3008;
	if (3008 != 3008) {
		int gpkwgijk;
		for (gpkwgijk = 51; gpkwgijk > 0; gpkwgijk--) {
			continue;
		}
	}
	if (3008 != 3008) {
		int ygkzf;
		for (ygkzf = 62; ygkzf > 0; ygkzf--) {
			continue;
		}
	}
	return string("");
}

int dtfovwc::hxrekdftiuwhabwonli(string lkkzcuxqrbvfq, double dzdht, double lgedjcdccnqr, double ouaoxqeeq, string dnywcmf, double qahtdgphmfjeije, bool qbwsbwb, double umigcjsaozmmqnu, double yjwqmr) {
	string pcpcubvvfqdtkkm = "aurlwwhqyybqrhzurfpymnwxhflcbipslgozgoltwlrokcfworoabcpbqpywbwhvojfvdkusnegwlsskkgrhhejpzxdr";
	int tolvizl = 1978;
	int uqpaw = 1;
	bool cccnmva = false;
	bool zvzkgqamauvlb = false;
	double rlrrsvhbfvskem = 52531;
	if (false != false) {
		int lyxt;
		for (lyxt = 89; lyxt > 0; lyxt--) {
			continue;
		}
	}
	if (1978 != 1978) {
		int sibpe;
		for (sibpe = 28; sibpe > 0; sibpe--) {
			continue;
		}
	}
	if (false != false) {
		int ipwk;
		for (ipwk = 70; ipwk > 0; ipwk--) {
			continue;
		}
	}
	return 47157;
}

string dtfovwc::tmokzjcpxtyvfnebxya(double ltpbsnqhww, int vhzzuvgt, int yakqhbpnilgkjdv, bool ofvpjq) {
	double cdqdaiggplvsag = 44927;
	double yvyvywmesjk = 82981;
	string beiwgorlmd = "ykcmxhlxierngxtydnxzkmvgyqpzykgskcwkqdniyyfthxyclveyembdoflpngwqpumnigqichkqxpcsj";
	int voanjkysjmci = 1873;
	string vpqdjpavteimzd = "celdxfwyy";
	int izdqsunpru = 2502;
	bool iuuzwscqucvwj = false;
	string bwfmrvtktceoso = "vkzcobdtagxprzmbndekfzkbrftlkxghdsexwbkrcklvbokooqo";
	if (false == false) {
		int cymrhu;
		for (cymrhu = 24; cymrhu > 0; cymrhu--) {
			continue;
		}
	}
	if (1873 == 1873) {
		int pp;
		for (pp = 7; pp > 0; pp--) {
			continue;
		}
	}
	return string("ilhjljlojwvaqepajb");
}

bool dtfovwc::ecixzvttuxerppyivl(double phdbml, string oynjzbmzjrn, int oiztvsuxexedbi, bool dzdhpqsmwr, double gpzjebcmytzzrsx, int fuhcwcu, double fxqydtojofinhlt, double kahydttfvyeov) {
	bool qnfjlfd = false;
	if (false != false) {
		int bfdanngsz;
		for (bfdanngsz = 27; bfdanngsz > 0; bfdanngsz--) {
			continue;
		}
	}
	if (false == false) {
		int tldxzn;
		for (tldxzn = 97; tldxzn > 0; tldxzn--) {
			continue;
		}
	}
	if (false != false) {
		int cqfnin;
		for (cqfnin = 50; cqfnin > 0; cqfnin--) {
			continue;
		}
	}
	return true;
}

void dtfovwc::whrfcmgpivwzlww(string kbjpv, bool mqyonenhwe, string chfhkii) {
	int mjeixfmpytu = 4538;
	string axfcifpwm = "grcrzypv";
	if (4538 != 4538) {
		int njizbnef;
		for (njizbnef = 88; njizbnef > 0; njizbnef--) {
			continue;
		}
	}

}

void dtfovwc::gwrojrqxctuo() {
	double fypqwhd = 27661;
	string wvmdtoo = "zzdyytthsszehqopjfwjiurfqyxosuzvwfcezraoamjazyybeyhwelmdvkzovkqglqcfxytmcviey";
	if (string("zzdyytthsszehqopjfwjiurfqyxosuzvwfcezraoamjazyybeyhwelmdvkzovkqglqcfxytmcviey") != string("zzdyytthsszehqopjfwjiurfqyxosuzvwfcezraoamjazyybeyhwelmdvkzovkqglqcfxytmcviey")) {
		int yj;
		for (yj = 76; yj > 0; yj--) {
			continue;
		}
	}
	if (27661 != 27661) {
		int pxjffiqkt;
		for (pxjffiqkt = 20; pxjffiqkt > 0; pxjffiqkt--) {
			continue;
		}
	}

}

int dtfovwc::nwsadfbwtauayrlfvzuqrlpgs(bool fitymr, string upfsmac, bool dnvekfzv, bool wadmmpja, double tjwpwyfn) {
	string dvpdleszw = "ibbecyo";
	string bucpftxsmsrvgk = "lawzxbsitswbndelzrvxxoqjabzogjx";
	int vypuhabfa = 9265;
	string tfwjaoihx = "paaebhydbhfizzyjlkbbijwhwwqeaftatjehfmlwhhheigyldujcucsnqyzfmp";
	int qsmwq = 1440;
	if (string("paaebhydbhfizzyjlkbbijwhwwqeaftatjehfmlwhhheigyldujcucsnqyzfmp") == string("paaebhydbhfizzyjlkbbijwhwwqeaftatjehfmlwhhheigyldujcucsnqyzfmp")) {
		int xlzbdpjix;
		for (xlzbdpjix = 82; xlzbdpjix > 0; xlzbdpjix--) {
			continue;
		}
	}
	if (string("lawzxbsitswbndelzrvxxoqjabzogjx") == string("lawzxbsitswbndelzrvxxoqjabzogjx")) {
		int qnahstzbl;
		for (qnahstzbl = 95; qnahstzbl > 0; qnahstzbl--) {
			continue;
		}
	}
	return 98748;
}

int dtfovwc::izmxnuesihvjam(bool yhegymmj, int awnemdneuygxgjs, string prhlq, double tvpze, int pskvslx, bool nwfxsrojrntrw, bool qrjvgmbxjpoyil, int nxkjtfxiuncdgo, int tsrlrb) {
	int lnipxcqng = 3279;
	double ahrtvvhgkdhvo = 681;
	if (681 != 681) {
		int khpsvolocn;
		for (khpsvolocn = 8; khpsvolocn > 0; khpsvolocn--) {
			continue;
		}
	}
	if (3279 == 3279) {
		int yoilkv;
		for (yoilkv = 69; yoilkv > 0; yoilkv--) {
			continue;
		}
	}
	return 66282;
}

double dtfovwc::kkmqirgxlcmtqlxqli(double dbyfveqebta) {
	bool rbyoi = false;
	int rhtyk = 4444;
	bool mazsfhsptbzyym = true;
	bool rrzzdmu = false;
	double jflyozbixtbr = 16552;
	return 66764;
}

string dtfovwc::mijfclfujbwojnrbpjjvcg(double obstlh) {
	bool vfrebxvcy = true;
	bool xvteype = false;
	int lgrugosgix = 479;
	string teagmrgrwfge = "exwtlekkbnqofkacfuoxuekvsya";
	bool vjhbhovgk = false;
	double esxvf = 35161;
	bool skozdtvubuujr = true;
	string knnpzjxbtmmchc = "mlovinadwnrsqlaiaysupxrvsyktdfbxvkajhqwrcnjyeupqhlhuoovigxgrdfbdibbldwkfuhapgvvzucymhmpsxjirdgthcvk";
	int dsvoizddia = 3381;
	double wrsrbhzrs = 39273;
	if (false == false) {
		int warbohaxm;
		for (warbohaxm = 40; warbohaxm > 0; warbohaxm--) {
			continue;
		}
	}
	if (true != true) {
		int xlkzsaxv;
		for (xlkzsaxv = 89; xlkzsaxv > 0; xlkzsaxv--) {
			continue;
		}
	}
	if (false != false) {
		int pbeqs;
		for (pbeqs = 9; pbeqs > 0; pbeqs--) {
			continue;
		}
	}
	if (479 != 479) {
		int dqjill;
		for (dqjill = 91; dqjill > 0; dqjill--) {
			continue;
		}
	}
	if (string("mlovinadwnrsqlaiaysupxrvsyktdfbxvkajhqwrcnjyeupqhlhuoovigxgrdfbdibbldwkfuhapgvvzucymhmpsxjirdgthcvk") == string("mlovinadwnrsqlaiaysupxrvsyktdfbxvkajhqwrcnjyeupqhlhuoovigxgrdfbdibbldwkfuhapgvvzucymhmpsxjirdgthcvk")) {
		int eifcpvkfis;
		for (eifcpvkfis = 18; eifcpvkfis > 0; eifcpvkfis--) {
			continue;
		}
	}
	return string("mwuuorokbntt");
}

bool dtfovwc::nbhhwhrnosueakaejnehjrib(string pinlxwptj, double csinrrizmdl, double pslikmouoxxeozy, string pwivdep) {
	string owygyi = "kykzmousqlfqtzpfasdjqozbhrmhkgsdbaxnmatwimwletymznwzvbxhzimpbtypxuxesavjfbiyjhxnubadgtghcpsyodujft";
	string ygkwanqtvc = "endndrfoconbfpldallqppwmnhyvybfwbmzqlginrpheorlymamuvmcqpawukiro";
	bool igfyvle = false;
	string vlsilxzmwzmha = "derxlrnemknolstwfgjzkgqgwpyhddehunmsmjpwyvqanydpcealntmlvxvrpfkynzckhqtsiqcgmmdbrmahijqzoyieog";
	if (false != false) {
		int zqedzjzq;
		for (zqedzjzq = 75; zqedzjzq > 0; zqedzjzq--) {
			continue;
		}
	}
	return true;
}

int dtfovwc::noeuugkmepkicszxd(bool hwgrmqwn, double ymztmyeffst, double jxlobrrd, string ipqsqtuuhviajpd, double kkqzsuuc, string tximqdzhk, double mkiomwqkutotm, double usqbfyrg, bool nhqnsauxths) {
	int nwdgnchv = 2886;
	double acocamz = 67325;
	string qcswqjfg = "oykwdrufornmzxmirrempihrhzcwiqkyvoxdqcphiiphtaeholwdyhghslxxlupkejgbwbrpsmfbkycshopsnutkslyfrcqfti";
	return 21531;
}

dtfovwc::dtfovwc() {
	this->noeuugkmepkicszxd(false, 164, 564, string("lfzaygizvgoejgogjowzahmomo"), 30458, string("gakjyqchulpiqtoymemcibwzorvzymtrtoyfjwwtagylintnemtqylhebxwalxiudszim"), 41154, 58379, false);
	this->nwsadfbwtauayrlfvzuqrlpgs(false, string("nahvzphhrftxwgnnkcznywqxqwsecd"), true, true, 56113);
	this->izmxnuesihvjam(true, 1562, string("eb"), 17754, 3777, false, false, 4771, 2195);
	this->kkmqirgxlcmtqlxqli(73686);
	this->mijfclfujbwojnrbpjjvcg(13308);
	this->nbhhwhrnosueakaejnehjrib(string("wlctjfdlojpkrgofdsmevsgzydbxgpiovicizizkaxvjzzgiqgstqxzdthaaurldprqndozqznclmriyxbv"), 2635, 22393, string("zgqhvigakqaaqolxihszunmgbgbzwkhifoljlpcw"));
	this->alzondwwtjiqiudana();
	this->hxrekdftiuwhabwonli(string("awtowpvdlzudvkeazhegomksvkflhiqzbxos"), 5207, 41797, 42911, string("tzkkbcxeypxskituvivutullh"), 7599, false, 34094, 4867);
	this->tmokzjcpxtyvfnebxya(44385, 3488, 6, false);
	this->ecixzvttuxerppyivl(28289, string("ttugbtdrdthxishkmnyxvminixrzettyinxjhtgpjqycttdmhjdjrgkfxcrucdcfradtjpalasaegdglnqtfufop"), 3548, false, 32060, 3645, 1825, 19893);
	this->whrfcmgpivwzlww(string("kyufkhkduosomipwisjddobyfoeayzlk"), false, string("wbpfpjaojwonkrypeyuwdszgkappjgeyglhkwavboarcgmsrftteavaaovyupxdgyzxntnwaywyehxzzkxutfssziq"));
	this->gwrojrqxctuo();
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class kzswycx {
public:
	int ztwdaiqwgu;
	bool esxsfemjqfijun;
	double hymriqhmw;
	kzswycx();
	bool piapgrogbbvghly(string lgknlzakkovrhkl, string papgmjxgdztqg, double asamkm, double sswnq, string cstqcm, bool vqdkx, int ntsamtanibopj);
	void aldfvvcgfhsdeqziluj(int eonivznsahj, int qplkj, int buxyhgpmflxjw, string thhfqirnttdq, double mqssgvk, bool mtqpnxbrlxpsx, bool blvby, string ovwjplucfg, bool jneeafiurhxbxj);
	void rsqrxyahzlcpenmmgsdnopxq(int pzvbsasvsb, string ubmnfmcmqiaihan, double xpsqhohk, double rtuudzsopu, bool lxddvzvewxxxm, bool cnsusmasqhasllx);

protected:
	double iqnzqbpywgots;
	int yrvclti;

	bool wtcekvkrngzvighcig(int pxppoqxosrx, string hmimkrdqz, bool kedgzuzjdf, int hkatjik, string owobo, int rfjgkbqpwgghsx);
	int dsfmzfeguyqcrda(bool opuwwoutvrivkor, double jpzdpr, string tntepkzly);
	string cdimnmirsoyzoaxyuojdlzds(double anqqdciskie, double cdhpathpqe, int fmihucpjkdtwx, double jenztooqnor, int qlbyd, bool pdhawtpqfeev, string qaeplxpopxq, string umalywxldhzle, bool qgutoyybk, double gwiqmbdisezlu);
	void cbrbfqzecwoxypgtowrt(string amymelfbsri, double srleyoneuudvlq, bool crqbrahyupjruto, int hkaomflby, string wtcngo, bool aazwjiwujcfvv, int rmfskzdopyvpwz, int sujhhviativhjw, string gcgxlqduv, double jgrqwpprbj);
	bool vgufdinekebulh(string gnwnkw, double zuekmxplkk, double zrgnz, int hqrehsmdv, string wzsbcuojhqmvfwy, bool hkqmsw);
	bool nyrunbutdtlexdlpfvnrncfl(int xicof, int alhmlfbptyjhiop, string diiplajf, double syiquejvrpsih, string qwarjxsgvqwd, string xrvweclmut, string jktubofsnmkkber, double lpfmqzuauvfydk, int oklwcrpnbfcto, double rhzstwusmhaxsc);
	bool ahkztcqreggpdhsmxgohr(bool zvohv, int jarvvnrrws, string ecxeij, string mymvl, double naawpo, double tmyolamia, double yzqlcxbyso, bool jtcfgvf);
	bool msoevnczyjvtujvenuoq(bool vwpxgzvprfildbq, double isxlulevzr, bool npgjhhdfoigff);

private:
	string ygxrcqvxun;

	double rbbwyezahpympbk(bool ainjpclheie, double eyydygdoc);
	string ytjowcdkjrvuoqxy(string gqkveujtlsihyv, string xzbdxeswzmgo);
	bool jsnwjdxqlbfnbeud(double wmcgnby, double bwhssbsylnzrdw, string srxguzoger, int ymmqy, string txlkp, double uyskqvtsqvc);
	void kfkqusiekdje(string qmrnlxzm, bool nsshs, string riukkqwxygtv, int fzxlgg, string mynhtbjekgkftwd);
	void ocqsdkelkd(int xjihjuy, int pbxtszwyfrsxfq, int hjjskjdvjgbgzw, string wbmhpc, bool qdsjvspgkwhpy, bool tgxfb, int vykuvwco, string lvjlm, int lmoqygdrqwcm, int jhueaolyd);
	double otkbfjsjhiubtbn();
	int boxlaghjekxgsoaolmzdeeyk(int shknikcc, double yvgtq, bool awvqou, double uatsfyz, int ztxef);
	double xyvgtcpidmupelafeaeegmpm(string lbpey, double vzkod, string qatmgaxbyef);
	string sfadwkodialnnk(double qvjax, string woazajd);
	bool quxlkftqbmqjqqddnx(double nheebdwxdpvrm, bool ttcemdsaqmrnf, bool gmjhcsqcptilpir, double bkozsslwmec, int fcajmgi, int uvlbikaj, int frpkyk);

};


double kzswycx::rbbwyezahpympbk(bool ainjpclheie, double eyydygdoc) {
	bool cxivxufwe = true;
	double ksuffmgrif = 19637;
	int uwpgcdzedhwjyb = 416;
	bool vvrslx = true;
	int rjsli = 1213;
	double rnlecl = 33038;
	int tqiespfww = 7292;
	double ildhzpyshqog = 13468;
	double clxzwseqec = 3937;
	double naypfqeojyjmknv = 11981;
	if (416 != 416) {
		int loccmqu;
		for (loccmqu = 35; loccmqu > 0; loccmqu--) {
			continue;
		}
	}
	return 62567;
}

string kzswycx::ytjowcdkjrvuoqxy(string gqkveujtlsihyv, string xzbdxeswzmgo) {
	string uhgdh = "oigxyycxzgrztzsztzyqdyohkczfuhqwymfibdmklhdhszlrpspdputyqzbekxlilqgslkxioyszvimjmbjndsulbqrcb";
	bool mayxnafnwvuhbw = true;
	double cnpjq = 82440;
	double alkfuqkrkrcwmw = 4345;
	int ztuhwre = 2312;
	double gvskmcinap = 43502;
	double okknrvejcihrbma = 19044;
	bool dyazfbvka = true;
	if (2312 != 2312) {
		int fvtgv;
		for (fvtgv = 78; fvtgv > 0; fvtgv--) {
			continue;
		}
	}
	return string("nvowahmla");
}

bool kzswycx::jsnwjdxqlbfnbeud(double wmcgnby, double bwhssbsylnzrdw, string srxguzoger, int ymmqy, string txlkp, double uyskqvtsqvc) {
	double dbodzec = 14831;
	bool ptqtp = false;
	double tkpaeviytdz = 13651;
	bool csvzinh = true;
	if (false != false) {
		int ldwppuvt;
		for (ldwppuvt = 53; ldwppuvt > 0; ldwppuvt--) {
			continue;
		}
	}
	if (true == true) {
		int fzqxvyb;
		for (fzqxvyb = 64; fzqxvyb > 0; fzqxvyb--) {
			continue;
		}
	}
	if (false != false) {
		int pqakaqi;
		for (pqakaqi = 67; pqakaqi > 0; pqakaqi--) {
			continue;
		}
	}
	if (false != false) {
		int mwkdi;
		for (mwkdi = 24; mwkdi > 0; mwkdi--) {
			continue;
		}
	}
	if (true != true) {
		int gvdo;
		for (gvdo = 25; gvdo > 0; gvdo--) {
			continue;
		}
	}
	return false;
}

void kzswycx::kfkqusiekdje(string qmrnlxzm, bool nsshs, string riukkqwxygtv, int fzxlgg, string mynhtbjekgkftwd) {
	int yachztb = 713;
	string mkiycoilog = "fwdhoznzcfiifuwcnlmdthgeaitjcvhlvymboxmsdhgivwpxnvcgdqrlrag";
	string deramnuva = "mfhdbfsceexpy";
	string orijgqsuu = "mzhvhcddzblarumkuptjcatesipzbdlhaohhshdscbkyzmmaqobbbf";
	bool ngchsyxhcry = false;
	if (false != false) {
		int atfaquzscp;
		for (atfaquzscp = 32; atfaquzscp > 0; atfaquzscp--) {
			continue;
		}
	}
	if (false != false) {
		int gyvwsfb;
		for (gyvwsfb = 37; gyvwsfb > 0; gyvwsfb--) {
			continue;
		}
	}
	if (string("mzhvhcddzblarumkuptjcatesipzbdlhaohhshdscbkyzmmaqobbbf") == string("mzhvhcddzblarumkuptjcatesipzbdlhaohhshdscbkyzmmaqobbbf")) {
		int kb;
		for (kb = 39; kb > 0; kb--) {
			continue;
		}
	}

}

void kzswycx::ocqsdkelkd(int xjihjuy, int pbxtszwyfrsxfq, int hjjskjdvjgbgzw, string wbmhpc, bool qdsjvspgkwhpy, bool tgxfb, int vykuvwco, string lvjlm, int lmoqygdrqwcm, int jhueaolyd) {
	int kbmjeepqkciw = 4815;
	string zuddcgku = "tblwsfzllocyhiglgkgpiccovdtfqhfllsijfxiafspeygwhjcmnqeyxbrauszwasiv";
	bool zwoboahllts = true;
	int yciyiqt = 519;
	string mrkrbbdstf = "kjwtbynlengzwkdvvqcztakntzkjtteihosvwxboj";
	int ozorhmbmwxm = 1163;
	double fasadgir = 61931;
	double sytgqvy = 57317;
	int naexrt = 2093;
	string jtrlwkqcqkzvlj = "hrgsvcotjerlimwwuurysjfsserfhmxwxkdyjiiwlbbvianrfvreilsmooccrzmzozzitpnjvgarwuqoibrnbrtppdyxxfekua";
	if (string("tblwsfzllocyhiglgkgpiccovdtfqhfllsijfxiafspeygwhjcmnqeyxbrauszwasiv") == string("tblwsfzllocyhiglgkgpiccovdtfqhfllsijfxiafspeygwhjcmnqeyxbrauszwasiv")) {
		int rszdljruyi;
		for (rszdljruyi = 6; rszdljruyi > 0; rszdljruyi--) {
			continue;
		}
	}
	if (61931 == 61931) {
		int jdp;
		for (jdp = 99; jdp > 0; jdp--) {
			continue;
		}
	}
	if (string("kjwtbynlengzwkdvvqcztakntzkjtteihosvwxboj") == string("kjwtbynlengzwkdvvqcztakntzkjtteihosvwxboj")) {
		int stt;
		for (stt = 71; stt > 0; stt--) {
			continue;
		}
	}

}

double kzswycx::otkbfjsjhiubtbn() {
	double ihjxkfzrryc = 25061;
	string iqazrapa = "xmbwclhbkyjvvdhsd";
	int jxqngtswxxbutq = 1613;
	double ttdrwqba = 34410;
	string seyvjizrquzcr = "djgczckbljnirimjqbflzbvdtgsajaiucnweaxskjsmlyorolgbnxukyzgwxpjojeiohducoqwlseviiktgqczicdvadn";
	if (1613 == 1613) {
		int jgswe;
		for (jgswe = 65; jgswe > 0; jgswe--) {
			continue;
		}
	}
	if (25061 == 25061) {
		int mshi;
		for (mshi = 54; mshi > 0; mshi--) {
			continue;
		}
	}
	if (34410 != 34410) {
		int axs;
		for (axs = 32; axs > 0; axs--) {
			continue;
		}
	}
	return 82068;
}

int kzswycx::boxlaghjekxgsoaolmzdeeyk(int shknikcc, double yvgtq, bool awvqou, double uatsfyz, int ztxef) {
	string xewatclvincj = "abjureouoofvjaeyxcsdfihlagjemxgojtjmeimubdqnfxjyhlud";
	double vwfhywwsuha = 22764;
	double tblwozvtyh = 80420;
	int qcrzyzacf = 3313;
	double csovwebrnl = 7183;
	double stafbv = 26301;
	if (22764 == 22764) {
		int fmsr;
		for (fmsr = 63; fmsr > 0; fmsr--) {
			continue;
		}
	}
	if (string("abjureouoofvjaeyxcsdfihlagjemxgojtjmeimubdqnfxjyhlud") == string("abjureouoofvjaeyxcsdfihlagjemxgojtjmeimubdqnfxjyhlud")) {
		int tbcseyt;
		for (tbcseyt = 88; tbcseyt > 0; tbcseyt--) {
			continue;
		}
	}
	if (3313 != 3313) {
		int zl;
		for (zl = 2; zl > 0; zl--) {
			continue;
		}
	}
	if (string("abjureouoofvjaeyxcsdfihlagjemxgojtjmeimubdqnfxjyhlud") != string("abjureouoofvjaeyxcsdfihlagjemxgojtjmeimubdqnfxjyhlud")) {
		int qg;
		for (qg = 89; qg > 0; qg--) {
			continue;
		}
	}
	return 76197;
}

double kzswycx::xyvgtcpidmupelafeaeegmpm(string lbpey, double vzkod, string qatmgaxbyef) {
	int ulseyoltglcl = 8136;
	string fdfxogtmvhkg = "cadbstnvscibirevtflmwwexxmxndtlpznlsljfprnujbkbiqrazi";
	int hnadobpxbojrq = 775;
	bool owhcvjybiemawmr = false;
	string wcnixdeark = "fica";
	bool jujkjict = false;
	int auqrw = 300;
	string cxzfxrfyee = "skbuqgigsuslwjvemjiqzhakjdwewcppkicznsefllslayxaepbkmqhuhctcequclwrbvxjajqxoxcqefygqdvmedlraeix";
	bool dfbxxwpirirwl = true;
	if (false == false) {
		int tfdtptjjp;
		for (tfdtptjjp = 71; tfdtptjjp > 0; tfdtptjjp--) {
			continue;
		}
	}
	if (775 != 775) {
		int gf;
		for (gf = 28; gf > 0; gf--) {
			continue;
		}
	}
	if (string("fica") == string("fica")) {
		int ly;
		for (ly = 93; ly > 0; ly--) {
			continue;
		}
	}
	if (775 == 775) {
		int ldi;
		for (ldi = 18; ldi > 0; ldi--) {
			continue;
		}
	}
	if (775 != 775) {
		int fc;
		for (fc = 46; fc > 0; fc--) {
			continue;
		}
	}
	return 87348;
}

string kzswycx::sfadwkodialnnk(double qvjax, string woazajd) {
	int fznefktowatg = 3943;
	bool fddtiu = false;
	bool sttinagdhnylmvj = true;
	int utahwvafq = 4909;
	int ghctwb = 5048;
	bool iaimpbgtijqnwb = true;
	bool napsgdmeobu = false;
	if (true == true) {
		int zxfmvf;
		for (zxfmvf = 13; zxfmvf > 0; zxfmvf--) {
			continue;
		}
	}
	if (true != true) {
		int rzvkaenjhu;
		for (rzvkaenjhu = 49; rzvkaenjhu > 0; rzvkaenjhu--) {
			continue;
		}
	}
	if (true != true) {
		int nl;
		for (nl = 100; nl > 0; nl--) {
			continue;
		}
	}
	if (4909 != 4909) {
		int dhloywcm;
		for (dhloywcm = 2; dhloywcm > 0; dhloywcm--) {
			continue;
		}
	}
	return string("frhzwwvfijezzr");
}

bool kzswycx::quxlkftqbmqjqqddnx(double nheebdwxdpvrm, bool ttcemdsaqmrnf, bool gmjhcsqcptilpir, double bkozsslwmec, int fcajmgi, int uvlbikaj, int frpkyk) {
	return true;
}

bool kzswycx::wtcekvkrngzvighcig(int pxppoqxosrx, string hmimkrdqz, bool kedgzuzjdf, int hkatjik, string owobo, int rfjgkbqpwgghsx) {
	return true;
}

int kzswycx::dsfmzfeguyqcrda(bool opuwwoutvrivkor, double jpzdpr, string tntepkzly) {
	int ptiwfbeamvm = 2735;
	int hjfju = 692;
	string gaovfb = "zmdhotgeebxkidomxwmsornkpbqvimthoamaxcmpjamqd";
	string pknbej = "xquqjmvjdguwuzdepejyvuxjgqqfwiaxhpqz";
	if (2735 != 2735) {
		int xbflo;
		for (xbflo = 49; xbflo > 0; xbflo--) {
			continue;
		}
	}
	if (2735 == 2735) {
		int akel;
		for (akel = 83; akel > 0; akel--) {
			continue;
		}
	}
	if (string("zmdhotgeebxkidomxwmsornkpbqvimthoamaxcmpjamqd") == string("zmdhotgeebxkidomxwmsornkpbqvimthoamaxcmpjamqd")) {
		int ghb;
		for (ghb = 39; ghb > 0; ghb--) {
			continue;
		}
	}
	if (692 == 692) {
		int onucz;
		for (onucz = 64; onucz > 0; onucz--) {
			continue;
		}
	}
	return 28750;
}

string kzswycx::cdimnmirsoyzoaxyuojdlzds(double anqqdciskie, double cdhpathpqe, int fmihucpjkdtwx, double jenztooqnor, int qlbyd, bool pdhawtpqfeev, string qaeplxpopxq, string umalywxldhzle, bool qgutoyybk, double gwiqmbdisezlu) {
	int iswvolkngu = 4427;
	string maohkqsrygasl = "iaqoxnijz";
	int ngrvgljqo = 627;
	bool ouomawstlpveq = true;
	string kwjbhsempq = "zjcrsspdaayqmsskpjpu";
	string uencxcbonqtuc = "eydylrhzgxilacazogrdsomajjidmyvqitzablzatildafvwwbfjvcpluqipdybbmabgnnxpthucyynpqxbksrue";
	string bqetdoyhknfhfgv = "otxlqpykojxasdcvuwj";
	int nebtzqihbjlnbq = 6900;
	bool jghmydzd = true;
	if (string("iaqoxnijz") == string("iaqoxnijz")) {
		int nqfpxnd;
		for (nqfpxnd = 7; nqfpxnd > 0; nqfpxnd--) {
			continue;
		}
	}
	if (true == true) {
		int lmaibcj;
		for (lmaibcj = 75; lmaibcj > 0; lmaibcj--) {
			continue;
		}
	}
	if (true == true) {
		int vpvidwlqer;
		for (vpvidwlqer = 10; vpvidwlqer > 0; vpvidwlqer--) {
			continue;
		}
	}
	if (627 != 627) {
		int bqbzff;
		for (bqbzff = 98; bqbzff > 0; bqbzff--) {
			continue;
		}
	}
	if (6900 != 6900) {
		int potvo;
		for (potvo = 87; potvo > 0; potvo--) {
			continue;
		}
	}
	return string("");
}

void kzswycx::cbrbfqzecwoxypgtowrt(string amymelfbsri, double srleyoneuudvlq, bool crqbrahyupjruto, int hkaomflby, string wtcngo, bool aazwjiwujcfvv, int rmfskzdopyvpwz, int sujhhviativhjw, string gcgxlqduv, double jgrqwpprbj) {
	string auwxdrx = "xtffuyjmqkqxvktmpwgsddppyptprdelzzgohenq";
	double splfouc = 16313;
	int qhtwwlklejy = 3125;
	int pdwuxjqp = 2066;
	int dndzhdxujadbqx = 2020;
	string nljsasnqsrobf = "lofcuosp";
	int muzdtvlachl = 825;
	double iruxxrd = 11620;
	bool ndujkcaqwzn = false;

}

bool kzswycx::vgufdinekebulh(string gnwnkw, double zuekmxplkk, double zrgnz, int hqrehsmdv, string wzsbcuojhqmvfwy, bool hkqmsw) {
	int yicjak = 8688;
	double ahwnb = 443;
	double qvznnhoxtdb = 23417;
	if (8688 != 8688) {
		int hsiaojd;
		for (hsiaojd = 87; hsiaojd > 0; hsiaojd--) {
			continue;
		}
	}
	return false;
}

bool kzswycx::nyrunbutdtlexdlpfvnrncfl(int xicof, int alhmlfbptyjhiop, string diiplajf, double syiquejvrpsih, string qwarjxsgvqwd, string xrvweclmut, string jktubofsnmkkber, double lpfmqzuauvfydk, int oklwcrpnbfcto, double rhzstwusmhaxsc) {
	return true;
}

bool kzswycx::ahkztcqreggpdhsmxgohr(bool zvohv, int jarvvnrrws, string ecxeij, string mymvl, double naawpo, double tmyolamia, double yzqlcxbyso, bool jtcfgvf) {
	string hlwcpv = "sekdogoccpxmx";
	string gwktyueu = "qcmbztzjdpuzqpmvqojamvaoezwssbtvilqjtsahlyrqi";
	if (string("qcmbztzjdpuzqpmvqojamvaoezwssbtvilqjtsahlyrqi") != string("qcmbztzjdpuzqpmvqojamvaoezwssbtvilqjtsahlyrqi")) {
		int gmda;
		for (gmda = 75; gmda > 0; gmda--) {
			continue;
		}
	}
	if (string("sekdogoccpxmx") == string("sekdogoccpxmx")) {
		int ndypxfwmy;
		for (ndypxfwmy = 18; ndypxfwmy > 0; ndypxfwmy--) {
			continue;
		}
	}
	if (string("qcmbztzjdpuzqpmvqojamvaoezwssbtvilqjtsahlyrqi") != string("qcmbztzjdpuzqpmvqojamvaoezwssbtvilqjtsahlyrqi")) {
		int iuetassh;
		for (iuetassh = 8; iuetassh > 0; iuetassh--) {
			continue;
		}
	}
	if (string("sekdogoccpxmx") == string("sekdogoccpxmx")) {
		int ltcttvgo;
		for (ltcttvgo = 55; ltcttvgo > 0; ltcttvgo--) {
			continue;
		}
	}
	if (string("qcmbztzjdpuzqpmvqojamvaoezwssbtvilqjtsahlyrqi") == string("qcmbztzjdpuzqpmvqojamvaoezwssbtvilqjtsahlyrqi")) {
		int ickw;
		for (ickw = 42; ickw > 0; ickw--) {
			continue;
		}
	}
	return true;
}

bool kzswycx::msoevnczyjvtujvenuoq(bool vwpxgzvprfildbq, double isxlulevzr, bool npgjhhdfoigff) {
	bool czwidznvtm = false;
	double hqrxdfwra = 11696;
	int zdxhzxmfpgzgd = 1974;
	string ktqxcxliekr = "xjsjunrgvgeijfyhppmcjai";
	return true;
}

bool kzswycx::piapgrogbbvghly(string lgknlzakkovrhkl, string papgmjxgdztqg, double asamkm, double sswnq, string cstqcm, bool vqdkx, int ntsamtanibopj) {
	int jftwjvpd = 2986;
	double gooprsoapweuckm = 34821;
	double ytfbrqebgflr = 51062;
	int taqjtu = 2412;
	string ifiltgwayakjwnr = "agotdfpbxwwzingisyjwboturwnchrhwysdrwvfojmorxjooicpwuaqtajh";
	int uvtahbkewurrjj = 922;
	bool uqyclfpqkvb = false;
	if (2986 == 2986) {
		int czz;
		for (czz = 39; czz > 0; czz--) {
			continue;
		}
	}
	if (922 == 922) {
		int rofpozpwt;
		for (rofpozpwt = 38; rofpozpwt > 0; rofpozpwt--) {
			continue;
		}
	}
	if (false != false) {
		int xjv;
		for (xjv = 25; xjv > 0; xjv--) {
			continue;
		}
	}
	if (51062 == 51062) {
		int txgcjbi;
		for (txgcjbi = 58; txgcjbi > 0; txgcjbi--) {
			continue;
		}
	}
	if (2986 == 2986) {
		int kvslrg;
		for (kvslrg = 61; kvslrg > 0; kvslrg--) {
			continue;
		}
	}
	return true;
}

void kzswycx::aldfvvcgfhsdeqziluj(int eonivznsahj, int qplkj, int buxyhgpmflxjw, string thhfqirnttdq, double mqssgvk, bool mtqpnxbrlxpsx, bool blvby, string ovwjplucfg, bool jneeafiurhxbxj) {
	string eljqnebhv = "pgrvxjodkdkjgddtvssrmzghfeoangpdlhpwdwbpeupdqxbkdlngyxfaahscqwzjcjaukylhscbyrrjysyrxpp";
	double nsiwliyaqhzj = 21889;
	double aafweonxmxnawkw = 47392;
	int dxjjqefiryy = 2526;
	int rxfxlzwlb = 3009;
	bool atnuksajc = false;
	if (47392 == 47392) {
		int mzgx;
		for (mzgx = 90; mzgx > 0; mzgx--) {
			continue;
		}
	}
	if (47392 == 47392) {
		int ktshgbhc;
		for (ktshgbhc = 88; ktshgbhc > 0; ktshgbhc--) {
			continue;
		}
	}
	if (false != false) {
		int silrdf;
		for (silrdf = 83; silrdf > 0; silrdf--) {
			continue;
		}
	}
	if (false != false) {
		int bsgoo;
		for (bsgoo = 58; bsgoo > 0; bsgoo--) {
			continue;
		}
	}
	if (21889 != 21889) {
		int korrivjn;
		for (korrivjn = 82; korrivjn > 0; korrivjn--) {
			continue;
		}
	}

}

void kzswycx::rsqrxyahzlcpenmmgsdnopxq(int pzvbsasvsb, string ubmnfmcmqiaihan, double xpsqhohk, double rtuudzsopu, bool lxddvzvewxxxm, bool cnsusmasqhasllx) {

}

kzswycx::kzswycx() {
	this->piapgrogbbvghly(string("enevyombikreoeijnlinlsowxfyngsykkdbgksfcsrazkdtnxk"), string("ddoiunnanfzeadsmljicstceiqxtfrmmesvhzyovualgwyo"), 16205, 33587, string("bmyzlurglzcffonuiokgtamhmkdeikbuypeekeewgfgoovbalexweqkwqfgfzvexjlngufzksasxr"), true, 7468);
	this->aldfvvcgfhsdeqziluj(345, 1328, 6724, string("toxtzybfwwrndhbftnnuzmarwayzqaucxofxuvalwurcgpzaxsiwxbxoynv"), 17482, true, false, string("xjafszyccylrif"), true);
	this->rsqrxyahzlcpenmmgsdnopxq(4509, string("oegfmcpsgbgtehieq"), 59173, 12795, true, false);
	this->wtcekvkrngzvighcig(7208, string("zauexxaglmrwf"), false, 5780, string("nrttoiiwonnntuhtghzlcrviiyvowvvubfqhtnzuybrbxikvdyqixlsygpeqdupmkoantdfydgribhpyvsetk"), 3387);
	this->dsfmzfeguyqcrda(true, 15253, string("ooajpscpdmhswixdzstwtuemovkfyudnvlrahpxamjdczklkacwnkoqtifkhpxebjpsxhqbdlskrzwobfurwfsawxzmxszcl"));
	this->cdimnmirsoyzoaxyuojdlzds(63710, 310, 2003, 6191, 7739, false, string("seu"), string("iuguxonbfbdihsuroeetgimndvpxspztwezgxqkwhpuaszezvexjeocbgsaisuikgjolqrdcdkkswhlcovmcrumzbu"), false, 46482);
	this->cbrbfqzecwoxypgtowrt(string("oovedglyjbbvmwcadzotiwugtpgylowfvqeoyjxysqjneyaznnddbljgmqzlxaoxndqjaqtmjzgtjjbzpfldvciptdh"), 78637, false, 5558, string("tkmizhoaqbydumctqzoocssgehspuwnermy"), true, 438, 886, string("mw"), 16974);
	this->vgufdinekebulh(string("weprqobywijbcfzvgkulrmvziyoodexxcuwlcbdnenfpelfpbkqlosvsrznlumwdtmxiznmijbwkvlqzsxifg"), 13314, 19390, 105, string("okqvimgkqbaqupbjqrmkaujyvurfyieranhvzqxbfbfdqexahsfvoapaqgpzyrrvehglihmdtjkvbvfamyin"), true);
	this->nyrunbutdtlexdlpfvnrncfl(843, 54, string("gczalaasgpueaiodekiivbssfrqqrzulfffzhyrtsrmoipoukxhbxxpsigpsfuw"), 15118, string("uglvovzmidgcayfyzlugyyduoosyermsmnrwwxknwqoizqmzxngtttrreaixpojnok"), string("e"), string("tacrhi"), 21030, 1137, 2956);
	this->ahkztcqreggpdhsmxgohr(true, 272, string("auhwlvofeummxvfm"), string("ocueayagrhrlb"), 26606, 5207, 15781, false);
	this->msoevnczyjvtujvenuoq(false, 76393, true);
	this->rbbwyezahpympbk(true, 3408);
	this->ytjowcdkjrvuoqxy(string("jkbleclxkvxlycidlrkwzuvwcagsvuufjeporsuyojsuxeihrsbsedtegzkyjzyzbfppgzqhcdxi"), string("jtygngbeprrolkqtijwttrcjovsondvcpaduzocckymnzfylhaaqautlqcesagkjmdldzxlohqkxcccodmiecwetrywto"));
	this->jsnwjdxqlbfnbeud(2334, 62624, string("fvg"), 2677, string("xzbqxntpiukyijafgxhidwfrfmtunymaexwazakgqtfyupykjzxrgvkcmdxbw"), 10541);
	this->kfkqusiekdje(string("gekdxfgorhojiqnmilffqgpnfxfegntkjc"), true, string("fexjxoktzadszxifnjprbxxzqkjamupjmmujnadjkjxbakikkxxiu"), 6430, string("fmsihticevuqnwrkocvcumzzzumpibj"));
	this->ocqsdkelkd(79, 5191, 4715, string("s"), false, false, 513, string("mmyuxusspgffakjjittlhegvyhudimgbybatsaswzesovtnsbnz"), 2371, 1074);
	this->otkbfjsjhiubtbn();
	this->boxlaghjekxgsoaolmzdeeyk(4128, 89321, true, 23376, 4503);
	this->xyvgtcpidmupelafeaeegmpm(string("qbvnuhmwhkvbbeljbwkqncjoqsbbfouahfuirovwhvgvruf"), 8772, string("uoulpueuuhtnyzfdsvqgyrhgkbajdtfdxdnaabdhledlpdhvrlxingnpzvljtzwseumjwxgzjlslmjysutv"));
	this->sfadwkodialnnk(26260, string("bamrartsjzivgniwqoqshyaiwecjijqvbou"));
	this->quxlkftqbmqjqqddnx(32689, false, true, 30402, 8596, 5161, 1894);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class byhvjfm {
public:
	int ksbioee;
	bool sofcvtonsjkqu;
	byhvjfm();
	int ccsgztkjlvqtkzmezcr(double hwmizxaddyhbrxy, int xsaekzrnvspkm, string coydhtjxh, string tuafmnmbjy, string iakvelryqs, string fqjokl, bool aypxsepee, string zhlfpptnrombt, int pfivwqjcqcuiv, string sdbuxorqzbv);

protected:
	string ixgmefynwvnvf;
	bool pygfbfr;
	double okcxutmfktflgt;
	int mmmlzqnhv;

	string sglpdeebtio(bool lbrxpz, double dzfsmimwn, double elfjf, string yogjswukmsdob, string zevux, int dhbshjdak, int xmtyiljixhp);
	void tjrzbyetkiycrrd(int thbfst, double vhjcppeu, double sjrmdbkeji, int jvbiqnmrc, string gqweivv, bool ffaccmyvzc, bool qsdylqqnscx, int eunzbnzhh, bool jldkxpegnubmzh);
	string ijmxckqakr(int rzqemebgp, bool tofxsmitlgiidx, string ngggfcipege, string wtjpzrqyekoeg, string qrlyowow, double afcva);
	void tjrgkgwpqkwvfjuc(int jtzbjqeauczoyk, bool swhzuenuiufwd, bool fsvqlzvngxu, int wfzbj, int eblgbohrtes, double bqksnzwnaeqgh, string qogwdbpzgk);
	int kpwxfpaapjfbghtnyrow(double wcurvfqkphgst, int wkmpwrhfhbwj, double xpjqtthxevocuvl, double tvqprkmsffrngnd, bool opnhyllxnem);
	void gpmcjxxpicvuizmwgeqlk(int sajckkbekvizyk, double ninhjezvzsdfp, bool tshyhin, double tlgzbfq, bool srfxdwq, string kvsowy, string pvrikbd, int ukagpgmzwlsjiws);
	string wwlsmzhsbrc();
	string ctagvjwxtxvdvudhjpvg(bool zkvzy, int ogokimgizhx, double qvpspqeif, int ddwisvwrv, string kvhwfuouwisjjv, double wsfjywrn, double xjhfdvkuqp);
	int mnocdjyhiuzqahdgs(bool uirjrqewnbwpjp, string bwnon, double himcxhvbuteulhb, bool yvmgtjzbjmfbqeb, int rbtctwlcftzolkx, int xoxidyxnpcrmj, bool vdjdoggwrkxwlxg, string usgatnbdkckncr, bool gnfwkuyhk);
	string ldamwvzgxvrezwuq(int zzoxjxgfofxhf, string aqjadalxacgsena, string quwfzzjruord, double nslyxgut, bool iogjopmwtg, bool xhqidqgqn, double baauye, string tbznxgdu, int wmpix);

private:
	string sszudk;
	string goznpbghpjmly;
	double bijzpmrwfxwgjmy;
	double haquyehyr;
	bool pmtvtu;

	void bplngstzku(string whcnovftjfs, int cgjmljuflopdaj, int mbgblxfcoduxpy, bool fxwbict);
	string sljrfkcxafmlv(bool yurexjnlmhy, double ipbxkwpwgayxkgz, string ziqjrkkjypm, string gupkcavqlszw, string dsbzgyfa);
	int fwtcndxhabz(int vuwpfssqidvnfa, bool unqcnehwcktathb, int hskjezv, double lgqjhrg, double lijvvznt, int kighoftwvrsh, double xitmzpsi, string mpqoahvliv);
	void ugsxwjyvmudqawhbdf(double ugflhebl, double mywaaoru, string jivzzoeuzkg);
	string nowblffoztdgvcxm(bool iejcnxmppnuk, string gjnyb, bool agxhyzorex, int xpsvqhmere, double capzphtwqgalnbp, double qzvdembarkpzukk, bool utkkgypnsap, double fdpsfrzbizmtyte, int ithhnltb);
	double racqsvurnxsxkk(int xokxjqc, bool yufqedgmxusjp, int tlwausvujiwroo, bool djrypmd, double beqeiwohtagxhh);

};


void byhvjfm::bplngstzku(string whcnovftjfs, int cgjmljuflopdaj, int mbgblxfcoduxpy, bool fxwbict) {
	bool hexyy = true;
	bool rqueadviqgizjp = false;
	string gdzxztxfcognopo = "sqtcdyrjbhgnnwmjz";
	bool ixreledzzujgcf = true;
	bool drqvirbwvrq = true;
	double lakjhviek = 823;
	int hzjhtmqbznlye = 2423;
	if (false == false) {
		int ueuezhss;
		for (ueuezhss = 75; ueuezhss > 0; ueuezhss--) {
			continue;
		}
	}
	if (true != true) {
		int hkl;
		for (hkl = 99; hkl > 0; hkl--) {
			continue;
		}
	}
	if (823 != 823) {
		int stkkemdr;
		for (stkkemdr = 91; stkkemdr > 0; stkkemdr--) {
			continue;
		}
	}
	if (true != true) {
		int wgshjunn;
		for (wgshjunn = 52; wgshjunn > 0; wgshjunn--) {
			continue;
		}
	}
	if (823 == 823) {
		int jnxipqw;
		for (jnxipqw = 95; jnxipqw > 0; jnxipqw--) {
			continue;
		}
	}

}

string byhvjfm::sljrfkcxafmlv(bool yurexjnlmhy, double ipbxkwpwgayxkgz, string ziqjrkkjypm, string gupkcavqlszw, string dsbzgyfa) {
	int rrzrhecoyrotl = 5034;
	bool hrobig = false;
	int luimxvesyr = 1970;
	string rmacftgxddecl = "wmgtxwbhj";
	bool afpltfpqbs = true;
	bool rvjdgttrltbjso = false;
	int vqrrt = 6507;
	int tynginjcav = 1013;
	string xymxvcthpehx = "kfmqovgorjxglyrddjtksmmrqeguetuxhwnvkujxblhyvugfgfktbykgnduhxutlisj";
	double zusdpa = 23910;
	if (false == false) {
		int qzzloh;
		for (qzzloh = 7; qzzloh > 0; qzzloh--) {
			continue;
		}
	}
	return string("huwvnlfeqkpcchhbh");
}

int byhvjfm::fwtcndxhabz(int vuwpfssqidvnfa, bool unqcnehwcktathb, int hskjezv, double lgqjhrg, double lijvvznt, int kighoftwvrsh, double xitmzpsi, string mpqoahvliv) {
	string xrhbexhg = "cqtrwlxragxwjuppvwfloyhayutrxrzrpyumfmve";
	double euvvdihqd = 12110;
	string zbkjure = "hhuysgukvvsqidchfgekgtpclxjgoiwyrhjbiccbyirfwdwwkystjautdtzkpgx";
	int qasieccxmmnca = 707;
	string hkvuez = "toaglukccgzimaeuwkuodxpxxoowjyc";
	string rleqndfkkku = "zsjxduuwsldyinqzkffyjrdokvgmcsafqkfmtsgaxhe";
	string jndhegsfv = "eaajyyljtnsfnfmxfxnyuzzlohikjqipfjdznpmyuhowotmbpeiwncvffbkfijhatwisocnkbwustpxkriwqgnrtcit";
	if (string("toaglukccgzimaeuwkuodxpxxoowjyc") != string("toaglukccgzimaeuwkuodxpxxoowjyc")) {
		int jkerlwpr;
		for (jkerlwpr = 39; jkerlwpr > 0; jkerlwpr--) {
			continue;
		}
	}
	if (string("zsjxduuwsldyinqzkffyjrdokvgmcsafqkfmtsgaxhe") != string("zsjxduuwsldyinqzkffyjrdokvgmcsafqkfmtsgaxhe")) {
		int bctg;
		for (bctg = 66; bctg > 0; bctg--) {
			continue;
		}
	}
	if (707 == 707) {
		int ttpa;
		for (ttpa = 97; ttpa > 0; ttpa--) {
			continue;
		}
	}
	return 27665;
}

void byhvjfm::ugsxwjyvmudqawhbdf(double ugflhebl, double mywaaoru, string jivzzoeuzkg) {
	string krpgprz = "hmdyyqkwzkhmutedgpjirrryxztyiqlrbwfwinjloaxrdmqrwlhuvdgsdkwynxodvfdtdeeoismotyxlwyvqe";
	string spzek = "auhqhwgltbiamhptwbwcbgfgjqhvtadmrcddpvzyicnuksfghnkjblbblmwrqjvgturyyxqkwvciverzmzlbm";
	int injopdabva = 6949;
	bool hwwnamv = true;

}

string byhvjfm::nowblffoztdgvcxm(bool iejcnxmppnuk, string gjnyb, bool agxhyzorex, int xpsvqhmere, double capzphtwqgalnbp, double qzvdembarkpzukk, bool utkkgypnsap, double fdpsfrzbizmtyte, int ithhnltb) {
	bool fspavxxm = false;
	bool zhtzl = true;
	double fwxenqjzvypgoe = 11277;
	bool uxwzmz = true;
	bool dxiufxasx = false;
	int ecnoubfyyenpsk = 322;
	double whlaeglcvkdn = 11809;
	string olidpnmolomou = "kksdnifdplbhtcsaqlxjsyraeghsyakhyurckawbnnyvxtdfzkezthefwovvttmojhlktjltihz";
	if (false == false) {
		int wevhzoovl;
		for (wevhzoovl = 88; wevhzoovl > 0; wevhzoovl--) {
			continue;
		}
	}
	if (true != true) {
		int wdumhg;
		for (wdumhg = 68; wdumhg > 0; wdumhg--) {
			continue;
		}
	}
	if (false != false) {
		int mhl;
		for (mhl = 33; mhl > 0; mhl--) {
			continue;
		}
	}
	if (false != false) {
		int ps;
		for (ps = 79; ps > 0; ps--) {
			continue;
		}
	}
	if (false != false) {
		int fod;
		for (fod = 99; fod > 0; fod--) {
			continue;
		}
	}
	return string("hilcwomaqruxxrce");
}

double byhvjfm::racqsvurnxsxkk(int xokxjqc, bool yufqedgmxusjp, int tlwausvujiwroo, bool djrypmd, double beqeiwohtagxhh) {
	bool ifkeobg = true;
	double nmkpbvvtt = 19972;
	int nagdlmrtydzo = 2754;
	if (2754 != 2754) {
		int vscyvxn;
		for (vscyvxn = 37; vscyvxn > 0; vscyvxn--) {
			continue;
		}
	}
	return 69325;
}

string byhvjfm::sglpdeebtio(bool lbrxpz, double dzfsmimwn, double elfjf, string yogjswukmsdob, string zevux, int dhbshjdak, int xmtyiljixhp) {
	return string("qxbmommkbwdgvm");
}

void byhvjfm::tjrzbyetkiycrrd(int thbfst, double vhjcppeu, double sjrmdbkeji, int jvbiqnmrc, string gqweivv, bool ffaccmyvzc, bool qsdylqqnscx, int eunzbnzhh, bool jldkxpegnubmzh) {
	bool fcczqzhum = false;
	int xuvichgq = 178;
	string owbhpov = "auuvbotcqlyfvpeudfwvmimnkggtnikbslbjaepahgzg";
	double gkercrwhbzo = 12591;
	string tkzkcrl = "fjmbufiijnxvokekizkgspvsqxkprbnwvrfdllnwaldthsunyaheqrvpxdffoowgyumyzrqgnzjmkhbra";
	bool njeqpy = false;
	double gbbulqueamm = 19435;
	int pvygvxavinrcw = 3831;
	if (false != false) {
		int zyhzuppdd;
		for (zyhzuppdd = 17; zyhzuppdd > 0; zyhzuppdd--) {
			continue;
		}
	}
	if (false == false) {
		int pvibzgycbi;
		for (pvibzgycbi = 83; pvibzgycbi > 0; pvibzgycbi--) {
			continue;
		}
	}
	if (19435 != 19435) {
		int crsevvsjql;
		for (crsevvsjql = 35; crsevvsjql > 0; crsevvsjql--) {
			continue;
		}
	}
	if (false == false) {
		int yokrlbwfy;
		for (yokrlbwfy = 74; yokrlbwfy > 0; yokrlbwfy--) {
			continue;
		}
	}
	if (false == false) {
		int hwaw;
		for (hwaw = 11; hwaw > 0; hwaw--) {
			continue;
		}
	}

}

string byhvjfm::ijmxckqakr(int rzqemebgp, bool tofxsmitlgiidx, string ngggfcipege, string wtjpzrqyekoeg, string qrlyowow, double afcva) {
	string zzdar = "rjbiktevetsklfehykdrliphemfrashhhlsyplkjvozllcsthhujzlqofbfe";
	int qenlhgyorwtw = 4904;
	string blegtekh = "tsqmleqenxecooskwqpcchknxcnjfhmqrkaweiolbfdbsmtvpmmdterlgvquodewqblwl";
	string tfzncbpifermsef = "yejwyjpstieyvqmofupqbkascyzwcbjzcemlatfk";
	if (string("tsqmleqenxecooskwqpcchknxcnjfhmqrkaweiolbfdbsmtvpmmdterlgvquodewqblwl") != string("tsqmleqenxecooskwqpcchknxcnjfhmqrkaweiolbfdbsmtvpmmdterlgvquodewqblwl")) {
		int sc;
		for (sc = 1; sc > 0; sc--) {
			continue;
		}
	}
	if (4904 == 4904) {
		int kp;
		for (kp = 40; kp > 0; kp--) {
			continue;
		}
	}
	if (string("rjbiktevetsklfehykdrliphemfrashhhlsyplkjvozllcsthhujzlqofbfe") != string("rjbiktevetsklfehykdrliphemfrashhhlsyplkjvozllcsthhujzlqofbfe")) {
		int ji;
		for (ji = 97; ji > 0; ji--) {
			continue;
		}
	}
	return string("mwkccext");
}

void byhvjfm::tjrgkgwpqkwvfjuc(int jtzbjqeauczoyk, bool swhzuenuiufwd, bool fsvqlzvngxu, int wfzbj, int eblgbohrtes, double bqksnzwnaeqgh, string qogwdbpzgk) {
	bool udaaypuzyvmcukv = false;
	bool nbudwfl = false;
	bool dqzauoxtg = false;
	bool ylxxw = false;
	bool oecwaelr = false;
	double smorfusuks = 17329;
	if (false != false) {
		int wttr;
		for (wttr = 79; wttr > 0; wttr--) {
			continue;
		}
	}
	if (false == false) {
		int wl;
		for (wl = 76; wl > 0; wl--) {
			continue;
		}
	}
	if (false != false) {
		int jbxyhcort;
		for (jbxyhcort = 77; jbxyhcort > 0; jbxyhcort--) {
			continue;
		}
	}
	if (false != false) {
		int bcgtgjpbm;
		for (bcgtgjpbm = 25; bcgtgjpbm > 0; bcgtgjpbm--) {
			continue;
		}
	}
	if (17329 == 17329) {
		int jujwaf;
		for (jujwaf = 39; jujwaf > 0; jujwaf--) {
			continue;
		}
	}

}

int byhvjfm::kpwxfpaapjfbghtnyrow(double wcurvfqkphgst, int wkmpwrhfhbwj, double xpjqtthxevocuvl, double tvqprkmsffrngnd, bool opnhyllxnem) {
	string awfzvbdqsxc = "nxfaahigsupznrgu";
	double ctinpfocycqxgp = 19345;
	int gsfdgpkgkg = 5553;
	if (19345 == 19345) {
		int mvcheli;
		for (mvcheli = 7; mvcheli > 0; mvcheli--) {
			continue;
		}
	}
	if (string("nxfaahigsupznrgu") == string("nxfaahigsupznrgu")) {
		int ozsqdcnp;
		for (ozsqdcnp = 70; ozsqdcnp > 0; ozsqdcnp--) {
			continue;
		}
	}
	return 86646;
}

void byhvjfm::gpmcjxxpicvuizmwgeqlk(int sajckkbekvizyk, double ninhjezvzsdfp, bool tshyhin, double tlgzbfq, bool srfxdwq, string kvsowy, string pvrikbd, int ukagpgmzwlsjiws) {
	bool ezxutbqwya = false;
	string ugkdhu = "afejimhjcrxdhazzhncmqsqckfoqgzdeoffykhfciyobpmkwpgmxboyenbycfacjfqjznoqbnekcsnnfwqsdcasquuuvqtuzmxo";
	string vfjegcrmwjqp = "bcxrxmnaxcrgyqvocsmfmwlwckbniytugduszepmofxsobsteyixbbrgptnnwcxmhgbuwwplcpxhyuavfaja";
	if (string("bcxrxmnaxcrgyqvocsmfmwlwckbniytugduszepmofxsobsteyixbbrgptnnwcxmhgbuwwplcpxhyuavfaja") != string("bcxrxmnaxcrgyqvocsmfmwlwckbniytugduszepmofxsobsteyixbbrgptnnwcxmhgbuwwplcpxhyuavfaja")) {
		int qxdrr;
		for (qxdrr = 51; qxdrr > 0; qxdrr--) {
			continue;
		}
	}
	if (string("afejimhjcrxdhazzhncmqsqckfoqgzdeoffykhfciyobpmkwpgmxboyenbycfacjfqjznoqbnekcsnnfwqsdcasquuuvqtuzmxo") == string("afejimhjcrxdhazzhncmqsqckfoqgzdeoffykhfciyobpmkwpgmxboyenbycfacjfqjznoqbnekcsnnfwqsdcasquuuvqtuzmxo")) {
		int vmh;
		for (vmh = 17; vmh > 0; vmh--) {
			continue;
		}
	}

}

string byhvjfm::wwlsmzhsbrc() {
	double bjcubtcgfwxi = 21326;
	double ecddt = 47504;
	int ykspucybgbjuymp = 3046;
	string bkvhf = "isobucpicktd";
	int shvamhvfumnub = 5475;
	double xigehmjkfc = 30106;
	string ukxguh = "mahzlgtixfwyjnlifcgtvqwyblpilmukxalnghfkijbksxvlxzdbfsqreukfqqxvxjznhilwxsbpw";
	double jwfqlnobircfaak = 2268;
	if (string("mahzlgtixfwyjnlifcgtvqwyblpilmukxalnghfkijbksxvlxzdbfsqreukfqqxvxjznhilwxsbpw") == string("mahzlgtixfwyjnlifcgtvqwyblpilmukxalnghfkijbksxvlxzdbfsqreukfqqxvxjznhilwxsbpw")) {
		int pysep;
		for (pysep = 54; pysep > 0; pysep--) {
			continue;
		}
	}
	if (3046 == 3046) {
		int ulyu;
		for (ulyu = 52; ulyu > 0; ulyu--) {
			continue;
		}
	}
	return string("ryjowsersbryms");
}

string byhvjfm::ctagvjwxtxvdvudhjpvg(bool zkvzy, int ogokimgizhx, double qvpspqeif, int ddwisvwrv, string kvhwfuouwisjjv, double wsfjywrn, double xjhfdvkuqp) {
	double tqgnid = 13580;
	double iwdks = 28722;
	bool sqfpy = true;
	double icduqrqscjkilzy = 65842;
	if (65842 != 65842) {
		int xoifogotkv;
		for (xoifogotkv = 14; xoifogotkv > 0; xoifogotkv--) {
			continue;
		}
	}
	if (28722 == 28722) {
		int pgk;
		for (pgk = 32; pgk > 0; pgk--) {
			continue;
		}
	}
	if (65842 == 65842) {
		int udosmscvbb;
		for (udosmscvbb = 33; udosmscvbb > 0; udosmscvbb--) {
			continue;
		}
	}
	if (true != true) {
		int agikdsbr;
		for (agikdsbr = 39; agikdsbr > 0; agikdsbr--) {
			continue;
		}
	}
	return string("eglreyluqrycm");
}

int byhvjfm::mnocdjyhiuzqahdgs(bool uirjrqewnbwpjp, string bwnon, double himcxhvbuteulhb, bool yvmgtjzbjmfbqeb, int rbtctwlcftzolkx, int xoxidyxnpcrmj, bool vdjdoggwrkxwlxg, string usgatnbdkckncr, bool gnfwkuyhk) {
	int pcdsimzdliacbiy = 1504;
	bool wyckuu = true;
	if (true == true) {
		int joffqb;
		for (joffqb = 13; joffqb > 0; joffqb--) {
			continue;
		}
	}
	if (true == true) {
		int qffjwgxsjb;
		for (qffjwgxsjb = 13; qffjwgxsjb > 0; qffjwgxsjb--) {
			continue;
		}
	}
	if (1504 == 1504) {
		int pplm;
		for (pplm = 9; pplm > 0; pplm--) {
			continue;
		}
	}
	if (true == true) {
		int fftgjt;
		for (fftgjt = 12; fftgjt > 0; fftgjt--) {
			continue;
		}
	}
	return 23285;
}

string byhvjfm::ldamwvzgxvrezwuq(int zzoxjxgfofxhf, string aqjadalxacgsena, string quwfzzjruord, double nslyxgut, bool iogjopmwtg, bool xhqidqgqn, double baauye, string tbznxgdu, int wmpix) {
	double tpaclyo = 67146;
	string xohucoundd = "jwosbhcman";
	bool jvjpcga = false;
	if (false == false) {
		int cbepwvwfhu;
		for (cbepwvwfhu = 47; cbepwvwfhu > 0; cbepwvwfhu--) {
			continue;
		}
	}
	if (67146 == 67146) {
		int zdft;
		for (zdft = 54; zdft > 0; zdft--) {
			continue;
		}
	}
	return string("qvkqxobkkelvpwqbync");
}

int byhvjfm::ccsgztkjlvqtkzmezcr(double hwmizxaddyhbrxy, int xsaekzrnvspkm, string coydhtjxh, string tuafmnmbjy, string iakvelryqs, string fqjokl, bool aypxsepee, string zhlfpptnrombt, int pfivwqjcqcuiv, string sdbuxorqzbv) {
	bool fcyjzomc = true;
	bool mxvrh = false;
	string yazje = "ywpyjczrdjnhcejyefgn";
	double zrngkyqiczpjc = 27432;
	if (27432 != 27432) {
		int havftgjmn;
		for (havftgjmn = 77; havftgjmn > 0; havftgjmn--) {
			continue;
		}
	}
	if (false == false) {
		int ij;
		for (ij = 72; ij > 0; ij--) {
			continue;
		}
	}
	if (string("ywpyjczrdjnhcejyefgn") == string("ywpyjczrdjnhcejyefgn")) {
		int guc;
		for (guc = 66; guc > 0; guc--) {
			continue;
		}
	}
	if (false == false) {
		int mdnemnslk;
		for (mdnemnslk = 34; mdnemnslk > 0; mdnemnslk--) {
			continue;
		}
	}
	if (true != true) {
		int xxoudxdek;
		for (xxoudxdek = 95; xxoudxdek > 0; xxoudxdek--) {
			continue;
		}
	}
	return 26465;
}

byhvjfm::byhvjfm() {
	this->ccsgztkjlvqtkzmezcr(7043, 1950, string("wwarnoovrkytcvclscdtpwbaeulkvmspaegtmglwjjcazmfqyhyplifqhpufduktygkg"), string("ywpinvubngoanjyrboegeacordgirlwqaed"), string("escmchkbhqvhpycryonuxsqrow"), string("cbxocljgpofzfplwxjaifcofwgcugnokosoxpgino"), false, string("tlsvrqggquokfensjtiwrtffgxnvrunptuivwxleshhvmanudcrsmfhnumsqopoqv"), 2824, string("fusyprbdedhzesppihtcfcfdhycjrzcpqtyzgyqxzsqepgbpbxedtbnsukloykpwipmybcrpmctbnsnnbmojpnfua"));
	this->sglpdeebtio(true, 63927, 4038, string("teynrobrzsetmftolemokwjzycvdhbzjuvjxwcjllhagyhfltfvi"), string("kanvgslziysthyfeikhdnqelbckgyexloyznfepntilyfflwxmyieb"), 2371, 6213);
	this->tjrzbyetkiycrrd(1716, 10207, 9916, 1013, string("ekakzzoifuattpohlqeoql"), false, true, 694, false);
	this->ijmxckqakr(3452, false, string("nrstpmpvdkdwchaddidkdztcxeirxpxerfvhrxdduqcescrpjhtbcabbssoadnmqllcythhobtaknkbnwmqdowfzttjhyxtkhyo"), string("gydsxiwqgyjmdzqpbqzekwcpqiblqquidfecgxakijfkqidnwtows"), string("pjwhkxzdifrxomwuxsrgqjohnahfskbgcgaltcehkvrmmomikiquzjngmivnmzdzkjftezszmaqfvasrnzo"), 12984);
	this->tjrgkgwpqkwvfjuc(4307, false, true, 1380, 1061, 18057, string("hltrtpargimednlqdppsnjvecupttxgomqdrbngkqngmghbrafxeieelekrbzqoxcxo"));
	this->kpwxfpaapjfbghtnyrow(34010, 6653, 623, 16574, true);
	this->gpmcjxxpicvuizmwgeqlk(6512, 8217, false, 1121, true, string("eohvejncsafzkhpxtxhschlsjpdelhkmoxpvtwxmqcbkkzdcjpjsxnengsbzzcyzsuilklsjkc"), string("cjrbxtabglnsjrqhrlaiwgnakilumgdwroezbqscucllvrzecpvxdntdkbuvizebcrvoem"), 4698);
	this->wwlsmzhsbrc();
	this->ctagvjwxtxvdvudhjpvg(true, 1534, 1278, 65, string("ixrulybqbfaczzbfepybbiycsixmbvrrduowvzewkncenxktxyxbadrwvxjgxwadrtfhubwnszxotknlteknynwlxyossomqiz"), 23746, 1866);
	this->mnocdjyhiuzqahdgs(false, string("oqdjvbrfixfvzphkdkginbeqpfawxftaprrudkz"), 15961, true, 3056, 442, false, string("vipwoeonxiymnxmeerygxgepnmhgqaxkxapoxuuqmxltnzbdbjajrljydpwelaicjlzaswvmepnbbbyvvqnx"), true);
	this->ldamwvzgxvrezwuq(654, string("guflulihhpruewvxvxzjzbkltgwbttvutclkrom"), string("fkhglkemcpkckvibxjslokxsstycdbkovlrauqr"), 39691, false, true, 19348, string("zxekuvppanjlhyhhwjdflhzapebtaewrstjjjt"), 599);
	this->bplngstzku(string("uljyywjfawvblhxxgfzfj"), 463, 942, true);
	this->sljrfkcxafmlv(true, 15330, string("qigdtdyanfnymljemwvxxboxauebzfz"), string("khhdqe"), string("dhnvexezcadlulhgyt"));
	this->fwtcndxhabz(495, false, 7403, 17906, 33038, 2571, 5186, string("rozyosjmtekojymeqqnocwosgiqsjbpehiecibgndatroceueuuskygkpfzrzkoklvrthvqmgpkvlqugnmwvu"));
	this->ugsxwjyvmudqawhbdf(61664, 17142, string("xpkoccokpkxkpmmlnjpggsnnlrdqyrbijxvaehcpistlxmvtgebm"));
	this->nowblffoztdgvcxm(false, string("qerxxvpobwemzrgljf"), true, 3525, 9576, 65641, true, 50548, 667);
	this->racqsvurnxsxkk(340, true, 6055, true, 54166);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ozsbneo {
public:
	string oxmzlxawxdxi;
	ozsbneo();
	double dnpqaarxehadioggumze(int iszftr, string mlyej, int kqpqapmlbwbjf, string ndzlpitohos, int quoxrgyb, int qtzcxa, double feaenps, string cwlor, string uapbjnhe, string zfympdquudi);
	void cxwiwgjclyzikcj();
	bool rdhaeqvezh();
	bool odnrymdiqwvulsknaeqmn(int clpxwsflwupggp, bool giktoqo, string uklbwblti);
	bool livfnwoxkdboqey(int ozjgbduanseb, int pkllscyy, int txrpikapdvg, string eoisxtn, int oyjglfpbgsrg, bool zqokbzpxykckidz, string jxqclyyixq, bool ybmriopxoi);
	bool iibimvwbbmccpedajzunpa(string joufpijgxavt, string dzdpiyupunhgiob, double epfta, bool padtdrhxiongn, bool upckzcqefsphbl);
	bool wxmvqbuyqqeqfh(string ughymla, string jykssgsvyqlqeqa, string stoxazcuidlglu, string jzfle, int ozbwjppmtyunkob, string xsnxnx, bool nlwaf, double ufezdjkszc, double psinqxiuvxri);
	void nqwhhprkfeliufxjhhglbek(string pqvnznm, double nyabhiaxbkyoze, int inesrumiisyirk, int sbjfcnwzgpz, double kzasfzisowurvk, double cnynpktlk, string vpsvuz);

protected:
	double enmfuzxkblkkgo;
	string tmhdzp;
	double ubxtwaaqokch;

	void taievcerhpnwdjvzenfmwxkgx(double lipnjtqssbi, double swgsfjovrn, double qmeumyv, string qvhdfoksgxvhnd, bool zhhvfvb, double ajydqsplhyxy, int mzomvlwmrk, string wnpuidqide);
	int mpwlhnsscqwjjftwbpddfj(double wrymlwkdhvwgmg, bool wejwrgjkgwmm, double puglfvbjcmweecs, string kwhojp, int ertlsrbftgpoc, bool vpbnlijxmhjbnb);
	int qyylsrtnsgdp(bool vqpuriof, int iahswjborelcbkb, string xsrmfxx, int ooasiseivdo, bool xnudyoala, bool diosivsidtd);
	int mxpszhzgwzqeizh(string jwdednjpopjotlu);
	bool qxesonxwcrpqvdpcvyuedjlqv();
	void evrfeodlgcgrjrp(bool kqajjbqcoqwkzmf, bool cwyvycnxmvhj, double mltesumeqzfh, string tlbzponfzaho, int aaerve, bool jtwjohgbpwnj, double rvrertxmzkaiju, bool cxbrbijohtea, string rcnafwrkki, string snyuonoyqqnjov);
	bool uxvpgsweihqulgvqsbhllt(double qebpagkal, string sosxqswraa, string umwvblmxokeiuvg, bool satilby, string ihwdba, bool nmecfkk, string ongqwzwa, double aecaeh);
	bool zmlzorcttvlbtnfwgcbmattzs(bool upigg, int arhcnr, string vdifiueuic, int adhpqjrgoxa, double nckfhwnixzd);

private:
	string lcbol;

	string ermgsuylnvm(int gmyujbyxkwzcg, string cypwrddjowrsurb, double smfjskgmubomtdg, string usxivovrpizrb, string qlbgcuishz, string ntloonmdjpoax, string hohtbigtgt, double cgeokumlivskx, bool kdsyxdk, double ebqupe);
	string lqfsigsbcywudcesxpjrrm();
	bool fubjoupqjnyfyyhsgejs(bool zvfdkmnawkxji, string oyqghc, int znaysc, int cqyhhh, int kgklilsymris);
	double rketsmsavg(string xanwmximfdtrau, string wpyhkxmztzn, int doprornlyh, string wfovectresk, string njfwvbfdiarw, bool ikpbccnkbe, int zpgafsltrikjt, string xbzwxow, bool wjfkbftnpacvu, bool rlxqeaykgvdztkg);
	void uaesddotznaqmij(bool vjslj, double pewxgspmby, double hfmqfoafigan, int xrtgjpxdwovlkez, double ykyuqtdbihjzlq, bool twibtlnfy, int nrknoshttsb, int yaxrydgigdytlg, bool vgqwkba);
	double ucnjcopttjavbzdidniz(double ymfhupceqmnjcd);
	string sbxpwuzduktkphcrojwsr(string zgmindkts, string sxenrawkrbmnp, bool jurhscnhqaxp, string yfpsdgrzzj, double imexmmprwdbyt, double pawybb, bool vfllqeivqalgqp, int buicgkkjbqsvrgg);
	string xuxwxohcgpvhvvrzfzucqewbg(string ikzggstljjfaan, int ckhzfbuw, bool sshofmr, double uvzke, string qvhjgjnehy, int fqvyk, double rnjek);

};


string ozsbneo::ermgsuylnvm(int gmyujbyxkwzcg, string cypwrddjowrsurb, double smfjskgmubomtdg, string usxivovrpizrb, string qlbgcuishz, string ntloonmdjpoax, string hohtbigtgt, double cgeokumlivskx, bool kdsyxdk, double ebqupe) {
	string cqgidrzte = "ksckkvwrdntgakvhepmzotxreenljeyxcxbbewocksflwykxyawcseovwuazkklyneicmvdx";
	if (string("ksckkvwrdntgakvhepmzotxreenljeyxcxbbewocksflwykxyawcseovwuazkklyneicmvdx") != string("ksckkvwrdntgakvhepmzotxreenljeyxcxbbewocksflwykxyawcseovwuazkklyneicmvdx")) {
		int wmsmt;
		for (wmsmt = 10; wmsmt > 0; wmsmt--) {
			continue;
		}
	}
	return string("duptcuxgywq");
}

string ozsbneo::lqfsigsbcywudcesxpjrrm() {
	bool pmbdumjfog = true;
	string hyllacr = "xnvgvnvimxrsvbuvnkdzsfbotrnydxdcvfnulnwczyf";
	int nlbkltaytjcc = 4168;
	return string("lwylxggqohehdwbidvv");
}

bool ozsbneo::fubjoupqjnyfyyhsgejs(bool zvfdkmnawkxji, string oyqghc, int znaysc, int cqyhhh, int kgklilsymris) {
	bool ovakgf = false;
	int uxiopc = 4083;
	bool wbfkgkwuzmulpkx = true;
	double ahgrfgalfkcfgx = 15818;
	double rbfvtn = 57691;
	if (4083 != 4083) {
		int wyhai;
		for (wyhai = 43; wyhai > 0; wyhai--) {
			continue;
		}
	}
	return true;
}

double ozsbneo::rketsmsavg(string xanwmximfdtrau, string wpyhkxmztzn, int doprornlyh, string wfovectresk, string njfwvbfdiarw, bool ikpbccnkbe, int zpgafsltrikjt, string xbzwxow, bool wjfkbftnpacvu, bool rlxqeaykgvdztkg) {
	string hnhfcxtcbum = "vjwjfnmqeaqxlxkvbhyjevwycrxqdanowbonvgyrfrfphtgjtjkwwczkobjvzavwfyaswgzmvxgjokpqv";
	int ewuayv = 8891;
	if (8891 == 8891) {
		int kabyglq;
		for (kabyglq = 96; kabyglq > 0; kabyglq--) {
			continue;
		}
	}
	if (string("vjwjfnmqeaqxlxkvbhyjevwycrxqdanowbonvgyrfrfphtgjtjkwwczkobjvzavwfyaswgzmvxgjokpqv") != string("vjwjfnmqeaqxlxkvbhyjevwycrxqdanowbonvgyrfrfphtgjtjkwwczkobjvzavwfyaswgzmvxgjokpqv")) {
		int lww;
		for (lww = 24; lww > 0; lww--) {
			continue;
		}
	}
	if (8891 != 8891) {
		int ej;
		for (ej = 63; ej > 0; ej--) {
			continue;
		}
	}
	if (string("vjwjfnmqeaqxlxkvbhyjevwycrxqdanowbonvgyrfrfphtgjtjkwwczkobjvzavwfyaswgzmvxgjokpqv") != string("vjwjfnmqeaqxlxkvbhyjevwycrxqdanowbonvgyrfrfphtgjtjkwwczkobjvzavwfyaswgzmvxgjokpqv")) {
		int hufldrbci;
		for (hufldrbci = 40; hufldrbci > 0; hufldrbci--) {
			continue;
		}
	}
	return 13964;
}

void ozsbneo::uaesddotznaqmij(bool vjslj, double pewxgspmby, double hfmqfoafigan, int xrtgjpxdwovlkez, double ykyuqtdbihjzlq, bool twibtlnfy, int nrknoshttsb, int yaxrydgigdytlg, bool vgqwkba) {
	int xkdfwdwembqux = 1836;
	bool pgganboanejebyi = false;
	int ywteylfm = 2090;
	string sjxjvsnc = "iqmuljjvnz";
	int elfvdvtq = 4553;
	int iwgiyy = 821;
	if (false != false) {
		int lvswjjbxpl;
		for (lvswjjbxpl = 87; lvswjjbxpl > 0; lvswjjbxpl--) {
			continue;
		}
	}
	if (2090 != 2090) {
		int lvvclilvor;
		for (lvvclilvor = 16; lvvclilvor > 0; lvvclilvor--) {
			continue;
		}
	}

}

double ozsbneo::ucnjcopttjavbzdidniz(double ymfhupceqmnjcd) {
	double fpeeiiykcaniu = 29216;
	bool fhtrjeq = true;
	bool ukevombcfy = false;
	string uumcuftqa = "guknsdhvfzuuaenmxdnlomxincoabnnkhyhhzgpsscnb";
	string onrblgszkuyb = "lzcratzcjpzjkhuzixxq";
	bool wcnodjfhyrw = false;
	double cjffnb = 33061;
	bool rxmeehdputxrnpb = true;
	string qtkztdt = "wgjzaebhmmpuproaxyuocvofcpxspbkceghjcjhmc";
	if (true != true) {
		int taesnrwzz;
		for (taesnrwzz = 76; taesnrwzz > 0; taesnrwzz--) {
			continue;
		}
	}
	return 80934;
}

string ozsbneo::sbxpwuzduktkphcrojwsr(string zgmindkts, string sxenrawkrbmnp, bool jurhscnhqaxp, string yfpsdgrzzj, double imexmmprwdbyt, double pawybb, bool vfllqeivqalgqp, int buicgkkjbqsvrgg) {
	bool mkyqawfyogmn = true;
	bool vfupugbsmitwsul = true;
	double tptwieh = 10561;
	int htmwuwneklfi = 2564;
	bool fdaywhdtbxy = true;
	int xhnovq = 4724;
	int ozssdzfkwsucf = 7356;
	return string("yvkntwgrtrwhfl");
}

string ozsbneo::xuxwxohcgpvhvvrzfzucqewbg(string ikzggstljjfaan, int ckhzfbuw, bool sshofmr, double uvzke, string qvhjgjnehy, int fqvyk, double rnjek) {
	return string("");
}

void ozsbneo::taievcerhpnwdjvzenfmwxkgx(double lipnjtqssbi, double swgsfjovrn, double qmeumyv, string qvhdfoksgxvhnd, bool zhhvfvb, double ajydqsplhyxy, int mzomvlwmrk, string wnpuidqide) {
	double vyiqmu = 41955;
	double jcnukou = 49281;
	string edyhtwgbsseggm = "keyyblrpdhkpkcqutnkmzfmwqbuduqifnsuytgqrtmngyojrrszodojboszetziadggsoxcgykhjtcm";
	if (41955 != 41955) {
		int gsjcugfoe;
		for (gsjcugfoe = 69; gsjcugfoe > 0; gsjcugfoe--) {
			continue;
		}
	}
	if (49281 == 49281) {
		int zexk;
		for (zexk = 15; zexk > 0; zexk--) {
			continue;
		}
	}
	if (string("keyyblrpdhkpkcqutnkmzfmwqbuduqifnsuytgqrtmngyojrrszodojboszetziadggsoxcgykhjtcm") != string("keyyblrpdhkpkcqutnkmzfmwqbuduqifnsuytgqrtmngyojrrszodojboszetziadggsoxcgykhjtcm")) {
		int nn;
		for (nn = 32; nn > 0; nn--) {
			continue;
		}
	}
	if (49281 == 49281) {
		int lwktyims;
		for (lwktyims = 3; lwktyims > 0; lwktyims--) {
			continue;
		}
	}
	if (41955 != 41955) {
		int ixjxzzxy;
		for (ixjxzzxy = 23; ixjxzzxy > 0; ixjxzzxy--) {
			continue;
		}
	}

}

int ozsbneo::mpwlhnsscqwjjftwbpddfj(double wrymlwkdhvwgmg, bool wejwrgjkgwmm, double puglfvbjcmweecs, string kwhojp, int ertlsrbftgpoc, bool vpbnlijxmhjbnb) {
	double trhzfpkryi = 2011;
	bool eoalrx = true;
	string oaxeohtxwsnn = "ytdyyocixvfktshzlbyqxhdyhubhqfswsjscdhjdpviksfrdddkfgtapwfqdbmvleiyxorvpfaehqoyzvgvfjvcvjj";
	string xeqmwz = "rwvtjihnnlf";
	bool fhnfbnikras = true;
	double oyojbs = 4785;
	string ilokqqufex = "nujctmwsnmjubxvnhxuhebjpsdftregdxurxwbqiehidprniolrgesklcirqjrodlndbsvcq";
	if (2011 != 2011) {
		int lc;
		for (lc = 48; lc > 0; lc--) {
			continue;
		}
	}
	if (string("ytdyyocixvfktshzlbyqxhdyhubhqfswsjscdhjdpviksfrdddkfgtapwfqdbmvleiyxorvpfaehqoyzvgvfjvcvjj") != string("ytdyyocixvfktshzlbyqxhdyhubhqfswsjscdhjdpviksfrdddkfgtapwfqdbmvleiyxorvpfaehqoyzvgvfjvcvjj")) {
		int yztjfz;
		for (yztjfz = 99; yztjfz > 0; yztjfz--) {
			continue;
		}
	}
	return 30840;
}

int ozsbneo::qyylsrtnsgdp(bool vqpuriof, int iahswjborelcbkb, string xsrmfxx, int ooasiseivdo, bool xnudyoala, bool diosivsidtd) {
	bool krrluxzlub = true;
	string lfppplqvs = "pfrchqrbvpajxrrmseyittlbcnmksriczkevtosgmmedritsmqxcqxjttkvgwriapvtcrkiqpxdd";
	double itccnthect = 59826;
	double lfqvujbupht = 39368;
	string eomcmd = "dcalqddnkrlxyzcyavjecnzzbbxxpxpivgazoqfemgbbnygecshgfgcipzenk";
	int usxyoygno = 2380;
	string rgpzbtrxnessofh = "xbvypfpfdkjdrmeayzgacgcibspwazpegxtktqpnlsmdf";
	double tluetyawdnkwz = 19192;
	string eqkyxim = "pirtkwkzkqqkrnrilhorlmktrno";
	string xlkoqddkakcf = "afqmoneajlvwwmyjqxvnqqqpwsuyyopanvkrfdmjdjyvznitwysapnirfcrwqipvyptnmnkatvilharefcercw";
	if (2380 != 2380) {
		int jtqlenffsr;
		for (jtqlenffsr = 35; jtqlenffsr > 0; jtqlenffsr--) {
			continue;
		}
	}
	if (true != true) {
		int pobotsud;
		for (pobotsud = 11; pobotsud > 0; pobotsud--) {
			continue;
		}
	}
	return 69065;
}

int ozsbneo::mxpszhzgwzqeizh(string jwdednjpopjotlu) {
	bool xqkhnlhqo = false;
	string jlvniqaz = "dsnxysenablkfbcvvblmutqazpwwrarkdnkwcqxieagmvpqcctqegpaghzlipykyhwolqvtddoljarcfsbtpwnzuul";
	double tvoxrle = 32872;
	int fiyxxht = 1845;
	bool xsuidpsqmot = true;
	bool ucmfmneo = true;
	if (true != true) {
		int patwxte;
		for (patwxte = 100; patwxte > 0; patwxte--) {
			continue;
		}
	}
	if (true == true) {
		int futgn;
		for (futgn = 82; futgn > 0; futgn--) {
			continue;
		}
	}
	return 97515;
}

bool ozsbneo::qxesonxwcrpqvdpcvyuedjlqv() {
	string vxvlfwbtn = "ujyrfpecatrqdocxtbzoiznogmtglvxlrynhwrhtmfkyndeaffyzyuxvfpbvrgykzvpsfpbvvvwxhvlyhsfarrelxbrrmfqargbp";
	int utqmoulejbk = 6939;
	double uevhmoppnmycb = 12515;
	string bsqsatk = "mvmfwctdpjhnjxqbznbxhmngdvbysyvaklzjfxggarnrlvjfsjwpcujzwgwalfkvnsqhudfydulbkgia";
	if (string("mvmfwctdpjhnjxqbznbxhmngdvbysyvaklzjfxggarnrlvjfsjwpcujzwgwalfkvnsqhudfydulbkgia") != string("mvmfwctdpjhnjxqbznbxhmngdvbysyvaklzjfxggarnrlvjfsjwpcujzwgwalfkvnsqhudfydulbkgia")) {
		int ytuct;
		for (ytuct = 30; ytuct > 0; ytuct--) {
			continue;
		}
	}
	if (6939 == 6939) {
		int ctkwuy;
		for (ctkwuy = 67; ctkwuy > 0; ctkwuy--) {
			continue;
		}
	}
	if (string("ujyrfpecatrqdocxtbzoiznogmtglvxlrynhwrhtmfkyndeaffyzyuxvfpbvrgykzvpsfpbvvvwxhvlyhsfarrelxbrrmfqargbp") == string("ujyrfpecatrqdocxtbzoiznogmtglvxlrynhwrhtmfkyndeaffyzyuxvfpbvrgykzvpsfpbvvvwxhvlyhsfarrelxbrrmfqargbp")) {
		int bunli;
		for (bunli = 89; bunli > 0; bunli--) {
			continue;
		}
	}
	if (6939 != 6939) {
		int gqvo;
		for (gqvo = 52; gqvo > 0; gqvo--) {
			continue;
		}
	}
	return false;
}

void ozsbneo::evrfeodlgcgrjrp(bool kqajjbqcoqwkzmf, bool cwyvycnxmvhj, double mltesumeqzfh, string tlbzponfzaho, int aaerve, bool jtwjohgbpwnj, double rvrertxmzkaiju, bool cxbrbijohtea, string rcnafwrkki, string snyuonoyqqnjov) {
	string mhwaqq = "fzsllmajhcfxrbhaxmhynwrawtuijqyubvrcwvesfvvrscbdzymnakezedgmbqxvrqjuawpsavcopsqqudzam";
	if (string("fzsllmajhcfxrbhaxmhynwrawtuijqyubvrcwvesfvvrscbdzymnakezedgmbqxvrqjuawpsavcopsqqudzam") == string("fzsllmajhcfxrbhaxmhynwrawtuijqyubvrcwvesfvvrscbdzymnakezedgmbqxvrqjuawpsavcopsqqudzam")) {
		int rbauk;
		for (rbauk = 79; rbauk > 0; rbauk--) {
			continue;
		}
	}
	if (string("fzsllmajhcfxrbhaxmhynwrawtuijqyubvrcwvesfvvrscbdzymnakezedgmbqxvrqjuawpsavcopsqqudzam") != string("fzsllmajhcfxrbhaxmhynwrawtuijqyubvrcwvesfvvrscbdzymnakezedgmbqxvrqjuawpsavcopsqqudzam")) {
		int ivgltkqli;
		for (ivgltkqli = 54; ivgltkqli > 0; ivgltkqli--) {
			continue;
		}
	}

}

bool ozsbneo::uxvpgsweihqulgvqsbhllt(double qebpagkal, string sosxqswraa, string umwvblmxokeiuvg, bool satilby, string ihwdba, bool nmecfkk, string ongqwzwa, double aecaeh) {
	double zxvvluboj = 65481;
	double rmuqmkmqtcx = 45929;
	string rkxnwuatztij = "yhywsaqjymrbykejbvwwlpnawwrzjctvvrxfqagqycnyp";
	double cxpitfzoglv = 30054;
	int fpceixrabarp = 5752;
	string pnfjub = "xkxjwlbjvljlhzzynsuxskhhferppetzyyqqwnmpoedufkpk";
	return false;
}

bool ozsbneo::zmlzorcttvlbtnfwgcbmattzs(bool upigg, int arhcnr, string vdifiueuic, int adhpqjrgoxa, double nckfhwnixzd) {
	int qbqfcdmmyhj = 2186;
	double yuhcax = 17929;
	int pkhigqefs = 934;
	bool pvehuzinxrgzfim = false;
	bool fheziftqlymoybx = false;
	int gewboqakbcijiiw = 321;
	return false;
}

double ozsbneo::dnpqaarxehadioggumze(int iszftr, string mlyej, int kqpqapmlbwbjf, string ndzlpitohos, int quoxrgyb, int qtzcxa, double feaenps, string cwlor, string uapbjnhe, string zfympdquudi) {
	string lutvethhin = "lphrfkajhgckynxkkjpidkqvmstsirjwwfiuzyznfzwjzeznwhcsqawpztomwfjfyubxttmxalhyqseavbshtjyfajily";
	int dzuzcpzf = 2051;
	return 14180;
}

void ozsbneo::cxwiwgjclyzikcj() {
	string owlrsqqpv = "jxxhjmroiairspoeczpmcwvtwprf";
	bool ztzlhrgisdyug = true;
	double tktdfgdu = 68872;
	if (68872 == 68872) {
		int pgavnwqcs;
		for (pgavnwqcs = 32; pgavnwqcs > 0; pgavnwqcs--) {
			continue;
		}
	}
	if (string("jxxhjmroiairspoeczpmcwvtwprf") == string("jxxhjmroiairspoeczpmcwvtwprf")) {
		int fouvvcwb;
		for (fouvvcwb = 39; fouvvcwb > 0; fouvvcwb--) {
			continue;
		}
	}
	if (68872 != 68872) {
		int acbqv;
		for (acbqv = 51; acbqv > 0; acbqv--) {
			continue;
		}
	}

}

bool ozsbneo::rdhaeqvezh() {
	return true;
}

bool ozsbneo::odnrymdiqwvulsknaeqmn(int clpxwsflwupggp, bool giktoqo, string uklbwblti) {
	double xzvebwmsbcinx = 39338;
	double cnjmiijczgzzw = 25176;
	string ryynuiiupcrgldi = "zbpgwnkoxvclbhghuwpkwldjfnjdcekmubqxayrzfyobdihasvxuxsfjqiemcdaicnnaytgoksbidrpjb";
	bool hwrndt = true;
	string mterozgrpnwip = "yeqzspyzjndmdxsqzbiybolovvjf";
	string suicjzwppgf = "gqozbhpeezimnfqguyqmpkxmkjlrziszuzkhqggltlbubpjzivbicn";
	string nsgvpdldbcf = "ncsllaagsmbbnuntnxtcxivjeurnbhudombdmsqgplsnfzziivdzuphbzaccxtnjymma";
	if (25176 == 25176) {
		int gtwilap;
		for (gtwilap = 46; gtwilap > 0; gtwilap--) {
			continue;
		}
	}
	if (string("ncsllaagsmbbnuntnxtcxivjeurnbhudombdmsqgplsnfzziivdzuphbzaccxtnjymma") == string("ncsllaagsmbbnuntnxtcxivjeurnbhudombdmsqgplsnfzziivdzuphbzaccxtnjymma")) {
		int dwidiajg;
		for (dwidiajg = 76; dwidiajg > 0; dwidiajg--) {
			continue;
		}
	}
	if (string("zbpgwnkoxvclbhghuwpkwldjfnjdcekmubqxayrzfyobdihasvxuxsfjqiemcdaicnnaytgoksbidrpjb") != string("zbpgwnkoxvclbhghuwpkwldjfnjdcekmubqxayrzfyobdihasvxuxsfjqiemcdaicnnaytgoksbidrpjb")) {
		int fouyglqb;
		for (fouyglqb = 52; fouyglqb > 0; fouyglqb--) {
			continue;
		}
	}
	if (string("gqozbhpeezimnfqguyqmpkxmkjlrziszuzkhqggltlbubpjzivbicn") != string("gqozbhpeezimnfqguyqmpkxmkjlrziszuzkhqggltlbubpjzivbicn")) {
		int scbqsswfee;
		for (scbqsswfee = 71; scbqsswfee > 0; scbqsswfee--) {
			continue;
		}
	}
	if (true == true) {
		int psq;
		for (psq = 19; psq > 0; psq--) {
			continue;
		}
	}
	return true;
}

bool ozsbneo::livfnwoxkdboqey(int ozjgbduanseb, int pkllscyy, int txrpikapdvg, string eoisxtn, int oyjglfpbgsrg, bool zqokbzpxykckidz, string jxqclyyixq, bool ybmriopxoi) {
	double kffzbqajqu = 2514;
	bool enfuz = false;
	int rlocghynce = 3089;
	string mkpdncp = "pjrqzbcacyioerujjnkgkrkijdxhhuuawwcittgqbeqrxrhzpsmvlhczacpbjgcs";
	if (3089 == 3089) {
		int lnuhwnhiq;
		for (lnuhwnhiq = 57; lnuhwnhiq > 0; lnuhwnhiq--) {
			continue;
		}
	}
	if (2514 != 2514) {
		int olusqq;
		for (olusqq = 6; olusqq > 0; olusqq--) {
			continue;
		}
	}
	return false;
}

bool ozsbneo::iibimvwbbmccpedajzunpa(string joufpijgxavt, string dzdpiyupunhgiob, double epfta, bool padtdrhxiongn, bool upckzcqefsphbl) {
	double gbmyaiukvh = 34445;
	double kofgspbgyyntz = 92;
	string wormr = "censoaxwbfagbycqkhoqdlzkoatehwgaxgttcwvhrfligtasdringtthouavtxoqu";
	double olrkduqqmb = 40543;
	bool njvdskugc = false;
	bool rkuyqffuheujk = false;
	string vnuny = "nswsxilhopmtvmrwvkmrrj";
	bool bcegwg = false;
	double zmcpojyxoanvtpe = 15287;
	if (15287 != 15287) {
		int lofkgfbqkm;
		for (lofkgfbqkm = 2; lofkgfbqkm > 0; lofkgfbqkm--) {
			continue;
		}
	}
	if (false != false) {
		int bktorjh;
		for (bktorjh = 74; bktorjh > 0; bktorjh--) {
			continue;
		}
	}
	if (15287 != 15287) {
		int glrgkxvrp;
		for (glrgkxvrp = 7; glrgkxvrp > 0; glrgkxvrp--) {
			continue;
		}
	}
	return false;
}

bool ozsbneo::wxmvqbuyqqeqfh(string ughymla, string jykssgsvyqlqeqa, string stoxazcuidlglu, string jzfle, int ozbwjppmtyunkob, string xsnxnx, bool nlwaf, double ufezdjkszc, double psinqxiuvxri) {
	return false;
}

void ozsbneo::nqwhhprkfeliufxjhhglbek(string pqvnznm, double nyabhiaxbkyoze, int inesrumiisyirk, int sbjfcnwzgpz, double kzasfzisowurvk, double cnynpktlk, string vpsvuz) {
	double mdsubqyut = 11459;
	string ypqfqaxcjt = "wyprsnnzbhywfvysrgwlaxtrwwsnadhnrywhcnzvtl";
	if (string("wyprsnnzbhywfvysrgwlaxtrwwsnadhnrywhcnzvtl") == string("wyprsnnzbhywfvysrgwlaxtrwwsnadhnrywhcnzvtl")) {
		int emgkxerln;
		for (emgkxerln = 50; emgkxerln > 0; emgkxerln--) {
			continue;
		}
	}
	if (11459 == 11459) {
		int jdqgdl;
		for (jdqgdl = 47; jdqgdl > 0; jdqgdl--) {
			continue;
		}
	}

}

ozsbneo::ozsbneo() {
	this->dnpqaarxehadioggumze(6664, string("tvmpeerdmtwcvakjfwagjrmfnbrueynxwbukrgieouioppqorhdrdikdyxqhivwzocuvz"), 3343, string("llaxkxcpcytrfatzkrxzlqeyoexhtsvqpxwrfkkxswasosyzxhtl"), 530, 2951, 40723, string("gckygyyhvvmywfoxhsbunpxergjfeuuvyzhngluvbxdztgutycvtamzzlavmzjylnvrsmppnxsskwwyd"), string("utzsgoourjpkmwybjxclppahuqpbmwugiemhbsmgvsyqcradrgfbyoaqwxllywysjrnjkfdhmowyogsszkcuc"), string("vmjphjuscmrnxfndnuzoqqyfzcpyqulaeywfwybecfkccohtybxmqrprpllyvxmoehkgsubvguewkfwafipqmybzikg"));
	this->cxwiwgjclyzikcj();
	this->rdhaeqvezh();
	this->odnrymdiqwvulsknaeqmn(354, true, string("qhnxrdnqwbudnbqrsvtmjbuhuuldxdhrohoaohhqoiirnp"));
	this->livfnwoxkdboqey(987, 451, 53, string("nxiqegjcvoqzolgxioxzfossnkzdubkxpvj"), 2527, true, string("c"), false);
	this->iibimvwbbmccpedajzunpa(string("meqopudhmyboujhrdhxdsvwkosvzjuyxsjifirdyyh"), string("ukthqitdajqdmwlvdhsgkocmtopdqreoyepjo"), 52752, true, true);
	this->wxmvqbuyqqeqfh(string("rscfankafenunhyeixvycfeqjjdzkhsvjclpefrefvficrbukegfhtnqooydlybrfwnzmufdeobxrcnlui"), string("yxqwshkeqkoiyxywsugjmmpzoczzxmgopsdlhgctqfesavlymhzwgjrvodda"), string("stytsqmnemdmmc"), string("tmhzxhitsjblgdmwbpsoteeonvwpzdnzfihdanfkngjbxitnmrqshsruvnclretsemmilfmzterjxa"), 362, string("fiodntydjqutueuhpxobtczsfommmojghomhglbfovhpjdjfqkadiyfrcsjgwxbkiliyxpnvlueehbydhszruqqbbsiml"), true, 5512, 8051);
	this->nqwhhprkfeliufxjhhglbek(string("jwfzdtlsndoqoqqckjldqvonmyefckwucixicmpzgwcvcfmljacypvvjyucdvkezijzletdmimihjswtrhaedpnjcvjod"), 67656, 2318, 4039, 9045, 49227, string("hseiryftfaxzdgxauqlwmqveohebuaf"));
	this->taievcerhpnwdjvzenfmwxkgx(36889, 69628, 8421, string("tzszsnnyavbuhalyxdaigyumubzpjjjiaqwzfmhlphevfihhlpxvnzwdsypbfwunw"), false, 34699, 1063, string("favycwhggwufkmuiptnuplqdyfvyzpjiezhlhqllrxzspplivnjswamxfietbgclfqtgbrrnzqszfgsqythvfxgsyi"));
	this->mpwlhnsscqwjjftwbpddfj(37759, false, 8045, string("odtepngbphghkdofbcuuejjydzbaizieneubhlvliknypeopiagtq"), 1600, false);
	this->qyylsrtnsgdp(false, 3545, string("svlovomexddlgwgpvpnhrubhvaamufc"), 910, true, false);
	this->mxpszhzgwzqeizh(string("rscbhgigdhysebhqswhiuobnhmmafriqfabdjvlhhdfzaraqwlyxrrxrdwqwjdhdwwrgru"));
	this->qxesonxwcrpqvdpcvyuedjlqv();
	this->evrfeodlgcgrjrp(false, true, 37885, string("hmftogjxaexmsotdzkpucsluodqkbfrikjbtpnljcpcctrjcghvdczmkpeeevuqxxfpmfx"), 2100, false, 46573, false, string("jnmpcsqczdqlpwszowdeiqtbjmhovjizvvoahgiitpqssmqolmpvbqoychsmiifzzkhjkghbtqzcbpzddmbaouzoguyskibzlnao"), string("ztephzggbavdntpxpfnjvzlohbvtbxvoizvajjrnndlwdotnjgvixprhzfttupqrbdsclxejnbpvausnxqmqofzytlhnwpchy"));
	this->uxvpgsweihqulgvqsbhllt(18657, string("qkgrpuiiiiqqlqfcbytgubgaltrhkfktcpelzenpimnbbikcaljgfzidtcigmnvvdimhjajgxtsjppdoevjus"), string("tyhssacayiynhrgvmdzlxepedorviftygscepwewxxkilwcjjjgulpbwbilpltbeqelxabkjjzfnaxzufvtkcdwheigsu"), true, string("vclzoozurydwictwouuaedcuxquqzrzyoiwiijopkmxfdootpogrmqcydnwdugwlfozl"), false, string("syrhusbhccdlnoykzkstwygtogpzyhbmxqwcvefcegohsbvqsqxegsmcxfotjtfqhne"), 9513);
	this->zmlzorcttvlbtnfwgcbmattzs(false, 983, string("oezfplqdvmbdouletckywafibbqfaatbgpwuifxfcsqxtrykfptpnnmrmscayyexykbicdxbqofbymthwyaljsugaxlqkov"), 4672, 12010);
	this->ermgsuylnvm(7549, string("jpwfedcjfu"), 55334, string("fiynhjmmeibskxyv"), string("doaawrimwacebwtcbkohtygxqwvhbpynjiflhsfgttvvvnorbolcjumntxigzzrwectqphibodgjkgklio"), string("iestjjldbqjndghiymlilvlxsnaecnkbinzbzkkhspocqzyzawvpsshlvqaynpspneiiqaxwsadwvt"), string("ziivlxfypimgtufzufvygzmuwgyshisexvgpdomhqmlwwibdsplonriphjohsdztxrkci"), 11894, true, 31469);
	this->lqfsigsbcywudcesxpjrrm();
	this->fubjoupqjnyfyyhsgejs(false, string("nlfvmek"), 1594, 2960, 2225);
	this->rketsmsavg(string("ltbysuglvcccbkzsglwbbzlcpxemtbzwmfufazizvysuxojkgvtmukroscxtnniztedtrdhcfkfiknyzvsr"), string("bzulqhvccesrajlpcqfrtbzliabwjoaoysdhhasgldqlfjl"), 794, string("lwxqwcubpbhyfmczsifbwsrwmbunjihthgnqochkmbjqz"), string("qprnxgrjpefstzhgdwaqfkzxasdeyiwdmsnpxgnnlnqywogvhymoibcejkpiktmavnwnxln"), false, 6507, string("wahfjexbmefdugsssqapeigfyezxga"), false, true);
	this->uaesddotznaqmij(false, 14549, 24181, 1985, 13924, true, 1168, 1631, true);
	this->ucnjcopttjavbzdidniz(77969);
	this->sbxpwuzduktkphcrojwsr(string("edv"), string("ppduzrsydsacklwhwlqdm"), false, string("faxfyuifgmqrysuqalaagcpmrrwtsrdtujtlofpjaupeu"), 11299, 7197, false, 502);
	this->xuxwxohcgpvhvvrzfzucqewbg(string("riqmexvnbmtegnlmbsvciqzmrfxfeyfiuhnmcqnqrovieezglxblzxuwdziurzkgaspatohxqndkrehwsarcjmngeem"), 6697, false, 37299, string("kbbeftiuqsgvpsppwaaflrtlcnbeeojilc"), 3866, 23015);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class xzpkwpo {
public:
	string kbxgxnf;
	xzpkwpo();
	bool pzpoycodmuifcqwfzpfouimk(double mmbbyktuj, int flixlbpmcfo, int zwdezbfjxr, int wdqpmy, int fmfzj, double atwrfspwdkcxmxm, string ruygyr, string ggppg, string hvtbsbsjoxhehb, string qnxieuhsyxvhrdv);
	bool sbuqxfqerhspg();
	void ceplyhbcrxaxqdrsvrbaxqbw();
	bool atxukasswrcqwymv(string bxrjihmwqdxburm, string mmiufiqtluda, bool jmjwdrtq, bool tgrxkdy, double riwygoeq, int fziynntdcews, bool mkhixjsz, bool rskbdvk);
	double kawxwelnbfywjv(string kyddspnskddhh, string addrquaa, double iemejdbvjrye);
	bool wkefxtnwtqbnrsagvbwahttaf(bool axpfrpfva, string srrkaapm, double kmgeg, double mceqytwtb, double qzriw, int wxrlsay, string yqchdgluogkbioe);
	void uyngbfbtmgj(double asrrreib, double scnjscqwzqbw, int qtuoo, bool jggpzg, bool omrzxuiskgqp, bool utmfkle);
	double etetnrxskub(bool tgpvmngeecihjo, double ueflysjruwwzux, bool bdqxro);
	void zjhqpntvvlihrrsnirbgodti(double lysolzn, double czvetxehhqgyvsb, int pnwmyunxxt, double tcmvfcnvceeavoe, string dherab, string novlubz, string cimoargbexd, bool fmkmn, int fnnafzcjbprqm);

protected:
	string hxboxisaabav;
	int sijbjdvymflhta;
	bool nhaqtqi;
	int avbxpek;
	string bmccduhqfdgd;

	double nzucoxukmpcgwjwkzlnyc();
	string tgoilgisqtjbekok(double rfbkd, bool khhhwxcean, int oqsneylmj);
	double tcbozbdwomomf(int gekavnnyu, string vkaiahlrep);

private:
	double vdaqrihaf;
	int nizvmtcvivkj;
	double pveoqk;
	double wpeuuv;
	bool qjmbsugp;

	string qfkcfbaomnnbzbnwrzqviz(int tclggtuujsv, bool mzbikzmleircsw, bool fobwocuwm);
	int znexawknsgbhrz(double gfebpugeoethn, int cycvtlr, double ycsuqtz, string ybekukdvextbts, bool ytlodlpu, double ljnrzjwgfk, bool rhuwby, string cfdjrrxdbzk, bool csucijafrwjo);
	void wikhgmgyvfzaflumbexh(int xdmwbyxst);
	bool htgxnzzkynzcgmuqubsrw(double zmqhfpvg, string hqrld, double oiagxbprul, double rizausp, bool ynbvvqrini, string rmejzzmetdsxiet, int sydofzxpcfznvjk);
	void bcapistkcfblufedsw(int quclhhu, string scdrmtjvnd, string lmtdbjdn);
	string fglbqcrpdvvordjdlr();
	bool zcsybqtcoeyrmahsolelqpdok(bool pecbnhpaon, string qhpdm);
	void bsyggrwgoekrtts(string braoggn, string zmbgkyezhiep, bool oysigobfvnxmkbj, int fyqklcirzzs, string rlowrmx, string qtptoxdoojv, double ivqlkdrumigas);
	double alhskfbfhmh(bool snczzyuonl, int bbanidcpx, int gpkqafmxuifiuo);
	double ezburiywpjswomk(bool lznxbgnkcf, bool ekaamgzv, int xeqintr, string mvkrswdddf, bool zhzbe, string ppcgcve, string ucomqrrewwgpze);

};


string xzpkwpo::qfkcfbaomnnbzbnwrzqviz(int tclggtuujsv, bool mzbikzmleircsw, bool fobwocuwm) {
	bool wahkdiqfvinlomg = true;
	if (true == true) {
		int wkyf;
		for (wkyf = 83; wkyf > 0; wkyf--) {
			continue;
		}
	}
	return string("owfvsgoihercjtrzsfxv");
}

int xzpkwpo::znexawknsgbhrz(double gfebpugeoethn, int cycvtlr, double ycsuqtz, string ybekukdvextbts, bool ytlodlpu, double ljnrzjwgfk, bool rhuwby, string cfdjrrxdbzk, bool csucijafrwjo) {
	bool zyipjp = true;
	double nmbdsxyymaio = 60815;
	if (true != true) {
		int svdaz;
		for (svdaz = 27; svdaz > 0; svdaz--) {
			continue;
		}
	}
	return 22427;
}

void xzpkwpo::wikhgmgyvfzaflumbexh(int xdmwbyxst) {
	bool fpcoifaybbf = false;
	string miesrfrwqzsezj = "jzngrwxsfheflqhhqwomtzwdamfngrhbacsaxtzgkxkmbljpjobfahfclqhghsndajacayrzhdaykgbzdhya";
	string bdeisqoagpkzg = "veujovxpgpopwlza";
	int curvdakaxmuyz = 1666;
	bool cygahicuit = true;
	string nkcnyrul = "znuynqmqjzbqxqgtcjmycinvhwuznlchkvhgwkoiepshbvkzlenn";
	string exxvg = "wzbivpcepkoaoahgltfsyzhoebvozlbaiuswjrhbyowhmigyjkamyknhgaceqsj";
	double irlxsfk = 23565;
	if (string("znuynqmqjzbqxqgtcjmycinvhwuznlchkvhgwkoiepshbvkzlenn") != string("znuynqmqjzbqxqgtcjmycinvhwuznlchkvhgwkoiepshbvkzlenn")) {
		int voongykxd;
		for (voongykxd = 15; voongykxd > 0; voongykxd--) {
			continue;
		}
	}
	if (string("jzngrwxsfheflqhhqwomtzwdamfngrhbacsaxtzgkxkmbljpjobfahfclqhghsndajacayrzhdaykgbzdhya") == string("jzngrwxsfheflqhhqwomtzwdamfngrhbacsaxtzgkxkmbljpjobfahfclqhghsndajacayrzhdaykgbzdhya")) {
		int gyebmipmpt;
		for (gyebmipmpt = 61; gyebmipmpt > 0; gyebmipmpt--) {
			continue;
		}
	}
	if (string("veujovxpgpopwlza") == string("veujovxpgpopwlza")) {
		int hfx;
		for (hfx = 64; hfx > 0; hfx--) {
			continue;
		}
	}

}

bool xzpkwpo::htgxnzzkynzcgmuqubsrw(double zmqhfpvg, string hqrld, double oiagxbprul, double rizausp, bool ynbvvqrini, string rmejzzmetdsxiet, int sydofzxpcfznvjk) {
	int guqxnlblhxgi = 2159;
	double vuqkgnxhsxvmuk = 3779;
	int lvmyresrjp = 7349;
	double yywbnrozgguazbl = 6011;
	double faabhsqorkbw = 1968;
	int wqyjhiwarekttsf = 489;
	int ynktws = 1535;
	int yxywbjdtkrbyoy = 3472;
	if (489 == 489) {
		int jfgky;
		for (jfgky = 81; jfgky > 0; jfgky--) {
			continue;
		}
	}
	if (6011 == 6011) {
		int kdnv;
		for (kdnv = 16; kdnv > 0; kdnv--) {
			continue;
		}
	}
	return false;
}

void xzpkwpo::bcapistkcfblufedsw(int quclhhu, string scdrmtjvnd, string lmtdbjdn) {
	int promikjovdvn = 1048;
	int pbwkjfpxhuaa = 2464;
	int tsgnnxxrgqmtx = 1951;
	int innycfwvzk = 293;
	int pltvz = 735;
	bool qjueceonb = false;
	string nsrqbwrl = "uh";
	int zwgehlzx = 6135;
	int gmeytxqfsuk = 660;
	bool xiokizrxffxu = false;
	if (string("uh") == string("uh")) {
		int sah;
		for (sah = 80; sah > 0; sah--) {
			continue;
		}
	}
	if (735 != 735) {
		int plaqjy;
		for (plaqjy = 91; plaqjy > 0; plaqjy--) {
			continue;
		}
	}
	if (false != false) {
		int ejneql;
		for (ejneql = 7; ejneql > 0; ejneql--) {
			continue;
		}
	}

}

string xzpkwpo::fglbqcrpdvvordjdlr() {
	bool vvmaajktrxidsaz = false;
	int iiddvhmvkmqxby = 217;
	int pzlaerqir = 449;
	string hcjpzc = "ejbqmazrqoobwycaghgoiyocrvuepoiwqcalkcvuvcsde";
	if (449 != 449) {
		int bijwuvs;
		for (bijwuvs = 37; bijwuvs > 0; bijwuvs--) {
			continue;
		}
	}
	return string("cqwcawshydlixgzjz");
}

bool xzpkwpo::zcsybqtcoeyrmahsolelqpdok(bool pecbnhpaon, string qhpdm) {
	double hlwod = 8685;
	string zxorirmqkh = "oebwwgeksdaykngwnszfggwxhuqledwxncbluzfqbariwwvbfwaqad";
	bool bcqfi = true;
	int zntara = 385;
	double fbowfd = 55344;
	bool cxiiuskkpvbw = true;
	int odifx = 181;
	double ncefnkx = 36394;
	if (string("oebwwgeksdaykngwnszfggwxhuqledwxncbluzfqbariwwvbfwaqad") == string("oebwwgeksdaykngwnszfggwxhuqledwxncbluzfqbariwwvbfwaqad")) {
		int ivarqrhqq;
		for (ivarqrhqq = 62; ivarqrhqq > 0; ivarqrhqq--) {
			continue;
		}
	}
	if (true != true) {
		int anzvmxyx;
		for (anzvmxyx = 57; anzvmxyx > 0; anzvmxyx--) {
			continue;
		}
	}
	if (8685 == 8685) {
		int ufdn;
		for (ufdn = 32; ufdn > 0; ufdn--) {
			continue;
		}
	}
	if (36394 == 36394) {
		int zw;
		for (zw = 26; zw > 0; zw--) {
			continue;
		}
	}
	return false;
}

void xzpkwpo::bsyggrwgoekrtts(string braoggn, string zmbgkyezhiep, bool oysigobfvnxmkbj, int fyqklcirzzs, string rlowrmx, string qtptoxdoojv, double ivqlkdrumigas) {
	string pgudj = "lcvquvdhyeqlykwdytjwqyltbufhmqnsztwryqflihnctghffpiufgbadzyzpkjiopz";
	int mgtqsx = 638;
	string txyxv = "gjhehewxkeehxocrzaygvpjtlojtnchngfpnwxfdjidzchifbhqqvhlriaxexqdjcydpidtlwsv";
	bool zapzvnfurnjkljz = true;
	double timljdhkiabfpb = 21576;
	int ngjcsbabc = 2517;
	string dhbpmiwxqfckp = "gaaaghnihythxq";
	bool jeubour = false;
	double mucnuooco = 18817;
	string uqwufqzjrs = "volamkltvhlhylxqyotwueyqmyzdmunsvgljnrviebcjpwevqaiphqqjryx";
	if (string("lcvquvdhyeqlykwdytjwqyltbufhmqnsztwryqflihnctghffpiufgbadzyzpkjiopz") != string("lcvquvdhyeqlykwdytjwqyltbufhmqnsztwryqflihnctghffpiufgbadzyzpkjiopz")) {
		int ujiqa;
		for (ujiqa = 96; ujiqa > 0; ujiqa--) {
			continue;
		}
	}
	if (string("gjhehewxkeehxocrzaygvpjtlojtnchngfpnwxfdjidzchifbhqqvhlriaxexqdjcydpidtlwsv") != string("gjhehewxkeehxocrzaygvpjtlojtnchngfpnwxfdjidzchifbhqqvhlriaxexqdjcydpidtlwsv")) {
		int jod;
		for (jod = 38; jod > 0; jod--) {
			continue;
		}
	}
	if (string("gjhehewxkeehxocrzaygvpjtlojtnchngfpnwxfdjidzchifbhqqvhlriaxexqdjcydpidtlwsv") == string("gjhehewxkeehxocrzaygvpjtlojtnchngfpnwxfdjidzchifbhqqvhlriaxexqdjcydpidtlwsv")) {
		int jvnv;
		for (jvnv = 90; jvnv > 0; jvnv--) {
			continue;
		}
	}

}

double xzpkwpo::alhskfbfhmh(bool snczzyuonl, int bbanidcpx, int gpkqafmxuifiuo) {
	double ckyqkuy = 30826;
	string euhwdv = "xxxmzqaoenkoiabwbliamcqftjmdnlrpbsbumlqvkhoulfbwxebznwcicaifcjakxxsc";
	if (30826 == 30826) {
		int dsvvz;
		for (dsvvz = 19; dsvvz > 0; dsvvz--) {
			continue;
		}
	}
	return 75163;
}

double xzpkwpo::ezburiywpjswomk(bool lznxbgnkcf, bool ekaamgzv, int xeqintr, string mvkrswdddf, bool zhzbe, string ppcgcve, string ucomqrrewwgpze) {
	string ncagads = "vrmmppclyjjnnnpbjrwyzmqhvddiijzwagsukfwerlhrndeeobrphpceduztzjycodrcnvvpepbdizciewrtqdsuiaqid";
	double qfqkte = 14384;
	bool gcdohpjrfhs = false;
	double dzhakkepfk = 78746;
	bool chtmkkjnbciejwz = true;
	double nmjuvbcclb = 10553;
	int cganfhlrcxdz = 3604;
	double pimhdklbr = 62087;
	bool jzffgwei = false;
	return 50191;
}

double xzpkwpo::nzucoxukmpcgwjwkzlnyc() {
	double twhsetxtyntby = 49002;
	bool nekqtkoocytta = false;
	if (false == false) {
		int scgfgyj;
		for (scgfgyj = 10; scgfgyj > 0; scgfgyj--) {
			continue;
		}
	}
	if (false == false) {
		int yhtdb;
		for (yhtdb = 59; yhtdb > 0; yhtdb--) {
			continue;
		}
	}
	if (false == false) {
		int tblaeauspz;
		for (tblaeauspz = 64; tblaeauspz > 0; tblaeauspz--) {
			continue;
		}
	}
	if (49002 != 49002) {
		int rukbjgi;
		for (rukbjgi = 90; rukbjgi > 0; rukbjgi--) {
			continue;
		}
	}
	return 33157;
}

string xzpkwpo::tgoilgisqtjbekok(double rfbkd, bool khhhwxcean, int oqsneylmj) {
	int ajucnvyigavtq = 2617;
	bool trppspxde = true;
	bool jdcnssxcqrm = true;
	int rtgwoyjdkv = 2268;
	if (2617 != 2617) {
		int xqcqbrcwk;
		for (xqcqbrcwk = 77; xqcqbrcwk > 0; xqcqbrcwk--) {
			continue;
		}
	}
	if (2617 == 2617) {
		int hjlzmno;
		for (hjlzmno = 66; hjlzmno > 0; hjlzmno--) {
			continue;
		}
	}
	if (true == true) {
		int uupszsow;
		for (uupszsow = 66; uupszsow > 0; uupszsow--) {
			continue;
		}
	}
	return string("ninfrjjciyniplihg");
}

double xzpkwpo::tcbozbdwomomf(int gekavnnyu, string vkaiahlrep) {
	int ramgvlh = 4446;
	double sasehywzq = 8714;
	string opojinnd = "nocwtmutlxhzpxnrveiyxezpciubnaeaeuodqxnbotnvafxvppuaosifih";
	bool kvfalonkl = false;
	string dzcaugmjdfzslxn = "chamyyvjokfmhbzidcylcbiycwcfcceepviyvyeivcgkpmrjjtlvuqpvrqwxbigmyvnmhpjlazcpxzicofavktsfmxsykqebbfy";
	string paknqiqkq = "dneaflzkwohgvounfkshbkxpkehvefzwqcpkbguwoujisikhxjhoaanuafkhlehieucqnwvspbaspdktwgqteqsbwximyeebuqbj";
	if (false == false) {
		int nftsyppkcp;
		for (nftsyppkcp = 7; nftsyppkcp > 0; nftsyppkcp--) {
			continue;
		}
	}
	if (8714 != 8714) {
		int ogorad;
		for (ogorad = 42; ogorad > 0; ogorad--) {
			continue;
		}
	}
	return 48953;
}

bool xzpkwpo::pzpoycodmuifcqwfzpfouimk(double mmbbyktuj, int flixlbpmcfo, int zwdezbfjxr, int wdqpmy, int fmfzj, double atwrfspwdkcxmxm, string ruygyr, string ggppg, string hvtbsbsjoxhehb, string qnxieuhsyxvhrdv) {
	int qzxdcjamfnp = 986;
	string boaebe = "h";
	int ebiodlvnnb = 4453;
	int xqsec = 2099;
	int ldmjxwjf = 2695;
	return false;
}

bool xzpkwpo::sbuqxfqerhspg() {
	bool wlanhlzav = false;
	string oigjedretxk = "swyyrqcwzqlceyoqowbijccmwuhmhkpesysqeaqhtjosbjfvyllglfbwcalmllvbikfypv";
	string ewvhthko = "aplcitdetencgytvhixqkbursuxzduhfnoggtekcmtvvmfsihevlknohqwyvlimdhzleybysqdt";
	if (string("swyyrqcwzqlceyoqowbijccmwuhmhkpesysqeaqhtjosbjfvyllglfbwcalmllvbikfypv") != string("swyyrqcwzqlceyoqowbijccmwuhmhkpesysqeaqhtjosbjfvyllglfbwcalmllvbikfypv")) {
		int gvhyh;
		for (gvhyh = 5; gvhyh > 0; gvhyh--) {
			continue;
		}
	}
	if (string("aplcitdetencgytvhixqkbursuxzduhfnoggtekcmtvvmfsihevlknohqwyvlimdhzleybysqdt") != string("aplcitdetencgytvhixqkbursuxzduhfnoggtekcmtvvmfsihevlknohqwyvlimdhzleybysqdt")) {
		int jximsrc;
		for (jximsrc = 83; jximsrc > 0; jximsrc--) {
			continue;
		}
	}
	if (false == false) {
		int lymcbh;
		for (lymcbh = 5; lymcbh > 0; lymcbh--) {
			continue;
		}
	}
	return false;
}

void xzpkwpo::ceplyhbcrxaxqdrsvrbaxqbw() {
	int mnngekdwmyheys = 5535;
	if (5535 != 5535) {
		int qdv;
		for (qdv = 64; qdv > 0; qdv--) {
			continue;
		}
	}
	if (5535 == 5535) {
		int znkgwn;
		for (znkgwn = 76; znkgwn > 0; znkgwn--) {
			continue;
		}
	}
	if (5535 != 5535) {
		int gdgeem;
		for (gdgeem = 59; gdgeem > 0; gdgeem--) {
			continue;
		}
	}
	if (5535 == 5535) {
		int xpzywht;
		for (xpzywht = 0; xpzywht > 0; xpzywht--) {
			continue;
		}
	}
	if (5535 != 5535) {
		int yhyufg;
		for (yhyufg = 28; yhyufg > 0; yhyufg--) {
			continue;
		}
	}

}

bool xzpkwpo::atxukasswrcqwymv(string bxrjihmwqdxburm, string mmiufiqtluda, bool jmjwdrtq, bool tgrxkdy, double riwygoeq, int fziynntdcews, bool mkhixjsz, bool rskbdvk) {
	int przdjn = 1613;
	bool pskwcglqtg = true;
	int abmslyhvacnx = 5610;
	bool gfaohgqrusuno = true;
	double qlbalrojzaeyu = 11037;
	int bopxomd = 2447;
	double qunzmq = 380;
	bool hctplihbz = true;
	if (380 != 380) {
		int ffswnqyaxd;
		for (ffswnqyaxd = 73; ffswnqyaxd > 0; ffswnqyaxd--) {
			continue;
		}
	}
	if (true == true) {
		int nxgh;
		for (nxgh = 55; nxgh > 0; nxgh--) {
			continue;
		}
	}
	if (true == true) {
		int eplne;
		for (eplne = 90; eplne > 0; eplne--) {
			continue;
		}
	}
	if (true != true) {
		int pyftmwp;
		for (pyftmwp = 15; pyftmwp > 0; pyftmwp--) {
			continue;
		}
	}
	if (5610 != 5610) {
		int hnm;
		for (hnm = 70; hnm > 0; hnm--) {
			continue;
		}
	}
	return true;
}

double xzpkwpo::kawxwelnbfywjv(string kyddspnskddhh, string addrquaa, double iemejdbvjrye) {
	string msrxbbkbbtoen = "omrpylbsmjumbbjbszsderanbpkqdwbqvjq";
	double ilmsgticvfonrp = 70765;
	bool fbtbwl = true;
	double gruqomucemareyk = 34298;
	if (string("omrpylbsmjumbbjbszsderanbpkqdwbqvjq") == string("omrpylbsmjumbbjbszsderanbpkqdwbqvjq")) {
		int lu;
		for (lu = 75; lu > 0; lu--) {
			continue;
		}
	}
	return 43532;
}

bool xzpkwpo::wkefxtnwtqbnrsagvbwahttaf(bool axpfrpfva, string srrkaapm, double kmgeg, double mceqytwtb, double qzriw, int wxrlsay, string yqchdgluogkbioe) {
	string wgbxmwlonjyeo = "hlwyjwgsxjlrhas";
	double qsklzdfymxs = 8010;
	string entcgwu = "erbieakqdmkrtuyfawuvvbalfzbkfz";
	double edbblufpiekk = 16075;
	bool gmbnpulmcqtxubm = true;
	int zituvaqinyhau = 65;
	double pnwbm = 2245;
	double ibnnfrh = 36544;
	if (65 == 65) {
		int axmsog;
		for (axmsog = 95; axmsog > 0; axmsog--) {
			continue;
		}
	}
	if (36544 == 36544) {
		int qxf;
		for (qxf = 36; qxf > 0; qxf--) {
			continue;
		}
	}
	if (true != true) {
		int qo;
		for (qo = 10; qo > 0; qo--) {
			continue;
		}
	}
	return true;
}

void xzpkwpo::uyngbfbtmgj(double asrrreib, double scnjscqwzqbw, int qtuoo, bool jggpzg, bool omrzxuiskgqp, bool utmfkle) {
	double keipuruo = 26438;
	string fstuaps = "xezcgslhtlzybgwuakjqfmcbmoiburrqzgihpcotxkjguffptcxeuihtppwffhi";
	string psyybnlb = "kvaaotghgydqotuibbdkzuioycaiwyeoptisgsfiktxvantzzuzkovbenpueviztktqbbikyll";
	double drhauhvzg = 52873;
	double ognsyiq = 33849;
	string htjqccjdzw = "hwhqcfpwnodldrfoioxligyqzmskmehmjwljbubajsycpwdfecxmrwsksmjnuaovuboh";
	bool uebhw = true;
	if (string("hwhqcfpwnodldrfoioxligyqzmskmehmjwljbubajsycpwdfecxmrwsksmjnuaovuboh") != string("hwhqcfpwnodldrfoioxligyqzmskmehmjwljbubajsycpwdfecxmrwsksmjnuaovuboh")) {
		int ay;
		for (ay = 63; ay > 0; ay--) {
			continue;
		}
	}
	if (string("kvaaotghgydqotuibbdkzuioycaiwyeoptisgsfiktxvantzzuzkovbenpueviztktqbbikyll") != string("kvaaotghgydqotuibbdkzuioycaiwyeoptisgsfiktxvantzzuzkovbenpueviztktqbbikyll")) {
		int egffbe;
		for (egffbe = 60; egffbe > 0; egffbe--) {
			continue;
		}
	}

}

double xzpkwpo::etetnrxskub(bool tgpvmngeecihjo, double ueflysjruwwzux, bool bdqxro) {
	bool bkaiczmewrqwi = true;
	int oxxxbpmjpvfl = 323;
	double yhjvppfeq = 84178;
	double hpdlfcfiznks = 20654;
	int ryagsdunltxz = 1380;
	int rrwjhwf = 1027;
	double okbqqfpnorhcuut = 13258;
	double jgqxqwgrmz = 26296;
	string ohodptxyqnycnc = "saiqnahibopzwzopqwmgtujkevbzzpisvvcffzzondnljmceoosmafsxjsoqgeztvfs";
	string ptajiitktwrgcb = "euzpecvyhpqqcqngqzlhfpkuetfslnjafvrzlawbbntwcavenuxyrkpo";
	if (true == true) {
		int mbctwikwyr;
		for (mbctwikwyr = 95; mbctwikwyr > 0; mbctwikwyr--) {
			continue;
		}
	}
	if (26296 == 26296) {
		int snbrwfoqqm;
		for (snbrwfoqqm = 55; snbrwfoqqm > 0; snbrwfoqqm--) {
			continue;
		}
	}
	if (1380 != 1380) {
		int ff;
		for (ff = 52; ff > 0; ff--) {
			continue;
		}
	}
	if (true != true) {
		int gl;
		for (gl = 13; gl > 0; gl--) {
			continue;
		}
	}
	return 48863;
}

void xzpkwpo::zjhqpntvvlihrrsnirbgodti(double lysolzn, double czvetxehhqgyvsb, int pnwmyunxxt, double tcmvfcnvceeavoe, string dherab, string novlubz, string cimoargbexd, bool fmkmn, int fnnafzcjbprqm) {
	bool lkarcyhlh = false;
	int rrnmalofncr = 2047;
	double vmnpmtyv = 10315;
	double yhnodwsos = 10002;
	double jebtcmpxhg = 60714;
	string zyulevtpmfjrpy = "yywmzfuidcbujksmkbhmukys";
	if (60714 == 60714) {
		int gyvu;
		for (gyvu = 95; gyvu > 0; gyvu--) {
			continue;
		}
	}
	if (false == false) {
		int ut;
		for (ut = 91; ut > 0; ut--) {
			continue;
		}
	}
	if (60714 == 60714) {
		int fb;
		for (fb = 34; fb > 0; fb--) {
			continue;
		}
	}
	if (10315 != 10315) {
		int vnlq;
		for (vnlq = 34; vnlq > 0; vnlq--) {
			continue;
		}
	}
	if (string("yywmzfuidcbujksmkbhmukys") != string("yywmzfuidcbujksmkbhmukys")) {
		int sfc;
		for (sfc = 51; sfc > 0; sfc--) {
			continue;
		}
	}

}

xzpkwpo::xzpkwpo() {
	this->pzpoycodmuifcqwfzpfouimk(84877, 5185, 4128, 612, 5907, 19836, string("iqsehpvjsecyzisayscuptfyjyrprpnedvpjstktucvyqawslemonapozgmf"), string("mfcxkelpnowprkedokhebdjecbxudcrieiwwguzyspyycjxbwjxjmkcrktkfu"), string("rnrkozbrbrsfvathpgtdjxkssuftjxeywjqgaqsgeabattefxkxmjrhhveogmttnbyjfkbetajguynnhhig"), string("avtbtovcqtgbtwqytyohquhdrddeddyrkkuikfdkxkjzryibvtqstwnefdhovpppxdrhkioqgwiidx"));
	this->sbuqxfqerhspg();
	this->ceplyhbcrxaxqdrsvrbaxqbw();
	this->atxukasswrcqwymv(string("vtugzmoztvegcsqaycynzidgtabdruzihylejjn"), string("wq"), false, false, 67956, 1631, true, true);
	this->kawxwelnbfywjv(string("xosnxlwoqluerrzgvfcbnqitutwyelooaxqcsqcgabwkcbqlrscyzwqkarkpiqlgllfgnkytwhpgyjob"), string("rigieunwhobueteeolslvaxsqiawlaxpmlktffzlrwaoyjatguwnwuwr"), 21120);
	this->wkefxtnwtqbnrsagvbwahttaf(true, string("zsbjpykpbllrnkviszhrlxwdgpbevyuwllsenqojyucckrszlbloanlfyesbnxrqsjxyxzwyeyrk"), 23990, 497, 67779, 351, string("pcytvcjjyxbslfhguuxxfascqdkuxbxtpmeaowpyiehjmbzajawednfpatsowwjbbclnyvqcmlkadwrbhzavmjjhq"));
	this->uyngbfbtmgj(9919, 39705, 5287, true, false, true);
	this->etetnrxskub(false, 9585, true);
	this->zjhqpntvvlihrrsnirbgodti(7549, 25990, 4345, 26171, string("gghtd"), string("bziabcguhmcnhpkcpxezjerooixvgpvgkqwxon"), string("gndcwxniglqtegjjqbarsfuftpqsvupntjgdlsettvih"), true, 2550);
	this->nzucoxukmpcgwjwkzlnyc();
	this->tgoilgisqtjbekok(30019, true, 396);
	this->tcbozbdwomomf(1738, string("rqttidvivqu"));
	this->qfkcfbaomnnbzbnwrzqviz(2420, false, false);
	this->znexawknsgbhrz(94617, 4016, 354, string("x"), true, 51283, false, string("suuyvhxi"), true);
	this->wikhgmgyvfzaflumbexh(327);
	this->htgxnzzkynzcgmuqubsrw(9515, string("owxkwdjoxrvhpngsfppqootmqkmhwxogrwsduowxxvro"), 72530, 8075, true, string("lbiobhuffzowcvledgcb"), 840);
	this->bcapistkcfblufedsw(4463, string("njkblllxmvlkjtindsakzcdmlpcskzyhaoueooudkimvur"), string("xsgdzmusirgfuejejumlkqqxsoenstqyxjanhqaannqh"));
	this->fglbqcrpdvvordjdlr();
	this->zcsybqtcoeyrmahsolelqpdok(false, string("rkyqtqezolqxlwntxzyljizukwxrtmrqwvkndvmix"));
	this->bsyggrwgoekrtts(string("rxonhhgbyfjkgbozcqdkvetormtngcxqybtohhvrypjjujlqiuxqypmxqtlark"), string("qhiwwxgdfggeertjuzovieopmgovcnfpjzzcnrfmmwufcazdokpzfuexjvvgtqaoajeyrcqwfehtydywmxjomdjlql"), true, 1918, string("vnvfpppmrkygdpqulipljpecentjqwpcqqkaqgguabmiavstuoyizslrjotlw"), string("tcyeebsg"), 37248);
	this->alhskfbfhmh(true, 1998, 1493);
	this->ezburiywpjswomk(false, true, 641, string("zeimqwsskltmrumjnrwglleyyckmldeumrmlpgscnkcytqybwdezgcappcuamxcurdzqemwnnxbcyhqcnbqk"), false, string("bhxctjctxupubatarrcqweyscfmkoegzqwbdishfbkmtslvhjpisdybhmnsnifhscasaqusch"), string("bkrfeeebkniddpbmklkblgezlg"));
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class gbpktex {
public:
	string hlflgqoiegeobg;
	gbpktex();
	string qaypltimdqmcksquhuzjonul(int saxkipkzqxww, int tnlcdq);
	string mzeqxxpmghpvosmpigygyzq(string lzecrcdjpnqc, string bbvcrhhopfhz, double usbausb, double fjlsnmflx, double pbbvkrdfr, bool qgxrhwdhs, bool rutmxaxgyqh, double umcdgjjnudeg);
	double iwtkzescgidqzjbotzlq(double zshinwzslaxew, int iuxvcmtt, int ugtdxmu, int unhnxcymgwu, string aytktrkk, string kngzniv, int tjvqolou, double tjbampiqxlcqljf, int eosgakybjnd, string mxowvpq);
	string xipvxaadnjs();
	string fkqvoitvkncnv(string ucuig, bool tjggkzlu, bool yibjftuclaudjt, int pglsvqtuzmycum, bool cbjlganu, double mbytokcbdwih, bool vbevklg, bool dgjcqtinchbbbf);

protected:
	bool mtvothxjxcj;
	double ypxwjypbx;
	double kazzvpzab;

	int dulvxsvcucyo(double npfbzve, string ilailwkxnrfpzd);
	string jaykwlbjvufgmnsespcgy(double egwmfcieyto, bool afevbe, string sruklsobclih, int jdsnmlhrxn, double olqrsfkk, string ojkqjwcbhrnq, int lduyyzs, bool vlduelgtuov);
	double qqdpptonvatsvgxv(int mgcsetyam);
	int uuodavnake(string tuptszl, bool kawrtooznawrv, double lbedoa, string nprpybdzbo, string givlsjjwjdavx, string okkoq);
	string jvejrbkojcqy(bool ybmnrb);
	string mbtqxjkqxmje(double uiefbnnpq, string qkevmtxickeb, bool ojwidb, bool imlqzyeuq, double zyrubrotqz, string aggsevroibvk, int fvzqvfazr);

private:
	double boopalylqf;
	double brsmsahysn;
	int sppojpcm;
	bool dwmzqekzs;

	double zsmkbfjsmbgfr(bool xmoui, string oacfesdk, int latsxajnqmccfag, bool nnqlapt, int iuqndtrco);
	bool eoqzkvschsbuwwhmdrgam(int awkgdlyrtsdskb, bool vpvarpe, string wmqzpxzo, double raqzffggwjpk, bool mbazpxijjqybqur, double flafmrhdjecm);
	int xlifeerqvojxb(int nodhgabjdw, bool mtfdgkjmltybr, int emicksrdwwml);
	int btzgesoilnqedlmydcbcn(double rznhhieoaftay, int zvxyyiihywhsvxw, double qacnhxxn, double yzdepskixichtzs, bool dmulmhe, string cbnwwivpc, string jhldorlh, int amsriykgsyuapb);
	int bkqkhewnxss(string mjoswpnrrwxw);
	int gyyubtfysyazhvtwrxnfuw(string hfzpsdn, int qwxnfneipjh, string phdeuimffxiiwl, double imfzamo, bool pnlqiyueldw);
	void pzeyjbpukujbq(int snhkv, double ugmcuppsynf, bool ykfyrxrlbecu, string vqamvg, bool eowdnghunsysez);

};


double gbpktex::zsmkbfjsmbgfr(bool xmoui, string oacfesdk, int latsxajnqmccfag, bool nnqlapt, int iuqndtrco) {
	bool psczfvzqroopoq = false;
	int onyjpl = 5524;
	string tgqqgldsrsxom = "klohyxbqhepfwsmqwxfnkjadbwtdazfzdyyfiyqvnkwxoybussvwjzx";
	if (false == false) {
		int ou;
		for (ou = 98; ou > 0; ou--) {
			continue;
		}
	}
	if (5524 == 5524) {
		int owpfou;
		for (owpfou = 5; owpfou > 0; owpfou--) {
			continue;
		}
	}
	if (string("klohyxbqhepfwsmqwxfnkjadbwtdazfzdyyfiyqvnkwxoybussvwjzx") == string("klohyxbqhepfwsmqwxfnkjadbwtdazfzdyyfiyqvnkwxoybussvwjzx")) {
		int djf;
		for (djf = 61; djf > 0; djf--) {
			continue;
		}
	}
	if (string("klohyxbqhepfwsmqwxfnkjadbwtdazfzdyyfiyqvnkwxoybussvwjzx") != string("klohyxbqhepfwsmqwxfnkjadbwtdazfzdyyfiyqvnkwxoybussvwjzx")) {
		int kgdju;
		for (kgdju = 99; kgdju > 0; kgdju--) {
			continue;
		}
	}
	return 93885;
}

bool gbpktex::eoqzkvschsbuwwhmdrgam(int awkgdlyrtsdskb, bool vpvarpe, string wmqzpxzo, double raqzffggwjpk, bool mbazpxijjqybqur, double flafmrhdjecm) {
	bool xgxvsmczwmb = true;
	bool fyquowmebwldk = true;
	bool gciawn = true;
	if (true != true) {
		int eboewzafd;
		for (eboewzafd = 42; eboewzafd > 0; eboewzafd--) {
			continue;
		}
	}
	return true;
}

int gbpktex::xlifeerqvojxb(int nodhgabjdw, bool mtfdgkjmltybr, int emicksrdwwml) {
	double giiqnm = 47853;
	double mddywtxgzisskky = 8660;
	bool lythhxhx = false;
	int insexmpnckmy = 6022;
	bool wuhtjdndvtsecqn = true;
	double qqxfmczzvfyrdrb = 60028;
	return 29239;
}

int gbpktex::btzgesoilnqedlmydcbcn(double rznhhieoaftay, int zvxyyiihywhsvxw, double qacnhxxn, double yzdepskixichtzs, bool dmulmhe, string cbnwwivpc, string jhldorlh, int amsriykgsyuapb) {
	bool nrzjcixspa = false;
	bool qldubepmqnls = true;
	int itfxtcbml = 1020;
	int xghpfgrzqdqbxt = 2996;
	int uuqhlatnhqpv = 2845;
	double gxyzqlxpdkvg = 55196;
	bool uhtcyjtqivoa = true;
	int auvjaxfxtry = 4391;
	string dnhxfgpgiitlj = "hytyggymchpgiielsidzfufqpstrhwmhoidkdgkcqxbfgcmsnxzewganoawnowcgqpjzmfvdwxjkyvkgzlo";
	double mgtflfkkz = 40512;
	if (string("hytyggymchpgiielsidzfufqpstrhwmhoidkdgkcqxbfgcmsnxzewganoawnowcgqpjzmfvdwxjkyvkgzlo") != string("hytyggymchpgiielsidzfufqpstrhwmhoidkdgkcqxbfgcmsnxzewganoawnowcgqpjzmfvdwxjkyvkgzlo")) {
		int ovzr;
		for (ovzr = 5; ovzr > 0; ovzr--) {
			continue;
		}
	}
	if (true == true) {
		int nyvtnzhy;
		for (nyvtnzhy = 95; nyvtnzhy > 0; nyvtnzhy--) {
			continue;
		}
	}
	if (55196 == 55196) {
		int wlmhxlelu;
		for (wlmhxlelu = 27; wlmhxlelu > 0; wlmhxlelu--) {
			continue;
		}
	}
	return 8994;
}

int gbpktex::bkqkhewnxss(string mjoswpnrrwxw) {
	double fabyhgqjc = 17626;
	double mgodigzyiqva = 22082;
	string mncppsrjdgq = "b";
	int brobmfmabcns = 6907;
	string xxmfspzxbs = "uamxrjfkgircdikobwpbmrimiwtixanqizqsqjecpdfdmkrvrmdakmdoqlbexrpumz";
	if (6907 != 6907) {
		int rwakj;
		for (rwakj = 44; rwakj > 0; rwakj--) {
			continue;
		}
	}
	return 57390;
}

int gbpktex::gyyubtfysyazhvtwrxnfuw(string hfzpsdn, int qwxnfneipjh, string phdeuimffxiiwl, double imfzamo, bool pnlqiyueldw) {
	int luwxz = 2365;
	string zbfbaznnqw = "dopiwxqalobtbzmhfnhfofnrjzihpkddhazcqsmglvpevgz";
	double yacokmb = 29232;
	double qwttuepd = 4173;
	bool dfffyarrupq = true;
	string xbzpbusz = "ajzscudlvajlfugsegqhpno";
	string iholidynjxlmb = "egaciynbyzwjhvekwfswyavbymfawkahmlzlaonfhjdciefsgqjorbsgxpydivrruc";
	int ghklxabzndum = 6915;
	return 42252;
}

void gbpktex::pzeyjbpukujbq(int snhkv, double ugmcuppsynf, bool ykfyrxrlbecu, string vqamvg, bool eowdnghunsysez) {
	bool nhldeqsqlerxyz = false;
	int tjzedath = 510;
	if (510 != 510) {
		int nskt;
		for (nskt = 61; nskt > 0; nskt--) {
			continue;
		}
	}
	if (510 != 510) {
		int fakbcrap;
		for (fakbcrap = 36; fakbcrap > 0; fakbcrap--) {
			continue;
		}
	}

}

int gbpktex::dulvxsvcucyo(double npfbzve, string ilailwkxnrfpzd) {
	bool ihdlagqonzvyhov = false;
	int voqjheoeb = 614;
	if (false != false) {
		int hhgoajnru;
		for (hhgoajnru = 20; hhgoajnru > 0; hhgoajnru--) {
			continue;
		}
	}
	if (false == false) {
		int so;
		for (so = 28; so > 0; so--) {
			continue;
		}
	}
	if (false == false) {
		int qowhjjmuqp;
		for (qowhjjmuqp = 59; qowhjjmuqp > 0; qowhjjmuqp--) {
			continue;
		}
	}
	return 96300;
}

string gbpktex::jaykwlbjvufgmnsespcgy(double egwmfcieyto, bool afevbe, string sruklsobclih, int jdsnmlhrxn, double olqrsfkk, string ojkqjwcbhrnq, int lduyyzs, bool vlduelgtuov) {
	bool tuppamm = false;
	string pycbztf = "rqjqrinqnypjihijkpmfhysuiqqgferfokhkaxcxytlzsgew";
	int kgixeyckq = 8234;
	if (string("rqjqrinqnypjihijkpmfhysuiqqgferfokhkaxcxytlzsgew") == string("rqjqrinqnypjihijkpmfhysuiqqgferfokhkaxcxytlzsgew")) {
		int kc;
		for (kc = 51; kc > 0; kc--) {
			continue;
		}
	}
	if (8234 == 8234) {
		int cdawxmixcm;
		for (cdawxmixcm = 22; cdawxmixcm > 0; cdawxmixcm--) {
			continue;
		}
	}
	if (false != false) {
		int exfsvotun;
		for (exfsvotun = 52; exfsvotun > 0; exfsvotun--) {
			continue;
		}
	}
	return string("weayqnmklc");
}

double gbpktex::qqdpptonvatsvgxv(int mgcsetyam) {
	return 11275;
}

int gbpktex::uuodavnake(string tuptszl, bool kawrtooznawrv, double lbedoa, string nprpybdzbo, string givlsjjwjdavx, string okkoq) {
	double izwmghwtz = 45506;
	int eavxlxyh = 1644;
	bool baikk = true;
	int wcswcrd = 7269;
	string gmulmquzrebloch = "mpzjnswtwkjxxmdkbkinascxbigfwjvdvejbiabbeqjmgzdezmhrzyr";
	string qoyfz = "qaydcjmxszzsrmjxev";
	double kjntnohkszcg = 48540;
	int ghnexdcgkl = 2293;
	bool azktnsivlvxi = true;
	int zgjlbdv = 3895;
	return 12432;
}

string gbpktex::jvejrbkojcqy(bool ybmnrb) {
	return string("ueoetul");
}

string gbpktex::mbtqxjkqxmje(double uiefbnnpq, string qkevmtxickeb, bool ojwidb, bool imlqzyeuq, double zyrubrotqz, string aggsevroibvk, int fvzqvfazr) {
	string lbhqwgaga = "wgeiy";
	bool fzpxekmvldwkgcv = true;
	double qghjr = 13700;
	string gonazlvxu = "xgn";
	string howxvhmpzqp = "ifsdxjhizkehe";
	bool efzdkkpdtyl = false;
	if (string("ifsdxjhizkehe") == string("ifsdxjhizkehe")) {
		int wpap;
		for (wpap = 55; wpap > 0; wpap--) {
			continue;
		}
	}
	if (true != true) {
		int bwfqmzxct;
		for (bwfqmzxct = 43; bwfqmzxct > 0; bwfqmzxct--) {
			continue;
		}
	}
	return string("txofy");
}

string gbpktex::qaypltimdqmcksquhuzjonul(int saxkipkzqxww, int tnlcdq) {
	return string("qdrtejlf");
}

string gbpktex::mzeqxxpmghpvosmpigygyzq(string lzecrcdjpnqc, string bbvcrhhopfhz, double usbausb, double fjlsnmflx, double pbbvkrdfr, bool qgxrhwdhs, bool rutmxaxgyqh, double umcdgjjnudeg) {
	int unyhedkzzikrit = 64;
	bool wathpoxswppfdlp = false;
	int qufsgd = 1239;
	double ntibowpnvagkr = 12998;
	int hqthafpskq = 378;
	bool dbnmxj = true;
	int gexokioznzzleb = 1025;
	bool jlrdbabokzcafa = true;
	int dbipwiwiauv = 1251;
	string qsdwty = "jockbzakpboggmxxkyqkehadwuskpsbqxddmyovrlcnonfdppjflcagdexcwqmpbdoyvogybcyygetgdmdkauqjhvzxiswvq";
	if (1239 == 1239) {
		int insmn;
		for (insmn = 11; insmn > 0; insmn--) {
			continue;
		}
	}
	if (378 == 378) {
		int qvakusd;
		for (qvakusd = 48; qvakusd > 0; qvakusd--) {
			continue;
		}
	}
	return string("gtwezdkjskgbrppdp");
}

double gbpktex::iwtkzescgidqzjbotzlq(double zshinwzslaxew, int iuxvcmtt, int ugtdxmu, int unhnxcymgwu, string aytktrkk, string kngzniv, int tjvqolou, double tjbampiqxlcqljf, int eosgakybjnd, string mxowvpq) {
	double sghciu = 93015;
	string kyqfwlaf = "xhtqucygsgoctuvfgiyqxdzqiqstvhzsezenyorjkuxqogwhjakcdurysyovqqhfvlkjfubkfmudkqarpeoxiciaepdqemjm";
	int rvqqvzxxlgoocjw = 3420;
	bool cxcqgqpcl = false;
	string bcjravb = "qgrbxvpaloqqdfwyxiizxvombgsyshlyrivudwryldpappwwxstz";
	return 52940;
}

string gbpktex::xipvxaadnjs() {
	bool lojtkrkgwrwtsi = false;
	string sxzqoj = "jopeqgzkdkllhdnymmjqqzopwfwbruy";
	bool bpohrijdzgibr = true;
	int oscsyewmapzfzd = 3884;
	bool nwluksolugqwx = false;
	bool yrdbki = true;
	bool kesbamfub = true;
	bool esson = true;
	if (true == true) {
		int mkdbw;
		for (mkdbw = 29; mkdbw > 0; mkdbw--) {
			continue;
		}
	}
	if (false != false) {
		int vfpoeadtrj;
		for (vfpoeadtrj = 53; vfpoeadtrj > 0; vfpoeadtrj--) {
			continue;
		}
	}
	return string("xkv");
}

string gbpktex::fkqvoitvkncnv(string ucuig, bool tjggkzlu, bool yibjftuclaudjt, int pglsvqtuzmycum, bool cbjlganu, double mbytokcbdwih, bool vbevklg, bool dgjcqtinchbbbf) {
	double ckwhmbxizoqoqv = 28968;
	double lzpdvsqvz = 10104;
	double nomwnqkav = 43110;
	if (28968 == 28968) {
		int fixyg;
		for (fixyg = 20; fixyg > 0; fixyg--) {
			continue;
		}
	}
	return string("pknjljzpsnovmps");
}

gbpktex::gbpktex() {
	this->qaypltimdqmcksquhuzjonul(6838, 823);
	this->mzeqxxpmghpvosmpigygyzq(string("wnvneechgzornnunenxlfyuxvrpga"), string("secgzufmxdyhubhdivtfegfbmofdkuskqobfvpokfgctjcyvtlcrtfyfmjzyxkzewlsohccysimaxkwzmvohntlzdkqbad"), 3645, 8742, 49943, true, false, 5181);
	this->iwtkzescgidqzjbotzlq(53014, 2508, 327, 4711, string("iccigwlwgaapfmfxvoedcomoasmkcfjmmmddhwxokmppvc"), string("addbaaoarbcucc"), 3542, 41733, 7012, string("gknupcmkidhzysecuaftixhcgnrgrdkacyvyeievrjfpefvpixzqczwyphfzoxvhzwlusuoanymgmpvayoyyxufdobollrlp"));
	this->xipvxaadnjs();
	this->fkqvoitvkncnv(string("yzavwfbrsocyieeqvutzheglc"), true, true, 27, false, 65594, false, true);
	this->dulvxsvcucyo(4791, string("wljqjjjkcclxo"));
	this->jaykwlbjvufgmnsespcgy(34100, false, string("sqjmjluasfytkljgfmpxuyaiuflfftmwcwtouuqkhuiwuyateqsjjcgrouhuxohuqvthy"), 2869, 21644, string("bvylyupgqjjacjgk"), 186, false);
	this->qqdpptonvatsvgxv(820);
	this->uuodavnake(string("wgdnffbixxsavzjbiwvwlq"), false, 27832, string("uyuqxmowlgsekuxohjjivhqhrhmoqoicgevdzupojdkumynnracvrovxryqkxfnasdningfxwyekxdyhtsfoktikytvovx"), string("qxddoyemrzgbvwtb"), string("fbwmlzadlpcbdnyqdlkayaqaljetqxstwiqivocrwibqzstpkekyirldmpfuurxnsfgbuvabofafdeah"));
	this->jvejrbkojcqy(false);
	this->mbtqxjkqxmje(19288, string("uutlalbgwtxilkemujbziotjsatxbeqgqwzwdwqkhddphqfhrexnqycbtwi"), false, true, 40809, string("wcrpsiqrfhugoulirdmlwknpupkknkqyoelmjhosasjhnbktyaphdnshrd"), 65);
	this->zsmkbfjsmbgfr(true, string("volpedooivardbgdefzkwvkryuxtgdcybqaah"), 100, true, 1623);
	this->eoqzkvschsbuwwhmdrgam(221, true, string("ztrylccdhtebueswdpuiulxcnnvfxcwbc"), 43783, false, 35419);
	this->xlifeerqvojxb(5659, false, 3677);
	this->btzgesoilnqedlmydcbcn(24100, 2677, 66087, 342, false, string("zqyayfyfuvvcbcanjrxfkqaydmglzrxyiwtxfkftuqs"), string("xujmgfqqeabbgzrzbahixlqwhtqfbmxobgeshhdcwhdsrnghybxkyibeccfanwykuenlouvpbewwzxvhkkmbczjrprgkermrnan"), 4668);
	this->bkqkhewnxss(string("vuqleofysfnavdngmvzeeiseotsylcxzsuhrjtstgqrzthluagqgtohgdnoggneappgi"));
	this->gyyubtfysyazhvtwrxnfuw(string("uagnifofpflcivftsbhyiwwepugqyufpxfyerdhvsbdvvhgqxpxukn"), 4858, string("mlvzgpprpkknomocifjrspwndntabgxuebghimgwjeiawjpribrthetntgkhskusgbmjsqpccqgjutgabwldxdzeiyvmby"), 20198, true);
	this->pzeyjbpukujbq(6067, 2358, true, string("jzmuudfqehuicsiunidggnfjsbvtyykcgdpjqwfuzopqdsdytiqjawdczpwb"), false);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class eruxgfh {
public:
	bool thqixnyobs;
	string fnogotmbbo;
	eruxgfh();
	double kcovluytprljktpyw(int qnonwbcltwoc, string uirhtdwqh, double acmjpmzv, string atthculecbpvzp, double tpeefwapwt, int pnowwiqvgskyu, int abuamlrn, double gcconndiii, bool hdxmjgp);
	int dhqfbbcugpgcegrp(double kxqrtyomja, bool pmkmmm, bool jnseokn, int gowhautfmyd, double tptdglmtptgkay);

protected:
	int kdyvroxlctsas;
	bool dmgydfgawt;

	int zfbmsckkbnrzhfvsbfdnrax(string xfiicyqhf, double eangd, string lfgav, string mkpga, string fywmgslcfn, bool ktpezqzu);
	double wnmflplzuljexrocpw(double rrzmox, string yrijpdavzovh, bool ttmiepklvt, double cdmrax, bool rhutmkncufa, string sluxegxswqx, bool rrwrpvy, double gxqzosvqd, double csxeewitr, double iskjqjdwkhksu);
	double dkywkvekhms(int ltpfqhzf, int qvdhpc, bool kfnbx, bool eqslcwsnumpa, double tiubsmvwgjpgygb, string xrlkek, string zymuynmflf, int xmilon, string qbflzms, int mlolrnkke);
	string tggmqtjwdrosvogw(string mbmbqq, bool prmymxhnqdnbj, double gvqxpi, bool cswnhs, int fyragnwfj, bool ulfiwoolorcmipx, bool eokeezofyvpqmeh);
	int xuuzmycwyssqqqqkagzunoq(int gjjkmnculweinp, string gfrhszjza, string mooomahuedallx, int pceqrgum, string myrekxi, int rhnuhm, string jkoenikeujph, bool wylxssrs, double uuxsevngok, int hidbyecyh);
	string pkszydpyazcligfmlugyxvyvq(double xltoy, bool qrhidvfeqv, double xgpwuyqcgve, bool puacmxbmx, int jmghoesa, double xreaotogi);
	bool yksctkwtrbsnbpztifpfi(int bmuazfkguct, bool xvlokby, double anrooiaocrzmap, int ijaiisviwss, int ggojfojhi, string ljxnka, double vewybcgxgg, int duiivas);

private:
	int jjvpmcen;
	int zsbdlpx;

	bool zsbrfecfeoehwhxqnavmloi(double anadkkztuhwdht, bool uqcazfoung, double vaupwbrazxau, bool trmdoyu, int zygteplq, string ftzmmjfyukn, double gpcjyvqapo);
	bool fntzotogzbyryxhr(string yzaan, int rqsplveqkvxopsf, int zvpmvjypzv, string tkdjvsbdbj, int hrjshtq, string rnyqq, bool qenoedkcy, string qbpfojqwisc, bool pxwosyo, int mzoszklehrugze);
	void caglujsqjcjazejcigayt(double kbbwy, bool lqdqkqsrvnitn);
	string esmjdlxawdwv();
	double stcwjagghjewgyeqbvxf(int gjjcafpvlxjtffe, double igpdhqbnxbo, double lnfjfwewiwlghsj, bool tlefqsyhvbfcsfc, double hhzmfr, string eeufvffd, double mylnutqf);
	void nqcrdzfkmbezhuaj(string dwlaa, bool nbqlhbiyxl, int yegomwnbxcatko, double ktxjqohk, bool bomynpmvqcn, double lrvcvdiobjl, string xvvmfelpmfyvf, string rxqxafpk, bool kuekxibpvraf, int bwgab);
	double lsbixvzhtvwprmsfhtg(int bttndglzrbvcmu);

};


bool eruxgfh::zsbrfecfeoehwhxqnavmloi(double anadkkztuhwdht, bool uqcazfoung, double vaupwbrazxau, bool trmdoyu, int zygteplq, string ftzmmjfyukn, double gpcjyvqapo) {
	double ieckqmkofdwbaek = 60705;
	int hdzaatobkhsvtah = 3498;
	int yfxwl = 4038;
	if (3498 != 3498) {
		int kmibdxozg;
		for (kmibdxozg = 93; kmibdxozg > 0; kmibdxozg--) {
			continue;
		}
	}
	if (3498 != 3498) {
		int wbfka;
		for (wbfka = 29; wbfka > 0; wbfka--) {
			continue;
		}
	}
	if (3498 != 3498) {
		int tjefoxt;
		for (tjefoxt = 61; tjefoxt > 0; tjefoxt--) {
			continue;
		}
	}
	if (4038 != 4038) {
		int dv;
		for (dv = 31; dv > 0; dv--) {
			continue;
		}
	}
	if (60705 != 60705) {
		int jdpexqx;
		for (jdpexqx = 56; jdpexqx > 0; jdpexqx--) {
			continue;
		}
	}
	return true;
}

bool eruxgfh::fntzotogzbyryxhr(string yzaan, int rqsplveqkvxopsf, int zvpmvjypzv, string tkdjvsbdbj, int hrjshtq, string rnyqq, bool qenoedkcy, string qbpfojqwisc, bool pxwosyo, int mzoszklehrugze) {
	int nnpjrlwwbloecbk = 2587;
	if (2587 != 2587) {
		int fufoth;
		for (fufoth = 15; fufoth > 0; fufoth--) {
			continue;
		}
	}
	if (2587 == 2587) {
		int dtklskkr;
		for (dtklskkr = 31; dtklskkr > 0; dtklskkr--) {
			continue;
		}
	}
	if (2587 != 2587) {
		int yddjqaki;
		for (yddjqaki = 31; yddjqaki > 0; yddjqaki--) {
			continue;
		}
	}
	return true;
}

void eruxgfh::caglujsqjcjazejcigayt(double kbbwy, bool lqdqkqsrvnitn) {
	string drysmsjporzjpfl = "sseczxbkwgpevcltmsphdyvwxitttvrfgegwwjhcyyutufioltvaolgsxtsizekhpe";
	double xobjtwhntwnpg = 64993;
	bool uvtcdijlwb = false;
	double gjcxouh = 4513;
	if (64993 != 64993) {
		int betzit;
		for (betzit = 31; betzit > 0; betzit--) {
			continue;
		}
	}
	if (4513 != 4513) {
		int txcp;
		for (txcp = 97; txcp > 0; txcp--) {
			continue;
		}
	}
	if (64993 == 64993) {
		int deeru;
		for (deeru = 78; deeru > 0; deeru--) {
			continue;
		}
	}
	if (64993 == 64993) {
		int gjffeeb;
		for (gjffeeb = 83; gjffeeb > 0; gjffeeb--) {
			continue;
		}
	}

}

string eruxgfh::esmjdlxawdwv() {
	string yqhvk = "luyyhjgypcogzncjyonzkhihgexkydbeovddcxlcozwjtmmtsaklxjseikrkmzzrzuvkaesjvfihcfhjbnrnib";
	int qbgskbeflntal = 2670;
	int fhoczurt = 1385;
	string fyqqu = "yehtnjaiamoknvacakgyofyzwztldjdagmwsrbvxxfdkpafeiarriccqh";
	bool jnonbhtezlyk = true;
	bool inyeimudahu = false;
	bool kjpdcovsccvy = true;
	if (1385 == 1385) {
		int ccbh;
		for (ccbh = 54; ccbh > 0; ccbh--) {
			continue;
		}
	}
	if (true != true) {
		int uv;
		for (uv = 26; uv > 0; uv--) {
			continue;
		}
	}
	if (string("luyyhjgypcogzncjyonzkhihgexkydbeovddcxlcozwjtmmtsaklxjseikrkmzzrzuvkaesjvfihcfhjbnrnib") != string("luyyhjgypcogzncjyonzkhihgexkydbeovddcxlcozwjtmmtsaklxjseikrkmzzrzuvkaesjvfihcfhjbnrnib")) {
		int vhtlguolb;
		for (vhtlguolb = 82; vhtlguolb > 0; vhtlguolb--) {
			continue;
		}
	}
	if (true == true) {
		int wertjaiky;
		for (wertjaiky = 77; wertjaiky > 0; wertjaiky--) {
			continue;
		}
	}
	if (true != true) {
		int nuautrxy;
		for (nuautrxy = 39; nuautrxy > 0; nuautrxy--) {
			continue;
		}
	}
	return string("zwt");
}

double eruxgfh::stcwjagghjewgyeqbvxf(int gjjcafpvlxjtffe, double igpdhqbnxbo, double lnfjfwewiwlghsj, bool tlefqsyhvbfcsfc, double hhzmfr, string eeufvffd, double mylnutqf) {
	bool aieoc = true;
	bool iymsahsrnjaophx = true;
	if (true != true) {
		int dadysm;
		for (dadysm = 95; dadysm > 0; dadysm--) {
			continue;
		}
	}
	return 84819;
}

void eruxgfh::nqcrdzfkmbezhuaj(string dwlaa, bool nbqlhbiyxl, int yegomwnbxcatko, double ktxjqohk, bool bomynpmvqcn, double lrvcvdiobjl, string xvvmfelpmfyvf, string rxqxafpk, bool kuekxibpvraf, int bwgab) {
	string lbxzaazq = "lybxrtnjkhafocsgattodjgveocqlnhcjbuynrwwizbinkzumomtextiarmqyhakwqawtbdkx";
	int bwipr = 2492;
	bool syigrmujqoqo = false;
	double gqpcom = 41547;
	string fmrslzkuot = "cvaubvovwmvsmwkiyktlrrfuoeeqeyeugfezzuyimilpqpssyzdvbcfanjfeknneswitqykftczmlmtbaliriitrgfhozjvljfqi";
	int upnknax = 1899;
	double fhborirmodgy = 19002;
	string pyinvqcna = "vyjiqfupcxwyfbifortkaxc";
	double xzupzvbejwyoq = 45407;
	int ydmlbubfbgrsdxk = 4345;
	if (4345 == 4345) {
		int zp;
		for (zp = 97; zp > 0; zp--) {
			continue;
		}
	}
	if (string("cvaubvovwmvsmwkiyktlrrfuoeeqeyeugfezzuyimilpqpssyzdvbcfanjfeknneswitqykftczmlmtbaliriitrgfhozjvljfqi") != string("cvaubvovwmvsmwkiyktlrrfuoeeqeyeugfezzuyimilpqpssyzdvbcfanjfeknneswitqykftczmlmtbaliriitrgfhozjvljfqi")) {
		int pjgxtmsb;
		for (pjgxtmsb = 53; pjgxtmsb > 0; pjgxtmsb--) {
			continue;
		}
	}
	if (45407 != 45407) {
		int prjle;
		for (prjle = 71; prjle > 0; prjle--) {
			continue;
		}
	}
	if (1899 != 1899) {
		int wk;
		for (wk = 66; wk > 0; wk--) {
			continue;
		}
	}

}

double eruxgfh::lsbixvzhtvwprmsfhtg(int bttndglzrbvcmu) {
	double yzuszdfa = 45870;
	double jhderjpj = 62349;
	double bhgxvvsqky = 7856;
	string wpgicifxtplm = "rubzdqhavltgxt";
	string roxyqrp = "ijhbqsgoduwocmicfurlrafohdyfmvelqltdjkpbgjzvnhfsgdvgeneynoollilkrcfqzwfoaiszoaznvwewdoabvstst";
	if (string("rubzdqhavltgxt") != string("rubzdqhavltgxt")) {
		int rt;
		for (rt = 17; rt > 0; rt--) {
			continue;
		}
	}
	if (string("ijhbqsgoduwocmicfurlrafohdyfmvelqltdjkpbgjzvnhfsgdvgeneynoollilkrcfqzwfoaiszoaznvwewdoabvstst") != string("ijhbqsgoduwocmicfurlrafohdyfmvelqltdjkpbgjzvnhfsgdvgeneynoollilkrcfqzwfoaiszoaznvwewdoabvstst")) {
		int ewmazvxv;
		for (ewmazvxv = 50; ewmazvxv > 0; ewmazvxv--) {
			continue;
		}
	}
	if (7856 == 7856) {
		int kxoh;
		for (kxoh = 65; kxoh > 0; kxoh--) {
			continue;
		}
	}
	return 41773;
}

int eruxgfh::zfbmsckkbnrzhfvsbfdnrax(string xfiicyqhf, double eangd, string lfgav, string mkpga, string fywmgslcfn, bool ktpezqzu) {
	int aqyca = 109;
	if (109 != 109) {
		int lrqieuh;
		for (lrqieuh = 52; lrqieuh > 0; lrqieuh--) {
			continue;
		}
	}
	if (109 != 109) {
		int qrwjfgeot;
		for (qrwjfgeot = 86; qrwjfgeot > 0; qrwjfgeot--) {
			continue;
		}
	}
	if (109 == 109) {
		int xdbbdu;
		for (xdbbdu = 20; xdbbdu > 0; xdbbdu--) {
			continue;
		}
	}
	if (109 == 109) {
		int jxkjozczob;
		for (jxkjozczob = 92; jxkjozczob > 0; jxkjozczob--) {
			continue;
		}
	}
	return 29311;
}

double eruxgfh::wnmflplzuljexrocpw(double rrzmox, string yrijpdavzovh, bool ttmiepklvt, double cdmrax, bool rhutmkncufa, string sluxegxswqx, bool rrwrpvy, double gxqzosvqd, double csxeewitr, double iskjqjdwkhksu) {
	int rrmxfipfpmylit = 2514;
	string eikilhjmqd = "igjxjuduuocvowqmypwtsyrojedllchagwzpdpmwuodhtaayfqgyzheippixgvm";
	if (string("igjxjuduuocvowqmypwtsyrojedllchagwzpdpmwuodhtaayfqgyzheippixgvm") == string("igjxjuduuocvowqmypwtsyrojedllchagwzpdpmwuodhtaayfqgyzheippixgvm")) {
		int gdfq;
		for (gdfq = 40; gdfq > 0; gdfq--) {
			continue;
		}
	}
	if (string("igjxjuduuocvowqmypwtsyrojedllchagwzpdpmwuodhtaayfqgyzheippixgvm") != string("igjxjuduuocvowqmypwtsyrojedllchagwzpdpmwuodhtaayfqgyzheippixgvm")) {
		int auzrasmmfq;
		for (auzrasmmfq = 38; auzrasmmfq > 0; auzrasmmfq--) {
			continue;
		}
	}
	if (string("igjxjuduuocvowqmypwtsyrojedllchagwzpdpmwuodhtaayfqgyzheippixgvm") == string("igjxjuduuocvowqmypwtsyrojedllchagwzpdpmwuodhtaayfqgyzheippixgvm")) {
		int qiqmggy;
		for (qiqmggy = 17; qiqmggy > 0; qiqmggy--) {
			continue;
		}
	}
	if (2514 != 2514) {
		int umuryuvdn;
		for (umuryuvdn = 71; umuryuvdn > 0; umuryuvdn--) {
			continue;
		}
	}
	return 74897;
}

double eruxgfh::dkywkvekhms(int ltpfqhzf, int qvdhpc, bool kfnbx, bool eqslcwsnumpa, double tiubsmvwgjpgygb, string xrlkek, string zymuynmflf, int xmilon, string qbflzms, int mlolrnkke) {
	bool yphvhryblsmue = false;
	int sqqtyeapuf = 2054;
	string qikhc = "tidm";
	int hehvzuoxc = 2230;
	double wuvyxrob = 23333;
	double gaipzina = 60592;
	string zgyvmdraaljulq = "qguwqfeqggmphxyltxhckpcukvwdzfhyfdicdlmcksqnmzbkrlvvpwxjxdjwmypqayvcdhpmozwxqatzjplnyvrkfdq";
	if (false == false) {
		int unmokjni;
		for (unmokjni = 32; unmokjni > 0; unmokjni--) {
			continue;
		}
	}
	if (60592 == 60592) {
		int bhdlxhwsyb;
		for (bhdlxhwsyb = 100; bhdlxhwsyb > 0; bhdlxhwsyb--) {
			continue;
		}
	}
	if (false != false) {
		int caierb;
		for (caierb = 9; caierb > 0; caierb--) {
			continue;
		}
	}
	if (string("qguwqfeqggmphxyltxhckpcukvwdzfhyfdicdlmcksqnmzbkrlvvpwxjxdjwmypqayvcdhpmozwxqatzjplnyvrkfdq") != string("qguwqfeqggmphxyltxhckpcukvwdzfhyfdicdlmcksqnmzbkrlvvpwxjxdjwmypqayvcdhpmozwxqatzjplnyvrkfdq")) {
		int xoky;
		for (xoky = 93; xoky > 0; xoky--) {
			continue;
		}
	}
	return 50602;
}

string eruxgfh::tggmqtjwdrosvogw(string mbmbqq, bool prmymxhnqdnbj, double gvqxpi, bool cswnhs, int fyragnwfj, bool ulfiwoolorcmipx, bool eokeezofyvpqmeh) {
	string tptosirpzxyh = "ufuvwxllcjilsdrpfnqi";
	bool qdzvnzlszuxv = false;
	if (false != false) {
		int xvnfc;
		for (xvnfc = 96; xvnfc > 0; xvnfc--) {
			continue;
		}
	}
	if (string("ufuvwxllcjilsdrpfnqi") != string("ufuvwxllcjilsdrpfnqi")) {
		int dqf;
		for (dqf = 7; dqf > 0; dqf--) {
			continue;
		}
	}
	if (false == false) {
		int arliotwk;
		for (arliotwk = 1; arliotwk > 0; arliotwk--) {
			continue;
		}
	}
	if (false != false) {
		int dboekuatfj;
		for (dboekuatfj = 90; dboekuatfj > 0; dboekuatfj--) {
			continue;
		}
	}
	return string("");
}

int eruxgfh::xuuzmycwyssqqqqkagzunoq(int gjjkmnculweinp, string gfrhszjza, string mooomahuedallx, int pceqrgum, string myrekxi, int rhnuhm, string jkoenikeujph, bool wylxssrs, double uuxsevngok, int hidbyecyh) {
	int kdmaafem = 567;
	double gkzafveasoqzq = 19351;
	if (567 == 567) {
		int ivgbqlbi;
		for (ivgbqlbi = 87; ivgbqlbi > 0; ivgbqlbi--) {
			continue;
		}
	}
	if (567 == 567) {
		int sleylbkhou;
		for (sleylbkhou = 8; sleylbkhou > 0; sleylbkhou--) {
			continue;
		}
	}
	return 29583;
}

string eruxgfh::pkszydpyazcligfmlugyxvyvq(double xltoy, bool qrhidvfeqv, double xgpwuyqcgve, bool puacmxbmx, int jmghoesa, double xreaotogi) {
	double laldt = 78288;
	bool cskbpmlca = true;
	double umwzl = 13253;
	bool nmqeows = false;
	double ygmsmdbxsabpen = 16374;
	double cgnwfowj = 45710;
	string ffdavpwhikpspx = "kveswdxcjgdvrdrpnlwuffupfnxkuckaasmkuqtegxoup";
	if (false == false) {
		int lxdm;
		for (lxdm = 62; lxdm > 0; lxdm--) {
			continue;
		}
	}
	if (13253 == 13253) {
		int wfbulewqum;
		for (wfbulewqum = 64; wfbulewqum > 0; wfbulewqum--) {
			continue;
		}
	}
	if (13253 == 13253) {
		int msbnsejhgy;
		for (msbnsejhgy = 30; msbnsejhgy > 0; msbnsejhgy--) {
			continue;
		}
	}
	if (true == true) {
		int wrzn;
		for (wrzn = 70; wrzn > 0; wrzn--) {
			continue;
		}
	}
	return string("or");
}

bool eruxgfh::yksctkwtrbsnbpztifpfi(int bmuazfkguct, bool xvlokby, double anrooiaocrzmap, int ijaiisviwss, int ggojfojhi, string ljxnka, double vewybcgxgg, int duiivas) {
	bool skaqdwsv = false;
	bool fsueojkxp = true;
	bool xsnnusgzvmwjbdw = true;
	bool mwrzpnvi = true;
	bool gyhhzzbtxonzss = true;
	bool tlwjjuxbqyn = false;
	double nftxlzhxvxppven = 60370;
	double bmzerbccxwrbjsi = 15632;
	if (15632 == 15632) {
		int wffrida;
		for (wffrida = 25; wffrida > 0; wffrida--) {
			continue;
		}
	}
	if (15632 == 15632) {
		int fybfxf;
		for (fybfxf = 21; fybfxf > 0; fybfxf--) {
			continue;
		}
	}
	return true;
}

double eruxgfh::kcovluytprljktpyw(int qnonwbcltwoc, string uirhtdwqh, double acmjpmzv, string atthculecbpvzp, double tpeefwapwt, int pnowwiqvgskyu, int abuamlrn, double gcconndiii, bool hdxmjgp) {
	int jnmqfxgricm = 636;
	double wbdbfthfvgf = 82887;
	int enujqrg = 689;
	bool jtnacgzkce = true;
	int vvpukuzoabeag = 3404;
	bool vdvflqlhuvw = true;
	int tqktebmxi = 1099;
	int vqslrjvhfuwhnh = 5360;
	string auozruljbqobg = "eddfmusswukuqxxrweh";
	if (5360 != 5360) {
		int hyc;
		for (hyc = 75; hyc > 0; hyc--) {
			continue;
		}
	}
	return 36245;
}

int eruxgfh::dhqfbbcugpgcegrp(double kxqrtyomja, bool pmkmmm, bool jnseokn, int gowhautfmyd, double tptdglmtptgkay) {
	int ykzoh = 59;
	double owwdpxfmvghnq = 45954;
	double zybgrrpljqs = 14851;
	if (45954 != 45954) {
		int rlagnwr;
		for (rlagnwr = 90; rlagnwr > 0; rlagnwr--) {
			continue;
		}
	}
	if (14851 == 14851) {
		int zvyztlwonl;
		for (zvyztlwonl = 57; zvyztlwonl > 0; zvyztlwonl--) {
			continue;
		}
	}
	if (14851 != 14851) {
		int mpdbyk;
		for (mpdbyk = 69; mpdbyk > 0; mpdbyk--) {
			continue;
		}
	}
	return 99191;
}

eruxgfh::eruxgfh() {
	this->kcovluytprljktpyw(1503, string("lbfsnhlbxcldbjjeqcpqqoodjuadiohtslqxqghdlkgwipzanogjbqhmclosaelctuqjhumusyqamxrsldmfgcs"), 242, string("dkutopsxtivtujynxqluvdjcgkoxbj"), 43296, 5133, 1144, 1583, true);
	this->dhqfbbcugpgcegrp(42396, true, true, 2718, 39276);
	this->zfbmsckkbnrzhfvsbfdnrax(string("vsagpiimk"), 2409, string("gxhkawqtuyvvidbehvwknzjqogtgnsigpfyrz"), string("klzolvbxticdniupgoaihjdpqajqlaodyuscdr"), string("pksqgmvsskyducdclxjxpbainarwdwszuzyukztgd"), false);
	this->wnmflplzuljexrocpw(27479, string("xxftyyizgukoblcaoctgweowmunwqcmabbtgtjqahdqwlghpdjmdjttrbmjwbtmvmnacversjpfrjjypiur"), false, 23100, false, string("xmramkzpevzveavyjmolubgdqyyteqazlsrsxonovbbobvjbq"), false, 16042, 39802, 7113);
	this->dkywkvekhms(951, 533, true, false, 82597, string("aldelcemmqypdxwpkajxidiosdacktezjshjyenmnyuuedovcqkeyjzmnalmcxrcynnpnlgjephgjcwdwkgrqoobfqrvpy"), string("lusxaifchvzoaflmvpozrkvfcenfjroipsfhmfwmohkerqlu"), 298, string("ctufmtujvbyieaksqomdbykqrbmbahfvrndvlhtgzvnlkyiqjcbboqrgqxrfjpjnnrygschwvpm"), 4298);
	this->tggmqtjwdrosvogw(string("vrq"), false, 7248, false, 8, true, false);
	this->xuuzmycwyssqqqqkagzunoq(2410, string("jasbvhrydxrfqwwtrjdwmnwueijbjwptqnooezquqpbbqcgeupdqtetlrvrt"), string("ssnruujdyscmclmkmoggwhqd"), 1417, string("vosphrffqpklcfmgkiikdmloknhxgsguddzfgripvxzygticjpdqkm"), 4586, string("xhqzjkihxzepqfntrageqsqlfqdbclyunvxwegdmimombbdgibwdqaplgjakrugmlisglegrjxwntniaoywdemcxsavu"), false, 22277, 422);
	this->pkszydpyazcligfmlugyxvyvq(528, false, 38617, true, 1257, 16185);
	this->yksctkwtrbsnbpztifpfi(5060, true, 13509, 8230, 1630, string("ccnejkykcfsdhvvpmzblffycctxdbtsxsibdqbrsqlxgzxbfaygwinjxxecpbidmttqjexracgdlu"), 23738, 3354);
	this->zsbrfecfeoehwhxqnavmloi(1652, true, 29560, false, 272, string("gdawsunhpijsvivlrhljojlvxbtiqtnmdyemlmtbngkqxpieozorkfjocsjvecpmkynaj"), 14902);
	this->fntzotogzbyryxhr(string("nkvxlstehyvcpvdmszusuieykwyxjjutbclamfnvdryvxmrznkjeyckmcnagdabcgwwbdtfdjzhzvrrn"), 1057, 473, string("tayqfqvwdcfvtyvpqgusueqrjzbonowusdkcknwicmdvmngqsvuksmoawxprerhekpoljpnpfnfapmhsyij"), 4696, string("wlpxbhepuzusmdkqwxspqhkzyvdjahiyqopdkscgygkopefgnixaeehrmqnhwkgjjrnmdufqacxxxeodgvdsh"), true, string("oidwpqgatovwfbkehqpfrftvofhepqiuuanlyykeqbfljfvenifxjjvyltyasvwdaonzarnsiyuptlkybhsdabmvi"), false, 1612);
	this->caglujsqjcjazejcigayt(3133, true);
	this->esmjdlxawdwv();
	this->stcwjagghjewgyeqbvxf(23, 1442, 7177, false, 46034, string("jtkpdowqdhkyuxqwmoxavgqjmmxyzzursilohplulzmclelxjezbuxerzwuvgfefzdaokbrmimpnenmz"), 41026);
	this->nqcrdzfkmbezhuaj(string("isnvvsdylcedgoqsntkbrldshhyl"), true, 523, 9140, false, 1027, string("qsqapwhbuiiztndjxrwgqohwtudnxvuffnuvjtpiwtchgyjsdkdrwgvhfgiknvsreqjfgmoxvsiajsrktuv"), string("ksywe"), false, 61);
	this->lsbixvzhtvwprmsfhtg(5792);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class xolasce {
public:
	bool ssmyliymcqt;
	double vuioavmsnw;
	double ynmpepa;
	double yqbfjfq;
	bool dobbbiienbsad;
	xolasce();
	double txwxwyiifbxyuzwfsvibqxvn(int qrpqkwtorfz);

protected:
	bool vavtwzbqoxjwpez;
	string uegkjqikqj;
	double lntii;
	double lwdyrrhs;

	int erimbkborbtxwujqzoqb(double vmrqjjlvmc, double rivjngdg, int qarllsqz, int auiujukwnnp, int pnlasfuzr, string wdrabcnxttfsoqn, string ipmmmlgrjn, string nnqgelelhrljvxp);
	void hopgnflwxocppnexrtfqjehfq(string lgvabktcocu);
	double degopxshtmroosnstxfn(bool epoqcl, double mfmmpxxirisovt, string gqvxhfpcj, int kgrexumg, double tzasyawngyuto, bool viqyyw, string bwqdvldzxtwnu, int gncwkfx);
	int fsrjreibbdvjnf(bool dqmhxxru, bool iqllkxxmptu);
	bool srkgzszddizhncyhgqqmta(double wfabz, int iejjaaaxnzas);
	double ideybkmvyslrs(double kjnxvygbslvdhc, bool ariemdpstsbfes, string ctwtskpvxmtwggd, string heduxirbpbktr, double udwviihkbivzzg, int ixmppqb, double vplsdbjzm, string bunkijdlsbbzi);
	string szzhfdmcpaogqigoyjalpit(bool elqjzamv, bool tjtgeywxdm, bool daigwpyusk, double cyjdzcukboqwilu);
	bool pvlwxwfnlwybzopagbavxkkmz(string czmnmopzjpcn, double fncdfvuhhxuipt, int xkrvfld, bool irjzlm, int nxzrixmzan, string ixuvl, double egxuydqhzsnxj, double zwxawkhpzzrwkcp);

private:
	int lytwpbu;
	double rwmayp;
	string robsfhreqdcnjcv;
	string mwbinufvrtyuar;

	bool luhygykdahmqzat(bool rgwemtiuln, string hscqoecp, double sojohswemtb, bool kigvnlc, int xznxio, string snuksyg, int ylydwxwgtjscw, int mwyjjxot, int glbggdc, string ffvdmrcezryfgp);

};


bool xolasce::luhygykdahmqzat(bool rgwemtiuln, string hscqoecp, double sojohswemtb, bool kigvnlc, int xznxio, string snuksyg, int ylydwxwgtjscw, int mwyjjxot, int glbggdc, string ffvdmrcezryfgp) {
	bool evfdovpduevlkn = true;
	string vislipzwx = "jnihvnufgcdebnuyjryidwwxsmkgmhllsaikuntxahkjviplazmdyraflsbxgzmllnzyydpymuwknsee";
	if (string("jnihvnufgcdebnuyjryidwwxsmkgmhllsaikuntxahkjviplazmdyraflsbxgzmllnzyydpymuwknsee") == string("jnihvnufgcdebnuyjryidwwxsmkgmhllsaikuntxahkjviplazmdyraflsbxgzmllnzyydpymuwknsee")) {
		int knhys;
		for (knhys = 13; knhys > 0; knhys--) {
			continue;
		}
	}
	return false;
}

int xolasce::erimbkborbtxwujqzoqb(double vmrqjjlvmc, double rivjngdg, int qarllsqz, int auiujukwnnp, int pnlasfuzr, string wdrabcnxttfsoqn, string ipmmmlgrjn, string nnqgelelhrljvxp) {
	bool dcewirzqlzbrgp = true;
	string bujhladgbaosb = "nszffctglxqwgmjefmittvfbuhmfztdtbceqfjdoznwprryotaxvdtxctjuerjzaombnshwxgfjt";
	int nhdwcwxkafjkv = 1541;
	bool oyivcdzyvfd = false;
	double ibhzvlosdydku = 24823;
	double uhrdoutpxbcj = 33705;
	bool ukzvftcqwaxky = true;
	if (true != true) {
		int gaidvlz;
		for (gaidvlz = 41; gaidvlz > 0; gaidvlz--) {
			continue;
		}
	}
	if (false == false) {
		int tyvsoawjg;
		for (tyvsoawjg = 17; tyvsoawjg > 0; tyvsoawjg--) {
			continue;
		}
	}
	if (false == false) {
		int yg;
		for (yg = 84; yg > 0; yg--) {
			continue;
		}
	}
	return 6985;
}

void xolasce::hopgnflwxocppnexrtfqjehfq(string lgvabktcocu) {
	double bdosm = 132;
	double udnmgrre = 20841;
	int vhusrsozxnyt = 5372;
	int knkuxiwjsx = 5871;
	if (20841 == 20841) {
		int rzduw;
		for (rzduw = 79; rzduw > 0; rzduw--) {
			continue;
		}
	}
	if (5871 == 5871) {
		int iurgzthi;
		for (iurgzthi = 27; iurgzthi > 0; iurgzthi--) {
			continue;
		}
	}
	if (132 != 132) {
		int zqmj;
		for (zqmj = 37; zqmj > 0; zqmj--) {
			continue;
		}
	}
	if (5871 == 5871) {
		int ucucybrqma;
		for (ucucybrqma = 96; ucucybrqma > 0; ucucybrqma--) {
			continue;
		}
	}

}

double xolasce::degopxshtmroosnstxfn(bool epoqcl, double mfmmpxxirisovt, string gqvxhfpcj, int kgrexumg, double tzasyawngyuto, bool viqyyw, string bwqdvldzxtwnu, int gncwkfx) {
	string kdkuuk = "obakqmxikonqnotmd";
	bool dxtdxemlihm = true;
	int hviplldccbxtfxp = 5097;
	double lqmpbnox = 9594;
	bool yknvodxyns = false;
	string giiumr = "btsravueqjtqhclxycjzy";
	int hhabtppitzxxar = 6512;
	if (9594 != 9594) {
		int imf;
		for (imf = 23; imf > 0; imf--) {
			continue;
		}
	}
	if (true == true) {
		int ws;
		for (ws = 28; ws > 0; ws--) {
			continue;
		}
	}
	if (false == false) {
		int zsbdnkhead;
		for (zsbdnkhead = 53; zsbdnkhead > 0; zsbdnkhead--) {
			continue;
		}
	}
	if (9594 != 9594) {
		int koakkl;
		for (koakkl = 74; koakkl > 0; koakkl--) {
			continue;
		}
	}
	return 24394;
}

int xolasce::fsrjreibbdvjnf(bool dqmhxxru, bool iqllkxxmptu) {
	double gsvjdgbfxk = 27379;
	return 25055;
}

bool xolasce::srkgzszddizhncyhgqqmta(double wfabz, int iejjaaaxnzas) {
	double jzjfix = 15769;
	string hworfqan = "swjdttsdbdnbeqykndqnoidakatpjxtgcmrpsgrxjakkwzpestogazefojntgtqxreazktbjixpcjhbtedb";
	string omzofkhkuc = "laetpdocfbnhyldtnwdnmdydnzfyzwolcicgiarpyotvwjedlkxdqtyevkipoqiktrrduidietiuhtaybgdxsvjwtqjepljuw";
	double ksffansbzqbew = 60988;
	double vappyymxh = 51500;
	bool tbevxzszfzmtm = false;
	double lifdoufxv = 8640;
	if (string("laetpdocfbnhyldtnwdnmdydnzfyzwolcicgiarpyotvwjedlkxdqtyevkipoqiktrrduidietiuhtaybgdxsvjwtqjepljuw") == string("laetpdocfbnhyldtnwdnmdydnzfyzwolcicgiarpyotvwjedlkxdqtyevkipoqiktrrduidietiuhtaybgdxsvjwtqjepljuw")) {
		int qa;
		for (qa = 17; qa > 0; qa--) {
			continue;
		}
	}
	if (51500 == 51500) {
		int ynpp;
		for (ynpp = 72; ynpp > 0; ynpp--) {
			continue;
		}
	}
	if (51500 != 51500) {
		int me;
		for (me = 70; me > 0; me--) {
			continue;
		}
	}
	return true;
}

double xolasce::ideybkmvyslrs(double kjnxvygbslvdhc, bool ariemdpstsbfes, string ctwtskpvxmtwggd, string heduxirbpbktr, double udwviihkbivzzg, int ixmppqb, double vplsdbjzm, string bunkijdlsbbzi) {
	double ctygngdpw = 65863;
	double kyhvsmubjt = 20633;
	bool wgojjvupoagmxi = false;
	string zbtekpfw = "bpqlefaqt";
	bool eqyttfs = true;
	double tstnlnecn = 57103;
	int mlyvxqnfjc = 234;
	int vjymu = 938;
	double vgexdlwonyn = 9566;
	double jabezqqetqc = 52875;
	if (9566 != 9566) {
		int lxqt;
		for (lxqt = 89; lxqt > 0; lxqt--) {
			continue;
		}
	}
	if (false == false) {
		int bsfm;
		for (bsfm = 93; bsfm > 0; bsfm--) {
			continue;
		}
	}
	if (9566 == 9566) {
		int enzpisshq;
		for (enzpisshq = 5; enzpisshq > 0; enzpisshq--) {
			continue;
		}
	}
	if (938 == 938) {
		int zuytmokgg;
		for (zuytmokgg = 24; zuytmokgg > 0; zuytmokgg--) {
			continue;
		}
	}
	return 78484;
}

string xolasce::szzhfdmcpaogqigoyjalpit(bool elqjzamv, bool tjtgeywxdm, bool daigwpyusk, double cyjdzcukboqwilu) {
	bool svvvtnnz = true;
	string hebxaujyo = "zuwrwmcdlrdmwzvxmbqokqtnqxxzyjksuofvowjkppynwdnaaceywbdmjjkraidlmkivuwhjfacuraammynico";
	double irajn = 37915;
	int mtpkgsxigjj = 25;
	string kdfia = "avkalceolgdtdhnwjwiopztuajhhovnmxalctjdohcsocwwbrmzpbwxzdodweciivwoigsrjtdxygsppsrzdazcjqtrvkol";
	bool fzhbeepgspph = true;
	if (string("zuwrwmcdlrdmwzvxmbqokqtnqxxzyjksuofvowjkppynwdnaaceywbdmjjkraidlmkivuwhjfacuraammynico") == string("zuwrwmcdlrdmwzvxmbqokqtnqxxzyjksuofvowjkppynwdnaaceywbdmjjkraidlmkivuwhjfacuraammynico")) {
		int tvrfhlo;
		for (tvrfhlo = 58; tvrfhlo > 0; tvrfhlo--) {
			continue;
		}
	}
	if (string("zuwrwmcdlrdmwzvxmbqokqtnqxxzyjksuofvowjkppynwdnaaceywbdmjjkraidlmkivuwhjfacuraammynico") != string("zuwrwmcdlrdmwzvxmbqokqtnqxxzyjksuofvowjkppynwdnaaceywbdmjjkraidlmkivuwhjfacuraammynico")) {
		int aptncyl;
		for (aptncyl = 96; aptncyl > 0; aptncyl--) {
			continue;
		}
	}
	return string("wnjlclwnxnckcorrjbm");
}

bool xolasce::pvlwxwfnlwybzopagbavxkkmz(string czmnmopzjpcn, double fncdfvuhhxuipt, int xkrvfld, bool irjzlm, int nxzrixmzan, string ixuvl, double egxuydqhzsnxj, double zwxawkhpzzrwkcp) {
	double lhgwh = 84015;
	string jgzzjs = "ouvfialfzopbrnmvxeyavxjohnlovjmbpdregsvvcuxovabyixilslymswl";
	string rvomqktdr = "pbjbenwmkfanaehberretuybqjcrayccyzxqdixuifdkmbktfdhmxtcslbeshmcqlymkibiljgu";
	bool lcjloiiagoq = true;
	bool kmclqeyn = true;
	string gosicqgmuj = "dqrdvwvdznjwxlfwzgpfifrytizqxcjsfltgolekfcp";
	double xlttshmr = 9284;
	bool bdgqlxkbqn = true;
	int zulbld = 3292;
	string gpeliaumk = "ruccencwaaodepalorwksbmeobabiyxxomarkzjlzklofyxpejyllbjxbrvrrcjgkfkzmhcfupvtshavxiqp";
	if (string("ouvfialfzopbrnmvxeyavxjohnlovjmbpdregsvvcuxovabyixilslymswl") != string("ouvfialfzopbrnmvxeyavxjohnlovjmbpdregsvvcuxovabyixilslymswl")) {
		int kobnugkiw;
		for (kobnugkiw = 48; kobnugkiw > 0; kobnugkiw--) {
			continue;
		}
	}
	return false;
}

double xolasce::txwxwyiifbxyuzwfsvibqxvn(int qrpqkwtorfz) {
	int yqqfihzt = 2494;
	double mjyjirfl = 9848;
	double elsuzlxchcrvvw = 29553;
	double lfuemavsymwa = 23392;
	string btnvxha = "mmtpvupmerezdplyhzqpmnozmnkprgmciddkhbwrjrdzlqlridjtlrdqc";
	if (29553 != 29553) {
		int ltndcya;
		for (ltndcya = 85; ltndcya > 0; ltndcya--) {
			continue;
		}
	}
	if (2494 != 2494) {
		int safknurlkv;
		for (safknurlkv = 70; safknurlkv > 0; safknurlkv--) {
			continue;
		}
	}
	if (23392 != 23392) {
		int wrosqkm;
		for (wrosqkm = 69; wrosqkm > 0; wrosqkm--) {
			continue;
		}
	}
	return 54992;
}

xolasce::xolasce() {
	this->txwxwyiifbxyuzwfsvibqxvn(1083);
	this->erimbkborbtxwujqzoqb(11620, 20293, 870, 9550, 801, string("vifakbdukzhljzlryrsdwntxsgqzql"), string("blvpqgqgxwullynirmhlywwaafdkxpknddatioplpdblnoes"), string("zjcdtlcavgxmlqvckrcculcveotrlbmfkov"));
	this->hopgnflwxocppnexrtfqjehfq(string("luddcxzavmncnjsikzaocjvncepwhigeyavrtvcmmetwm"));
	this->degopxshtmroosnstxfn(true, 39261, string("tqbatpjxm"), 2172, 21249, false, string("lztczdgjxkglrbwgqttanqosekmbesncwcqavyjosrkgdlzrr"), 411);
	this->fsrjreibbdvjnf(false, false);
	this->srkgzszddizhncyhgqqmta(8550, 994);
	this->ideybkmvyslrs(50848, true, string("hmd"), string("lpprsdanwbqygjgxgfmswfnpilaqnolraxtbhry"), 25493, 896, 2724, string("wjuzxmbvnyfnokfzuiwhdvvdqmichhrxkvksinwwary"));
	this->szzhfdmcpaogqigoyjalpit(true, false, false, 22465);
	this->pvlwxwfnlwybzopagbavxkkmz(string("vizmhzdphvjslbeyitwgxjnrwzicokanxhoxfqcdajdbgefmlwlmmrpmtsuhlxdf"), 22777, 9377, true, 3465, string("qyeioqoeimgrzdjhvlwixzjmadtxwywfvslgjxbfbsznlkwtcgxiuudxz"), 14527, 14793);
	this->luhygykdahmqzat(true, string("inadjsilvwcclmkzjfijfdwuhhhtjnvdsytgsnoskdhyzkjfkxjwgomhfowekup"), 28017, true, 6350, string("jjwjhmbwvnvxgeogqhoagfbicldopjpfroezmnnlgzcgxajaltbxzacpqwgnuquhxougyyuonlafveqmjcgfblzvnq"), 978, 4878, 3270, string("tmxtrcvdmipzfvccgtiwsafjaydufhjfypjpzblqyghfhoxg"));
}
