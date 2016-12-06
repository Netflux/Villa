#include "gtest/gtest.h"
#include "villager.hpp"

using namespace villa;

/**
 * Tests whether the Villager can get its x-coord
 */
TEST(VillagerTest, GetX)
{
	std::unique_ptr<villager> target(new villager(0, 0));

	// The villager should start at the 0 x-coord
	EXPECT_EQ(0, target->get_x());
}

/**
 * Tests whether the Villager can set its x-coord
 */
TEST(VillagerTest, SetX)
{
	std::unique_ptr<villager> target(new villager(0, 0));

	// The villager should start at the 0 x-coord
	EXPECT_EQ(0, target->get_x());

	target->set_x(100);

	// The villager should be at the 100 x-coord
	EXPECT_EQ(100, target->get_x());
}

/**
 * Tests whether the Villager can get its y-coord
 */
TEST(VillagerTest, GetY)
{
	std::unique_ptr<villager> target(new villager(0, 0));

	// The villager should start at the 0 y-coord
	EXPECT_EQ(0, target->get_y());
}

/**
 * Tests whether the Villager can set its y-coord
 */
TEST(VillagerTest, SetY)
{
	std::unique_ptr<villager> target(new villager(0, 0));

	// The villager should start at the 0 y-coord
	EXPECT_EQ(0, target->get_y());

	target->set_y(100);

	// The villager should be at the 100 y-coord
	EXPECT_EQ(100, target->get_y());
}

/**
 * Tests whether the Villager can get its inventory
 */
TEST(VillagerTest, GetInventory)
{
	std::unique_ptr<villager> target(new villager(0, 0));
	inventory* inv = target->get_inventory();

	// The villager should have an inventory
	EXPECT_NE(nullptr, inv);
}

/**
 * Tests whether the Villager starts with an empty inventory
 */
TEST(VillagerTest, GetInventoryDefaultEmpty)
{
	std::unique_ptr<villager> target(new villager(0, 0));
	inventory* inv = target->get_inventory();

	// The villager should start with a fresh inventory (0 items)
	EXPECT_EQ(0, inv->get_item_count());
}

/**
 * Tests whether the Villager can get its speed
 */
TEST(VillagerTest, GetSpeed)
{
	std::unique_ptr<villager> target(new villager(0, 0));

	// The villager should start with a speed of 100
	EXPECT_EQ(100, target->get_speed());
}

/**
 * Tests whether the Villager can set its speed
 */
TEST(VillagerTest, SetSpeed)
{
	std::unique_ptr<villager> target(new villager(0, 0));

	// The villager should start with a speed of 100
	EXPECT_EQ(100, target->get_speed());

	target->set_speed(200);

	// The villager should now have a speed of 200
	EXPECT_EQ(200, target->get_speed());
}

/**
 * Tests whether the Villager can move towards a coordinate
 */
TEST(VillagerTest, Move)
{
	std::unique_ptr<villager> target(new villager(0, 0));

	// The villager should start at the coords (0, 0)
	EXPECT_EQ(0, target->get_x());
	EXPECT_EQ(0, target->get_y());

	target->move(100, 100, 1.0);

	// The villager should move by (speed / 60)
	EXPECT_EQ(target->get_speed() / 60, target->get_x());
	EXPECT_EQ(target->get_speed() / 60, target->get_y());

	target->move(200, -100, 1.0);

	// The villager should move by (speed / 60)
	EXPECT_EQ((target->get_speed() / 60) * 2, target->get_x());
	EXPECT_EQ(0, target->get_y());
}

/**
 * Tests whether the Villager can check its location
 */
TEST(VillagerTest, IsAt)
{
	std::unique_ptr<villager> target(new villager(0, 0));

	// The villager should start at the coords (0, 0)
	EXPECT_EQ(true, target->is_at(0, 0));

	target->move(100, 100, 1.0);

	// The villager should move by (speed / 60)
	EXPECT_EQ(true, target->is_at(target->get_speed() / 60, target->get_speed() / 60));
}

/**
 * Tests whether the Villager can check its not at a location
 */
TEST(VillagerTest, IsNotAt)
{
	std::unique_ptr<villager> target(new villager(0, 0));

	// The villager should start at the coords (0, 0)
	EXPECT_EQ(true, target->is_at(0, 0));

	// The villager should not be at random coords
	EXPECT_EQ(false, target->is_at(200, 200));
	EXPECT_EQ(false, target->is_at(100, 100));
	EXPECT_EQ(false, target->is_at(50, 50));
	EXPECT_EQ(false, target->is_at(25, 25));
}

/**
 * Tests whether the Villager can add a task to its list
 */
TEST(VillagerTest, AddTask)
{
	std::unique_ptr<villager> target(new villager(0, 0));
	task* target_task = new task(tasktype::rest, taskdata(std::make_pair(0, 0)));
	task* target_task2 = new task(tasktype::rest, taskdata(std::make_pair(50, 50), 50));

	target->add_task(target_task);

	// The villager task should be equal to the first inserted task
	EXPECT_EQ(target_task, target->get_task());

	target->add_task(target_task2);

	// The villager task should be equal to the second inserted task
	EXPECT_EQ(target_task2, target->get_task());
}

/**
 * Tests whether the Villager can remove a task from its list
 */
TEST(VillagerTest, RemoveTask)
{
	std::unique_ptr<villager> target(new villager(0, 0));
	task* target_task = new task(tasktype::rest, taskdata(std::make_pair(0, 0)));
	task* initial_task = target->get_task();

	target->add_task(target_task);

	// The villager task should be equal to the newly inserted task
	EXPECT_EQ(target_task, target->get_task());

	target->remove_task();

	// The villager task should still be equal to the initial task
	EXPECT_EQ(initial_task, target->get_task());
}

/**
 * Tests whether the Villager can get a task from its list
 */
TEST(VillagerTest, GetTask)
{
	std::unique_ptr<villager> target(new villager(0, 0));
	task* target_task = new task(tasktype::rest, taskdata(std::make_pair(0, 0)));

	target->add_task(target_task);

	// The villager should return the newly inserted task
	EXPECT_EQ(target_task, target->get_task());
}
