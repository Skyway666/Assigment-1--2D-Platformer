#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"

struct TileSet
{

	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

struct MapLayer
{
	p2SString name;
	int width;
	int height;
	uint* data = nullptr;
};



enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*> layer_array;
	fPoint player_starting_value; //NEW VARIABLE, ADD IT TO XML
	SDL_Texture* background_image; //NEW VARIABLE, ADD IT TO XML
	float parallax_speed; //NEW VARIABLE, ADD IT TO XML
	int camera_y_limit; //NEW VARIABLE, ADD IT TO XML
	iPoint bone_position;
	fPoint background_offset;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	//Called to create colliders
	void CreateColliders();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;

	void Get(int* x, int* y); //la x es el numero, la y cuantos elementos hay en una fila

	SDL_Rect Tile_Rect(int tileid);

	void convert_to_real_world(int*, int*);

	//starting map
  
	int map = 0; //Starting map //NEW VARIABLE, ADD IT TO XML

	SDL_Texture* bone_graphics = nullptr;

	Animation bone_animation;


private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node);
	bool LoadBackground(pugi::xml_node& node);
	bool LoadMapPropierties(pugi::xml_node& node);

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__