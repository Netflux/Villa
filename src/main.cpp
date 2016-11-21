#include "app.hpp"
#include "inventory.hpp"
#include <iostream>

using namespace villa;

int main(int argc, char *argv[])
{
	// Initialize and start the application
	std::unique_ptr<app> application(new app());
	application->start();

	return 0;
}
