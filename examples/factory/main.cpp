// TerraLib Examples
#include "Animals.h"
#include "AnimalsFactory.h"

// STL
#include <cstdlib>
#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
  /* first of all, let's initialize all factories */
  KillerWhaleFactory::initialize();
  PlatypusFactory::initialize();
  MyrmecophagidaeFactory::initialize();

  /* now we can use the factory support */
  Animal* a1 = AnimalFactory::make("Killer");
  Animal* a2 = AnimalFactory::make("Plat");
  Animal* a3 = AnimalFactory::make("Killer");
  Animal* a4 = AnimalFactory::make("Plat");
  Animal* a5 = AnimalFactory::make("Myr");

  a1->printName();
  a2->printName();
  a3->printName();
  a4->printName();
  a5->printName();

  delete a1;
  delete a2;
  delete a3;
  delete a4;
  delete a5;

  /* at the end of program execution, let's finalize all factories */
  KillerWhaleFactory::finalize();
  PlatypusFactory::finalize();
  MyrmecophagidaeFactory::finalize();

  return EXIT_SUCCESS;
}
