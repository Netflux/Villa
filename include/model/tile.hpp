#ifndef INCLUDE_TILE_H_
#define INCLUDE_TILE_H_

namespace villa
{
	/**
	 * Tile type enumeration.
	 */
	enum class tiletype
	{
		water,//!< water
		dirt, //!< dirt
		grass,//!< grass
		sand  //!< sand
	};

	/**
	 * Tile class.
	 * Represents each square of the grid-based map.
	 */
	class tile
	{
		public:
			tile();
			tile(tiletype type, bool pathable, bool has_road);
			tiletype get_type();
			void set_type(tiletype type);
			bool get_pathable();
			void set_pathable(bool value);

		private:
			tiletype type;
			bool pathable;
	};
}

#endif /* INCLUDE_TILE_H_ */
