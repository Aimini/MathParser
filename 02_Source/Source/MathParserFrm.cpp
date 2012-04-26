///-----------------------------------------------------------------
///
/// @file      MathParserFrm.cpp
/// @author    ruett1
/// Created:   24.04.2012 12:17:13
/// @section   DESCRIPTION
///            MathParserFrm class implementation
///
///------------------------------------------------------------------

#include <iostream>
#include <string>
#include "MathParserFrm.h"
#include "MathParser.h"
#include "array.h"
#include "xy_T.h"
using namespace std;

#define GRAPH_X         8
#define GRAPH_Y         120
#define GRAPH_WIDTH     324
#define GRAPH_HEIGHT    168

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// MathParserFrm
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(MathParserFrm,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(MathParserFrm::OnClose)
	EVT_BUTTON(ID_WXBUTTON1,MathParserFrm::WxButton1Click)
END_EVENT_TABLE()
////Event Table End

MathParserFrm::MathParserFrm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

MathParserFrm::~MathParserFrm()
{
}

void MathParserFrm::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, wxT("Formel"), wxPoint(8, 8), wxDefaultSize, 0, wxT("WxStaticText4"));

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, wxT("Value Step"), wxPoint(8, 88), wxDefaultSize, 0, wxT("WxStaticText3"));

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("Maximum Value"), wxPoint(8, 64), wxDefaultSize, 0, wxT("WxStaticText2"));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("Minimum Value"), wxPoint(8, 40), wxDefaultSize, 0, wxT("WxStaticText1"));

	wxValueStep = new wxSpinCtrl(this, ID_WXVALUESTEP, wxT("10"), wxPoint(104, 88), wxSize(120, 24), wxSP_ARROW_KEYS, 0, 100, 10);

	wxMaxValue = new wxSpinCtrl(this, ID_WXMAXVALUE, wxT("100"), wxPoint(104, 64), wxSize(120, 24), wxSP_ARROW_KEYS, 0, 100, 100);

	wxMinValue = new wxSpinCtrl(this, ID_WXMINVALUE, wxT("0"), wxPoint(104, 40), wxSize(120, 24), wxSP_ARROW_KEYS, 0, 100, 0);

	WxMemo1 = new wxTextCtrl(this, ID_WXMEMO1, wxEmptyString, wxPoint(344, 40), wxSize(150, 248), wxTE_MULTILINE, wxDefaultValidator, wxT("WxMemo1"));
	WxMemo1->SetMaxLength(0);
	WxMemo1->SetFocus();
	WxMemo1->SetInsertionPointEnd();

	WxEdit1 = new wxTextCtrl(this, ID_WXEDIT1, wxT("1+log(cos(x),7)*2"), wxPoint(68, 8), wxSize(340, 24), 0, wxDefaultValidator, wxT("WxEdit1"));

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("Berechnen"), wxPoint(416, 8), wxSize(80, 24), 0, wxDefaultValidator, wxT("WxButton1"));

	SetTitle(wxT("MathParser"));
	SetIcon(wxNullIcon);
	SetSize(8,8,514,336);
	Center();
	
	////GUI Items Creation End
	
	WxMemo1->SetEditable(false);
    dc = new wxClientDC(this);
}

void MathParserFrm::OnClose(wxCloseEvent& event)
{
	Destroy();
}

/*
 * WxButton1Click
 */
void MathParserFrm::WxButton1Click(wxCommandEvent& event)
{
	char * formelPtr;
	
	WxMemo1->SetEditable(false);
	WxMemo1->SetValue("");
	dc->Clear();
	
	// read the WxEdit1 field
	wxString wstr = WxEdit1->GetValue();
	formelPtr = new char[wstr.Length()+1];
	strcpy(formelPtr, wstr.c_str());
	std::string formelStr(formelPtr);
	
	// get min, max and step values
	int minValue = wxMinValue->GetValue();
	int maxValue = wxMaxValue->GetValue();
	int ValueStep = wxValueStep->GetValue();
	
	// calc
	MathParser *parser = new MathParser();
	parser->parse(formelStr);
	
	// draw the rectangle around the graph panel
    dc->SetPen(wxPen(wxColor(0,0,0), 1)); // black line, 1 pixels thick
    dc->DrawRectangle(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT);
	
	// evaluate points
	int StepNbrs = (maxValue-minValue)/ValueStep + 1;
    PointArray points(StepNbrs);
	for(int i = 0; i < StepNbrs; i = i++) {
        points[i].x = minValue + i*ValueStep;
        points[i].y = parser->evaluate(points[i].x);
        (*WxMemo1) << "f(" << points[i].x << ")=" << points[i].y << "\n";
    }
    
    // draw graph
	double StepWidth = (double)GRAPH_WIDTH / (double)(StepNbrs-1);
	
	double min = points.min_y();
	double max = points.max_y();
	double StepHeight = GRAPH_HEIGHT / (max-min);
	
	(*WxMemo1) << "Step Nbrs: " << StepNbrs << "\n";
    (*WxMemo1) << "Graph width: " << GRAPH_WIDTH << "\n";
    (*WxMemo1) << "Graph height: " << GRAPH_HEIGHT << "\n";
    (*WxMemo1) << "Step width: " << StepWidth << "\n";
    (*WxMemo1) << "Step height: " << StepHeight << "\n";
    (*WxMemo1) << "x min: " << GRAPH_X << "\n";
    (*WxMemo1) << "x max: " << GRAPH_X+StepNbrs*StepWidth << "\n";
    (*WxMemo1) << "y min: " << min << "\n";
    (*WxMemo1) << "y max: " << max << "\n";
	
	for(int i = 1; i < StepNbrs; i = i++) {
        dc->DrawLine(GRAPH_X+(i-1)*StepWidth, GRAPH_Y+GRAPH_HEIGHT-points[i-1].y*StepHeight,
                GRAPH_X+i*StepWidth, GRAPH_Y+GRAPH_HEIGHT-points[i].y*StepHeight);
        (*WxMemo1) << i << " ";
    }
}
