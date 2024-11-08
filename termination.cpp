#include "termination.h"

Termination* Termination::instance;

Termination::Termination() 
{
	this->isTerminated = false;
}

Termination* Termination::GetInstance() 
{
	if (Termination::instance == nullptr)
		Termination::instance = new Termination();

	return Termination::instance;
}

void Termination::Terminate() 
{
	if (Termination::IsTerminated())
		throw "The event loop has already been terminated.\n";

	Termination::GetInstance()->isTerminated = true;
}

bool Termination::IsTerminated() 
{
	return Termination::GetInstance()->isTerminated;
}