#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"winmm.lib")

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <mmsystem.h>
#include <omp.h>
#include <tchar.h>
#include "Digitalv.h"

#include "resource.h"	

#include "MainStruct.h"
#include "Arraylist.h"
#include "Util.h"

#include "SoundSystem.h"
#include "CameraSystem.h"
#include "ColiderSystem.h"
#include "AnimationSystem.h"
#include "gameSystem.h"
#include "uiSystem.h"
#include "ShootingSystem.h"
#include "GunSystem.h"
#include "EnemySystem.h"
#include "MapSystem.h"
#include "InventorySystem.h"
#include "PlayerSystem.h"
#include "StageManager.h"
#include "ItemSystem.h"

#define INCRESED_SPEED 5
#define NOMAL_SPEED 5

//키
#define KEY_W 0x57
#define KEY_A 0x44
#define KEY_S 0x53
#define KEY_D 0x41
#define KEY_Q 0x51
#define KEY_E 0x45

#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33

#define KEY_SPACE 5

// 콘솔 윈도우 창의 핸들값
static HWND hWnd;
// 프로그램의 인스턴스 핸들값
static HINSTANCE hInst;
// 현재 윈도우의 Rect값(크기)를 얻어온다. Rect는 왼쪽위, 오른쪽 아래의 값을 가지는 정사각형을 나타내는 구조체이다.
static RECT WindowRect;

//UI 오브젝트가 그려질 캔버스
UI_OBJECT canvas[10];
int canvas_size;

//콜라이더 연산에 사용할 배열
COLIDER colider[100];
int colider_size;

HANDLE hStdin;
HANDLE hStdout;

INPUT_RECORD rec;
DWORD        dwNOER;

HHOOK hHook;

HDC hDC;
HDC hMemDC, hAlphaDC;
HDC hMapDC;
static HDC hBackDC;

BLENDFUNCTION blendFunction;

HBITMAP hOldBitmap, hBackBitmap;
HBITMAP hBitmap, hTempBitmap;

BITMAP Bitmap;
MAP test;

int drawing_done;

//키 입력 받는 함수
inline int KEY_DOWN(int vk_code)
{
	SHORT tmp = GetAsyncKeyState(vk_code);
	return ((tmp & 0x8000) || (tmp & 0x8001)) ? 1 : 0;
}

inline int KEY_DOWN_ONCE(int vk_code)
{
	SHORT tmp = GetAsyncKeyState(vk_code);
	return tmp & 0x0001;
}

typedef struct PALETE
{
	int start;
	int end;
	ArrayList bullet;
	CAMERA camera;
	HBITMAP hBitmap;
	HDC hMemDC;
	HDC hBackDC;
	HINSTANCE hInst;
} PALETE;

void Drawing_Thread(PALETE* arg) //Thread 선언
{
	BITMAP Bitmap;
	//총알 그리는 부분, 연산 부담 심함
	for (int i = arg->start; i < arg->end; i++)
	{
		arg->hBitmap = LoadBitmap(arg->hInst, MAKEINTRESOURCE(arg->bullet.bullet_list[i].type.sprite));

		GetObject(arg->hBitmap, sizeof(BITMAP), &Bitmap);
		SelectObject(arg->hMemDC, arg->hBitmap);
		GdiTransparentBlt(arg->hBackDC, arg->bullet.bullet_list[i].point.x - arg->camera.def.x, arg->bullet.bullet_list[i].point.y - arg->camera.def.y, Bitmap.bmWidth, Bitmap.bmHeight, arg->hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));

		DeleteObject(SelectObject(arg->hMemDC, arg->hBitmap));
		DeleteObject(arg->hBitmap);
	}
}

// 그래픽 렌더링
void Render(CAMERA camera, ArrayList* pool, UI_OBJECT canvas[], ArrayList* enemy, ArrayList* bullet, ArrayList* colider, HDC map, PLAYER playerStatus, int pool_size , int ui_size, int m_size, int new_item)
{
	int i;
	hBackBitmap = CreateCompatibleBitmap(hDC, WindowRect.right, WindowRect.bottom);
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);

	camera.def.x += camera.margin.x;
	camera.def.y += camera.margin.y;

	if(m_size)
		BitBlt(hBackDC, -1 * camera.def.x, -1 * camera.def.y, 5000, 5000, map, 0, 0, SRCCOPY);
	
	//PALETE pal;
	//pal.bullet = bullet;
	//pal.camera = camera;
	//pal.hBackDC = hBackDC;
	//pal.hBitmap = hBitmap;
	//pal.hInst = hInst;
	//pal.hMemDC = hMemDC;
	//
	//HANDLE handle[3];

	//pal.start = 0;
	//pal.end = bullet.size / 3;
	//handle[0] = _beginthread(Drawing_Thread, 0, &pal);
	//pal.start = pal.end;
	//pal.end = pal.start + bullet.size / 3;
	//handle[1] = _beginthread(Drawing_Thread, 0, &pal);
	//pal.start = pal.end;
	//pal.end = bullet.size;
	//handle[2] = _beginthread(Drawing_Thread, 0, &pal);

	
	//적 그리는 부분
//#pragma omp parallel for
	for (i = 0; i < enemy->size; i++)
	{
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(enemy->enemy_list[i].sprite));

		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		SelectObject(hMemDC, hBitmap);
		GdiTransparentBlt(hBackDC, enemy->enemy_list[i].point.x - camera.def.x, enemy->enemy_list[i].point.y - camera.def.y, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
			
		DeleteObject(SelectObject(hMemDC, hBitmap));
		DeleteObject(hBitmap);
	}
	

	//게임 오브젝트 그리는 부분
//#pragma omp parallel for
	for (i = 1; i < pool_size; i++)
	{
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(pool->game_list[i].sprite));

		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		SelectObject(hMemDC, hBitmap);
		GdiTransparentBlt(hBackDC, pool->game_list[i].point.x - camera.def.x, pool->game_list[i].point.y - camera.def.y, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));

		DeleteObject(SelectObject(hMemDC, hBitmap));
		DeleteObject(hBitmap);
	}

	i = 0;

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(pool->game_list[i].sprite));

	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hBitmap);
	GdiTransparentBlt(hBackDC, pool->game_list[i].point.x + camera.def.x - camera.margin.x * 2, pool->game_list[i].point.y + camera.def.y - camera.margin.y * 2, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));

	DeleteObject(SelectObject(hMemDC, hBitmap));
	DeleteObject(hBitmap);

	//총알 그리는 부분, 연산 부담 심함
//#pragma omp parallel for
	
	for (i = 0; i < bullet->size; i++)
	{
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(bullet->bullet_list[i].type.sprite));
	
		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		SelectObject(hMemDC, hBitmap);
		GdiTransparentBlt(hBackDC, bullet->bullet_list[i].point.x - camera.def.x, bullet->bullet_list[i].point.y - camera.def.y, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
	
		DeleteObject(SelectObject(hMemDC, hBitmap));
		DeleteObject(hBitmap);
	}

	//WaitForSingleObject(handle,INFINITE);
	//WaitForMultipleObjects(3,handle,TRUE,INFINITE);

	//UI 그리는 부분
//#pragma omp parallel for
	for (i = 0; i < ui_size; i++)
	{
		switch (canvas[i].type)
		{
			case UI_TEXT:
				SetBkMode(hBackDC,TRANSPARENT);
				SetTextColor(hBackDC, canvas[i].text.color);
				TextOut(hBackDC, canvas[i].text.point.x, canvas[i].text.point.y, canvas[i].text.text, lstrlen(canvas[i].text.text));
				break;
			case UI_BACKGROUND:
				hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(canvas[i].background.image));

				SelectObject(hMemDC, hBitmap);
				BitBlt(hBackDC, 0, 0, WindowRect.right, WindowRect.bottom, hMemDC, 0, 0, SRCCOPY);

				DeleteObject(SelectObject(hMemDC, hBitmap));
				DeleteObject(hBitmap);
				break;
			case UI_BUTTON:
				hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(canvas[i].button.image));
				GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
				SelectObject(hMemDC, hBitmap);

				hTempBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(canvas[i].button.image));
				GetObject(hTempBitmap, sizeof(BITMAP), &Bitmap);
				SelectObject(hAlphaDC, hTempBitmap);

				blendFunction.BlendOp = AC_SRC_OVER;
				blendFunction.BlendFlags = 0;
				blendFunction.AlphaFormat = 0;
				blendFunction.SourceConstantAlpha = canvas[i].button.alpha;

				BitBlt(hAlphaDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hBackDC, canvas[i].button.point.x, canvas[i].button.point.y, SRCCOPY);
				GdiTransparentBlt(hAlphaDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
				GdiAlphaBlend(hBackDC, canvas[i].button.point.x, canvas[i].button.point.y, Bitmap.bmWidth, Bitmap.bmHeight, hAlphaDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, blendFunction);
				
				DeleteObject(SelectObject(hMemDC, hBitmap));
				DeleteObject(SelectObject(hAlphaDC, hTempBitmap));
				DeleteObject(hBitmap);
				break;
			default:
				break;
		}
	}

	//총 그리는 부분
	if (!playerStatus.inv.e_blank[playerStatus.gunInvCursor])
	{
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(playerStatus.curGun.sprite));

		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		SelectObject(hMemDC, hBitmap);
		GdiTransparentBlt(hBackDC, pool->game_list[0].point.x + camera.def.x + 50 - camera.margin.x * 2, pool->game_list[0].point.y + camera.def.y + 30 - camera.margin.y * 2, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));

		DeleteObject(SelectObject(hMemDC, hBitmap));
		DeleteObject(hBitmap);

		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(playerStatus.curGun.image));

		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		SelectObject(hMemDC, hBitmap);
		GdiTransparentBlt(hBackDC, WindowRect.right - 30 - Bitmap.bmWidth, WindowRect.bottom - 30 - Bitmap.bmHeight, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));

		DeleteObject(SelectObject(hMemDC, hBitmap));
		DeleteObject(hBitmap);
	}

	for (i = 0; i < 3; i++)
	{
		if (!playerStatus.inv.e_blank[i])
		{
			hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(playerStatus.inv.equipGun[i].sprite));

			GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hBitmap);
			GdiTransparentBlt(hBackDC, WindowRect.right - 30 - Bitmap.bmWidth, WindowRect.bottom - 50 * (i + 2) - 150, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));

			DeleteObject(SelectObject(hMemDC, hBitmap));
			DeleteObject(hBitmap);
		}
	}

	//피 통 그리는 부분
    //#pragma omp parallel for
	for (i = 0; i < playerStatus.maxhealth; i++)
	{
		if(playerStatus.health <= i)
			hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_EMPTHY_HEART));
		else
			hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FULL_HEART));

		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		POINT tmp = cp((Bitmap.bmWidth * i) + 10, 10);

		SelectObject(hMemDC, hBitmap);
		GdiTransparentBlt(hBackDC, tmp.x, tmp.y, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));

		DeleteObject(SelectObject(hMemDC, hBitmap));
		DeleteObject(hBitmap);
	}


	if (new_item)
	{
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NEW_ITEM));

		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		SelectObject(hMemDC, hBitmap);
		GdiTransparentBlt(hBackDC, 522, 735, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));

		DeleteObject(SelectObject(hMemDC, hBitmap));
		DeleteObject(hBitmap);
	}

	//콜라이더 시각화
	//for (int i = 0; i < colider->size; i++)
	//{
	//	COLIDER col = colider->colider_list[i];
	//	Rectangle(hBackDC, col.start_x - camera.def.x, col.start_y - camera.def.y, col.end_x - camera.def.x , col.end_y - camera.def.y);
	//}

	//전체 그리는 부분
	BitBlt(hDC, 0, 0, WindowRect.right, WindowRect.bottom, hBackDC, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hBackDC, hOldBitmap));
}

// 그래픽 렌더링 카메라 없음
void RenderNoCamera(GAME_OBJECT pool[], UI_OBJECT canvas[], ENEMY enemy[], ArrayList bullet, int pool_size, int enemy_size, int ui_size)
{
	hBackBitmap = CreateCompatibleBitmap(hDC, WindowRect.right, WindowRect.bottom);
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);

	//for (int i = 0; i < enemy_size; i++)
	//{
	//	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(enemy[i].sprite));
	//
	//	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	//	SelectObject(hMemDC, hBitmap);
	//	GdiTransparentBlt(hBackDC, enemy[i].point.x, enemy[i].point.y, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
	//
	//	DeleteObject(SelectObject(hMemDC, hBitmap));
	//	DeleteObject(hBitmap);
	//}
	//
	//for (int i = 0; i < pool_size; i++)
	//{
	//	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(pool[i].sprite));
	//
	//	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	//	SelectObject(hMemDC, hBitmap);
	//	GdiTransparentBlt(hBackDC, pool[i].point.x, pool[i].point.y, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
	//
	//	DeleteObject(SelectObject(hMemDC, hBitmap));
	//	DeleteObject(hBitmap);
	//}

	//for (int i = 0; i < bullet.size; i++)
	//{
	//	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(bullet.bullet_list[i].type.sprite));
	//
	//	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	//	SelectObject(hMemDC, hBitmap);
	//	GdiTransparentBlt(hBackDC, bullet.bullet_list[i].point.x, bullet.bullet_list[i].point.y, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
	//
	//	DeleteObject(SelectObject(hMemDC, hBitmap));
	//	DeleteObject(hBitmap);
	//}

	for (int i = 0; i < ui_size; i++)
	{
		switch (canvas[i].type)
		{
		case UI_TEXT:
			AddFontResourceA("upheavtt.ttf"); // 2. 정보 설정하기 
			HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));
			HFONT OldFont = (HFONT)SelectObject(hBackDC, hFont);

			SetBkMode(hBackDC, TRANSPARENT);
			SetTextColor(hBackDC, canvas[i].text.color);
			TextOut(hBackDC, canvas[i].text.point.x, canvas[i].text.point.y, canvas[i].text.text, lstrlen(canvas[i].text.text));
			break;
		case UI_BACKGROUND:
			hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(canvas[i].background.image));

			SelectObject(hMemDC, hBitmap);
			BitBlt(hBackDC, 0, 0, WindowRect.right, WindowRect.bottom, hMemDC, 0, 0, SRCCOPY);

			DeleteObject(SelectObject(hMemDC, hBitmap));
			DeleteObject(hBitmap);
			break;
		case UI_BUTTON:
			hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(canvas[i].button.image));
			GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hBitmap);

			hTempBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(canvas[i].button.image));
			GetObject(hTempBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hAlphaDC, hTempBitmap);

			blendFunction.BlendOp = AC_SRC_OVER;
			blendFunction.BlendFlags = 0;
			blendFunction.AlphaFormat = 0;
			blendFunction.SourceConstantAlpha = canvas[i].button.alpha;

			BitBlt(hAlphaDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hBackDC, canvas[i].button.point.x, canvas[i].button.point.y, SRCCOPY);
			GdiTransparentBlt(hAlphaDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
			GdiAlphaBlend(hBackDC, canvas[i].button.point.x, canvas[i].button.point.y, Bitmap.bmWidth, Bitmap.bmHeight, hAlphaDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, blendFunction);

			DeleteObject(SelectObject(hMemDC, hBitmap));
			DeleteObject(SelectObject(hAlphaDC, hTempBitmap));
			DeleteObject(hBitmap);
			break;
		default:
			break;
		}
	}

	BitBlt(hDC, 0, 0, WindowRect.right, WindowRect.bottom, hBackDC, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hBackDC, hOldBitmap));
}

// 그래픽 렌더링 급한 관계로 이렇게 함
void RenderInven(int page, INVENTORY inv)
{
	hBackBitmap = CreateCompatibleBitmap(hDC, WindowRect.right, WindowRect.bottom);
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);

	//인벤토리 창 그림
	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(page));

	SelectObject(hMemDC, hBitmap);
	BitBlt(hBackDC, 0, 0, WindowRect.right, WindowRect.bottom, hMemDC, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hMemDC, hBitmap));
	DeleteObject(hBitmap);

	for (int i = 0; i < 3 && page == GUN_PAGE; i++)
	{
		if (!inv.e_blank[i])
		{
			hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(inv.equipGun[i].sprite));

			GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hBitmap);
			GdiTransparentBlt(hBackDC, 1305 + (138 * i) - (Bitmap.bmWidth/2), 254 + 38, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));

			DeleteObject(SelectObject(hMemDC, hBitmap));
			DeleteObject(hBitmap);
		}
	}

	for (int i = 0; i < inv.invSize && page == GUN_PAGE; i++)
	{
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(inv.invGun[i].sprite));

		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		SelectObject(hMemDC, hBitmap);
		GdiTransparentBlt(hBackDC, 1305 + (138 * (i % 3)) - (Bitmap.bmWidth / 2), 254 + 170 + (70 * (i / 3)), Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));

		DeleteObject(SelectObject(hMemDC, hBitmap));
		DeleteObject(hBitmap);
	}

	for (int i = 0; i < inv.invItemSize && page == ITEM_PAGE; i++)
	{
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(inv.invItem[i].sprite));

		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		SelectObject(hMemDC, hBitmap);
		GdiTransparentBlt(hBackDC, 1305 + (138 * (i % 3)) - (Bitmap.bmWidth / 2), 254 + 20 +(70 * (i / 3)), Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));

		DeleteObject(SelectObject(hMemDC, hBitmap));
		DeleteObject(hBitmap);
	}

	BitBlt(hDC, 0, 0, WindowRect.right, WindowRect.bottom, hBackDC, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hBackDC, hOldBitmap));
}


//시스템 초기 설정 (전체화면, 커서 숨김 등)
void Init()
{
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD prev_mode;
	GetConsoleMode(hStdin, &prev_mode);
	SetConsoleMode(hStdin, ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE) | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	hWnd = GetConsoleWindow();

	SendMessage(hWnd, WM_SYSKEYDOWN, VK_RETURN, 0x20000000);

	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hStdout, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(hStdout, new_size);

	hDC = GetDC(hWnd);
	hMemDC = CreateCompatibleDC(hDC);
	hAlphaDC = CreateCompatibleDC(hDC);
	hBackDC = CreateCompatibleDC(hDC);
	hInst = GetModuleHandle(NULL);
	GetWindowRect(hWnd, &WindowRect);
	
	setcursortype(NOCURSOR);
	Sleep(1000);
}

//게임 종료 DC해제하고 프로그램 종료
void exitGame()
{
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);
	DeleteDC(hAlphaDC);

	SendMessage(hWnd, WM_CLOSE, 0, 0);
}

// 프로그램 진입점
void main(void)
{
	Init();

	srand(time(NULL));

	// 하드 -- 코딩

	//풀 배열
	ArrayList poolList;
	ArrayList * ptrPool = &poolList;
	initList(ptrPool);

	//총알 배열
	ArrayList bulletList;
	ArrayList* ptrBullet = &bulletList;
	initList(ptrBullet);

	//적 배열
	ArrayList enemyList;
	ArrayList* ptrEnemy = &enemyList;
	initList(ptrEnemy);

	//콜라이더 배열
	ArrayList coliders;
	ArrayList* ptrCol = &coliders;
	initList(ptrCol);

	POINT player_pos = { -800, -100 };
	int direction = IDB_CAT_PLAYER;

	//속도
	int speed = NOMAL_SPEED;

	COLIDER tmp = { 100, 200, 175, 283 }; // 100, 200, 175, 283
	
	POINT size = getBitmapSize(hInst, IDB_CAT_PLAYER);
	POINT s_size = { WindowRect.right, WindowRect.bottom };
	POINT pos = getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_CAT_PLAYER), s_size);

	canvas_size = 3;

	canvas[0] = createBackground(IDB_TITLE_BACKGROUND);
	canvas[1] = createButton(
		getColider(hInst, getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_TITLE_START), s_size), 260), IDB_TITLE_START),
		getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_TITLE_START), s_size),260),
		IDB_TITLE_START,
		255);
	canvas[2] = createButton(
		getColider(hInst, getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_TITLE_QUIT), s_size), 370), IDB_TITLE_QUIT),
		getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_TITLE_QUIT), s_size), 370),
		IDB_TITLE_QUIT,
		130);

	FILE* fp = fopen("highscore.txt", "rt");

	if (fp != NULL)
	{
		canvas_size = 4;
		char s1[10];
		int best;
		fscanf(fp, "%d", &best);
		sprintf(s1, "지금까지 도달한 최저 층수는 B%d층 입니다.", best);
		canvas[3] = createTextView(cp(50, 100), s1, RGB(255, 0, 0));
	}

	//char s1[10];
	//
	//sprintf(s1, "지금 까지 도달한 최저층 B%d층", );
	//canvas[3] = createTextView(cp(50, 100), s1, RGB(255,0,0));

	// 애니메이션은 int 배열입니다.
	// 이미지 리소스를 나중에 불러오기 위해 만들어졌습니다.
	// 이 타이틀 애니메이션의 리소스 크기는 각각

	ANIMATION title_animation = createAnimation(
		2,
		IDB_TITLE_1,
		IDB_TITLE_2);

	ANIMATION_TASK title_animation_task = createAnimationTask(title_animation, 3);

	// 하드 -- 코딩

	//맵 테스트
	test = createMap();
	MAP_TILE rock_tile = createMapTile(IDB_CARPET, 0);
	MAP_TILE rock_tile_floor = createMapTile(IDB_CARPET_S, 0);

	MAP_TILE rock_wall = createMapTile(IDB_MIDDLE_WALL, 1);
	MAP_TILE rock_wall_shadow = createMapTile(IDB_MIDDLE_WALL_S, 0);
	MAP_TILE rock_wall_down = createMapTile(IDB_MIDDLE_D, 1);
	MAP_TILE rock_wall_up = createMapTile(IDB_MIDDLE_U, 1);
	MAP_TILE rock_wall_left = createMapTile(IDB_MIDDLE_L, 1);
	MAP_TILE rock_wall_right = createMapTile(IDB_MIDDLE_R, 1);

	MAP_TILE rock_wall_u_r = createMapTile(IDB_MIDDLE_R_U, 1);
	MAP_TILE rock_wall_u_l = createMapTile(IDB_MIDDLE_L_U, 1);
	MAP_TILE rock_wall_d_r = createMapTile(IDB_MIDDLE_D_R, 1);
	MAP_TILE rock_wall_d_l = createMapTile(IDB_MIDDLE_L_D, 1);

	MAP_ROOM_COMPONENT rock_tiles = createMapRoomComponent(rock_tile, rock_tile_floor, rock_wall, rock_wall_shadow, rock_wall_down, rock_wall_up, rock_wall_left, rock_wall_right, rock_wall_d_l, rock_wall_u_l, rock_wall_d_r, rock_wall_u_r);

	HDC map = CreateCompatibleDC(hDC);
	
	int selected = 0;
	//메인화면 - 고정
	PlaySoundCUS("title_ost",1,1);
	while (1) 
	{
		POINT cursorPosition;
		GetCursorPos(&cursorPosition);

		if (title_animation_task.cursor == title_animation_task.animation.size - 1)
		{
			title_animation_task.cursor = 0;
			title_animation_task.timer = 0;
		}
		canvas[0].background.image = showAnimation(&title_animation_task);

		if (selected == 1 && (isClicked(cursorPosition, canvas[1].button.colider) || (selected != 0 && KEY_DOWN(VK_UP))))
		{
			canvas[1].button.alpha = 255;
			canvas[2].button.alpha = 128;
			selected = 0;
			PlaySoundWAVE("select", 3, 0);
		}
		else if (selected == 0 && (isClicked(cursorPosition, canvas[2].button.colider) || (selected != 1 && KEY_DOWN(VK_DOWN))))
		{
			canvas[1].button.alpha = 128;
			canvas[2].button.alpha = 255;
			selected = 1;
			PlaySoundWAVE("select", 3, 0);
		}

		if (KEY_DOWN(VK_LBUTTON))
		{
			PlaySoundWAVE("click", 4, 0);
			if (isClicked(cursorPosition, canvas[1].button.colider))
				break;
			else if (isClicked(cursorPosition, canvas[2].button.colider))
				exitGame();
		}
		else if (KEY_DOWN(VK_RETURN))
		{
			PlaySoundWAVE("click", 4, 0);
			if (selected == 0)
				break;
			else if (selected == 1)
				exitGame();
		}
	
		RenderNoCamera(ptrPool->game_list, canvas, ptrEnemy->enemy_list, bulletList, poolList.size, ptrEnemy->size, canvas_size);
		Sleep(0);
	}
	StopSoundCUS(1);
	//Shoot Manage
	int isShoot = 0;

	//애니메이션 생성부
	ANIMATION hurt_cat = createAnimation(
		8,
		IDB_CAT_HURT_1,
		IDB_CAT_HURT_2,
		IDB_CAT_HURT_3,
		IDB_CAT_HURT_4,
		IDB_CAT_HURT_1,
		IDB_CAT_HURT_2,
		IDB_CAT_HURT_3,
		IDB_CAT_HURT_4);

	ANIMATION_TASK hurt_cat_task = createAnimationTask(hurt_cat, 8);

	ANIMATION blink_drone = createAnimation(
		5,
		IDB_MULCHI_HURT_1,
		IDB_MULCHI_HURT_2,
		IDB_MULCHI_HURT_3,
		IDB_MULCHI_HURT_1,
		IDB_MULCHI_HURT_3,
		IDB_MULCHI_HURT_1);

	ANIMATION_TASK blink_task = createAnimationTask(blink_drone, 5);

	ANIMATION droni_idle = createAnimation(
		4,
		IDB_MULCHI,
		IDB_MULCHI_WALK_1,
		IDB_MULCHI_WALK_2,
		IDB_MULCHI_WALK_1);

	ANIMATION_TASK droni_idle_task = createAnimationTask(droni_idle, 20);

	ANIMATION droni_death = createAnimation(
		10,
		IDB_MULCHI,
		IDB_DEATH_1,
		IDB_DEATH_2,
		IDB_DEATH_3,
		IDB_DEATH_4,
		IDB_DEATH_5,
		IDB_DEATH_6,
		IDB_DEATH_7,
		IDB_DEATH_8,
		IDB_DEATH_9,
		IDB_DEATH_10,
		IDB_DEATH_11);

	ANIMATION_TASK droni_death_task = createAnimationTask(droni_death, 5);

	ANIMATION nup_idle = createAnimation(
		4,
		IDB_NUP_MOVE_1,
		IDB_NUP_MOVE_2,
		IDB_NUP_MOVE_3,
		IDB_NUP_MOVE_1);

	ANIMATION_TASK nup_idle_task = createAnimationTask(nup_idle, 20);

	ANIMATION nup_hurt = createAnimation(
		4,
		IDB_NUP_HURT_1,
		IDB_NUP_HURT_2,
		IDB_NUP_HURT_3,
		IDB_NUP_HURT_1);

	ANIMATION_TASK nup_hurt_task = createAnimationTask(nup_hurt, 5);

	ANIMATION mideduk_blink = createAnimation(
		5,
		IDB_MIDEDUK_HURT_1,
		IDB_MIDEDUK_HURT_2,
		IDB_MIDEDUK_HURT_3,
		IDB_MIDEDUK_HURT_1,
		IDB_MIDEDUK_HURT_2,
		IDB_MIDEDUK_HURT_3);

	ANIMATION_TASK mideduk_blink_task = createAnimationTask(mideduk_blink, 5);

	ANIMATION mideduk_idle = createAnimation(
		4,
		IDB_MIDEDUK_MOVE_1,
		IDB_MIDEDUK_MOVE_2,
		IDB_MIDEDUK_MOVE_1,
		IDB_MIDEDUK_MOVE_2);

	ANIMATION_TASK mideduk_idle_task = createAnimationTask(mideduk_idle, 20);

	ANIMATION mideduk_death = createAnimation(
		7,
		IDB_EXPLOSION_1,
		IDB_EXPLOSION_2,
		IDB_EXPLOSION_3,
		IDB_EXPLOSION_4,
		IDB_EXPLOSION_5,
		IDB_EXPLOSION_6,
		IDB_EXPLOSION_7,
		IDB_EXPLOSION_8);

	ANIMATION_TASK mideduk_death_task = createAnimationTask(mideduk_death, 8);

	ANIMATION bullet_ex = createAnimation(
		6,
		IDB_RED_BULLET_EX_1,
		IDB_RED_BULLET_EX_3,
		IDB_RED_BULLET_EX_4,
		IDB_RED_BULLET_EX_5,
		IDB_RED_BULLET_EX_6,
		IDB_RED_BULLET_EX_7,
		IDB_RED_BULLET_EX_8);

	ANIMATION_TASK task = createAnimationTask(bullet_ex, 3);

	//패턴 제작
	POINT p = { 500,400 };

	STATE_PATTERN state;
	state.size = 0;
	state.cursor = 0;

	STATE follow_player = createState(STATE_FOLLOW_PLAYER, 40);
	STATE shoot_to_player = createState(STATE_SHOOT_TO_PLAYER, 20);
	STATE shoot_player_with_dist = createState(STATE_FOLLOW_PLAYER_WITH_DISTANCE, 40);
	STATE move_random = createState(STATE_MOVE_RANDOM, 20);

	addStatePattern(&state, shoot_player_with_dist);
	addStatePattern(&state, shoot_to_player);
	addStatePattern(&state, shoot_player_with_dist);
	addStatePattern(&state, shoot_to_player);

	// ENEMY GUN 선언 탄 퍼짐, 속도, 쿨타임, 탄창개수, 데미지, 넉백
	BULLET_TYPE enemy_gun_type = createBulletType(IDB_BULLET, 1, 3, 60, 3, 3, 10, task);
	GUN enemy_gun = createGun(enemy_gun_type, IDB_IMAGE_BEAN_GUN, IDB_GUN_BEAN, "gun_shoot_light");

	BULLET_TYPE nupchi_gun_type = createBulletType(IDB_YELLOW_BULLET, 200, 5, 20, 3, 3, 10, task);
	GUN nupchi_gun = createGun(nupchi_gun_type, IDB_IMAGE_ASSERT_GUN, IDB_GUN_ASSERT, "gun_shoot_light");

	//적 생성
	ENEMY_TYPE Droni = createEnemyType(50, IDB_MULCHI, enemy_gun, blink_task, droni_idle_task, droni_death_task);
	ENEMY_TYPE Mideduk = createEnemyType(100, IDB_MIDEDUK, enemy_gun, mideduk_blink_task, mideduk_idle_task, mideduk_death_task);
	ENEMY_TYPE Nup = createEnemyType(30, IDB_NUP_MOVE_1, nupchi_gun, nup_hurt_task, nup_idle_task, droni_death_task);

	//총알 타입 선언 탄 퍼짐, 속도, 쿨타임, 탄창개수, 데미지, 넉백
	BULLET_TYPE bean_gun_type = createBulletType(IDB_BULLET, 15, 4, 16, 3, 10, 15, task);
	BULLET_TYPE assert_gun_type = createBulletType(IDB_YELLOW_BULLET, 100, 10, 10, 3, 300, 15, task);
	BULLET_TYPE ice_gun_type = createBulletType(IDB_ICE_BULLET, 0, 25, 100, 1, 1000, 5, task);
	BULLET_TYPE wood_gun_type = createBulletType(IDB_WOOD_BULLET, 0, 5, 50, 1, 1000, 5, task);

	//총 생성
	GUN bean_gun = createGun(bean_gun_type, IDB_IMAGE_BEAN_GUN, IDB_GUN_BEAN, "gun_shoot_very_light");
	GUN ice_gun = createGun(ice_gun_type, IDB_IMAGE_ICE_GUN, IDB_GUN_ICE, "gun_shoot_light");
	GUN assert_gun = createGun(assert_gun_type, IDB_IMAGE_ASSERT_GUN, IDB_GUN_ASSERT, "gun_shoot_light");
	GUN wood_gun = createGun(wood_gun_type, IDB_WOOD_GUN_IMAGE, IDB_WOOD_GUN, "gun_shoot_light");

	ITEM speed_ring = createItem(IDB_ITEM_SPEED_RING, SPEED_UP);
	ITEM health_bag = createItem(IDB_HEART_BAG, HEALTH_UP);

	CAMERA camera;

	GAME_OBJECT player;
	GAME_OBJECT stair;

	GAME_OBJECT* ptrPlayer = &(ptrPool->game_list[0]);
	GAME_OBJECT* ptrStair = &(ptrPool->game_list[1]);

	HBITMAP hBackBitmap;

	COLIDER player_col;

	GAME_INFO game_info;

	PLAYER playerStatus;

	ENEMYS enemys;

	clock_t st, et;

	int pool_size = 1;

restart:

	//카메라 생성
	player_pos = cp( -800, -100 );

	pos = getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_CAT_PLAYER), s_size);

	camera = createCamera(pos);
	attachToCamera(&camera, &(ptrPool->game_list[0]));

	//게임오브젝트에 플레이어 생성
	player.colider = tmp;
	player.point = pos;
	player.sprite = IDB_CAT_PLAYER;

	addGameObjectItem(ptrPool, player);

	stair.colider = getColider(hInst, cp(120, 320), IDB_STAIR);
	stair.point = cp(120, 320);
	stair.sprite = IDB_STAIR;
	addGameObjectItem(ptrPool, stair);

	canvas_size = 0;

	player_col = getColider(hInst, player_pos, getGameObjectItem(ptrPool, 0).sprite);

	// 액션 부분 테스트

	playerStatus = createPlayer(5,5,bean_gun);

	game_info = startGame();

	enemys.DRONI = Droni;
	enemys.Mideduk = Mideduk;
	enemys.Nup = Nup;

	test = createMap();
	map = CreateCompatibleDC(hDC);

	beginStage(hInst,&test, ptrCol, ptrEnemy, &game_info, enemys, rock_tiles);

	hBackBitmap = CreateCompatibleBitmap(hDC, 5000, 5000);
	SelectObject(map, hBackBitmap);

	drawMap(hInst, hDC, &map, test);

	st = clock();
	et = clock();

	game_info.stage = 10;

	int new_item = 0;

	PlaySoundCUS("ost2", 1, 1);
	while (1)
	{
		speed = playerStatus.speed;

		//FPS 표시
		char s1[10];
		sprintf(s1, "%d FPS", 1000 / (et - st + 1));
		canvas[0] = createTextView(cp(50, 100), s1, RGB(255,0,0));
		st = clock();

		//애니메이션 처리 때문에 여기 계심
		ptrPlayer->sprite = IDB_CAT_PLAYER;

		//플레이어 위치를 실제 물리적 위치와 동기화 시킴
		ptrPool->game_list[0].point = addPoint(camera.anchor, camera.def);

		//커서 위치 구하기
		POINT cursorPosition;
		GetCursorPos(&cursorPosition);
		cursorPosition = addPoint(cursorPosition, camera.def);
		playerStatus.cursor = cursorPosition;

		//게임오버
		if (playerStatus.health <= 0)
		{
			FILE* fp = fopen("highscore.txt", "wt");
			fprintf(fp, "%d", game_info.stage);
			fclose(fp);

			StopSoundCUS(1);
			PlaySoundCUS("ost", 1, 1);

			canvas_size = 3;
			canvas[0] = createBackground(IDB_GAMEOVER);
			canvas[1] = createButton(
				getColider(hInst, getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_RETRY), s_size), 270), IDB_RETRY),
				getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_RETRY), s_size), 270),
				IDB_RETRY,
				255);
			canvas[2] = createButton(
				getColider(hInst, getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_TITLE_QUIT), s_size), 370), IDB_TITLE_QUIT),
				getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_TITLE_QUIT), s_size), 370),
				IDB_TITLE_QUIT,
				130);

			while (1)
			{
				POINT cursorPosition;
				GetCursorPos(&cursorPosition);

				if (selected == 1 && (isClicked(cursorPosition, canvas[1].button.colider) || (selected != 0 && KEY_DOWN(VK_UP))))
				{
					canvas[1].button.alpha = 255;
					canvas[2].button.alpha = 128;
					selected = 0;
					PlaySoundWAVE("select", 3, 0);
				}
				else if (selected == 0 && (isClicked(cursorPosition, canvas[2].button.colider) || (selected != 1 && KEY_DOWN(VK_DOWN))))
				{
					canvas[1].button.alpha = 128;
					canvas[2].button.alpha = 255;
					selected = 1;
					PlaySoundWAVE("select", 3, 0);
				}

				if (KEY_DOWN(VK_LBUTTON))
				{
					PlaySoundWAVE("click", 4, 0);
					if (isClicked(cursorPosition, canvas[1].button.colider))
					{
						StopSoundCUS(1);
						pool_size = 1;
						
						initList(ptrBullet);
						initList(ptrEnemy);
						initList(ptrPool);
						initList(ptrCol);

						goto restart;
					}
					else if (isClicked(cursorPosition, canvas[2].button.colider))
						exitGame();
				}
				else if (KEY_DOWN(VK_RETURN))
				{
					PlaySoundWAVE("click", 4, 0);
					if (selected == 0)
					{
						StopSoundCUS(1);
						pool_size = 1;

						initList(ptrBullet);
						initList(ptrEnemy);
						initList(ptrPool);
						initList(ptrCol);

						goto restart;
					}
					else if (selected == 1)
						exitGame();
				}

				RenderNoCamera(ptrPool->game_list, canvas, ptrEnemy->enemy_list, bulletList, 0, 0, canvas_size);
			}
		}

		//적 총알에 맞으면
		COLIDE_INFO col_info = isColideForPlayerAndBullet(ptrPlayer->colider,ptrBullet, ptrEnemy);
		if (col_info.isColide && hurt_cat_task.timer == 0)
		{
			playerStatus.health -= 1;
			ptrPlayer->sprite = showAnimation(&hurt_cat_task);
		}
		else if (hurt_cat_task.timer != 0 && hurt_cat_task.cursor != hurt_cat_task.animation.size - 1)
		{\
			ptrPlayer->sprite = showAnimation(&hurt_cat_task);
		}
		else if (hurt_cat_task.cursor == hurt_cat_task.animation.size - 1)
		{
			ptrPlayer->sprite = IDB_CAT_PLAYER;
			hurt_cat_task.cursor = 0;
			hurt_cat_task.timer = 0;
		}
		
		//동시 입력 처리 대각선 방향
		if (KEY_DOWN(KEY_W) && KEY_DOWN(KEY_A)) //w + a
		{
			if(isColideWithMoving(ptrPlayer, cp(0, -speed * 2), ptrCol))
				player_pos.y -= speed;
			if(isColideWithMoving(ptrPlayer, cp(speed * 2, 0), ptrCol))
				player_pos.x += speed;
			//direction = IDB_CHAR_UP;
		}
		else if (KEY_DOWN(KEY_S) && KEY_DOWN(KEY_A)) //s + a
		{
			if (isColideWithMoving(ptrPlayer, cp(0, speed * 2), ptrCol))
				player_pos.y += speed;
			if (isColideWithMoving(ptrPlayer, cp(speed * 2, 0), ptrCol))
				player_pos.x += speed;
			//direction = IDB_CHAR_DOWN;
		}
		else if (KEY_DOWN(KEY_W) && KEY_DOWN(KEY_D)) // w + d
		{
			if (isColideWithMoving(ptrPlayer, cp(0, -speed * 2), ptrCol))
				player_pos.y -= speed;
			//direction = IDB_CHAR_UP;
			if (isColideWithMoving(ptrPlayer, cp(-speed * 2, 0), ptrCol))
				player_pos.x -= speed;
		}
		else if (KEY_DOWN(KEY_S) && KEY_DOWN(KEY_D)) // s + d
		{
			if (isColideWithMoving(ptrPlayer, cp(0, speed * 2), ptrCol))
				player_pos.y += speed;
			if (isColideWithMoving(ptrPlayer, cp(-speed * 2, 0), ptrCol))
				player_pos.x -= speed;
			//direction = IDB_CHAR_DOWN;
		}

		//일반 입력 처리 WASD
		else if (KEY_DOWN(KEY_W) && isColideWithMoving(ptrPlayer, cp(0, -speed * 2),ptrCol)) //w
		{
			player_pos.y -= speed;
			//direction = IDB_CHAR_UP;
		}
		else if (KEY_DOWN(KEY_S) && isColideWithMoving(ptrPlayer, cp(0, speed * 2), ptrCol)) //s
		{
			player_pos.y += speed;
			//direction = IDB_CHAR_DOWN;
		}
		else if (KEY_DOWN(KEY_D) && isColideWithMoving(ptrPlayer, cp(-speed * 2, 0), ptrCol)) // d
		{
			player_pos.x -= speed;
			//direction = IDB_CHAR_LEFT;
		}
		else if (KEY_DOWN(KEY_A) && isColideWithMoving(ptrPlayer, cp(speed * 2, 0), ptrCol))
		{
			player_pos.x += speed;
			//direction = IDB_CHAR_RIGHT;
		}

		//총 변경
		if (KEY_DOWN_ONCE(KEY_Q))
		{
			switchGun(&playerStatus, -1);
		}

		if (KEY_DOWN_ONCE(KEY_E))
		{
			switchGun(&playerStatus, 1);
		}

		//인벤토리 창
		if (KEY_DOWN_ONCE(VK_TAB))
		{
			new_item = 0;
			PlaySoundCUS("popup", 3, 0);

			int page = GUN_PAGE;
			COLIDER gun_page_button = {1132, 340, 1214, 405};
			COLIDER item_page_button = {1132, 423, 1214, 498};

			COLIDER gun_1 = {1251, 250, 1381, 350};
			COLIDER gun_2 = {1389, 250, 1502, 350 };
			COLIDER gun_3 = {1527, 250, 1643, 350};

			while (1)
			{
				POINT cursorPosition;
				GetCursorPos(&cursorPosition);

				if (KEY_DOWN_ONCE(KEY_Q) || (isClicked(cursorPosition, gun_page_button) && KEY_DOWN(VK_LBUTTON)))
				{
					PlaySoundWAVE("select", 3, 0);
					page = GUN_PAGE;
				}
				if (KEY_DOWN_ONCE(KEY_E) || (isClicked(cursorPosition, item_page_button) && KEY_DOWN(VK_LBUTTON)))
				{
					PlaySoundWAVE("select", 3, 0);
					page = ITEM_PAGE;
				}
				if (KEY_DOWN_ONCE(VK_ESCAPE))
				{
					PlaySoundCUS("popup", 3, 0);
					break;
				}
				if (KEY_DOWN_ONCE(VK_TAB))
				{
					PlaySoundCUS("popup", 3, 0);
					break;
				}

				if (page == GUN_PAGE && KEY_DOWN_ONCE(KEY_1))
				{
					for (int i = 0; i < playerStatus.inv.invSize; i++)
					{
						COLIDER gun = { 1251 + (138 * (i % 3)), 400 + (100 * (i / 3)), 1381 + (138 * (i % 3)) , 500 + (150 * (i / 3)) };
						if (isClicked(cursorPosition, gun))
						{
							playerStatus.inv.e_blank[0] = 0;
							playerStatus.inv.equipGun[0] = playerStatus.inv.invGun[i];
						}
					}
				}
				if (page == GUN_PAGE && KEY_DOWN_ONCE(KEY_2))
				{
					for (int i = 0; i < playerStatus.inv.invSize; i++)
					{
						COLIDER gun = { 1251 + (138 * (i % 3)), 400 + (100 * (i / 3)), 1381 + (138 * (i % 3)) , 500 + (150 * (i / 3)) };
						if (isClicked(cursorPosition, gun))
						{
							playerStatus.inv.e_blank[1] = 0;
							playerStatus.inv.equipGun[1] = playerStatus.inv.invGun[i];
						}
					}
				}
				if (page == GUN_PAGE && KEY_DOWN_ONCE(KEY_3))
				{
					for (int i = 0; i < playerStatus.inv.invSize; i++)
					{
						COLIDER gun = { 1251 + (138 * (i % 3)), 400 + (100 * (i / 3)), 1381 + (138 * (i % 3)) , 500 + (150 * (i / 3)) };
						if (isClicked(cursorPosition, gun))
						{
							playerStatus.inv.e_blank[2] = 0;
							playerStatus.inv.equipGun[2] = playerStatus.inv.invGun[i];
						}
					}
				}

				RenderInven(page, playerStatus.inv);
			}
		}

		//일시정지
		if (KEY_DOWN_ONCE(VK_ESCAPE))
		{
			canvas_size = 3;
			canvas[0] = createBackground(IDB_PAUSE);
			canvas[1] = createButton(
				getColider(hInst, getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_TITLE_CONTINUE), s_size), -50), IDB_TITLE_CONTINUE),
				getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_TITLE_CONTINUE), s_size), -50),
				IDB_TITLE_CONTINUE,
				255);
			canvas[2] = createButton(
				getColider(hInst, getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_TITLE_QUIT), s_size), 50), IDB_TITLE_QUIT),
				getMarinPoint(MARGIN_TOP, getAlignPoint(ALIGN_CENTER, getBitmapSize(hInst, IDB_TITLE_QUIT), s_size), 50),
				IDB_TITLE_QUIT,
				130);

			Sleep(20);
			StopSoundCUS(1);
			PlaySoundCUS("ost", 1, 1);
			while (1)
			{
				POINT cursorPosition;
				GetCursorPos(&cursorPosition);

				if (isClicked(cursorPosition, canvas[1].button.colider) || (selected != 0 && KEY_DOWN(VK_UP)))
				{
					canvas[1].button.alpha = 255;
					canvas[2].button.alpha = 128;
					selected = 0;
				}
				else if (isClicked(cursorPosition, canvas[2].button.colider) || (selected != 1 && KEY_DOWN(VK_DOWN)))
				{
					canvas[1].button.alpha = 128;
					canvas[2].button.alpha = 255;
					selected = 1;
				}

				if (KEY_DOWN(VK_LBUTTON))
				{
					if (isClicked(cursorPosition, canvas[1].button.colider))
					{
						Sleep(20);
						StopSoundCUS(1);
						PlaySoundCUS("ost2", 1, 1);
						break;
					}
					else if (isClicked(cursorPosition, canvas[2].button.colider))
						exitGame();
				}
				else if (KEY_DOWN(VK_RETURN))
				{
					if (selected == 0)
					{
						Sleep(20);
						StopSoundCUS(1);
						PlaySoundCUS("ost2", 1, 1);
						break;
					}
					else if (selected == 1)
						exitGame();
				}

				if (KEY_DOWN_ONCE(VK_ESCAPE))
				{
					Sleep(20);
					{
						Sleep(20);
						StopSoundCUS(1);
						PlaySoundCUS("ost2", 1, 1);
						break;
					}
				}
				RenderNoCamera(ptrPool->game_list, canvas, ptrEnemy->enemy_list, bulletList, 0, 0, canvas_size);
			}
		}
		
		//총 발사
		if (KEY_DOWN(VK_LBUTTON))
		{
			if (!playerStatus.inv.e_blank[playerStatus.gunInvCursor])
			{
				//총 발사
				playerStatus.cursor.x += camera.margin.x;
				playerStatus.cursor.y += camera.margin.y;

				POINT tmp = getBitmapSize(hInst, playerStatus.curGun.sprite);
				tmp.x += 40;
				Shoot(hWnd, hInst, &camera, &(playerStatus.curGun), addPoint(ptrPlayer->point, tmp), playerStatus.cursor, ptrPool, ptrBullet, TAG_BULLET);
			}
		}

		//계단에 닿으면
		if(!isColideForMath(ptrPlayer->colider, ptrStair->colider) && ptrEnemy->size == 0)
		{
			PlaySoundCUS("teleport",2,0);
			pool_size = 1;
			MAP map_tmp;
			map_tmp = createMap();
			map = CreateCompatibleDC(hDC);
			initList(ptrCol);
			initList(ptrBullet);
			beginStage(hInst, &map_tmp, ptrCol, ptrEnemy, &game_info, enemys, rock_tiles);

			if (game_info.stage == 3)
			{
				addGunToPlayer(&playerStatus, ice_gun);
				new_item = 1;
			}
			if (game_info.stage == 5)
			{
				AddItem(&playerStatus, health_bag);
				new_item = 1;
			}
			if (game_info.stage == 6)
			{
				addGunToPlayer(&playerStatus, wood_gun);
				new_item = 1;
			}
			if (game_info.stage == 13)
			{
				new_item = 1;
				addGunToPlayer(&playerStatus, assert_gun);
			}
			if (game_info.stage == 10)
			{
				new_item = 1;
				AddItem(&playerStatus, speed_ring);
			}

			HBITMAP hBackBitmap = CreateCompatibleBitmap(hDC, 5000, 5000);
			SelectObject(map, hBackBitmap);

			drawMap(hInst, hDC, &map, map_tmp);
		}

		//플레이어 콜라이더 생성
		player_col = getColider(hInst, ptrPool->game_list[0].point, getGameObjectItem(ptrPool, 0).sprite);

		//총알, 적에 대한 모든 연산 처리
		doBulletTask(ptrBullet, ptrPool, ptrEnemy, ptrCol, s_size);
		doEnemytask(hInst, &camera, ptrEnemy, ptrPool, ptrBullet, ptrCol);

		//플레이어 정보 갱신
		ptrPlayer->point = player_pos;
		ptrPlayer->colider = player_col;

		//플레이어 위치를 카메라에 동기화 시킴
		attachToCamera(&camera, ptrPlayer);
		cursorPosition.x /= 10;
		cursorPosition.y /= 10;

		camera.margin = cursorPosition;

		//카메라
		doCameraTask(&camera);

		//적이 없으면 스테이지 재생성
		if (ptrEnemy->size == 0)
		{
			pool_size = 2;
		}

		//화면 렌더링
		Render(camera, ptrPool, canvas, ptrEnemy, ptrBullet, ptrCol, map, playerStatus, pool_size ,1, 1, new_item);
		Sleep(0);
		et = clock();
	}

	ReleaseDC(hWnd, hDC);
	exitGame();
}