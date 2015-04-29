//
//  ParseFASTA.h
//  dnawx-mac
//
//  Created by Jerry Zhang on 3/7/15.
//  Copyright (c) 2015 Jerry Zhang. All rights reserved.
//

#pragma once

#include <string>

class FASTAfile
{
public:
    FASTAfile(const std::string & path);
    std::string& getHeader() { return _header; }
    std::string& getSequence() { return _sequence; }
private:
    FASTAfile(); // disable default constructor
    std::string _header;
    std::string _sequence;
};