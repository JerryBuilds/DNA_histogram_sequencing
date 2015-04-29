//
//  CodonTranslate.cpp
//  dnawx-mac
//
//  Created by Jerry Zhang on 3/15/15.
//  Copyright (c) 2015 Jerry Zhang. All rights reserved.
//

#include "CodonTranslator.h"

#include <iostream> // for debuggin purposes

CodonTranslator::CodonTranslator(std::string& sequence)
{
    // initialize each amino acid's count to 0
    for (const char& c : _codonLetters)
    {
        _codons[c] = new DNACodon();
    }
    
    int state = 0; // initial state is 0
    _total = 0;
    char aminoacid = ' '; // tracks the next amino acid
    
    // feed the sequence through the state machine
    for (char& c : sequence)
    {
        if (c == 'T')
        {
            aminoacid = _sm[state][0].codon2Produce;
            state = _sm[state][0].nextState;
        }
        else if (c == 'C')
        {
            aminoacid = _sm[state][1].codon2Produce;
            state = _sm[state][1].nextState;
        }
        else if (c == 'A')
        {
            aminoacid = _sm[state][2].codon2Produce;
            state = _sm[state][2].nextState;
        }
        else if (c == 'G')
        {
            aminoacid = _sm[state][3].codon2Produce;
            state = _sm[state][3].nextState;
        }
        
        if (aminoacid != ' ')
        {
            _codons[aminoacid]->count += 1;
            ++_total;
        }
    }
    
    for (const char& c : _codonLetters)
    {
        _codons[c]->percentage = 100 * _codons[c]->count / static_cast<double>(_total);
    }
}

const SMelement CodonTranslator::_sm[24][4] = {
    // STOP CODON denoted by "no character",
    // meaning must look for START CODON again
    
    //   T          C          A          G
    { { 0, ' '}, { 0, ' '}, { 1, ' '}, { 0, ' '} }, // state 0
    { { 2, ' '}, { 0, ' '}, { 1, ' '}, { 0, ' '} }, // state 1
    { { 0, ' '}, { 0, ' '}, { 1, ' '}, { 3, 'M'} }, // state 2
    { { 4, ' '}, { 9, ' '}, {14, ' '}, {19, ' '} }, // state 3
    { { 5, ' '}, { 6, ' '}, { 7, ' '}, { 8, ' '} }, // state 4
    { { 3, 'F'}, { 3, 'F'}, { 3, 'L'}, { 3, 'L'} }, // state 5
    { { 3, 'S'}, { 3, 'S'}, { 3, 'S'}, { 3, 'S'} }, // state 6
    { { 3, 'Y'}, { 3, 'Y'}, { 0, ' '}, { 0, ' '} }, // state 7
    { { 3, 'C'}, { 3, 'C'}, { 0, ' '}, { 3, 'W'} }, // state 8
    { {10, ' '}, {11, ' '}, {12, ' '}, {13, ' '} }, // state 9
    { { 3, 'L'}, { 3, 'L'}, { 3, 'L'}, { 3, 'L'} }, // state 10
    { { 3, 'P'}, { 3, 'P'}, { 3, 'P'}, { 3, 'P'} }, // state 11
    { { 3, 'H'}, { 3, 'H'}, { 3, 'Q'}, { 3, 'Q'} }, // state 12
    { { 3, 'R'}, { 3, 'R'}, { 3, 'R'}, { 3, 'R'} }, // state 13
    { {15, ' '}, {16, ' '}, {17, ' '}, {18, ' '} }, // state 14
    { { 3, 'I'}, { 3, 'I'}, { 3, 'I'}, { 3, 'M'} }, // state 15
    { { 3, 'T'}, { 3, 'T'}, { 3, 'T'}, { 3, 'T'} }, // state 16
    { { 3, 'N'}, { 3, 'N'}, { 3, 'K'}, { 3, 'K'} }, // state 17
    { { 3, 'S'}, { 3, 'S'}, { 3, 'R'}, { 3, 'R'} }, // state 18
    { {20, ' '}, {21, ' '}, {22, ' '}, {23, ' '} }, // state 19
    { { 3, 'V'}, { 3, 'V'}, { 3, 'V'}, { 3, 'V'} }, // state 20
    { { 3, 'A'}, { 3, 'A'}, { 3, 'A'}, { 3, 'A'} }, // state 21
    { { 3, 'D'}, { 3, 'D'}, { 3, 'E'}, { 3, 'E'} }, // state 22
    { { 3, 'G'}, { 3, 'G'}, { 3, 'G'}, { 3, 'G'} } // state 23
};

const char CodonTranslator::_codonLetters[20] = {'A', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y'};

const std::string CodonTranslator::_codonNames[20] =
    {"Alanine",
    "Cysteine",
    "Aspartic Acid",
    "Glutamic Acid",
    "Phenylalanine",
    "Glycine",
    "Histidine",
    "Isoleucine",
    "Lysine",
    "Leucine",
    "Methionine",
    "Asparagine",
    "Proline",
    "Glutamine",
    "Arginine",
    "Serine",
    "Threonine",
    "Valine",
    "Tryptophan",
    "Tyrosine"};
