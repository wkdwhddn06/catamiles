#pragma once

//게임의 맵 시스템을 담당하는 헤더입니다
//타일맵 시스템이 구현되어 있습니다.

#define BIG_ROOM 1
#define BIG_ROOM_WIDTH 20
#define BIG_ROOM_HEIGHT 20

#define MIDDLE_ROOM 2
#define SMALL_ROOM 3
#define CHEST_ROOM 4

//한 타일의 크기
#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#define HORIZONTAL 0
#define VERTICAL 1

#define LAYER_HEIGHT 200	
#define LAYER_WIDTH 200

//타일 구조체입니다
typedef struct MAP_TILE {
	int able;
	int sprite;
	int colider;
} MAP_TILE;

typedef struct MAP_ROOM_COMPONENT {
	MAP_TILE no_tile;

	MAP_TILE tile;
	MAP_TILE tile_floor;

	MAP_TILE wall;
	MAP_TILE wall_shadow;

	MAP_TILE wall_down;
	MAP_TILE wall_up;
	MAP_TILE wall_left;
	MAP_TILE wall_right;

	MAP_TILE wall_l_d;
	MAP_TILE wall_l_u;
	MAP_TILE wall_r_d;
	MAP_TILE wall_r_u;
} MAP_ROOM_COMPONENT;

typedef struct MAP_ROOM {
	int type;
	MAP_TILE tile[100][100];
} MAP_ROOM;

typedef struct MAP_RANDOM_ROOM {
	int rooms[4][4];
} MAP_RANDOM_ROOM;

//맵의 레이어 구조체입니다
typedef struct MAP_LAYER {
	MAP_TILE tile[LAYER_HEIGHT][LAYER_WIDTH];
} MAP_LAYER;

//맵 구조체 입니다 이 구조체를 이용해 맵 관련 작업을 진행합니다
typedef struct MAP {
	MAP_LAYER layer[1];
} MAP;

//타일을 만드는 함수입니다
MAP_TILE createMapTile(int sprite, int colider)
{
	MAP_TILE tmp;
	if(sprite == NULL)
		tmp.able = 0;
	else
		tmp.able = 1;
	tmp.sprite = sprite;
	tmp.colider = colider;
	return tmp;
}

//맵을 초기화하는 함수입니다
MAP createMap()
{
	MAP map_tmp;

	MAP_TILE tile_tmp;
	tile_tmp.able = 0;
	tile_tmp.sprite = 0;
	tile_tmp.colider = 0;

	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < LAYER_HEIGHT; j++)
		{
			for (int k = 0; k < LAYER_WIDTH; k++)
			{
				map_tmp.layer[i].tile[j][k] = tile_tmp;
			}
		}
	}

	return map_tmp;
}

void drawMapLine(MAP_LAYER* layer, ArrayList* colider, POINT a, POINT b, MAP_TILE tile)
{
	if (a.y == b.y)
	{
		POINT real_a, real_b;
		real_a.x = a.x * 64;
		real_a.y = a.y * 64;

		real_b.x = b.x * 64;
		real_b.y = (b.y + 1) * 64;
		for (int i = a.x; i < b.x; i++)
		{
			layer->tile[a.y][i] = tile;
		}
		if(tile.colider)
			addColiderItem(colider, makeColiderFromPoint(real_a, real_b));
	}
	else if (a.x == b.x)
	{
		POINT real_a, real_b;
		real_a.x = a.x * 64;
		real_a.y = a.y * 64;

		real_b.x = (b.x + 1) * 64;
		real_b.y = b.y * 64;

		for (int i = a.y; i < b.y; i++)
		{
			layer->tile[i][a.x] = tile;
		}
		if (tile.colider)
			addColiderItem(colider, makeColiderFromPoint(real_a, real_b));
	}
}
void FixedrawMapLine(MAP_LAYER* layer, ArrayList* colider, POINT a, POINT b, MAP_TILE tile)
{
	if (a.y == b.y)
	{
		POINT real_a, real_b;
		real_a.x = a.x * 64;
		real_a.y = a.y * 64;

		real_b.x = (b.x + 1) * 64;
		real_b.y = (b.y + 1) * 64;
		for (int i = a.x; i <= b.x; i++)
		{
			layer->tile[a.y][i] = tile;
		}
		if (tile.colider)
			addColiderItem(colider, makeColiderFromPoint(real_a, real_b));
	}
	else if (a.x == b.x)
	{
		POINT real_a, real_b;
		real_a.x = a.x * 64;
		real_a.y = a.y * 64;

		real_b.x = (b.x + 1) * 64;
		real_b.y = b.y * 64;

		for (int i = a.y; i <= b.y; i++)
		{
			layer->tile[i][a.x] = tile;
		}
		if (tile.colider)
			addColiderItem(colider, makeColiderFromPoint(real_a, real_b));
	}
}

//맵을 그릴때 사용하는 함수입니다 타일을 한칸에 그려줍니다.
void drawMapPoint(MAP_LAYER* layer, POINT a, MAP_TILE tile)
{
	layer->tile[a.y][a.x] = tile;
}

//맵을 그릴때 사용하는 함수입니다 타일을 직선으로 그려줍니다.
void drawRoomLine(MAP_ROOM* layer, POINT a, POINT b, MAP_TILE tile)
{
	if (a.y == b.y)
	{
		//POINT real_a, real_b;
		//real_a.x = a.x * 64;
		//real_a.y = a.y * 64;

		//real_b.x = b.x * 64;
		//real_b.y = (b.y + 1) * 64;
		for (int i = a.x; i < b.x; i++)
		{
			layer->tile[a.y][i] = tile;
		}
		//addColiderItem(colider, makeColiderFromPoint(real_a, real_b));
	}
	else if (a.x == b.x)
	{
		//POINT real_a, real_b;
		//real_a.x = a.x * 64;
		//real_a.y = a.y * 64;

		//real_b.x = (b.x + 1) * 64;
		//real_b.y = b.y * 64;

		for (int i = a.y; i < b.y; i++)
		{
			layer->tile[i][a.x] = tile;
		}
		//addColiderItem(colider, makeColiderFromPoint(real_a, real_b));
	}
}

//맵을 그릴때 사용하는 함수입니다 타일을 한칸에 그려줍니다.
void drawRoomPoint(MAP_ROOM* layer, POINT a, MAP_TILE tile)
{
	layer->tile[a.y][a.x] = tile;
}

//맵을 DC에 그려줍니다
void drawMap(HINSTANCE hInst, HDC hDC, HDC * mapDC, MAP map)
{
	HDC tileDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap;

	int i, j, k;

	for (i = 0; i < 1; i++)
	{
		for (j = 0; j < LAYER_HEIGHT; j++)
		{
			for (k = 0; k < LAYER_WIDTH; k++)
			{
				if (map.layer[i].tile[j][k].able)
				{
					hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(map.layer[i].tile[j][k].sprite));
					SelectObject(tileDC, hBitmap);
					GdiTransparentBlt(*mapDC, k * TILE_WIDTH, j * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, tileDC, 0, 0, TILE_WIDTH, TILE_HEIGHT, RGB(255, 0, 228));

					DeleteObject(SelectObject(tileDC, hBitmap));
					DeleteObject(hBitmap);
				}
			}
		}
	}

	DeleteDC(tileDC);
}

void drawRandomMap(MAP_LAYER* layer, MAP_RANDOM_ROOM room, MAP_ROOM big)
{ 
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (room.rooms[i][j] == BIG_ROOM)
			{
				for (int k = 0; k < BIG_ROOM_HEIGHT; k++)
				{
					for (int g = 0; g < BIG_ROOM_WIDTH; g++)
					{
						layer->tile[(i * 50) + k][(j * 50) + g] = big.tile[k][g];
					}
				}
			}
		}
	}
}

MAP_RANDOM_ROOM createRandomMap()
{
	MAP_RANDOM_ROOM tmp;
	POINT cursor = { random(1,2) , random(1,2) };
	tmp.rooms[cursor.x][cursor.y] = 1;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tmp.rooms[i][j] = 1;
		}
	}

	return tmp;
}

MAP_ROOM_COMPONENT createMapRoomComponent(
	MAP_TILE tile, 
	MAP_TILE tile_floor, 
	MAP_TILE wall, 
	MAP_TILE wall_shadow, 
	MAP_TILE wall_down,
	MAP_TILE wall_up,
	MAP_TILE wall_left,
	MAP_TILE wall_right,
	MAP_TILE wall_l_d,
	MAP_TILE wall_l_u,
	MAP_TILE wall_r_d,
	MAP_TILE wall_r_u)
{
	MAP_ROOM_COMPONENT tmp;
	tmp.tile = tile;
	tmp.tile_floor = tile_floor;
	tmp.wall = wall;
	tmp.wall_down = wall_down;
	tmp.wall_left = wall_left;
	tmp.wall_right = wall_right;
	tmp.wall_up = wall_up;
	tmp.wall_shadow = wall_shadow;
	tmp.wall_l_d = wall_l_d;
	tmp.wall_l_u = wall_l_u;
	tmp.wall_r_d = wall_r_d;
	tmp.wall_r_u = wall_r_u;
	tmp.no_tile = createMapTile(0, 1);
	return tmp;
}

MAP_ROOM createRoom(int type, MAP_ROOM_COMPONENT component)
{
	MAP_ROOM tmp;
	tmp.type = type;
	switch (type)
	{
		case BIG_ROOM:
			drawRoomLine(&(tmp), cp(1, 1), cp(BIG_ROOM_WIDTH, 1), component.wall_down);
			drawRoomLine(&(tmp), cp(1, 2), cp(BIG_ROOM_WIDTH, 2), component.wall);
			drawRoomLine(&(tmp), cp(1, 3), cp(BIG_ROOM_WIDTH, 3), component.wall_shadow);
			drawRoomLine(&(tmp), cp(1, 4), cp(BIG_ROOM_WIDTH, 4), component.tile_floor);
			drawRoomLine(&(tmp), cp(1, BIG_ROOM_HEIGHT), cp(BIG_ROOM_WIDTH, BIG_ROOM_HEIGHT), component.wall_up);

			drawRoomLine(&(tmp), cp(0, 2), cp(0, BIG_ROOM_HEIGHT), component.wall_right);
			drawRoomLine(&(tmp), cp(BIG_ROOM_WIDTH, 2), cp(BIG_ROOM_WIDTH, BIG_ROOM_HEIGHT), component.wall_left);

			for (int i = 5; i < BIG_ROOM_HEIGHT; i++)
				drawRoomLine(&(tmp), cp(1, i), cp(BIG_ROOM_WIDTH, i), component.tile);
			break;
	}

	return tmp;
}

void drawSqureRoom(POINT size, MAP_LAYER * layer,ArrayList * ptrCol,MAP_ROOM_COMPONENT component)
{
	drawMapLine(layer, ptrCol, cp(1, 1), cp(size.x, 1), component.wall_down);
	drawMapLine(layer, ptrCol, cp(1, 2), cp(size.x, 2), component.wall);
	drawMapLine(layer, ptrCol, cp(1, 3), cp(size.x, 3), component.wall_shadow);
	drawMapLine(layer, ptrCol, cp(1, 4), cp(size.x, 4), component.tile_floor);
	drawMapLine(layer, ptrCol, cp(1, size.y), cp(size.x, size.y), component.wall_up);

	drawMapLine(layer, ptrCol, cp(0, 2), cp(0, size.y), component.wall_right);
	drawMapLine(layer, ptrCol, cp(size.x, 2), cp(size.x, size.y), component.wall_left);

	for (int i = 5; i < size.y; i++)
		drawMapLine(layer, ptrCol, cp(1, i), cp(size.x, i), component.tile);
}

void drawPillar(POINT size, POINT point, MAP_LAYER* layer, ArrayList* ptrCol, MAP_ROOM_COMPONENT component)
{
	MAP_TILE l_U = component.wall_l_u;
	MAP_TILE R_U = component.wall_r_u;
	MAP_TILE up = component.wall_up;

	//l_U.colider = 0;
	//R_U.colider = 0;
	//up.colider = 0;

	size = addPoint(size, point);

	FixedrawMapLine(layer, ptrCol, point, point, l_U);
	FixedrawMapLine(layer, ptrCol, cp(point.x + 1, point.y), cp(size.x - 1, point.y), up);
	FixedrawMapLine(layer, ptrCol, cp(size.x, point.y), cp(size.x, point.y), R_U);

	for (int i = point.y + 1; i < size.y; i++)
	{
		FixedrawMapLine(layer, ptrCol, cp(point.x, i), cp(point.x, i), component.wall_left);
		FixedrawMapLine(layer, ptrCol, cp(point.x + 1, i), cp(size.x - 1, i), component.no_tile);
		FixedrawMapLine(layer, ptrCol, cp(size.x, i), cp(size.x, i), component.wall_right);
	}

	FixedrawMapLine(layer, ptrCol, cp(point.x, size.y), cp(point.x, size.y), component.wall_l_d);
	FixedrawMapLine(layer, ptrCol, cp(point.x + 1, size.y), cp(size.x - 1, size.y), component.wall_down);
	FixedrawMapLine(layer, ptrCol, size, size, component.wall_r_d);

	FixedrawMapLine(layer, ptrCol, cp(point.x, size.y + 1), cp(size.x, size.y + 1), component.wall);
	FixedrawMapLine(layer, ptrCol, cp(point.x, size.y + 2), cp(size.x, size.y + 2), component.wall_shadow);
}
