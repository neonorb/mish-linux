/*
 * syscalls.cpp
 *
 *  Created on: Oct 12, 2016
 *      Author: chris13524
 */

#include <mish.h>
#include <iostream>
#include <feta.h>

using namespace mish;

bool hasNewline = true;
Value* printlnFunction(List<Value*>* arguments) {
	std::cout << ((StringValue*) arguments->get(0))->value << std::endl;
	hasNewline = true;

	return NULL;
}

Value* printFunction(List<Value*>* arguments) {
	String string = ((StringValue*) arguments->get(0))->value;
	std::cout << string;
	hasNewline = string[stringlength(string) - 1] == '\n';

	return NULL;
}

extern bool consoleLoop;
Value* exitFunction(List<Value*>* arguments) {
	UNUSED(arguments);
	consoleLoop = false;

	return NULL;
}

List<Function*> syscalls;

void registerSyscalls() {
	// println
	List<ValueType>* printlnParameterTypes = new List<ValueType>();
	printlnParameterTypes->add(ValueType::STRING);
	Function* println = new Function("__println"_H, printlnParameterTypes,
			ValueType::VOID, BIND_FREE_CB(printlnFunction));
	syscalls.add(println);
	mish_syscalls.add(println);

	// _print
	List<ValueType>* printParameterTypes = new List<ValueType>();
	printParameterTypes->add(ValueType::STRING);
	Function* print = new Function("__print"_H, printParameterTypes,
			ValueType::VOID, BIND_FREE_CB(printFunction));
	syscalls.add(print);
	mish_syscalls.add(print);

	// exit
	List<ValueType>* exitParameterTypes = new List<ValueType>();
	Function* exit = new Function("__exit"_H, exitParameterTypes,
			ValueType::VOID, BIND_FREE_CB(exitFunction));
	syscalls.add(exit);
	mish_syscalls.add(exit);
}

void unregisterSyscalls() {
	Iterator<Function*> iterator = syscalls.iterator();
	while (iterator.hasNext()) {
		Function* function = iterator.next();
		mish_syscalls.remove(function);
		delete function;
	}

	syscalls.clear();
}
