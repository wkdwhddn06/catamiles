#pragma once

//typedef struct INVENTORY
//{
//	int tmp[3];
//} INVENTORY;

typedef struct PLAYER
{
	int speed;
	int maxhealth;
	int health;
	int gunInvCursor;
	GUN curGun;
	POINT cursor;
	INVENTORY inv;
} PLAYER;

PLAYER createPlayer(int health, int speed,GUN firstGun)
{
	PLAYER tmp;
	tmp.speed = speed;
	tmp.maxhealth = health;
	tmp.health = health;
	tmp.curGun = firstGun;
	tmp.gunInvCursor = 0;
	tmp.inv = createInventory(firstGun);
	return tmp;
}

void switchGun(PLAYER * playerStatus, int dire)
{
	if (playerStatus->gunInvCursor + dire < 3 && playerStatus->gunInvCursor + dire >= 0)
	{
		playerStatus->gunInvCursor += dire;
		playerStatus->curGun = playerStatus->inv.equipGun[playerStatus->gunInvCursor];
	}
}

void addGunToPlayer(PLAYER* playerStatus, GUN gun)
{
	int isEquiped = 1;

	for (int i = 0; i < 3; i++)
	{
		if (playerStatus->inv.e_blank[i])
		{
			playerStatus->inv.equipGun[i] = gun;
			playerStatus->inv.e_blank[i] = 0;
			isEquiped = 0;

			break;
		}
	}		

	if (playerStatus->inv.invSize != 8)
	{
		playerStatus->inv.invGun[playerStatus->inv.invSize] = gun;
		playerStatus->inv.invSize++;
	}
}