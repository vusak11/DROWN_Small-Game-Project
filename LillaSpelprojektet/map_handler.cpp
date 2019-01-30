#include "map_handler.h"
#include "map.h"


MapHandler::MapHandler(){
}

MapHandler::~MapHandler(){
}

Map MapHandler::GetMap(int cam_index)
{
	return maps_[cam_index];
}
