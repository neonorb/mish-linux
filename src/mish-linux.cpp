#include <iostream>
#include <mish.h>
#include <stdlib.h>
#include <syscalls.h>
#include "optionparser.h"

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

namespace fetaimpl {

void* malloc(feta::size size) {
	return ::malloc(size);
}

void free(void* thing) {
	::free(thing);
}

}

enum optionIndex {
	UNKNOWN, HELP, PLUS, RUN
};
const option::Descriptor usage[] = {

{ UNKNOWN, 0, "", "", option::Arg::None,
		"USAGE: mish-linux [options] [file-path] \n\n" "Options:" },

{ HELP, 0, "", "help", option::Arg::None, "  --help  \tPrint usage and exit." },

//{ PLUS, 0, "p", "plus", option::Arg::None, "  --plus, -p  \tIncrement count." },

{ RUN, 0, "r", "run", option::Arg::None, "  --run, -r  \tRun command." },

{ 0, 0, 0, 0, 0, 0 }

};

int main(int argc, char* argv[]) {
	// command-line options

	argc -= (argc > 0);
	argv += (argc > 0); // skip program name argv[0] if present
	option::Stats stats(usage, argc, argv);
	option::Option options[stats.options_max], buffer[stats.buffer_max];
	option::Parser parse(usage, argc, argv, options, buffer);

	if (parse.error())
		return 1;

	if (options[HELP] || argc == 0) {
		option::printUsage(std::cout, usage);
		return 0;
	}

	std::cout << "--plus count: " << options[PLUS].count() << "\n";

	for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
		std::cout << "Unknown option: " << opt->name << "\n";

	for (int i = 0; i < parse.nonOptionsCount(); ++i)
		std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";

	// execution

	registerSyscalls();

	Code* code = mish_compile("__println('Hello, World!')");
	if (code != NULL) {
		mish_execute(code);
		delete code;
	}

	unregisterSyscalls();

	// return

	return 0;
}
