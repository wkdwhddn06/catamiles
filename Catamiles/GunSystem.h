#pragma once

//총을 만드는 함수입니다
GUN createGun(BULLET_TYPE type, int image, int sprite, char * sound)
{
	GUN gun = {type, image, sprite, type.max,0};
	gun.sound = sound;
	return gun;
}

//총을 이용하여 총알을 발사하는 함수입니다
void Shoot(HWND hWnd, HINSTANCE hInst, CAMERA * camera, GUN * gun, POINT shooterPosition ,POINT cursorPosition, ArrayList* ptrPool ,ArrayList * ptrBullet, int TAG)
{
	if (gun->cooltime == 0)
	{
		gun->cooltime = gun->type.cooltime;
		if (gun->clip != 0)
		{
			shakeCamera(camera, 3, 8);
			int a = random(10, 20);
			StopSoundCUS(a);
			PlaySoundCUS(gun->sound, a, 0);

			POINT target = cursorPosition;
			target.x += random(-gun->type.spread, gun->type.spread);
			target.y += random(-gun->type.spread, gun->type.spread);
			BULLET temp = createBullet(hInst,gun->type, shooterPosition, target, TAG);
			addBulletItem(ptrBullet, temp);
		}
		else
			gun->clip = gun->type.max;
	}
	else
		gun->cooltime -= 1;
}

void ShootNoCamera(HINSTANCE hInst, GUN* gun, POINT shooterPosition, POINT cursorPosition, ArrayList* ptrPool, ArrayList* ptrBullet, int TAG)
{
	if (gun->cooltime == 0)
	{
		gun->cooltime = gun->type.cooltime;
		if (gun->clip != 0)
		{
			POINT target = cursorPosition;
			target.x += random(-gun->type.spread, gun->type.spread);
			target.y += random(-gun->type.spread, gun->type.spread);
			BULLET temp = createBullet(hInst, gun->type, shooterPosition, target, TAG);
			addBulletItem(ptrBullet, temp);
		}
		else
			gun->clip = gun->type.max;
	}
	else
		gun->cooltime -= 1;
}
