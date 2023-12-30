#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("S5Ke");
	mainFrame->SetClientSize(310, 180);
	mainFrame->Center();
	mainFrame->Show();
	mainFrame->SetIcon(wxIcon(wxT("S5Ke.ico"), wxBITMAP_TYPE_ICO, 16, 16));
	return true;
}