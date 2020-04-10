#include "VirtualMcuApp.h"

wxIMPLEMENT_APP(VirtualMcuApp);

VirtualMcuApp::VirtualMcuApp()
{

}

VirtualMcuApp::~VirtualMcuApp()
{

}

bool VirtualMcuApp::OnInit()
{
	m_mainFrame = new VirtualMcuMain();
	m_mainFrame->Show();

	return true;
}