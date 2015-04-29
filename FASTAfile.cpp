//
//  ParseFASTA.cpp
//  dnawx-mac
//
//  Created by Jerry Zhang on 3/7/15.
//  Copyright (c) 2015 Jerry Zhang. All rights reserved.
//

#include "FASTAfile.h"
#include "Exceptions.h"
#include <fstream>
#include <iostream> // debugging purpose

FASTAfile::FASTAfile(const std::string & path)
{
    std::ifstream::pos_type size; // prepare to get file's size
    
    std::ifstream file(path, std::ios::in|std::ios::ate); // load plain text with cursor At The End (ate)
    if (file.is_open()) // only performs operation if file exists
    {
        size = file.tellg(); // get file's size
        _sequence.reserve(size); // reserve enough space in string to save cost of continuously resizing
        file.seekg(0, std::ios::beg); // place fp back to start of file
        
        // reads in file
        // if file turns out to be invalid, throw FileLoadException
        
        // start with header
        // valid only if first character is '>'
        char temp = '\0';
        temp = file.get();
        if (!(temp == '>'))
        {
            throw FileLoadExcept();
        }
        std::getline(file, _header, '\n');
        
        // then proceed to read in sequence
        // valid only if they are 'T', 'C', 'A', or 'G'
        // can safely ignore '\n', '\r', and '\xff' (EOF)
        while (!file.eof())
        {
            temp = file.get();
            if (temp == '\n' || temp == '\r' || temp == '\xff')
            {
                // ignore
            }
            else if (temp == 'T' || temp == 'C' || temp == 'A' || temp == 'G')
            {
                _sequence += temp; // add to _sequence
            }
            else
            {
                throw FileLoadExcept();
            }
        }
        _sequence += '\0'; // terminate the string
        
        
        // debugging purpose:
        // print out string to check if it worked correctly
/*
        std::cout << "Header: " << std::endl << _header << std::endl;
        std::cout << "Sequence: " << std::endl << _sequence << std::endl;
*/
        // end debugging
        
    }
    
}