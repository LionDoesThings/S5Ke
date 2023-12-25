#pragma once
#include <wx/wx.h>

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
	void HotKeyPromptRestart();

	void SpamText();
	void Click();
	void StartStop(wxCommandEvent& evt);
	

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


	wxRadioBox* clickTypeSelection;
	wxStaticText* clickRepsLabel;
	wxSpinCtrl* clickReps;
	wxStaticText* clickIntervalLabel;
	wxSpinCtrl* clickInterval;
	wxButton* clickStartStop;
};

