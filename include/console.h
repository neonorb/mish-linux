/*
 * console.h
 *
 *  Created on: Oct 14, 2016
 *      Author: chris13524
 */

#ifndef INCLUDE_CONSOLE_H_
#define INCLUDE_CONSOLE_H_

#include <mish.h>

extern Thread* currentThread;
void console();
int execute(feta::String sourceCode);
void killThread();

#endif /* INCLUDE_CONSOLE_H_ */
