#include "NotGay.h"


bool willBeyondDest(Point now, Point dest, Point offset)
{
	if (now + offset == dest)
	{
		return true;
	}

	if (now.x<dest.x&&now.x + offset.x>dest.x
		|| now.x > dest.x&&now.x + offset.x < dest.x)
	{
		return true;
	}
	if (now.y<dest.y&&now.y + offset.y>dest.y
		|| now.y > dest.y&&now.y + offset.y < dest.y)
	{
		return true;
	}

	return false;
}