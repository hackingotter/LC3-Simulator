#include "Saver.h"
#include "computer.h"
#include <iostream>
#include <fstream>
Saver::Saver()
{

}

void Saver::vanguard()
{
    std::ofstream myfile;
      myfile.open ("example.txt");

      myfile << "Writing this to a file.\n";
      Computer::getDefault()->saveWorkSpace(&myfile);
      myfile.close();
}

