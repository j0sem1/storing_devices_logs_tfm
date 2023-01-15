#include <iostream>
#include <regex>

int main ()
{
  std::cmatch m;          // default constructor

  std::regex_match ( "subject", m, std::regex("sub(.*)") );

  std::cmatch mcopy (m);  // copy constructor

  for (unsigned i=0; i<mcopy.size(); ++i)
    std::cout << "match " << i+1 << ": " << mcopy[i] << std::endl;

  return 0;
}