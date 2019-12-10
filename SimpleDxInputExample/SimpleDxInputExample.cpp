#include <iostream>

#include "GInput.h"

int main( )
{
	std::unique_ptr<GInput> pInputInstance = std::unique_ptr<GInput>(new GInput(GetConsoleWindow( ), 320, 320));

	return 0;
}
