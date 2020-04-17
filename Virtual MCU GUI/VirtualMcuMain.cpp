#include "VirtualMcuMain.h"

wxBEGIN_EVENT_TABLE(VirtualMcuMain, wxFrame)
	EVT_BUTTON(ID_output_0, OnButtonClick)
	EVT_BUTTON(ID_output_1, OnButtonClick)
	EVT_BUTTON(ID_output_2, OnButtonClick)
	EVT_BUTTON(ID_output_3, OnButtonClick)
	EVT_BUTTON(ID_input_0, OnButtonClick)
	EVT_BUTTON(ID_input_1, OnButtonClick)
	EVT_BUTTON(ID_input_2, OnButtonClick)
	EVT_BUTTON(ID_input_3, OnButtonClick)
wxEND_EVENT_TABLE()

// main frame doesn't have any parent, ID can be any, and at last give it a title
VirtualMcuMain::VirtualMcuMain() : wxFrame(nullptr, wxID_ANY, "VIRTUAL MCU")
{
	this->SetSizeHints(500, 250);

	wxBoxSizer* wx_box_sizer_0;
	wx_box_sizer_0 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* wx_box_sizer_1;
	wx_box_sizer_1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* wx_box_sizer_1_1;
	wx_box_sizer_1_1 = new wxBoxSizer(wxHORIZONTAL);
	wxGridSizer* wx_grid_sizer_1_1_1;
	wx_grid_sizer_1_1_1 = new wxGridSizer(0, 4, 0, 0);
	wxGridSizer* wx_grid_sizer_1_1_2;
	wx_grid_sizer_1_1_2 = new wxGridSizer(0, 4, 0, 0);
	wxBoxSizer* wx_box_sizer_1_2;
	wx_box_sizer_1_2 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* wx_box_sizer_2;
	wx_box_sizer_2 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* wx_box_sizer_3;
	wx_box_sizer_3 = new wxBoxSizer(wxVERTICAL);

	for (int i = 0; i < 8; i++)
	{
		if (i < 4)
		{
			pin_labels[i] = new wxStaticText(this, wxID_ANY, wxString::Format("OUT%i", i), wxDefaultPosition, wxDefaultSize, 0);
			wx_grid_sizer_1_1_1->Add(pin_labels[i], 0, wxALIGN_CENTER, 0);
		}
		else
		{
			pin_labels[i] = new wxStaticText(this, wxID_ANY, wxString::Format("IN%i", i-4), wxDefaultPosition, wxDefaultSize, 0);
			wx_grid_sizer_1_1_2->Add(pin_labels[i], 0, wxALIGN_CENTER, 0);
		}		
	}

	for (int i = 0; i < 4; i++)
	{
		output_pin[i] = new wxToggleButton(this,ID_output_0 + i, wxString::Format("LOW",i), wxDefaultPosition, wxDefaultSize, 0);
		output_pin[i]->SetMinSize(wxSize(50, 50));
		output_pin[i]->SetValue(false);
		output_pin[i]->Bind(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &VirtualMcuMain::OnButtonClick, this);
		wx_grid_sizer_1_1_1->Add(output_pin[i], 0, wxEXPAND, 0);
		

		input_pin[i] = new wxToggleButton(this, ID_input_0 + i, wxString::Format("LOW", i), wxDefaultPosition, wxDefaultSize, 0);
		input_pin[i]->SetMinSize(wxSize(50, 50));
		output_pin[i]->SetValue(false);
		input_pin[i]->Bind(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &VirtualMcuMain::OnButtonClick, this);
		wx_grid_sizer_1_1_2->Add(input_pin[i], 0, wxEXPAND, 0);
	}	

	wx_box_sizer_1_1->Add(wx_grid_sizer_1_1_1, 1,  wxLEFT, 5);
	wx_box_sizer_1_1->Add(wx_grid_sizer_1_1_2, 1,  wxLEFT|wxRIGHT, 5);
	wx_box_sizer_1->Add(wx_box_sizer_1_1, 1, wxEXPAND, 5);
	wx_box_sizer_1->Add(wx_box_sizer_1_2, 1, wxEXPAND, 5);
	wx_box_sizer_0->Add(wx_box_sizer_1, 1, wxEXPAND, 5);
	wx_box_sizer_0->Add(wx_box_sizer_2, 1, wxEXPAND, 5);
	wx_box_sizer_0->Add(wx_box_sizer_3, 1, wxEXPAND, 5);

	this->SetSizer(wx_box_sizer_0);
	this->Layout();
	this->Centre(wxBOTH);
}

void VirtualMcuMain::OnButtonClick(wxCommandEvent& evt)
{
	int current_button_ID = 0;
	bool current_button_state = false;
	wxString current_button_label;

	current_button_ID = evt.GetId() - 8000; // gives 0-7

	if (current_button_ID < 4)
	{
		current_button_state = output_pin[current_button_ID]->GetValue();
		current_button_label = ((true == current_button_state) ? "HIGH" : "LOW");
		output_pin[current_button_ID]->SetLabel(current_button_label);
	}
	else
	{
		current_button_state = input_pin[current_button_ID-4]->GetValue();
		current_button_label = ((true == current_button_state) ? "HIGH" : "LOW");
		input_pin[current_button_ID-4]->SetLabel(current_button_label);
	}
	evt.Skip();
}


VirtualMcuMain::~VirtualMcuMain()
{

}