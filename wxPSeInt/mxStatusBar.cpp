#include <wx/dcclient.h>
#include "mxStatusBar.h"
#include <wx/textfile.h>
#include <wx/event.h>
#include "mxMainWindow.h"
#include <wx/utils.h>
#include <wx/settings.h>
#include "ConfigManager.h"
#include "string_conversions.h"

BEGIN_EVENT_TABLE(mxStatusBar,wxPanel)
	EVT_PAINT(mxStatusBar::OnPaint)
	EVT_LEFT_DOWN(mxStatusBar::OnClick)
END_EVENT_TABLE();
	
struct st_aux {
	wxString text;
	wxColour col;
	st_aux(){}
	st_aux(const wxColour &c, const wxString &t):text(t),col(c){}
};

static st_aux texts[STATUS_COUNT];
mxStatusBar *status_bar = NULL;
	
mxStatusBar::mxStatusBar(wxWindow *parent):wxPanel(parent,wxID_ANY,wxDefaultPosition,wxDefaultSize) {
	wxColour negro(0,0,0),rojo(128,0,0),verde(0,75,0),azul(0,0,128);
	font = wxFont(11,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL);
//	if (config->big_icons) font.SetPointSize(font.GetPointSize()*1.4);
	bg_color=wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
	wxTextFile fil("version"); 
	if (fil.Exists()) {
		fil.Open();
		texts[STATUS_WELCOME]=st_aux(verde,
			wxString(_Z("Welcome to PSeInt v"))<<fil.GetFirstLine()<<
			wxString(_Z(" (your current profile is: "))<< _S2W(cfg_lang.name) <<_Z(")"));
		fil.Close();
	} else {
		texts[STATUS_WELCOME]=st_aux(rojo,_Z("An error occurred when starting the editor."));
	}
	texts[STATUS_PROFILE]             = st_aux( verde ,_Z("Your current profile is: "));
	texts[STATUS_SYNTAX_OK]           = st_aux( verde ,_Z("The pseudocode is correct. Press F9 to run it."));
	texts[STATUS_SYNTAX_ERROR]        = st_aux( rojo  ,_Z("The pseudocode contains errors. Press F9 for a better description."));
	texts[STATUS_SYNTAX_ERROR_DETAIL] = st_aux( rojo  ,_Z("The pseudocode contains errors. Select a bug to see its description."));
	texts[STATUS_SYNTAX_CHECK_OK]     = st_aux( verde ,_Z("The syntax is correct."));
	texts[STATUS_SYNTAX_CHECK_ERROR]  = st_aux( azul  ,_Z("Select a bug to see its description."));
	texts[STATUS_FLOW]                = st_aux( azul  ,_Z("This pseudocode is being edited as flowchart."));
	texts[STATUS_FLOW_CHANGED]        = st_aux( azul  ,_Z("The flowchart has been modified. Click here to update the pseudocode."));
	texts[STATUS_RUNNING]             = st_aux( azul  ,_Z("The pseudocode is being executed."));
	texts[STATUS_RUNNING_CHANGED]     = st_aux( azul  ,_Z("The pseudocode has changed. Press F9 to see the changes in the execution."));
	texts[STATUS_RUNNING_UPDATED]     = st_aux( azul  ,_Z("The run has been updated to reflect the changes."));
	texts[STATUS_RUNNED_OK]           = st_aux( azul  ,_Z("The execution has finished without errors."));
	texts[STATUS_RUNNED_INT]          = st_aux( rojo  ,_Z("Execution has been interrupted."));
	texts[STATUS_EXAMPLE]             = st_aux( azul  ,_Z("This is an example, you cannot modify it."));
	texts[STATUS_DEBUG_RUNNING]       = st_aux( azul  ,_Z("The pseudocode is being executed step by step."));
	texts[STATUS_DEBUG_PAUSED]        = st_aux( azul  ,_Z("Stepping has been paused."));
	texts[STATUS_DEBUG_STOPPED]       = st_aux( azul  ,_Z("Stepping has been stopped."));
	texts[STATUS_DEBUG_ENDED]         = st_aux( azul  ,_Z("The step-by-step execution is finished."));
	texts[STATUS_UPDATE_CHECKING]     = st_aux( azul  ,_Z("checking for updates..."));
	texts[STATUS_UPDATE_ERROR]        = st_aux( rojo  ,_Z("Failed to connect to the server to check for updates."));
	texts[STATUS_UPDATE_NONEWS]       = st_aux( azul  ,_Z("No updates available."));
	texts[STATUS_UPDATE_FOUND]        = st_aux( azul  ,_Z("There is a new version to download!"));
	texts[STATUS_NEW_SOURCE]          = st_aux( azul  ,_Z("You can use the commands and structures panel to add instructions."));
	texts[STATUS_COMMAND]             = st_aux( azul  ,_Z("You must complete the fields marked with rectangles."));
	status = STATUS_WELCOME;
}

void mxStatusBar::OnPaint (wxPaintEvent & event) {
	wxPaintDC dc(this);
	PrepareDC(dc);
	dc.SetBackground(bg_color);
	dc.SetTextForeground(texts[status].col);
	dc.Clear();
	dc.SetFont(font);
//	wxString text;
	dc.DrawText(texts[status].text,5,3);
}

void mxStatusBar::OnClick (wxMouseEvent & event) {
	if (status==STATUS_SYNTAX_OK||status==STATUS_SYNTAX_ERROR) {
		wxCommandEvent evt;
		main_window->RunCurrent(false);
	} else if (status==STATUS_UPDATE_FOUND) {
		wxLaunchDefaultBrowser("http://pseint.sourceforge.net?page=descargas.php");
	} else if (status==STATUS_DEBUG_RUNNING||status==STATUS_DEBUG_PAUSED||status==STATUS_DEBUG_ENDED||status==STATUS_DEBUG_STOPPED) {
		main_window->ShowDebugPanel(true);
	} else if (status==STATUS_NEW_SOURCE) {
		main_window->ShowCommandsPanel(true);
	} else if (status==STATUS_RUNNING_CHANGED) {
		if (main_window->GetCurrentSource()) main_window->GetCurrentSource()->UpdateRunningTerminal();
	} else if (status==STATUS_FLOW_CHANGED) {
		if (main_window->GetCurrentSource()) main_window->GetCurrentSource()->UpdateFromFlow();
	}
}

void mxStatusBar::SetStatus (int what) {
//	if ((what==STATUS_NEW_SOURCE || what==STATUS_SYNTAX_OK) && (status<STATUS_NEW_SOURCE)) return;
//	else 
		if (what==STATUS_SYNTAX_ERROR && main_window->QuickHelp().IsVisible()) what=STATUS_SYNTAX_ERROR_DETAIL;
	else if (what==STATUS_PROFILE) {
		texts[STATUS_PROFILE].text =
			texts[STATUS_PROFILE].text.BeforeFirst(':') + _Z(": ") + _S2W(cfg_lang.name);
	}
	status=what;
	Refresh();
}

