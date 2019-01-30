#include "map.h"

Map::Map(){
}

Map::~Map(){
	delete[] map_data_;
}

bool Map::LoadMap(char* texture_name){

	//int capacity = map_height_ * map_width_;
	//map_data_ = new glm_vec4[capacity];

	return true;
}

void Map::SetHeight(int height){
	map_height_ = height;
}

void Map::SetWidth(int width){
	map_width_ = width;
}

int Map::GetHeight(){
	return map_height_;
}

int Map::GetWidth(){
	return map_width_;
}
