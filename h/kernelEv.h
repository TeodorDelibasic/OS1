#ifndef KERNELEV_H_
#define KERNELEV_H_

class PCB;

typedef unsigned char IVTNo;

class KernelEv {

public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();

private:
	PCB* owner;
	IVTNo ivtNo;
	int value;
};

#endif
