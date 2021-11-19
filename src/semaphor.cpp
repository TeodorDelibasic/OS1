#include "semaphor.h"
#include "kerSem.h"

Semaphore::Semaphore(int init) {
	disableContextSwitch;
	this->myImpl = new KernelSem(init);
	enableContextSwitch;
}

Semaphore::~Semaphore() {
	disableContextSwitch;
	delete this->myImpl;
	enableContextSwitch;
}

int Semaphore::wait(Time maxTimeToWait) {
	return this->myImpl->wait(maxTimeToWait);
}

void Semaphore::signal() {
	this->myImpl->signal();
}

int Semaphore::val() const {
	return this->myImpl->val();
}
