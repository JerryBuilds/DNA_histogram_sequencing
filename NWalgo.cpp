//
//  NWalgo.cpp
//  dnawx-mac
//
//  Created by Jerry Zhang on 3/21/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "NWalgo.h"


NWalgo::NWalgo(const std::string& ffile1, const std::string& ffile2)
{
    mFFileA = std::make_shared<FASTAfile>(ffile1);
    mFFileB = std::make_shared<FASTAfile>(ffile2);
}

NWalgo::~NWalgo()
{
    for (short i=0; i < _rowLen; ++i)
    {
        delete [] _scores[i];
        delete [] _directions[i];
    }
    delete [] _scores;
    delete [] _directions;
    delete [] _hSeq;
    delete [] _vSeq;
    delete [] _newASeq;
    delete [] _newBSeq;
}

void NWalgo::align()
{
    // A to column
    // B to row
    _colLen = mFFileA->getSequence().size(); // column length
    _rowLen = mFFileB->getSequence().size(); // row length]
    
    // dynamically allocate data structures
    _scores = new short*[_rowLen];
    _directions = new char*[_rowLen];
    for (short i=0; i < _rowLen; ++i)
    {
        _scores[i] = new short[_colLen];
        _directions[i] = new char[_colLen];
    }
    _hSeq = new char[_colLen];
    _vSeq = new char[_rowLen];
    if (_colLen > _rowLen)
    {
        _longestLen = _colLen;
    }
    else
    {
        _longestLen = _rowLen;
    }
    _newASeq = new char[_longestLen - 1];
    _newBSeq = new char[_longestLen - 1];
    
    // initialize data structures
    _scores[0][0] = 0;
    _directions[0][0] = ' ';
    for (short i=1; i < _colLen; ++i)
    {
        _scores[0][i] = -1*i;
        _directions[0][i] = 'L';
    }
    for (short i=1; i < _rowLen; ++i)
    {
        _scores[i][0] = -1*i;
        _directions[i][0] = 'T';
    }
    _hSeq[0] = ' ';
    for (short i=1; i < _colLen; ++i)
    {
        _hSeq[i] = mFFileA->getSequence().at(i-1);
    }
    _vSeq[0] = ' ';
    for (short i=1; i < _rowLen; ++i)
    {
        _vSeq[i] = mFFileB->getSequence().at(i-1);
    }
    
    // fill the grid
    short score1 = 0, score2 = 0, score3 = 0, match = 0, gap = -1;
    for (short i=1; i < _rowLen; ++i)
    {
        for (short j=1; j < _colLen; ++j)
        {
            // check whether A and B matches
            if (_vSeq[i] == _hSeq[j])
            {
                match = 1;
            }
            else
            {
                match = -1;
            }
            
            // calculate the scores
            score1 = _scores[i-1][j-1] + match; // top left
            score2 = _scores[i][j-1] + gap; // left
            score3 = _scores[i-1][j] + gap; // top
            
            // find highest score
            if (score1 > score2 && score1 > score3) // score1 is highest
            {
                _scores[i][j] = score1;
                _directions[i][j] = 'C';
            }
            else if (score2 > score1 && score2 > score3) // score2 is highest
            {
                _scores[i][j] = score2;
                _directions[i][j] = 'L';
            }
            else if (score3 > score1 && score3 > score2) // score3 is highest
            {
                _scores[i][j] = score3;
                _directions[i][j] = 'T';
            }
            else if (score1 == score2 && score2 == score3) // all tie--score1 has first priority
            {
                _scores[i][j] = score1;
                _directions[i][j] = 'C';
            }
            else if (score1 == score2 && score1 > score3)
            // score1 and score2 tied for highest--score1 has first priority
            {
                _scores[i][j] = score1;
                _directions[i][j] = 'C';
            }
            else if (score1 == score3 && score1 > score2)
            // score1 and score3 tied for highest--score1 has first priority
            {
                _scores[i][j] = score1;
                _directions[i][j] = 'C';
            }
            else if (score2 == score3 && score2 > score1)
            // score2 and score3 tied for highest--score2 has second priority
            {
                _scores[i][j] = score2;
                _directions[i][j] = 'L';
            }
        }
    }
    _finalscore = _scores[_rowLen-1][_colLen-1];
    
    // traceback
    short curRow = _rowLen - 1;
    short curCol = _colLen - 1;
    char curDir = 'C';
    short counter = _longestLen - 2;
    while (!(curRow == 0 && curCol == 0))
    {
        curDir = _directions[curRow][curCol];
        if (curDir == 'C')
        {
            _newASeq[counter] = _hSeq[curCol];
            _newBSeq[counter] = _vSeq[curRow];
            --curRow;
            --curCol;
        }
        else if (curDir == 'L')
        {
            _newASeq[counter] = _hSeq[curCol];
            _newBSeq[counter] = '_';
            --curCol;
        }
        else if (curDir == 'T')
        {
            _newASeq[counter] = '_';
            _newBSeq[counter] = _vSeq[curRow];
            --curRow;
        }
        --counter;
    }
}

void NWalgo::output(const std::string& ofile) const
{
    std::ofstream of(ofile);
    if (!of.fail())
    {
        of << "A: " << mFFileA->getHeader() << std::endl;
        of << "B: " << mFFileB->getHeader() << std::endl;
        of << "Score: " << _finalscore << std::endl;
        of << std::endl;
        
        char * matching = new char[71];
        matching[70] = '\0';
        
        for (short i=0; i <= (_longestLen - 1); ++i)
        {
            if (i == (_longestLen - 1))
            {
                matching[i % 70] = '\0';
                for (short j=70*(i/70); j < i; ++j)
                {
                    of << _newASeq[j];
                }
                of << std::endl;
                of << matching << std::endl;
                for (short j=70*(i/70); j < i; ++j)
                {
                    of << _newBSeq[j];
                }
                of << std::endl;
                of << std::endl;
                break;
            }
            if ((i % 70 == 0) && (i != 0))
            {
                for (short j=i-70; j < i; ++j)
                {
                    of << _newASeq[j];
                }
                of << std::endl;
                of << matching << std::endl;
                for (short j=i-70; j < i; ++j)
                {
                    of << _newBSeq[j];
                }
                of << std::endl;
                of << std::endl;
            }
            if (_newASeq[i] == _newBSeq[i])
            {
                matching[i % 70] = '|';
            }
            else
            {
                matching[i % 70] = ' ';
            }
        }
        
        of.close();
        
        delete [] matching;
    }
}