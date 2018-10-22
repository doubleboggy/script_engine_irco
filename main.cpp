#include <iostream>
#include "ParseDriver.hpp"

int main(int argc, char *argv[]) {
	
	if (argc != 2) {
		printf("\n\nusage: %s <filename>\n\n", argv[0]);
		return 1;
	}
	
	ParseDriver driver;
	driver.run(argv[1]);
	return 0;
}
