#pragma once
#include "wx/wx.h"
#include <wx/tglbtn.h>

#define MCU_NOMINAL_VOLTAGE (3.3f)
#define ADC_MAX_VALUE (4095u)

class VirtualMcuMain : public wxFrame
{
public:
	VirtualMcuMain();
	~VirtualMcuMain();
	void OnButtonClick(wxCommandEvent &evt);
	void OnSliderUpdate(wxCommandEvent& evt);

protected:
	wxToggleButton* output_pin[4];
	wxToggleButton* input_pin[4];
	wxStaticText* pin_labels[8];
	wxSlider* adc_slider_0;
	wxSlider* adc_slider_1;
	wxStaticText* adc_label_0;
	wxStaticText* adc_label_1;
	wxStaticText* adc_voltage_label_0;
	wxStaticText* adc_voltage_label_1;

private:
	wxDECLARE_EVENT_TABLE();
	int adc_channel_0_value = 0;
	int adc_channel_1_value = 0;
	
};

enum IDs{
	ID_output_0 = wxID_HIGHEST,
	ID_output_1,
	ID_output_2,
	ID_output_3,
	ID_input_0,
	ID_input_1,
	ID_input_2,
	ID_input_3,
	ID_slider_0,
	ID_slider_1
};