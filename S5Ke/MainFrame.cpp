#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/msgdlg.h>
#include <windows.h>
#include <thread>

bool started{ false };

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	CreateControls();
	BindEventHandlers();
	HotKeyDetection();
}

void MainFrame::CreateControls() { //400,240
	mainPanel = new wxPanel(this);

	panelSpam = new wxPanel(mainPanel, wxID_ANY, wxPoint(30, 0), wxSize(370, 240));
	panelClick = new wxPanel(mainPanel, wxID_ANY, wxPoint(30, 0), wxSize(370, 240));

	sideButtonSpam = new wxButton(mainPanel, wxID_ANY, "S\np\na\nm", wxPoint(0, 0), wxSize(30, 90));
	sideButtonClick = new wxButton(mainPanel, wxID_ANY, "C\nl\ni\nc\nk", wxPoint(0, 90), wxSize(30, 90));


	spamTextLabel = new wxStaticText(panelSpam, wxID_ANY, "Enter a text to spam:", wxPoint(20, 10), wxSize(240, 30));
	spamText= new wxTextCtrl(panelSpam, wxID_ANY, "", wxPoint(20, 30), wxSize(240, 30));
	spamRepsLabel = new wxStaticText(panelSpam, wxID_ANY, "Repetitions", wxPoint(20, 80), wxSize(100, 30));
	spamReps = new wxSpinCtrl(panelSpam, wxID_ANY, "", wxPoint(22, 100), wxSize(100, 30), wxALIGN_LEFT, 1, 1000, 1);
	spamDelayLabel = new wxStaticText(panelSpam, wxID_ANY, "Delay (ms)", wxPoint(160, 80), wxSize(100, 30));
	spamDelay = new wxSpinCtrl(panelSpam, wxID_ANY, "", wxPoint(162, 100), wxSize(100, 30), wxALIGN_LEFT, 0, 1000000, 70);
	spamStartStop = new wxButton(panelSpam, wxID_ANY, "Start/Stop (F6)", wxPoint(85, 140), wxSize(100, 30));
}

void MainFrame::BindEventHandlers() {
	sideButtonSpam->Bind(wxEVT_BUTTON, &MainFrame::SwitchSpam, this);
	sideButtonClick->Bind(wxEVT_BUTTON, &MainFrame::SwitchClick, this);

	spamStartStop->Bind(wxEVT_BUTTON, &MainFrame::StartSpamStop, this);
}

void MainFrame::HotKeyDetection() {
	const auto hkdetect = [this]() {
		while (true) {
			if (GetKeyState(VK_F6) & 0x8000) {
				if (started) {
					started = false;
					Iconize(false);
				}
				else {
					if (spamText->GetValue().ToStdString().empty()) { wxMessageBox(wxT("Text to spam is empty!"), wxT("S5Ke")); continue; }
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
			std::string text = spamText->GetValue().ToStdString();
			int reps = spamReps->GetValue();
			int delay = spamDelay->GetValue();

			INPUT ip;
			ip.type = INPUT_KEYBOARD;
			ip.ki.wScan = 0;
			ip.ki.time = 0;
			ip.ki.dwExtraInfo = 0;

			for (int rep = reps; rep > 0; rep--) {
				for (int chars = 0; chars < text.length(); chars++) {
					ip.ki.wVk = VkKeyScanEx(text[chars], GetKeyboardLayout(0));
					ip.ki.dwFlags = 0; // 0 for key press
					SendInput(1, &ip, sizeof(INPUT));

					ip.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &ip, sizeof(INPUT));
				}
				ip.ki.wVk = 0x0D; // Enter key
				ip.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &ip, sizeof(INPUT));

				ip.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &ip, sizeof(INPUT));

				Sleep(delay);
			}
			started = false;
			Iconize(false);
		}
	};
	std::thread bck{ spamtext };
	bck.detach();
}

void MainFrame::SwitchSpam(wxCommandEvent& evt) {
	panelSpam->Show();
	panelClick->Hide();
}

void MainFrame::SwitchClick(wxCommandEvent& evt) {
	panelSpam->Hide();
	panelClick->Show();
}

void MainFrame::StartSpamStop(wxCommandEvent& evt) {
	if (spamText->GetValue().ToStdString().empty()) {
		wxMessageBox(wxT("Text to spam is empty!"), wxT("S5Ke"));
		return;
	}

	if (!started) {
		started = true;
		Iconize(true);
		Sleep(3000);
		SpamText();
	}
	else
	{
		started = false;
	}
}