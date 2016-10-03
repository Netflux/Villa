#ifndef INCLUDE_ITEM_H_
#define INCLUDE_ITEM_H_

namespace villa
{
	/**
	 * Item type enumeration.
	 */
	enum class itemtype
	{
		null,   //!< null
		water,  //!< water
		food,   //!< food
		lumber,   //!< tree
		stone,  //!< stone
		ore,    //!< ore
		pickaxe,//!< pickaxe
		axe,    //!< axe
		bucket  //!< bucket
	};

	/**
	 * Item class.
	 * Represents items that are collected and stored in inventories.
	 */
	class item
	{
		public:
			item();
			item(itemtype type);
			itemtype get_type();
			void set_type(itemtype value);

		private:
			itemtype type;
	};
}

#endif /* INCLUDE_ITEM_H_ */
