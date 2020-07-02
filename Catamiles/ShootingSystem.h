#pragma once

#define MIN_SPEED 100

BULLET_TYPE createBulletType(int sprite, int spread, int speed, int cooltime, int max, int damage, int knockback, ANIMATION_TASK ani) {
	BULLET_TYPE tmp = {sprite, spread, speed, cooltime, max, damage, knockback, ani};
	return tmp;
}

BULLET createBullet(HINSTANCE hInst, BULLET_TYPE type, POINT shootPoint, POINT dest, int TAG)
{
	// 불완전함
	BULLET tmp;
	tmp.id = 0;
	tmp.type = type;
	tmp.startPoint = shootPoint;
	tmp.direction = MoveForwardVector(shootPoint, dest, type.speed);
	tmp.cooltime = 0;
	tmp.colider = getColider(hInst, shootPoint, type.sprite);
	tmp.tag = TAG;
	tmp.point = toVector(shootPoint);
	return tmp;
}

void doBulletTask(ArrayList* bullet, ArrayList* list, ArrayList* enemy, ArrayList * coliders, POINT s_size)
{
	int ani;
	for (int i = 0; i < bullet->size; i++)
	{
		if (!isColideWithColiders(bullet->bullet_list[i].colider, coliders))
		{
			ani = showAnimation(&(bullet->bullet_list[i].type.explode));
			if (ani == 1)
			{
				removeBulletItem(bullet, i);
			}
			else
			{
				bullet->bullet_list[i].type.sprite = ani;
			}
		}
		else if (!isColideForBullet(bullet->bullet_list[i].colider, list, enemy))
		{
			if (bullet->bullet_list[i].tag == TAG_BULLET)
			{
				ani = showAnimation(&(bullet->bullet_list[i].type.explode));
				if (ani == 1)
				{
					removeBulletItem(bullet, i);
				}
				else
				{
					bullet->bullet_list[i].type.sprite = ani;
				}
			}
		}
		else if (bullet->bullet_list[i].type.explode.cursor != 0)
		{
			if (bullet->bullet_list[i].tag == TAG_BULLET)
			{
				ani = showAnimation(&(bullet->bullet_list[i].type.explode));
				if (ani == 1)
				{
					removeBulletItem(bullet, i);
				}
				else
				{
					bullet->bullet_list[i].type.sprite = ani;
				}
			}
		}

		if (bullet->bullet_list[i].type.explode.cursor == 0)
		{
			bullet->bullet_list[i].point.x += bullet->bullet_list[i].direction.x;
			bullet->bullet_list[i].point.y += bullet->bullet_list[i].direction.y;

			//콜라이더
			bullet->bullet_list[i].colider.end_x += bullet->bullet_list[i].direction.x;
			bullet->bullet_list[i].colider.start_x += bullet->bullet_list[i].direction.x;
			bullet->bullet_list[i].colider.end_y += bullet->bullet_list[i].direction.y;
			bullet->bullet_list[i].colider.start_y += bullet->bullet_list[i].direction.y;
		}
	}
}

