#pragma once

COLIDER makeColiderFromPoint(POINT a, POINT b)
{
	COLIDER tmp;
	tmp.start_x = a.x;
	tmp.start_y = a.y;
	tmp.end_x = b.x;
	tmp.end_y = b.y;
	return tmp;
}