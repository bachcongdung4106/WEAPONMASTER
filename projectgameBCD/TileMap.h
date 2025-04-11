#ifndef Tile_Map_H
#define Tile_Map_H
#include "CommonFunction.h"
#include "Object.h"
#define MAX_TILES 20// cac dang map gach/co/dat/nuoc

class TileMat : public Object //TileMat ke thua toan bo pubic va protected
{
public:
	TileMat() { ; }
	~TileMat() { ; }

};
class GameMap
{
public:
	GameMap() { ; }
	~GameMap() { ; }

	void LoadMap(const char* name);// da sua them const
	void LoadTiles(SDL_Renderer* screen);
	void DrawMap(SDL_Renderer* screen);

private:
	Map game_map_;
	TileMat tile_mat[MAX_TILES];



};
#endif
