/*
 * syscalls.cpp
 *
 *  Created on: Oct 12, 2016
 *      Author: chris13524
 */

#include <mish.h>
#include <iostream>

Value* printlnFunction(List<Value*>* arguments) {
	std::cout << ((StringValue*) arguments->get(0))->value << std::endl;

	return NULL;
}

Value* printFunction(List<Value*>* arguments) {
	std::cout << ((StringValue*) arguments->get(0))->value;

	return NULL;
}

extern bool consoleLoop;
Value* exitFunction(List<Value*>* arguments) {
	consoleLoop = false;

	return NULL;
}

List<Function*> syscalls;

void registerSyscalls() {
	// println
	List<ValueType>* printlnParameterTypes = new List<ValueType>();
	printlnParameterTypes->add(STRING_VALUE);
	Function* println = new Function("__println"_H, printlnParameterTypes,
			VOID_VALUE, printlnFunction);
	syscalls.add(println);
	mish_syscalls.add(println);

	// _print
	List<ValueType>* printParameterTypes = new List<ValueType>();
	printParameterTypes->add(STRING_VALUE);
	Function* print = new Function("__print"_H, printParameterTypes, VOID_VALUE,
			printFunction);
	syscalls.add(print);
	mish_syscalls.add(print);

	// exit
	List<ValueType>* exitParameterTypes = new List<ValueType>();
	Function* exit = new Function("__exit"_H, exitParameterTypes, VOID_VALUE, exitFunction);
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
