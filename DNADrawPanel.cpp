//
//  DNADrawPanel.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNADrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <array>
#include <sstream>
#include <iomanip>

BEGIN_EVENT_TABLE(DNADrawPanel, wxPanel)
	EVT_PAINT(DNADrawPanel::paintEvent)
END_EVENT_TABLE()

DNADrawPanel::DNADrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void DNADrawPanel::paintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	render(dc);
}

void DNADrawPanel::paintNow()
{
	wxClientDC dc(this);
	render(dc);
}

void DNADrawPanel::render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// TODO: Draw histogram, if one has been generated
    if (mTranslator.use_count() > 0)
    {
        // write header
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawText(mFFile->getHeader(), 50, 25);
        
        // prepare
        std::stringstream ss;
        std::unordered_map<char, DNACodon*> allCodons = mTranslator->getCodons();
        std::string cperc, cname, ccount, ctext; // c prefix stands for codon
        int largestCount = 0, graphLength = 0;
        
        // find codon with largest count
        // use this as a reference for all other codon counts
        for (int i=0; i < 20; ++i)
        {
            DNACodon * codon = allCodons[CodonTranslator::_codonLetters[i]];
            if (codon->count > largestCount)
            {
                largestCount = codon->count;
            }
        }
        
        // loop through each codon
        for (int i=0; i < 20; ++i)
        {
            DNACodon * codon = allCodons[CodonTranslator::_codonLetters[i]];
            
            // figure out which color
            if (i % 3 == 0) // if divisible by 3, use red
            {
                dc.SetTextForeground(*wxRED);
                dc.SetBrush(*wxRED_BRUSH);
            }
            else if (i % 3 == 1) // if remainder 1, use green
            {
                dc.SetTextForeground(*wxGREEN);
                dc.SetBrush(*wxGREEN_BRUSH);
            }
            else // if remainder 2, use blue
            {
                dc.SetTextForeground(*wxBLUE);
                dc.SetBrush(*wxBLUE_BRUSH);
            }
            
            // use sstream and iomanip to format double output to 2 decimals
            cperc = "";
            ss << std::setprecision(2) << codon->percentage;
            ss >> cperc;
            ss.str(std::string()); // clear stringstream (for next loop)
            ss.clear();
            
            // grab all other portions of codon name's output
            cname = CodonTranslator::_codonNames[i];
            ccount = std::to_string(codon->count);
            
            // construct final output string
            ctext = cname + ": " + cperc + "% (" + ccount + ")";
            
            // display names of codons
            dc.DrawText(ctext, 50, 90+i*30);
            
            // calculate adjusted length of each bar
            graphLength = static_cast<int>(ceil(750*static_cast<double>(codon->count)/static_cast<double>(largestCount)));
            
            // draw the graph portion
            dc.DrawRectangle(250, 80+i*30, graphLength, 30);
        }
        
    }
}

void DNADrawPanel::drawHistogram(std::shared_ptr<FASTAfile> ffile, std::shared_ptr<CodonTranslator> translator)
{
    mFFile = ffile;
    mTranslator = translator;
    paintNow();
}