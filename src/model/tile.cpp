#include "tile.hpp"

namespace villa
{
	/**
	 * Constructor for the Tile class.
	 */
	tile::tile() : tile(tiletype::water) { }

	/**
	 * Constructor for the Tile class.
	 * @param type - The tile type.
	 */
	tile::tile(tiletype type) : type(type), pathable(true), has_road(false) { }

	/**
	 * Gets the type of the tile.
	 * @return The tile type.
	 */
	tiletype tile::get_type()
	{
		return type;
	}

	/**
	 * Sets the type of the tile.
	 * @param type - The tile type.
	 */
	void tile::set_type(tiletype type)
	{
		this->type = type;
	}

	/**
	 * Gets whether the tile is pathable.
	 * @return Boolean representing whether the tile is pathable.
	 */
	bool tile::get_pathable()
	{
		return pathable;
	}

	/**
	 * Sets whether the tile is pathable.
	 * @param value - Boolean representing whether the tile is pathable.
	 */
	void tile::set_pathable(bool value)
	{
		pathable = value;
	}

	/**
	 * Gets whether the tile has a road.
	 * @return Boolean representing whether the tile has a road.
	 */
	bool tile::get_has_road()
	{
		return has_road;
	}

	/**
	 * Sets whether the tile has a road.
	 * @param value - Boolean representing whether the tile has a road.
	 */
	void tile::set_has_road(bool value)
	{
		has_road = value;
	}
}
