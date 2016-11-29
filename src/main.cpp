#include "app.hpp"

using namespace villa;

int main(int argc, char *argv[])
{
	// Initialize and start the application
	std::unique_ptr<app> application(new app());
	application->start();

	return 0;
}
