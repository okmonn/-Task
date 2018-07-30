#include "DirectX/Union.h"
#include <iostream>

#define WINDOW_X 640
#define WINDOW_Y 480

// エントリーポイント
int main()
{
	Union::Get().ChangeWindowSize(WINDOW_X, WINDOW_Y);
	Union::Get().Create();
	
	while (Union::Get().CheckMsg())
	{

	}

	return 0;
}