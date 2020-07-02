#pragma once

#define GUN_PAGE IDB_INV_GUN
#define ITEM_PAGE IDB_INV_ITEM

INVENTORY createInventory(GUN defalutGun)
{
	INVENTORY tmp;
	tmp.e_blank[0] = 0;
	tmp.e_blank[1] = 1;
	tmp.e_blank[2] = 1;
	tmp.invSize = 1;
	tmp.equipGun[0] = defalutGun;
	tmp.invGun[0] = defalutGun;
	tmp.invItemSize = 0;
	return tmp;
}