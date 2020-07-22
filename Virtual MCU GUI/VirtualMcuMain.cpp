#include "VirtualMcuMain.h"
#include <string>

wxBEGIN_EVENT_TABLE(VirtualMcuMain, wxFrame)
	EVT_BUTTON(ID_output_0, VirtualMcuMain::OnButtonClick)
	EVT_BUTTON(ID_output_1, VirtualMcuMain::OnButtonClick)
	EVT_BUTTON(ID_output_2, VirtualMcuMain::OnButtonClick)
	EVT_BUTTON(ID_output_3, VirtualMcuMain::OnButtonClick)
	EVT_BUTTON(ID_input_0, VirtualMcuMain::OnButtonClick)
	EVT_BUTTON(ID_input_1, VirtualMcuMain::OnButtonClick)
	EVT_BUTTON(ID_input_2, VirtualMcuMain::OnButtonClick)
	EVT_BUTTON(ID_input_3, VirtualMcuMain::OnButtonClick)
	EVT_COMMAND_SCROLL_CHANGED(ID_slider_0, VirtualMcuMain::OnSliderUpdate)
	EVT_COMMAND_SCROLL_CHANGED(ID_slider_0, VirtualMcuMain::OnSliderUpdate)
	EVT_MENU(ID_CLIENT_CONNECT, VirtualMcuMain::OnConnectToServer)
	EVT_SOCKET(ID_SOCKET, VirtualMcuMain::OnSocketEvent)
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

	m_menu_bar = new wxMenuBar(0);
	m_main_menu = new wxMenu();
	m_item_connect = new wxMenuItem(m_main_menu, ID_CLIENT_CONNECT, wxString(wxT("Connect")), wxEmptyString, wxITEM_NORMAL);
	m_main_menu->Append(m_item_connect);
	m_menu_bar->Append(m_main_menu, wxT("Menu"));
	this->SetMenuBar(m_menu_bar);

	m_console_output = new wxRichTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 | wxVSCROLL | wxHSCROLL | wxNO_BORDER | wxWANTS_CHARS);
	m_console_output->Enable(true);
	wx_box_sizer_3->Add(m_console_output, 1, wxEXPAND | wxALL, 5);

	

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
	adc_slider_0->Bind(wxEVT_SCROLL_CHANGED, &VirtualMcuMain::OnSliderUpdate, this);
	adc_slider_1 = new wxSlider(this, ID_slider_1, 2047, 0, 4095, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	adc_slider_1->Bind(wxEVT_SCROLL_CHANGED, &VirtualMcuMain::OnSliderUpdate, this);

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
	wx_box_sizer_0->Add(wx_box_sizer_3, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

	this->SetSizer(wx_box_sizer_0);

	this->Layout();
	this->Centre(wxBOTH);
}

void VirtualMcuMain::OnButtonClick(wxCommandEvent& evt)
{
	int current_button_ID = 0;
	bool current_button_state = false;
	wxString current_button_label;
	wxString current_button_ID_label;
	wxString socket_message;

	current_button_ID = evt.GetId() - wxID_HIGHEST; // gives 0-7

	if (current_button_ID < 4)
	{
		current_button_state = output_pin[current_button_ID]->GetValue();
		if (true == current_button_state)
		{
			current_button_label = "HIGH";
			output_pin[current_button_ID]->SetBackgroundColour(*wxBLUE);
		}
		else
		{
			current_button_label = "LOW";
			output_pin[current_button_ID]->SetBackgroundColour(wxNullColour);
		}
		output_pin[current_button_ID]->SetLabel(current_button_label);				
	}
	else
	{
		current_button_state = input_pin[current_button_ID - 4]->GetValue();
		current_button_label = ((true == current_button_state) ? "HIGH" : "LOW");
		input_pin[current_button_ID - 4]->SetLabel(current_button_label);
		current_button_ID_label = std::to_string(current_button_ID - 4);
		if (m_socket != nullptr && m_socket->IsConnected())
		{
			socket_message = wxString("SET INPUT" + current_button_ID_label + " " + current_button_label);
			m_console_output->WriteText(wxT("< sending: " + socket_message));
			m_console_output->Newline();
			m_socket->Write(socket_message, sizeof(socket_message));
		}
	}
	evt.Skip();
}


void VirtualMcuMain::OnSliderUpdate(wxScrollEvent& evt)
{
	int current_slider_ID = 0;
	int slider_value = 0;
	static int slider_value_prev0 = 0;
	static int slider_value_prev1 = 0;
	float voltage = 0.0;
	wxString socket_message;

	current_slider_ID = evt.GetId();

	if (ID_slider_0 == current_slider_ID)
	{
		slider_value = adc_slider_0->GetValue();
		if (slider_value != slider_value_prev0)
		{
			voltage = slider_value * (MCU_NOMINAL_VOLTAGE / (ADC_MAX_VALUE + 1));
			adc_voltage_label_0->SetLabel(wxString::Format("%.2f V", voltage));			
			socket_message = wxString("SET ADC0 " + wxString::Format("%.2f", voltage));
			slider_value_prev0 = slider_value;
		}
	}
	else if(ID_slider_1 == current_slider_ID)
	{
		slider_value = adc_slider_1->GetValue();
		if (slider_value != slider_value_prev1)
		{
			voltage = slider_value * (MCU_NOMINAL_VOLTAGE / (ADC_MAX_VALUE + 1));
			adc_voltage_label_1->SetLabel(wxString::Format("%.2f V", voltage));;
			socket_message = wxString("SET ADC1 " + wxString::Format("%.2f", voltage));
			slider_value_prev1 = slider_value;
		}		
	}
	m_console_output->AppendText("< sending: ");
	m_console_output->AppendText(socket_message);
	m_console_output->Newline();
	if (m_socket != nullptr && m_socket->IsConnected())
	{
		m_socket->Write(socket_message, sizeof(socket_message));
	}	
	evt.Skip();
}

void VirtualMcuMain::OnConnectToServer(wxCommandEvent& WXUNUSED(event))
{
	wxIPV4address addr;
	addr.Hostname(wxT("localhost"));
	addr.Service(8080);
	// Create the socket
	m_socket = new wxSocketClient();
	// Set up the event handler and subscribe to most events
	m_socket->SetEventHandler(*this, ID_SOCKET);
	m_socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	m_socket->Notify(true);
	// Wait for the connection event
	m_socket->Connect(addr, false);
	m_console_output->WriteText(wxT("(info) Connecting to localhost:8080"));
	m_console_output->Newline();
}

void VirtualMcuMain::OnSocketEvent(wxSocketEvent& event)
{
	// The socket that had the event
	wxSocketBase* sock = event.GetSocket();
	// Common buffer shared by the events
	char buf[40];
	wxString wxBuf;
	int foundAtIndex = 0;
	switch (event.GetSocketEvent())
	{
		case wxSOCKET_CONNECTION:
		{
			break;
		}
		case wxSOCKET_INPUT:
		{
			sock->Read(buf, sizeof(buf));
			m_console_output->AppendText("> received: ");
			m_console_output->AppendText(buf);
			m_console_output->Newline();
			wxBuf = wxString::Format("%s", buf);
			foundAtIndex = wxBuf.Find("OUTPUT_STATE");
			if (wxNOT_FOUND != foundAtIndex)
			{
				for (int i = 0; i < 4; i++)
				{
					if ('1' == buf[foundAtIndex + 13 + i])
					{
						output_pin[i]->SetValue(true);
						output_pin[i]->SetLabel("HIGH");
						output_pin[i]->SetBackgroundColour(*wxBLUE);
					}
					else if ('N' == buf[foundAtIndex + 13 + i])
					{
						output_pin[i]->SetValue(false);
						output_pin[i]->SetLabel("LOW");
						output_pin[i]->SetBackgroundColour(wxNullColour);
					}
				}
			}
			break;
		}// The server hangs up after sending the data
		case wxSOCKET_LOST:
		{
			sock->Destroy();
			break;
		}
	}
}

VirtualMcuMain::~VirtualMcuMain()
{
	// if socket has been initialized
	if (m_socket != nullptr)
	{
		m_socket->Destroy();
	}
}

