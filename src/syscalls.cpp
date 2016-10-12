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

List<Function*> syscalls;

void registerSyscalls() {
	List<ValueType>* printlnParameterTypes = new List<ValueType>();
	printlnParameterTypes->add(STRING_VALUE);
	Function* println = new Function("__println"_H, printlnParameterTypes,
			VOID_VALUE, printlnFunction);
	syscalls.add(println);
	mish_syscalls.add(println);
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
