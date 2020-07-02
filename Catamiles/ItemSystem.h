#pragma once

#define HEALTH_UP 1
#define HEALTH_UP_LITTE 2
#define SPEED_UP 3

ITEM createItem(int sprite, int type)
{
	ITEM item;
	item.sprite = sprite;
	item.type = type;
	return item;
}

void ApplyItems(PLAYER * player, ITEM item) 
{
	if (item.type == HEALTH_UP)
	{
		player->maxhealth += 3;
		player->health = player->maxhealth;
	}
	else if (item.type == HEALTH_UP_LITTE)
	{
		player->maxhealth += 1;
	}
	else if (item.type == SPEED_UP)
	{
		player->speed += 3;
	}
}

void AddItem(PLAYER* player, ITEM item)
{
	if (player->inv.invItemSize < 8)
	{
		player->inv.invItem[player->inv.invItemSize] = item;
		player->inv.invItemSize++;
		ApplyItems(player, item);
	}
}
