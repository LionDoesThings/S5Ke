//TO-DO LIST:
//1. CLICKER SET POSITION
//2. SENDINPUT() DIRECTLY TO ANOTHER APPLICATION
//3. 
//4. 
//5. MORE FUNCTIONS?
#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <windows.h>
#include <thread>
#include <simpleini.h>
#include <fstream>
#include <conio.h>

bool started{ false };
int panel{ 0 }; //0 for spam page, 1 for click page

std::string home = std::getenv("USERPROFILE"); //Windows
std::string docs = "\\Documents\\S5Ke_settings.ini";
std::string homeDir = (home + docs);

std::string spamTextINI{ "" };
int spamRepsINI{ 1 };
int spamDelayINI{ 70 };
int clickTypeINI{ 0 };
int clickRepsINI{ 0 };
int clickDelayINI{ 10 };
int hotkeyextendINI{ 0 };
int hotkeyINI{ 117 }; //117 is F6

int VKCodesExtended[] = { VK_LWIN , VK_RWIN , VK_LSHIFT , VK_RSHIFT , VK_LCONTROL , VK_RCONTROL , VK_LMENU , VK_RMENU };
int VKCodes[] = { VK_BACK, VK_TAB , VK_CLEAR, VK_RETURN , VK_PAUSE , VK_CAPITAL , VK_ESCAPE , VK_SPACE , VK_PRIOR , VK_NEXT , VK_END , VK_HOME ,VK_LEFT , VK_UP , VK_RIGHT , VK_DOWN ,
	VK_SELECT , VK_PRINT  , VK_EXECUTE , VK_SNAPSHOT , VK_INSERT , VK_DELETE , VK_HELP , 0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 , 0x38 , 0x39 , 0x41 , 0x42 , 0x43 ,
	0x44 , 0x45 , 0x46 , 0x47 , 0x48 , 0x49 , 0x4A , 0x4B , 0x4C , 0x4D , 0x4E , 0x4F , 0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 , 0x58 , 0x59 , 0x5A , VK_APPS ,
	VK_NUMPAD0 , VK_NUMPAD1 , VK_NUMPAD2 , VK_NUMPAD3 , VK_NUMPAD4 , VK_NUMPAD5 , VK_NUMPAD6 , VK_NUMPAD7 , VK_NUMPAD8 , VK_NUMPAD9 , VK_MULTIPLY , VK_ADD , VK_SEPARATOR ,
	VK_SUBTRACT , VK_DECIMAL , VK_DIVIDE , VK_F1 , VK_F2 , VK_F3 , VK_F4 , VK_F5 , VK_F6 , VK_F7 , VK_F8 , VK_F9 , VK_F10 , VK_F11 , VK_F12 , VK_F13 , VK_F14 , VK_F15 , VK_F16 ,
	VK_F17 , VK_F18 , VK_F19 , VK_F20 , VK_F21 , VK_F22 , VK_F23 , VK_F24 , VK_NUMLOCK , VK_SCROLL };

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	LoadHotKey();
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
	spamText= new wxTextCtrl(panelSpam, wxID_ANY, spamTextINI, wxPoint(20, 30), wxSize(240, 30));
	spamRepsLabel = new wxStaticText(panelSpam, wxID_ANY, "Repetitions", wxPoint(20, 80), wxSize(100, 30));
	spamReps = new wxSpinCtrl(panelSpam, wxID_ANY, "", wxPoint(22, 100), wxSize(100, 30), wxALIGN_LEFT, 1, 1000, spamRepsINI);
	spamDelayLabel = new wxStaticText(panelSpam, wxID_ANY, "Delay (ms)", wxPoint(160, 80), wxSize(100, 30));
	spamDelay = new wxSpinCtrl(panelSpam, wxID_ANY, "", wxPoint(162, 100), wxSize(100, 30), wxALIGN_LEFT, 1, 1000, spamDelayINI);
	spamStartStop = new wxButton(panelSpam, wxID_ANY, "Start/Stop", wxPoint(85, 140), wxSize(100, 30));
	spamHK = new wxButton(panelSpam, wxID_ANY, "HK", wxPoint(250, 150), wxSize(30,30));


	wxArrayString clickType;
	clickType.Add("Left");
	clickType.Add("Right");
	clickType.Add("Middle");
	clickTypeSelection = new wxRadioBox(panelClick, wxID_ANY, "Click Type", wxPoint(20, 10), wxSize(240, 50), clickType);
	clickTypeSelection->Select(clickTypeINI);
	clickRepsLabel = new wxStaticText(panelClick, wxID_ANY, "Repetitions (0 for inf)", wxPoint(20, 80), wxSize(120, 30));
	clickReps = new wxSpinCtrl(panelClick, wxID_ANY, "", wxPoint(22, 100), wxSize(100, 30), wxALIGN_LEFT, 0, 10000, clickRepsINI);
	clickDelayLabel = new wxStaticText(panelClick, wxID_ANY, "Delay (ms)", wxPoint(160, 80), wxSize(100, 30));
	clickDelay = new wxSpinCtrl(panelClick, wxID_ANY, "", wxPoint(162, 100), wxSize(100, 30), wxALIGN_LEFT, 0, 3600, clickDelayINI);
	clickStartStop = new wxButton(panelClick, wxID_ANY, "Start/Stop", wxPoint(85, 140), wxSize(100, 30));
	clickHK = new wxButton(panelClick, wxID_ANY, "HK", wxPoint(340, 210), wxSize(30, 30));
}

void MainFrame::BindEventHandlers() { //Button handler
	sideButtonSpam->Bind(wxEVT_BUTTON, &MainFrame::SwitchSpam, this);
	sideButtonClick->Bind(wxEVT_BUTTON, &MainFrame::SwitchClick, this);

	spamStartStop->Bind(wxEVT_BUTTON, &MainFrame::StartStop, this);
	clickStartStop->Bind(wxEVT_BUTTON, &MainFrame::StartStop, this);

	spamHK->Bind(wxEVT_BUTTON, &MainFrame::ChangeNewHotkey, this);
	clickHK->Bind(wxEVT_BUTTON, &MainFrame::ChangeNewHotkey, this);

	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClosed, this);
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
			while ((GetKeyState(hotkeyextendINI) & 0x8000) or hotkeyextendINI == 0) {
				if (GetKeyState(hotkeyINI) & 0x8000) {
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
		}
	};
	std::thread hkd{ hkdetect };
	hkd.detach();
}

void MainFrame::HotKeyPromptRestart() { //Jank code so hotkeydetection doesn't stop working for no reason
	wxMessageBox(wxT("Text to spam is empty!"), wxT("S5Ke"));
	HotKeyDetection();
}

void MainFrame::LoadHotKey() {
	CSimpleIniA ini; //https://github.com/brofield/simpleini#examples
	ini.SetUnicode();

	SI_Error filecheck = ini.LoadFile(homeDir.c_str()); //Check if file exist + Creation
	if (filecheck < 0) {
		std::ofstream file(homeDir);
		const char* ini_file_default =
			"[spam]\n"
			"text = \n"
			"reps = 1\n"
			"delay = 70\n"
			"\n"
			"[click]\n"
			"type = 0\n"
			"reps = 0\n"
			"delay = 10\n"
			"\n"
			"[hotkey]\n"
			"extend = 0\n"
			"key = 117";
		file << ini_file_default;

		file.close();
	}
	SI_Error fileload = ini.LoadFile(homeDir.c_str()); //If file creation failed
	if (fileload < 0) {
		return;
	}

	spamTextINI = ini.GetValue("spam", "text");
	spamRepsINI = std::stoi(ini.GetValue("spam", "reps"));
	spamDelayINI = std::stoi(ini.GetValue("spam", "delay"));
	clickTypeINI = std::stoi(ini.GetValue("click", "type"));
	clickRepsINI = std::stoi(ini.GetValue("click", "reps"));
	clickDelayINI = std::stoi(ini.GetValue("click", "delay"));
	hotkeyextendINI = std::stoi(ini.GetValue("hotkey", "extend"));
	hotkeyINI = std::stoi(ini.GetValue("hotkey", "key"));
}

void MainFrame::ChangeNewHotkey(wxCommandEvent& evt) {
	const auto changenewhk = [this]() {
		while (true) {
			for (int codeextend = (std::size(VKCodesExtended) - 1); codeextend > 0; codeextend--) {
				while (GetKeyState(VKCodesExtended[codeextend]) & 0x8000) {
					for (int code = (std::size(VKCodes) - 1); code > 0; code--) {
						if (GetKeyState(VKCodes[code]) & 0x8000) {
							hotkeyextendINI = VKCodesExtended[codeextend];
							hotkeyINI = VKCodes[code];
							wxMessageBox(wxT("Hotkey changed"), wxT("S5Ke"));
							break;
						}
					}
				}
			}

			for (int code = (std::size(VKCodes) - 1); code > 0; code--) {
				if (GetKeyState(VKCodes[code]) & 0x8000) {
					hotkeyextendINI = 0;
					hotkeyINI = VKCodes[code];
					wxMessageBox(wxT("Hotkey changed"), wxT("S5Ke"));
					break;
				}
			}
		}
	};
	std::thread chk{ changenewhk };
	chk.detach();
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
	std::thread st{ spamtext };
	st.detach();
}

void MainFrame::Click() {
	const auto click = [this]() {
		while (started) {
			int type = clickTypeSelection->GetSelection(); //Return selection index
			int reps = clickReps->GetValue();
			int delay = clickDelay->GetValue();

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
	std::thread cl{ click };
	cl.detach();
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

void MainFrame::OnWindowClosed(wxCloseEvent& evt) {
	CSimpleIniA ini; //https://github.com/brofield/simpleini#examples
	ini.SetUnicode();
	SI_Error filecheck = ini.LoadFile(homeDir.c_str());
	if (filecheck < 0) { evt.Skip(); }

	ini.SetValue("spam", "text", spamText->GetValue());
	ini.SetValue("spam", "reps", std::to_string(spamReps->GetValue()).c_str());
	ini.SetValue("spam", "delay", std::to_string(spamDelay->GetValue()).c_str());
	ini.SetValue("click", "type", std::to_string(clickTypeSelection->GetSelection()).c_str());
	ini.SetValue("click", "reps", std::to_string(clickReps->GetValue()).c_str());
	ini.SetValue("click", "delay", std::to_string(clickDelay->GetValue()).c_str());
	ini.SetValue("hotkey", "extend", std::to_string(hotkeyextendINI).c_str());
	ini.SetValue("hotkey", "key", std::to_string(hotkeyINI).c_str());

	ini.SaveFile(homeDir.c_str());

	evt.Skip();
}
