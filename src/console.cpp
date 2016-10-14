/*
 * console.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: chris13524
 */

#include <mish.h>
#include <iostream>
#include <console.h>

Thread* currentThread = NULL;
void console_onThreadExit(Thread* thread) {
	currentThread = NULL;
}
static void newThread(Code* code) {
	Thread* thread = new Thread(code, ACTIVE);
	thread->onThreadExit = console_onThreadExit;
	mish_spawnThread(thread);
	currentThread = thread;
}

bool shouldKill = false;
int execute(String sourceCode) {
	Code* code = mish_compile(sourceCode);
	if (code != NULL) {
		newThread(code);
	} else {
		return 1;
	}

	while (mish_activeThreadCount() > 0) {
		mish_runScheduler();
		if (shouldKill) {
			shouldKill = false;
			mish_killThread(currentThread);
		}
	}

	return 0;
}

bool consoleLoop = true;
void console() {
	while (consoleLoop) {
		std::cout << "> ";

		// get command
		std::string command;
		std::getline(std::cin, command);

		// execute
		execute(command.data());
	}
}

void killThread() {
	shouldKill = true;
}
