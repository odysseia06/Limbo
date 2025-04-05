// Limbo.cpp : Defines the entry point for the application.
//

#include "Limbo.h"
#include <thread>
#include <chrono>

using namespace std;
using namespace Limbo;

int main()
{
	// Example usage
	LOG_TRACE("This is a trace message");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	LOG_DEBUG("This is a debug message");
	LOG_INFO("This is an info message");
	LOG_WARNING("This is a warning message");
	LOG_ERROR("This is an error message");
	LOG_FATAL("This is a fatal message");
	return 0;
}
