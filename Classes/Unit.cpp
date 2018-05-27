#include "Unit.h"

void Unit::setProperties()
{
	_state = Unit::State::WONDERING;

	_moveSpeed = 16;
}

void Unit::setGridPath(const GridMap::GridVector& path)
{
	_gridPath = path;
}

void Unit::setDestination(const Grid& g)
{
	_finalDest = g;
}

void Unit::findPath()
{
	
}