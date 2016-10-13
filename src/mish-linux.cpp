#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <iterator>
#include <fstream>
#include <cstdlib>

#include "optionparser.h"

#include <mish.h>
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

namespace fetaimpl {

void* malloc(feta::size size) {
	return ::malloc(size);
}

void free(void* thing) {
	::free(thing);
}

}

static option::ArgStatus MandatoryOption(const option::Option& option, bool) {
	if (option.arg) {
		return option::ARG_OK;
	} else {
		return option::ARG_IGNORE;
	}
}

enum optionIndex {
	UNKNOWN, HELP, PLUS, COMMAND
};
const option::Descriptor usage[] = {

{ UNKNOWN, 0, "", "", option::Arg::None,
		"USAGE: mish [options] [file-path] \n\n" "Options:" },

{ HELP, 0, "h", "help", option::Arg::None,
		"  --help, -h  \tPrint usage and exit." },

//{ PLUS, 0, "p", "plus", option::Arg::None, "  --plus, -p  \tIncrement count." },

		{ COMMAND, 0, "c", "command", MandatoryOption,
				"  --command, -c  \tRun command." },

		{ 0, 0, 0, 0, 0, 0 }

};

static void execute(String sourceCode) {
	Code* code = mish_compile(sourceCode);
	if (code != NULL) {
		mish_execute(code);
		delete code;
	}
}

int main(int argc, char* argv[]) {
	// command-line options

	argc -= (argc > 0);
	argv += (argc > 0); // skip program name argv[0] if present
	option::Stats stats(usage, argc, argv);
	option::Option options[stats.options_max], buffer[stats.buffer_max];
	option::Parser parse(usage, argc, argv, options, buffer);

	if (parse.error())
		return 1;

	if (options[HELP]) {
		option::printUsage(std::cout, usage);
		return 0;
	}

	/*std::cout << "--plus count: " << options[PLUS].count() << "\n";

	 for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
	 std::cout << "Unknown option: " << opt->name << "\n";

	 for (int i = 0; i < parse.nonOptionsCount(); ++i)
	 std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";*/

	registerSyscalls();

	if (options[COMMAND]) {
		String sourceCode = options[COMMAND].arg;

		if (sourceCode == NULL) {
			std::cout << "Missing command." << std::endl;
			option::printUsage(std::cout, usage);
			return 1;
		} else {
			execute(sourceCode);
		}
	} else if (parse.nonOptionsCount() == 1) {
		String fileName = parse.nonOption(0);

		// open stream
		std::ifstream stream(fileName, std::ios::in);

		// read the piped data
		std::istream_iterator<char> it(stream);
		std::istream_iterator<char> end;
		std::string results(it, end);

		// execute it
		execute(results.data());

		// close stream
		stream.close();
	} else {
		if (isatty(0)) {
			// TODO console
			std::cout << "console not implemented yet" << std::endl;
		} else {
			// read the piped data
			std::istream_iterator<char> it(std::cin);
			std::istream_iterator<char> end;
			std::string results(it, end);

			// execute it
			execute(results.data());
		}
	}

	unregisterSyscalls();

// return

	return 0;
}
