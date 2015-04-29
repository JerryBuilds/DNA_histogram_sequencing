//
//  DNADrawPanel.h
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <string>
#include <memory>

#include "FASTAfile.h"
#include "CodonTranslator.h"

class DNADrawPanel : public wxPanel
{
public:
	DNADrawPanel(wxFrame* parent);
	void paintNow();

protected:
	void paintEvent(wxPaintEvent & evt);
	void render(wxDC& dc);
 
	DECLARE_EVENT_TABLE()
	
public:
	// Variables here
    std::shared_ptr<FASTAfile> mFFile;
    std::shared_ptr<CodonTranslator> mTranslator;
    void drawHistogram(std::shared_ptr<FASTAfile> ffile, std::shared_ptr<CodonTranslator> translator);
};

