#ifndef TERMINATION_H_
#define TERMINATION_H_

class Termination {
public:
	static bool IsTerminated();
	static void Terminate();
	static Termination* GetInstance();
private:
	Termination();

	static Termination* instance;
	bool isTerminated;
};

#endif