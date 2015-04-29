//
//  CodonTranslate.h
//  dnawx-mac
//
//  Created by Jerry Zhang on 3/15/15.
//  Copyright (c) 2015 Jerry Zhang. All rights reserved.
//

#pragma once

#include <string>
#include <unordered_map>

struct SMelement // state machine element
{
    int nextState;
    char codon2Produce; // empty if none
};

struct DNACodon
{
    double percentage;
    int count;
    DNACodon() : percentage(0), count(0) { };
};

class CodonTranslator
{
public:
    CodonTranslator(std::string& sequence);
    std::unordered_map<char, DNACodon*>& getCodons() { return _codons; }

    // DNA Codon's letters and names
    const static char _codonLetters[20];
    const static std::string _codonNames[20];
    
private:
    CodonTranslator(); // disable default constructor
    std::unordered_map<char, DNACodon*> _codons;
    int _total;
    
    const static SMelement _sm[24][4]; // state machine
};