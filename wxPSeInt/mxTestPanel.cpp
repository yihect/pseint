#include <wx/button.h>
#include <wx/sizer.h>
#include "mxTestPanel.h"
#include <wx/stattext.h>
#include "string_conversions.h"
#include "ids.h"
#include "mxSource.h"
#include "ConfigManager.h"
#include "mxProcess.h"
#include "Logger.h"

BEGIN_EVENT_TABLE(mxTestPanel,wxPanel)
	EVT_BUTTON(mxID_TESTPACK_RUN,mxTestPanel::OnRun)
END_EVENT_TABLE()

mxTestPanel::mxTestPanel(wxWindow *parent) : wxPanel(parent,wxID_ANY) {
	sizer = new wxBoxSizer(wxHORIZONTAL);
	eval_button = new wxButton(this,mxID_TESTPACK_RUN,_Z("Evaluar..."));
	label = new wxStaticText(this,wxID_ANY,_Z("Cargando ejercicio..."),wxDefaultPosition,wxDefaultSize);
	sizer->Add(eval_button,wxSizerFlags().Border(wxALL,5));
	sizer->Add(label,wxSizerFlags().Center().Centre().Border(wxALL,5));
	SetSizerAndFit(sizer);
}

bool mxTestPanel::Load (const wxString & path, const wxString &key, mxSource *src) {
	this->path=path; this->key=key; this->src=src;
	if (!pack.Load(path)) return false;
	label->SetLabel(_Z(" <- click aqu� para evaluar su respuesta"));
	sizer->Layout();
	src->SetText(pack.GetBaseSrc());
	return true;
}

void mxTestPanel::OnRun (wxCommandEvent & event) {
	src->SaveTemp();
	wxString cmd = config->pseval_command +" \""+path+"\" "+key+" "+ config->pseint_command+mxProcess::GetProfileArgs() + " \""+src->GetTempFilenamePSC()+"\"";
	_LOG("mxTestPanel::OnRun");
	_LOG("    "<<cmd);
	wxExecute(cmd,wxEXEC_ASYNC);
}
