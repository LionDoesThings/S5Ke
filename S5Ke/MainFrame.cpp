#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <windows.h>
#include <thread>

bool started{ false };
int panel{ 0 }; //0 for spam page, 1 for click page
bool emptyTextBox{ false };

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	CreateControls();
	BindEventHandlers();
	HotKeyDetection();

	panelSpam->Show();
	panelClick->Hide();
	sideButtonSpam->Disable();
	sideButtonClick->Enable();
}

void MainFrame::CreateControls() { //UI
	mainPanel = new wxPanel(this);

	panelSpam = new wxPanel(mainPanel, wxID_ANY, wxPoint(30, 0), wxSize(370, 240));
	panelClick = new wxPanel(mainPanel, wxID_ANY, wxPoint(30, 0), wxSize(370, 240));

	sideButtonSpam = new wxButton(mainPanel, wxID_ANY, "S\np\na\nm", wxPoint(0, 0), wxSize(30, 90));
	sideButtonClick = new wxButton(mainPanel, wxID_ANY, "C\nl\ni\nc\nk", wxPoint(0, 90), wxSize(30, 90));


	spamTextLabel = new wxStaticText(panelSpam, wxID_ANY, "Enter a text to spam", wxPoint(20, 10), wxSize(240, 30));
	spamText= new wxTextCtrl(panelSpam, wxID_ANY, "", wxPoint(20, 30), wxSize(240, 30));
	spamRepsLabel = new wxStaticText(panelSpam, wxID_ANY, "Repetitions", wxPoint(20, 80), wxSize(100, 30));
	spamReps = new wxSpinCtrl(panelSpam, wxID_ANY, "", wxPoint(22, 100), wxSize(100, 30), wxALIGN_LEFT, 1, 1000);
	spamDelayLabel = new wxStaticText(panelSpam, wxID_ANY, "Delay (ms)", wxPoint(160, 80), wxSize(100, 30));
	spamDelay = new wxSpinCtrl(panelSpam, wxID_ANY, "", wxPoint(162, 100), wxSize(100, 30), wxALIGN_LEFT, 1, 1000, 70);
	spamStartStop = new wxButton(panelSpam, wxID_ANY, "Start/Stop (F6)", wxPoint(85, 140), wxSize(100, 30));


	wxArrayString clickType;
	clickType.Add("Left");
	clickType.Add("Right");
	clickType.Add("Middle");
	clickTypeSelection = new wxRadioBox(panelClick, wxID_ANY, "Click Type", wxPoint(20, 10), wxSize(240, 50), clickType);
	clickRepsLabel = new wxStaticText(panelClick, wxID_ANY, "Repetitions (0 for inf)", wxPoint(20, 80), wxSize(120, 30));
	clickReps = new wxSpinCtrl(panelClick, wxID_ANY, "", wxPoint(22, 100), wxSize(100, 30), wxALIGN_LEFT);
	clickIntervalLabel = new wxStaticText(panelClick, wxID_ANY, "Delay (ms)", wxPoint(160, 80), wxSize(100, 30));
	clickInterval = new wxSpinCtrl(panelClick, wxID_ANY, "", wxPoint(162, 100), wxSize(100, 30), wxALIGN_LEFT, 0, 3600, 10);
	clickStartStop = new wxButton(panelClick, wxID_ANY, "Start/Stop (F6)", wxPoint(85, 140), wxSize(100, 30));
}

void MainFrame::BindEventHandlers() { //Button handler
	sideButtonSpam->Bind(wxEVT_BUTTON, &MainFrame::SwitchSpam, this);
	sideButtonClick->Bind(wxEVT_BUTTON, &MainFrame::SwitchClick, this);

	spamStartStop->Bind(wxEVT_BUTTON, &MainFrame::StartStop, this);
	clickStartStop->Bind(wxEVT_BUTTON, &MainFrame::StartStop, this);
}

void MainFrame::SwitchSpam(wxCommandEvent& evt) { //Switch to spam page
	panelSpam->Show();
	panelClick->Hide();
	sideButtonSpam->Disable();
	sideButtonClick->Enable();
	panel = 0; //0 for spam
}

void MainFrame::SwitchClick(wxCommandEvent& evt) { //Switch to click page
	panelSpam->Hide();
	panelClick->Show();
	sideButtonSpam->Enable();
	sideButtonClick->Disable();
	panel = 1; //1 for click
}


void MainFrame::HotKeyDetection() {
	const auto hkdetect = [this]() {
		while (true) {
			if (GetKeyState(VK_F6) & 0x8000) {
				if (!started) { 
					if (panel == 0) {
						if (spamText->GetValue().ToStdString().empty()) { HotKeyPromptRestart(); break; }
						started = true;
						Iconize(true);
						SpamText();
					}
					else {
						started = true;
						Iconize(false);
						Click();
					}
				}
				else {
					started = false;
					Iconize(false);
				}
				Sleep(200);
			}
		}
	};
	std::thread bck{ hkdetect };
	bck.detach();
}

void MainFrame::HotKeyPromptRestart() { //Jank code and attempt so hotkeydetection doesn't stop working for no reason
	wxMessageBox(wxT("Text to spam is empty!"), wxT("S5Ke"));
	HotKeyDetection();
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
		started = false; //Double started false and iconize false to make sure window pops up when ended normally or when force stopped
		Iconize(false);
	};
	std::thread bck{ spamtext };
	bck.detach();
}

void MainFrame::Click() {
	const auto click = [this]() {
		while (started) {
			int type = clickTypeSelection->GetSelection(); //Return selection index
			int reps = clickReps->GetValue();
			int delay = clickInterval->GetValue();

			DWORD type_UP{};
			DWORD type_DOWN{};
			switch (type)
			{
			case 0: //Left click
				type_UP = MOUSEEVENTF_LEFTDOWN;
				type_DOWN = MOUSEEVENTF_LEFTUP;
				break;
			case 1: //Right click
				type_UP = MOUSEEVENTF_RIGHTDOWN;
				type_DOWN = MOUSEEVENTF_RIGHTUP;
				break;
			case 2: //Middle click
				type_UP = MOUSEEVENTF_MIDDLEDOWN;
				type_DOWN = MOUSEEVENTF_MIDDLEUP;
				break;
			}

			POINT cursorpos;
			GetCursorPos(&cursorpos);

			INPUT ip;
			ip.type = INPUT_MOUSE;
			ip.mi.dx = cursorpos.x;
			ip.mi.dy = cursorpos.y;
			ip.mi.mouseData = 0;
			ip.mi.time = 0;

			if (reps == 0) {
				while (started)
				{
					ip.mi.dwFlags = type_DOWN;
					SendInput(1, &ip, sizeof(INPUT));

					ip.mi.dwFlags = type_UP;
					SendInput(1, &ip, sizeof(INPUT));

					Sleep(delay);
				}
			} else {
				for (int rep = reps; rep > 0; rep--) {
					ip.mi.dwFlags = type_DOWN;
					SendInput(1, &ip, sizeof(INPUT));

					ip.mi.dwFlags = type_UP;
					SendInput(1, &ip, sizeof(INPUT));

					Sleep(delay);
					started = false;
					Iconize(false);
				}
			}
		}
		started = false; //Double started false and iconize false to make sure window pops up when ended normally or when force stopped
		Iconize(false);
		};
	std::thread bck{ click };
	bck.detach();
}

void MainFrame::StartStop(wxCommandEvent& evt) {
	if (!panel && spamText->GetValue().ToStdString().empty()) {
		wxMessageBox(wxT("Text to spam is empty!"), wxT("S5Ke"));
		return;
	}

	if (!started) {
		started = true;
		Iconize(true);
		if (!panel) {
			Sleep(3000);
			SpamText();
		}
		else {
			Click();
		}
	}
	else
	{
		started = false;
	}
}