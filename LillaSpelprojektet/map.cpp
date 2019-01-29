#include "map.h"

Map::Map(){
}

Map::~Map(){
	delete[] mMapData_;
}

bool Map::LoadMap(char* textureName){





	int capacity = mMapHeight_ * mMapWidth_;
	mMapData_ = new glm_vec4[capacity];



	return true;
}

void Map::SetHeight(int height){
	mMapHeight_ = height;
}

void Map::SetWidth(int width){
	mMapWidth_ = width;
}

int Map::GetHeight(){
	return mMapHeight_;
}

int Map::GetWidth(){
	return mMapWidth_;
}
