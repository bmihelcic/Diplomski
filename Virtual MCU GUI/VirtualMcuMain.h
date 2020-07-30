#pragma once
#include "wx/wx.h"
#include <wx/tglbtn.h>
#include <wx/sckipc.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/slider.h>

#define MCU_NOMINAL_VOLTAGE (3.3f)
#define ADC_MAX_VALUE (4095u)

class VirtualMcuMain : public wxFrame
{
public:
	VirtualMcuMain();
	~VirtualMcuMain();
	void OnButtonClick(wxCommandEvent& evt);
	void OnSliderUpdate(wxScrollEvent& evt);
	void OnConnectToServer(wxCommandEvent& evt);
	void OnRefresh(wxCommandEvent& evt);
	void OnClientClose(wxCommandEvent& evt);
	void OnSocketEvent(wxSocketEvent& evt);

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
	wxMenuBar* m_menu_bar;
	wxMenu* m_main_menu;
	wxMenuItem* m_item_connect;
	wxMenuItem* m_item_refresh;
	wxMenuItem* m_item_close;
	wxRichTextCtrl* m_console_output;
	wxSocketClient* m_socket;

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
	ID_slider_1,
	ID_CLIENT_CONNECT,
	ID_CLIENT_REFRESH,
	ID_CLIENT_CLOSE,
	ID_SOCKET,
	ID_RICHTEXT_WINDOW
};