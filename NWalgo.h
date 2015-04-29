//
//  NWalgo.h
//  dnawx-mac
//
//  Created by Jerry Zhang on 3/21/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <fstream>

#include "FASTAfile.h"

class NWalgo
{
public:
    NWalgo(const std::string& ffile1, const std::string& ffile2);
    ~NWalgo();
    void align();
    void output(const std::string& ofile) const;
    
private:
    NWalgo(); // disable default constructor
    NWalgo(const NWalgo& rhs); // disable default copy constructor
    NWalgo& operator=(const NWalgo& rhs); // disable default assignment operator
    
    // files
    std::shared_ptr<FASTAfile> mFFileA;
    std::shared_ptr<FASTAfile> mFFileB;
    
    // data
    short _colLen;
    short _rowLen;
    short _longestLen; // longest of the two (between _colLen and _rowLen)
    
    short ** _scores; // grid of scores
    char ** _directions; // grid of directions
    /*
     'L' = left
     'T' = top
     'C' = corner (top left)
     ' ' = (0,0)
    */
    
    char * _hSeq; // A (horizontal)
    char * _vSeq; // B (vertical)
    
    short _finalscore;
    char * _newASeq;
    char * _newBSeq;
};