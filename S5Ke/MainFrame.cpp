#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <windows.h>
#include <thread>

bool started{ false }; int i{ 0 };

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	CreateControls();
	BindEventHandlers();
	HotKeyDetection();
}

void MainFrame::CreateControls() { //400,240
	mainPanel = new wxPanel(this);

	panelSpam = new wxPanel(mainPanel, wxID_ANY, wxPoint(30, 0), wxSize(370, 240));
	panelClick = new wxPanel(mainPanel, wxID_ANY, wxPoint(30, 0), wxSize(370, 240));
	panelEmpty = new wxPanel(mainPanel, wxID_ANY, wxPoint(30, 0), wxSize(370, 240));

	sideButtonSpam = new wxButton(mainPanel, wxID_ANY, "S\np\na\nm", wxPoint(0, 0), wxSize(30, 80));
	sideButtonClick = new wxButton(mainPanel, wxID_ANY, "C\nl\ni\nc\nk", wxPoint(0, 80), wxSize(30, 80));
	sideButtonEmpty = new wxButton(mainPanel, wxID_ANY, "", wxPoint(0, 160), wxSize(30, 80));


	spamTextLabel = new wxStaticText(panelSpam, wxID_ANY, "Enter a text to spam:", wxPoint(20, 10), wxSize(240, 30));
	spamText= new wxTextCtrl(panelSpam, wxID_ANY, "", wxPoint(20, 30), wxSize(240, 30));
	spamRepsLabel = new wxStaticText(panelSpam, wxID_ANY, "Repetitions (0 for infinite):", wxPoint(20, 80), wxSize(240, 30));
	spamReps = new wxSpinCtrl(panelSpam, wxID_ANY, "", wxPoint(22, 100), wxSize(100, 30));
	spamStart = new wxButton(panelSpam, wxID_ANY, "Start", wxPoint(60, 140), wxSize(80, 30));
	spamStop = new wxButton(panelSpam, wxID_ANY, "Stop", wxPoint(200, 140), wxSize(80, 30));

	CreateStatusBar();
}

void MainFrame::BindEventHandlers() {
	sideButtonSpam->Bind(wxEVT_BUTTON, &MainFrame::SwitchSpam, this);
	sideButtonClick->Bind(wxEVT_BUTTON, &MainFrame::SwitchClick, this);
	sideButtonEmpty->Bind(wxEVT_BUTTON, &MainFrame::SwitchEmpty, this);

	spamStart->Bind(wxEVT_BUTTON, &MainFrame::StartSpam, this);
	spamStop->Bind(wxEVT_BUTTON, &MainFrame::StopSpam, this);
}

void MainFrame::HotKeyDetection() {
	const auto hkdetect = [this]() {
		while (true) {
			if (GetKeyState(VK_F5) & 0x8000) {
				if (started) {
					started = false;
					Iconize(false);
				}
				else {
					started = true;
					Iconize(true);
					SpamText();
				}
				Sleep(300);
			}
		}
	};
	std::thread bck{ hkdetect };
	bck.detach();
}

void MainFrame::SpamText() {
	const auto spamtext = [this]() {
		while (started) {
			i++;
			wxLogStatus("%d", i);
		}
	};
	std::thread bck{ spamtext };
	bck.detach();
}

void MainFrame::SwitchSpam(wxCommandEvent& evt) {
	panelSpam->Show();
	panelClick->Hide();
	panelEmpty->Hide();
}

void MainFrame::SwitchClick(wxCommandEvent& evt) {
	panelSpam->Hide();
	panelClick->Show();
	panelEmpty->Hide();
}

void MainFrame::SwitchEmpty(wxCommandEvent& evt) {
	panelSpam->Hide();
	panelClick->Hide();
	panelEmpty->Show();
}

void MainFrame::StartSpam(wxCommandEvent& evt) {
	started = true;
	Iconize(true);
	Sleep(3000);
	SpamText();
}

void MainFrame::StopSpam(wxCommandEvent& evt) {
	started = false;
}