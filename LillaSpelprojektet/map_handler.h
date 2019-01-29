#pragma once

// Forward declaration
class Map;

// This class is used as a handler of the map(s) and how it will work in the game.  
// Example:
//    handler->GetMap(0)->load_map();
//   


class MapHandler
{
public:
	MapHandler();
	~MapHandler();

	Map GetMap(int camIndex = 0);

private:
	Map* mMaps;	// A list of all the maps in the current game.
};
