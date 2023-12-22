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
	void HotKeyDetection();
	void SpamText();

	void SwitchSpam(wxCommandEvent& evt);
	void SwitchClick(wxCommandEvent& evt);
	void SwitchEmpty(wxCommandEvent& evt);

	wxPanel* mainPanel;

	wxPanel* panelSpam;
	wxPanel* panelClick;
	wxPanel* panelEmpty;
	wxButton* sideButtonSpam;
	wxButton* sideButtonClick;
	wxButton* sideButtonEmpty;


	wxStaticText* spamTextLabel;
	wxTextCtrl* spamText;
	wxStaticText* spamRepsLabel;
	wxSpinCtrl* spamReps;
	wxButton* spamStart;
	wxButton* spamStop;

	void StartSpam(wxCommandEvent& evt);
	void StopSpam(wxCommandEvent& evt);
};

