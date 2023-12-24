#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>

class MainFrame : public wxFrame
{
public: 
	MainFrame(const wxString& title);
private:
	void CreateControls();
	void BindEventHandlers();
	void SwitchSpam(wxCommandEvent& evt);
	void SwitchClick(wxCommandEvent& evt);

	void HotKeyDetection();

	void SpamText();
	void StartStopSpam(wxCommandEvent& evt);
	

	wxPanel* mainPanel;

	wxPanel* panelSpam;
	wxPanel* panelClick;
	wxButton* sideButtonSpam;
	wxButton* sideButtonClick;


	wxStaticText* spamTextLabel;
	wxTextCtrl* spamText;
	wxStaticText* spamRepsLabel;
	wxSpinCtrl* spamReps;
	wxStaticText* spamDelayLabel;
	wxSpinCtrl* spamDelay;
	wxButton* spamStartStop;
};

