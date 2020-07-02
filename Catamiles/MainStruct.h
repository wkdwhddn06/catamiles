#pragma once

//게임 오브젝트의 태그입니다
#define TAG_ENEMY_BULLET 2
#define TAG_BULLET 3

//헤더 충돌 관련 이슈로 인하여 중요한 구조체들이 모여있습니다

//Bresenham 알고리즘을 이용하기 위한 구조체입니다
typedef struct BRESENHAM
{
	int F;
	int dF1;
	int dF2;
} BRESENHAM;

// 콜라이더 충돌판정을 위해 사용합니다
typedef struct COLIDER
{
	float start_x;
	float start_y;
	float end_x;
	float end_y;
} COLIDER;

//소수계산을 할때 사용하는 POINT 구조체 입니다 
typedef struct VECTOR {
	float x;
	float y;
} VECTOR;

// 게임 오브젝트는 콜라이더와 위치 그리고 이미지 정보를 지니고 있습니다
typedef struct GAME_OBJECT
{
	COLIDER colider;
	POINT point;
	int sprite;
	int tag;
} GAME_OBJECT;

//애니메이션 시스템에서 사용합니다
typedef struct ANIMATION {
	int size;
	int frames[100];
} ANIMATION;

//애니메이션을 실행시키기위한 구조체입니다
typedef struct ANIMATION_TASK {
	ANIMATION animation;
	int cursor;
	int timer;
	int length;
} ANIMATION_TASK;

//총알의 타입 구조체입니다
typedef struct BULLET_TYPE {
	int sprite;
	int spread;
	int speed;
	int cooltime;
	int max;
	int damage;
	int knockback;
	ANIMATION_TASK explode;
} BULLET_TYPE;

//총알 구조체 입니다
typedef struct BULLET {
	int id;
	BULLET_TYPE type;
	POINT startPoint;
	VECTOR direction;
	int cooltime;
	COLIDER colider;
	int tag;
	VECTOR point;
} BULLET;

//총 구조체 입니다
typedef struct GUN {
	BULLET_TYPE type;
	int image;
	int sprite;
	int clip;
	int cooltime;
	char * sound;
} GUN;

//적 패턴을 만들기 위한 상태 구조체입니다
typedef struct STATE {
	int type;
	int length;
} STATE;

//적을 행동하기 위해 사용하는 구조체입니다
typedef struct STATE_TASK {
	STATE state;
	int timer;
} STATE_TASK;

//적의 행동패턴이 담겨있는 구조체입니다
typedef struct STATE_PATTERN {
	int size;
	int cursor;
	STATE_TASK states[20];
} STATE_PATTERN;

//적의 타입을 결정하는 구조체입니다
typedef struct ENEMY_TYPE {
	int type;
	int health_point;
	GUN gun;

	ANIMATION_TASK ani_hited_task;
	ANIMATION_TASK ani_idle_task;
	ANIMATION_TASK ani_death_task;
} ENEMY_TYPE;

//아이템의 정보가 담겨있는 구조체입니다
typedef struct ITEM
{
	int type;
	int sprite;
}ITEM;

//적을 오브젝트로 사용하기위한 구조체입니다
typedef struct ENEMY {
	COLIDER colider;
	POINT point;
	int sprite;
	STATE_PATTERN state;
	ENEMY_TYPE type;
} ENEMY;

//인벤토리의 정보가 담겨있는 구조체입니다
typedef struct INVENTORY {
	int e_blank[3];
	GUN equipGun[3];
	int invSize;
	GUN invGun[8];
	int invItemSize;
	ITEM invItem[8];
} INVENTORY;

//ArrayList 구조체입니다
typedef struct ArrayList {
	int size;
	GAME_OBJECT game_list[100];
	BULLET bullet_list[100];
	ENEMY enemy_list[100];
	COLIDER colider_list[100];
} ArrayList;