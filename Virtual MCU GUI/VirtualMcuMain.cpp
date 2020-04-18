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
	EVT_SLIDER(ID_slider_0, OnSliderUpdate)
	EVT_SLIDER(ID_slider_1, OnSliderUpdate)
wxEND_EVENT_TABLE()

// main frame doesn't have any parent, ID can be any, and at last give it a title
VirtualMcuMain::VirtualMcuMain() : wxFrame(nullptr, wxID_ANY, "VIRTUAL MCU")
{
	this->SetSizeHints(500, 250);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));

	wxBoxSizer* wx_box_sizer_0;
	wx_box_sizer_0 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* wx_box_sizer_1;
	wx_box_sizer_1 = new wxBoxSizer(wxHORIZONTAL);

	wxFlexGridSizer* wx_flex_grid_sizer_1_1;
	wx_flex_grid_sizer_1_1 = new wxFlexGridSizer(0, 4, 0, 0);
	wx_flex_grid_sizer_1_1->SetFlexibleDirection(wxBOTH);
	for (int i = 0; i < 4; i++) { wx_flex_grid_sizer_1_1->AddGrowableCol(i);}
	//wx_flex_grid_sizer_1_1->AddGrowableRow(1);	
	wx_flex_grid_sizer_1_1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxFlexGridSizer* wx_flex_grid_sizer_1_2;
	wx_flex_grid_sizer_1_2 = new wxFlexGridSizer(0, 4, 0, 0);
	wx_flex_grid_sizer_1_2->SetFlexibleDirection(wxBOTH);
	for (int i = 0; i < 4; i++) { wx_flex_grid_sizer_1_2->AddGrowableCol(i); }
	//wx_flex_grid_sizer_1_2->AddGrowableRow(1);	
	wx_flex_grid_sizer_1_2->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	
	wxBoxSizer* wx_box_sizer_2;
	wx_box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);

	wxFlexGridSizer* wx_flex_grid_sizer_2_1;
	wx_flex_grid_sizer_2_1 = new wxFlexGridSizer(0, 3, 0, 0);
	//for (int i = 0; i < 3; i++) { wx_flex_grid_sizer_2_1->AddGrowableCol(i); }
	wx_flex_grid_sizer_2_1->AddGrowableCol(1);	
	wx_flex_grid_sizer_2_1->SetFlexibleDirection(wxBOTH);
	wx_flex_grid_sizer_2_1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxBoxSizer* wx_box_sizer_3;
	wx_box_sizer_3 = new wxBoxSizer(wxHORIZONTAL);

	for (int i = 0; i < 8; i++)
	{
		if (i < 4)
		{
			pin_labels[i] = new wxStaticText(this, wxID_ANY, wxString::Format("OUT%i", i), wxDefaultPosition, wxDefaultSize, 0);
			pin_labels[i]->Wrap(-1);
			wx_flex_grid_sizer_1_1->Add(pin_labels[i], 0, wxALIGN_CENTER, 0);
		}
		else
		{
			pin_labels[i] = new wxStaticText(this, wxID_ANY, wxString::Format("IN%i", i-4), wxDefaultPosition, wxDefaultSize, 0);
			pin_labels[i]->Wrap(-1);
			wx_flex_grid_sizer_1_2->Add(pin_labels[i], 0, wxALIGN_CENTER, 0);
		}		
	}

	for (int i = 0; i < 4; i++)
	{
		output_pin[i] = new wxToggleButton(this,ID_output_0 + i, wxString::Format("LOW",i), wxDefaultPosition, wxDefaultSize, 0);
		output_pin[i]->SetMinSize(wxSize(50, 50));
		output_pin[i]->SetValue(false);
		output_pin[i]->Bind(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &VirtualMcuMain::OnButtonClick, this);
		wx_flex_grid_sizer_1_1->Add(output_pin[i], 0, wxEXPAND, 0);
		

		input_pin[i] = new wxToggleButton(this, ID_input_0 + i, wxString::Format("LOW", i), wxDefaultPosition, wxDefaultSize, 0);
		input_pin[i]->SetMinSize(wxSize(50, 50));
		output_pin[i]->SetValue(false);
		input_pin[i]->Bind(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &VirtualMcuMain::OnButtonClick, this);
		wx_flex_grid_sizer_1_2->Add(input_pin[i], 0, wxEXPAND, 0);
	}	

	adc_label_0 = new wxStaticText(this, wxID_ANY, wxT("ADC_CH0"), wxDefaultPosition, wxDefaultSize, 0);
	adc_label_1 = new wxStaticText(this, wxID_ANY, wxT("ADC_CH1"), wxDefaultPosition, wxDefaultSize, 0);
	adc_voltage_label_0 = new wxStaticText(this, wxID_ANY, wxT("1.65 V"), wxDefaultPosition, wxDefaultSize, 0);
	adc_voltage_label_1 = new wxStaticText(this, wxID_ANY, wxT("1.65 V"), wxDefaultPosition, wxDefaultSize, 0);
	adc_slider_0 = new wxSlider(this, ID_slider_0, 2047, 0, 4095, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	adc_slider_0->Bind(wxEVT_SLIDER, &VirtualMcuMain::OnSliderUpdate, this);
	adc_slider_1 = new wxSlider(this, ID_slider_1, 2047, 0, 4095, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	adc_slider_1->Bind(wxEVT_SLIDER, &VirtualMcuMain::OnSliderUpdate, this);

	wx_flex_grid_sizer_2_1->Add(adc_label_0, 0, wxALIGN_CENTER | wxALL, 5);
	wx_flex_grid_sizer_2_1->Add(adc_slider_0, 1, wxEXPAND | wxALL, 5);
	wx_flex_grid_sizer_2_1->Add(adc_voltage_label_0, 0, wxALIGN_CENTER | wxALL, 5);
	wx_flex_grid_sizer_2_1->Add(adc_label_1, 0, wxALIGN_CENTER | wxALL, 5);
	wx_flex_grid_sizer_2_1->Add(adc_slider_1, 1, wxEXPAND | wxALL, 5);
	wx_flex_grid_sizer_2_1->Add(adc_voltage_label_1, 0, wxALIGN_CENTER | wxALL, 5);

	wx_box_sizer_2->Add(wx_flex_grid_sizer_2_1, 1, wxEXPAND | wxRIGHT, 20);

	wx_box_sizer_1->Add(wx_flex_grid_sizer_1_1, 1, wxRIGHT | wxEXPAND, 10);
	wx_box_sizer_1->Add(wx_flex_grid_sizer_1_2, 1, wxLEFT | wxEXPAND, 10);
	wx_box_sizer_0->Add(wx_box_sizer_1, 1, wxLEFT | wxRIGHT | wxTOP| wxSHAPED, 10);
	wx_box_sizer_0->Add(wx_box_sizer_2, 1, wxLEFT | wxRIGHT | wxSHAPED, 10);
	wx_box_sizer_0->Add(wx_box_sizer_3, 1, wxLEFT | wxRIGHT | wxSHAPED, 10);

	this->SetSizer(wx_box_sizer_0);
	this->Layout();
	this->Centre(wxBOTH);
}

void VirtualMcuMain::OnButtonClick(wxCommandEvent& evt)
{
	int current_button_ID = 0;
	bool current_button_state = false;
	wxString current_button_label;

	current_button_ID = evt.GetId() - wxID_HIGHEST; // gives 0-7

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


void VirtualMcuMain::OnSliderUpdate(wxCommandEvent& evt)
{
	int current_slider_ID = 0;
	int slider_value = 0;
	float voltage = 0.0;

	current_slider_ID = evt.GetId();

	if (ID_slider_0 == current_slider_ID)
	{
		slider_value = adc_slider_0->GetValue();
		voltage = slider_value * (MCU_NOMINAL_VOLTAGE / (ADC_MAX_VALUE + 1));
		adc_voltage_label_0->SetLabel(wxString::Format("%.2f V", voltage));
	}
	else if(ID_slider_1 == current_slider_ID)
	{
		slider_value = adc_slider_1->GetValue();
		voltage = slider_value * (MCU_NOMINAL_VOLTAGE / (ADC_MAX_VALUE + 1));
		adc_voltage_label_1->SetLabel(wxString::Format("%.2f V", voltage));;
	}
	evt.Skip();
}


VirtualMcuMain::~VirtualMcuMain()
{

}