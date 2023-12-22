#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("S5Ke");
	mainFrame->SetClientSize(400, 240);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}