//
// Concurrency test
//
//    Eduard Grasa          <eduard.grasa@i2cat.net>
//    Francesco Salvestrini <f.salvestrini@nextworks.it>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <iostream>
#include <math.h>
#include <unistd.h>

#include "concurrency.h"

#define NUM_THREADS 5
#define TRIGGER 10

using namespace rina;

void * doWork(void * arg) {
	intptr_t number = (intptr_t) arg;
	std::cout << "Thread " << number << " started work\n";
	return (void *) number;
}

class LockableCounter: public Lockable {
public:
	LockableCounter() :
                Lockable() {
		counter = 0;
	}
	void count() {
		lock();
		counter++;
		std::cout << "Incremented counter, current value: " << counter << "\n";
		unlock();
	}

	int getCount() {
		return counter;
	}
private:
	int counter;
};

void * doWorkMutex(void * arg) {
	LockableCounter * counter = (LockableCounter *) arg;
	counter->count();
	return (void *) 0;
}

class ReadWriteLockableCounter: public ReadWriteLockable {
public:
	ReadWriteLockableCounter() :
                ReadWriteLockable() {
		counter = 0;
	}

	void count() {
		writelock();
		counter++;
		std::cout<<"Incremented counter; current value is "<<
                        counter<<"\n";
		unlock();
	}

	int getCounter() {
		int result;
		readlock();
		result = counter;
		unlock();
		return result;
	}

private:
	int counter;
};

void * doWorkReadWriteLock(void * arg) {
	ReadWriteLockableCounter * counter = (ReadWriteLockableCounter *) arg;
	counter->count();
	return (void *) 0;
}

class ConditionVariableCounter: public ConditionVariable{
public:
	ConditionVariableCounter() : ConditionVariable(){
		counter = 0;
	}

	bool count(){
		bool result = false;
		lock();
		counter ++;
		std::cout<<"Incremented counter; current value is "<<
                        counter<<"\n";
		if (counter >= TRIGGER){
			std::cout<<"Counter reached threshold, signaling \n";
			result = true;
			signal();
		}
		unlock();
		return result;
	}

	int getCounter(){
		int result;
		lock();
		if (counter < TRIGGER){
			std::cout<<"Counter below threshold, waiting \n";
			wait();
		}
		std::cout<<"Counter reached threshold, value: "<<counter<<" \n";
		result = counter;
		unlock();
		return result;
	}

private:
	int counter;
};

void * doWorkConditionVariable(void * arg)
{
	ConditionVariableCounter * counter = (ConditionVariableCounter *) arg;
	while(!counter->count()){
		usleep(1000*100);
	}
	return (void *) 0;
}

void * doWorkWaitForTrigger(void * arg)
{
	ConditionVariableCounter * counter = (ConditionVariableCounter *) arg;

	std::cout<<"Trying to read counter \n";
	intptr_t result = counter->getCounter();

	return (void *) result;
}

int main(int argc, char * argv[])
{
	std::cout << "TESTING CONCURRENCY WRAPPER CLASSES\n";

	/* Test get concurrency */
	std::cout << "Concurrency: " << Thread::getConcurrency() << "\n";

	/* Test self */
	Thread * myself = Thread::self();
	std::cout << "Thread id: " << myself->getThreadType() << "\n";

	/* Should throw an Exception, I cannot join myself */
	try {
		myself->join(NULL);
	} catch (ConcurrentException &e) {
		std::cout << "Caught expected exception. " << e.what() << "\n";
	}

	/* Test equals */
	Thread *myself2 = Thread::self();
	if ((*myself) != (*myself2)) {
		std::cout << "Error, both threads should have been equal\n";
		return -1;
	}

	delete myself;
	delete myself2;

	/* Test Thread creation and joining */
	Thread * threads[NUM_THREADS];
	ThreadAttributes * threadAttributes = new ThreadAttributes();
	threadAttributes->setJoinable();
	for (intptr_t i = 0; i < NUM_THREADS; i++) {
		threads[i] = new Thread(threadAttributes, &doWork, (void *) i);
		std::cout << "Created thread " << i << " with id "
                          << threads[i]->getThreadType() << "\n";
	}
	delete threadAttributes;

	void * status;
	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i]->join(&status);
		std::cout << "Completed join with thread " << i
                          << " having a status of " << status << "\n";
		delete threads[i];
	}

	/* Test mutex */
	threadAttributes = new ThreadAttributes();
	threadAttributes->setJoinable();
	LockableCounter * counter = new LockableCounter();
	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i] = new Thread(threadAttributes, &doWorkMutex,
                                        (void *) counter);
		std::cout << "Created thread " << i << " with id "
                          << threads[i]->getThreadType() << "\n";
	}
	delete threadAttributes;

	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i]->join(&status);
		std::cout << "Completed join with thread " << i
                          << " having a status of " << status << "\n";
		delete threads[i];
	}

	std::cout << "Count value is " << counter->getCount() << "\n";
	if (counter->getCount() != NUM_THREADS){
		return -1;
	}
	delete counter;

	/* Test read-write lock */
	threadAttributes = new ThreadAttributes();
	threadAttributes->setJoinable();
	ReadWriteLockableCounter * counter2 = new ReadWriteLockableCounter();
	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i] = new Thread(threadAttributes, &doWorkReadWriteLock,
                                        (void *) counter2);
		std::cout << "Created thread " << i << " with id "
                          << threads[i]->getThreadType() << "\n";
	}
	delete threadAttributes;

	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i]->join(&status);
		std::cout << "Completed join with thread " << i
                          << " having a status of " << status
                          << ". Current counter value is " << counter2->getCounter()
                          << "\n";
		delete threads[i];
	}

	std::cout << "Count value is " << counter->getCount() << "\n";
	if (counter2->getCounter() != NUM_THREADS){
		return -1;
	}
	delete counter2;

	/* Test condition variable */
	threadAttributes = new ThreadAttributes();
	threadAttributes->setJoinable();
	ConditionVariableCounter * counter3 = new ConditionVariableCounter();
	threads[0] = new Thread(threadAttributes, &doWorkWaitForTrigger,
                                (void *) counter3);
	std::cout << "Created thread 0 with id "
                  << threads[0]->getThreadType() << "\n";
	for (int i = 1; i < NUM_THREADS; i++) {
		threads[i] = new Thread(threadAttributes, &doWorkConditionVariable,
                                        (void *) counter3);
		std::cout << "Created thread " << i << " with id "
                          << threads[i]->getThreadType() << "\n";
	}
	delete threadAttributes;

	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i]->join(&status);
		std::cout << "Completed join with thread " << i
                          << " having a status of " << status
                          << "\n";
		delete threads[i];
	}

	delete counter3;

	/* Test exit */
	Thread::exit(NULL);
	/* This piece of code won't be invoked since it is called after Thread::exit() */
	return -1;
}
