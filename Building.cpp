#include"Building.h"
#include "PathFinder.h"
#include "NotGay.h"

void Building::addToMap(GridMap* gridMap, TMXTiledMap* _tiledMap)
{
	_gridMap = gridMap;

	_tiledMap->addChild(this, 5);
}

 Barracks* Barracks::create()
	{
		const std::string filename = "Barracks.png";

		String::create(filename);
		Barracks *ret = new Barracks();

		if (ret && ret->initWithFile(filename))
		{
			ret->autorelease();

			return ret;
		}
	}

 Warfactory* Warfactory::create()
 {
	 const std::string filename = "Warfactory.png";

	 String::create(filename);
	 Warfactory *ret = new Warfactory();

	 if (ret && ret->initWithFile(filename))
	 {
		 ret->autorelease();

		 return ret;
	 }
 }

 Storage* Storage::create()
 {
	 const std::string filename = "Storage.png";

	 String::create(filename);
	 Storage *ret = new Storage();

	 if (ret && ret->initWithFile(filename))
	 {
		 ret->autorelease();

		 return ret;
	 }
 }

 Producer* Producer::create()
 {
	 const std::string filename = "Producer.png";

	 String::create(filename);
	 Producer *ret = new Producer();

	 if (ret && ret->initWithFile(filename))
	 {
		 ret->autorelease();

		 return ret;
	 }
 }

