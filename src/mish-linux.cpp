#include <iostream>
#include <mish.h>
#include <stdlib.h>
#include <syscalls.h>

namespace feta {

void crash(String message) {
	std::cout << message << std::endl;
}

void fault(String message) {
	std::cout << "[FAULT] " << message << std::endl;
}

void debugPrint(String message) {
	std::cout << message;
}

}

void* fetaimpl::malloc(feta::size size) {
	return ::malloc(size);
}

void fetaimpl::free(void* thing) {
	::free(thing);
}

int main() {
	registerSyscalls();

	Code* code = mish_compile("while(true){__println('hi')}");
	if (code != NULL) {
		mish_execute(code);
		delete code;
	}

	unregisterSyscalls();

	return 0;
}
