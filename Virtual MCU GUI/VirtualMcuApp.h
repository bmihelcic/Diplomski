#pragma once

#include "wx/wx.h"
#include "VirtualMcuMain.h"

// Application derived from wxWidgets application
// doesn't show an
class VirtualMcuApp : public wxApp
{
public:
	VirtualMcuApp();
	~VirtualMcuApp();

public:
	virtual bool OnInit();

private:
	VirtualMcuMain* m_mainFrame = nullptr;
};

