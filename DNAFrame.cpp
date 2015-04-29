//
//  DNAFrame.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNAFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/busyinfo.h>
#include "DNADrawPanel.h"
#include "Exceptions.h"
#include "DNAAlignDlg.h"

enum
{
	ID_AMINO_HIST=1000,
    ID_PAIRWISE_ALIGNMENT=2000
};

wxBEGIN_EVENT_TABLE(DNAFrame, wxFrame)
	EVT_MENU(wxID_EXIT, DNAFrame::onExit)
	EVT_MENU(wxID_NEW, DNAFrame::onNew)
	EVT_MENU(ID_AMINO_HIST, DNAFrame::onAminoHist)
    EVT_MENU(ID_PAIRWISE_ALIGNMENT, DNAFrame::onPairwiseAlignment)
wxEND_EVENT_TABLE()

DNAFrame::DNAFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(ID_AMINO_HIST, "Amino Acid Histogram...",
					 "Create a histogram from a FASTA file.");
    menuFile->Append(ID_PAIRWISE_ALIGNMENT, "DNA Pairwise Alignment...",
                     "Create a global sequence alignment of two FASTA files.");
	menuFile->Append(wxID_EXIT);
	
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new DNADrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}

void DNAFrame::onExit(wxCommandEvent& event)
{
	Close(true);
}

void DNAFrame::onNew(wxCommandEvent& event)
{
	// Doesn't do anything...
}

void DNAFrame::onAminoHist(wxCommandEvent& event)
{
	// TODO: Implement (File>Amino Acid Histogram...)
    wxFileDialog fileDlg(this, "Load a FASTA file...",
                 "./data", "", "Fasta Files|*.fasta",
                 wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (fileDlg.ShowModal() == wxID_OK)
    {
        std::string mInputFile;
        mInputFile = fileDlg.GetPath();
        try
        {
            std::shared_ptr<FASTAfile> mFFile = std::make_shared<FASTAfile>(mInputFile);
            std::shared_ptr<CodonTranslator> mTranslator = std::make_shared<CodonTranslator>(mFFile->getSequence());
            mPanel->drawHistogram(mFFile, mTranslator);
        }
        catch (std::exception& e)
        {
            wxMessageBox("FASTA file is invalid", "Error", wxOK | wxICON_ERROR);
        }
        
    }
}

void DNAFrame::onPairwiseAlignment(wxCommandEvent& event)
{
    DNAAlignDlg daDlg;
    
    if (daDlg.ShowModal() == wxID_OK)
    {
        std::string mInputFile1 = daDlg.GetInputAPath();
        std::string mInputFile2 = daDlg.GetInputBPath();
        std::string mOutputFile = daDlg.GetOutputPath();
        try
        {
            // start timer to test efficiency
            Timer timer;
            timer.start();
            std::cout << "Aligning..." << std::endl;
            
            // perform the alignment
            NWalgo nwa(mInputFile1, mInputFile2);
            nwa.align();
            nwa.output(mOutputFile);
            
            double elapsed = timer.getElapsed();
            std::cout << "Done. Time elapsed: " << elapsed << " seconds." << std::endl;
        }
        catch (std::exception& e)
        {
            wxMessageBox("FASTA file is invalid", "Error", wxOK | wxICON_ERROR);
        }
    }
}