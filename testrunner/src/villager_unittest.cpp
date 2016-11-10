#include "gtest/gtest.h"
#include "villager.hpp"

using namespace villa;

TEST(VillagerTest, Move)
{
	std::unique_ptr<villager> target(new villager());

	// The villager should start at the coords (0, 0)
	EXPECT_EQ(0, target->get_x());
	EXPECT_EQ(0, target->get_y());

	target->move(100, 100);

	// The villager should move by (speed / 60)
	EXPECT_EQ(100 / 60, target->get_x());
	EXPECT_EQ(100 / 60, target->get_y());
}

TEST(VillagerTest, AddTask)
{
	std::unique_ptr<villager> target(new villager());
	task* target_task = new task(tasktype::rest, taskdata(std::make_pair(0, 0)));
	task* target_task2 = new task(tasktype::rest, taskdata(std::make_pair(50, 50), 50));

	target->add_task(target_task);

	// The villager task should be equal to the first inserted task
	EXPECT_EQ(target_task, target->get_task());

	target->add_task(target_task2);

	// The villager task should be equal to the second inserted task
	EXPECT_EQ(target_task2, target->get_task());
}

TEST(VillagerTest, RemoveTask)
{
	std::unique_ptr<villager> target(new villager());
	task* target_task = new task(tasktype::rest, taskdata(std::make_pair(0, 0)));
	task* initial_task = target->get_task();

	target->add_task(target_task);

	// The villager task should be equal to the newly inserted task
	EXPECT_EQ(target_task, target->get_task());

	target->remove_task();

	// The villager task should be equal to the initial task
	EXPECT_EQ(initial_task, target->get_task());
}

TEST(VillagerTest, GetTask)
{
	std::unique_ptr<villager> target(new villager());
	task* target_task = new task(tasktype::rest, taskdata(std::make_pair(0, 0)));

	target->add_task(target_task);

	// The villager should return the newly inserted task
	EXPECT_EQ(target_task, target->get_task());
}
