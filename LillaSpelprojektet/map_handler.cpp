#include "map_handler.h"
#include "map.h"


MapHandler::MapHandler(){
}

MapHandler::~MapHandler(){
}

Map MapHandler::GetMap(int camIndex)
{
	return mMaps[camIndex];
}
