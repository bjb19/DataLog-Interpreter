#ifndef SCHEME_H
#define SCHEME_H

#include <vector>
#include <string>

struct Scheme : public std::vector<std::string> 
{
	Scheme() {}
	Scheme(std::vector<std::string> myVect) : vector(myVect) {}
};

#endif