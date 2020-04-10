#pragma once

#include "wx/wx.h"
#include "VirtualMcuMain.h"

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

