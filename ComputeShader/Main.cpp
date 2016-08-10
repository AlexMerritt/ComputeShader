#include "System.h"
#include <ctime>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool result;

	srand(static_cast <unsigned> (time(0)));

	System* s = new System();

	result = s->Initialize();
	if (result)
	{
		s->Run();
	}

	s->Shutdown();
	delete s;

	return 0;
}
