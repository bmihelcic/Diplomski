#pragma once

#include "wx/wx.h"

class VirtualMcuApp : public wxApp
{
public:
	VirtualMcuApp();
	~VirtualMcuApp();

public:
	virtual bool OnInit();
};

