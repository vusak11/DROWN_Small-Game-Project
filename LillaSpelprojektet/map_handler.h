#ifndef MAP_HANDLER_H
#define MAP_HANDLER_H

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

	Map GetMap(int cam_index = 0);

private:
	Map* maps_;	// A list of all the maps in the current game.
};

#endif