#pragma once
#include "wx/wx.h"
#include <wx/tglbtn.h>

class VirtualMcuMain : public wxFrame
{
	enum {
		ID_output_0 = 8000,
		ID_output_1,
		ID_output_2,
		ID_output_3,
		ID_input_0,
		ID_input_1,
		ID_input_2,
		ID_input_3,
	};
public:
	VirtualMcuMain();
	~VirtualMcuMain();
	void OnButtonClick(wxCommandEvent &evt);

protected:
	wxToggleButton* output_pin[4];
	wxToggleButton* input_pin[4];
	wxStaticText* pin_labels[8];

private:
	wxDECLARE_EVENT_TABLE();
	
};