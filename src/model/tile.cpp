#include "tile.hpp"

namespace villa
{
	/**
	 * Constructor for the Tile class.
	 */
	tile::tile() : tile(tiletype::water, false) { }

	/**
	 * Constructor for the Tile class.
	 * @param type - The tile type.
	 * @param pathable - Boolean representing whether the tile is pathable.
	 */
	tile::tile(tiletype type, bool pathable) : type(type), pathable(pathable) { }

	/**
	 * Gets the type of the tile.
	 * @return The tile type.
	 */
	tiletype tile::get_type()
	{
		return this->type;
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
		return this->pathable;
	}

	/**
	 * Sets whether the tile is pathable.
	 * @param value - Boolean representing whether the tile is pathable.
	 */
	void tile::set_pathable(bool value)
	{
		this->pathable = value;
	}
}
