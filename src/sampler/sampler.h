#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <string>

namespace FTL
{

void Open(std::string filename, int skip = 3);
void OpenDepth(std::string filename, int skip = 3, int position = 450);

}

#endif
