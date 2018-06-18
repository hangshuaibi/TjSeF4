#pragma once

#include "GridMap.h"
#include "Data.h"
#include "Unit.h"
#define Debug 1
#include <fstream>

//static std::ofstream mylog;


//判断是否执行move后会不会由直接越过了目标点
bool willBeyondDest(Point now, Point dest, Point offset);//Unit::move

														 //弃用
bool rectTooSmall(const Rect& rect);//UnitManager::selectIdByRect

