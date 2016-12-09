/*
 * console.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: chris13524
 */

#include <mish.h>
#include <feta.h>
#include <iostream>
#include <console.h>

Thread* currentThread = NULL;
void console_onThreadExit(Thread* thread) {
	UNUSED(thread);
	currentThread = NULL;
}
static void newThread(Code* code) {
	Thread* thread = new Thread(code, ACTIVE);
	thread->onThreadExit = BIND_FREE_CB(console_onThreadExit);
	mish::execute::schedule::spawn(thread);
	currentThread = thread;
}

bool shouldKill = false;
int execute(String sourceCode) {
	Code* code = mish::compile::compile(sourceCode);
	if (code != NULL) {
		newThread(code);
	} else {
		return 1;
	}

	while (mish::execute::schedule::activeThreadCount() > 0) {
		mish::execute::schedule::run();
		if (shouldKill) {
			shouldKill = false;
			mish::execute::schedule::kill(currentThread);
			currentThread = NULL;
		}
	}

	return 0;
}

void killThread() {
	shouldKill = true;
}

bool consoleLoop = true;
extern bool hasNewline;
void console() {
	std::cout
			<< "Welcome to Mish! Type __exit() to exit and return back to your shell."
			<< std::endl;
	while (consoleLoop) {
		if (!hasNewline) {
			std::cout << "%\n";
			hasNewline = true;
		}
		std::cout << "mish> ";

		// get command
		std::string command;
		std::getline(std::cin, command);
		// if fail (a.k.a. CTRL+C), then reset fail
		if (std::cin.fail()) {
			std::cin.clear();
			std::cout << "\n";
			continue;
		}

		// execute
		::execute(command.data());
	}
}
