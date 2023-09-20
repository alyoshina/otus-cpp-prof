#include "version.h"

#include <iostream>

int main(int argc, char **argv) {
    std::cout << "Version: " << get_version() << std::endl;
    std::cout << "Hello, World!" << std::endl;
}