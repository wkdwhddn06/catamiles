#pragma once

//분류하기 난감하거나, 형식에 얽메이지 않아도 되는 함수들이 모여있습니다

//커서 종류
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

//커서를 숨기는 함수입니다
void setcursortype(CURSOR_TYPE c)
{
	CONSOLE_CURSOR_INFO CurInfo;
	switch (c)
	{
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

//비트맵의 사이즈를 구하는 함수입니다
POINT getBitmapSize(HINSTANCE hInst, int bitmap)
{
	BITMAP Bitmap;
	HBITMAP hbit = LoadBitmap(hInst, MAKEINTRESOURCE(bitmap));
	GetObject(hbit, sizeof(BITMAP), &Bitmap);
	POINT size = {Bitmap.bmWidth,Bitmap.bmHeight};
	DeleteObject(hbit);
	return size;
}

//비트맵의 COLIDER를 만들어주는 함수입니다
COLIDER getColider(HINSTANCE hInst, POINT point, int bitmap)
{
	BITMAP Bitmap;
	HBITMAP hbit = LoadBitmap(hInst, MAKEINTRESOURCE(bitmap));
	GetObject(hbit, sizeof(BITMAP), &Bitmap);
	POINT size = { Bitmap.bmWidth,Bitmap.bmHeight };
	COLIDER colider = { point.x, point.y, point.x + size.x, point.y + size.y };
	DeleteObject(hbit);
	return colider;
}

//오브젝트가 화면 밖으로 나갔는지 체크하는 함수입니다
int isObjectOutWindow(POINT point, POINT s_size)
{
	if ((point.x > s_size.x || point.y > s_size.y) || (point.x < 0 || point.y < 0))
		return 1;
	else
		return 0;
}

VECTOR toVector(POINT a)
{
	VECTOR tmp;
	tmp.x = a.x;
	tmp.y = a.y;
	return tmp;
}

//랜덤 테스트 중
int random(int min, int max)
{
	max += 1;
	int a = (rand() % (max - min)) + min;
	return a;
}

//이름이 이런건 사용하기 편하기 위함입니다, 포인트를 만드는 함수입니다
POINT cp(int a, int b)
{
	POINT tmp;
	tmp.x = a;
	tmp.y = b;
	return tmp;
}

POINT addPoint(POINT a, POINT b)
{
	POINT tmp;
	tmp.x = a.x + b.x;
	tmp.y = a.y + b.y;
	return tmp;
}

POINT subtractPoint(POINT a, POINT b)
{
	POINT tmp;
	tmp.x = a.x - b.x;
	tmp.y = a.y - b.y;
	return tmp;
}

COLIDER addPointToColider(COLIDER col, POINT a)
{
	col.start_x += a.x;
	col.end_x += a.x;
	col.start_y += a.y;
	col.end_y += a.y;
	return col;
}