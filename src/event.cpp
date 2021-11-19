#include "event.h"

#include "kernelEv.h"

Event::Event(IVTNo ivtNo) {
	disableContextSwitch;
	this->myImpl = new KernelEv(ivtNo);
	enableContextSwitch;
}

Event::~Event() {
	disableContextSwitch;
	delete this->myImpl;
	enableContextSwitch;
}

void Event::wait() {
	this->myImpl->wait();
}

void Event::signal() {

}
