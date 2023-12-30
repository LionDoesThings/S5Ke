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
	void HotKeyPromptRestart();
	void LoadHotKey();
	void ChangeNewHotkey(wxCommandEvent& evt);

	void SpamText();
	void Click();
	void StartStop(wxCommandEvent& evt);

	void OnWindowClosed(wxCloseEvent& evt);
	

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
	wxButton* spamHK;


	wxRadioBox* clickTypeSelection;
	wxStaticText* clickRepsLabel;
	wxSpinCtrl* clickReps;
	wxStaticText* clickDelayLabel;
	wxSpinCtrl* clickDelay;
	wxButton* clickStartStop;
	wxButton* clickHK;
};

