// TeacherDuties2.cpp : main project file.
#include "stdafx.h"
#include "Form1.h"

// Added
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

// For random numbers
#include <cstdlib>
#include <ctime>


;
using namespace TeacherDuties2;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}

