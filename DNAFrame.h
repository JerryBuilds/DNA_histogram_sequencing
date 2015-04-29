//
//  DNAFrame.h
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/frame.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include <memory>

#include <iostream>
#include "timer.h"
#include "FASTAfile.h"
#include "CodonTranslator.h"
#include "NWalgo.h"

class DNAFrame : public wxFrame
{
public:
	DNAFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	void onExit(wxCommandEvent& event);
	void onNew(wxCommandEvent& event);
	void onAminoHist(wxCommandEvent& event);
    void onPairwiseAlignment(wxCommandEvent& event);
	
	wxDECLARE_EVENT_TABLE();
private:
	// Panel for drawing
	class DNADrawPanel* mPanel;
	
	// Variables here
};
