#include "VirtualMcuApp.h"

//generate entry point for the application, like registering "main()" 
wxIMPLEMENT_APP(VirtualMcuApp);

VirtualMcuApp::VirtualMcuApp()
{
}

VirtualMcuApp::~VirtualMcuApp()
{
}

// By overriding wxApp's OnInit() virtual method the program can be initialized
// e.g. by creating a new main window.
bool VirtualMcuApp::OnInit()
{
	// generate a new frame
	m_mainFrame = new VirtualMcuMain();
	// show the frame
	m_mainFrame->Show();

	return true;
}