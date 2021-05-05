#include <stdio.h>
#include <windows.h>
#include "subhook.h"
#include "atlstr.h"
#include "COpenJ2K.h"

int main()
{
	COpenJ2K::instance().J2K_InitializeEx("CSUSER123455", "Dat\\");
	return 0;
}