#include "Animals.h"
#include "AnimalsFactory.h"

KillerWhaleFactory* KillerWhaleFactory::sm_factory(0);
PlatypusFactory* PlatypusFactory::sm_factory(0);
MyrmecophagidaeFactory* MyrmecophagidaeFactory::sm_factory(0);

static const std::string killerWhaleFactoryKey("Killer");
static const std::string PlatypusFactoryKey("Plat");
static const std::string MyrmecophagidaeFactoryKey("Myr");

AnimalFactory::AnimalFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<Animal, std::string>(factoryKey)
{
}

void KillerWhaleFactory::initialize()
{
  sm_factory = new KillerWhaleFactory;
}
void KillerWhaleFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

KillerWhaleFactory::KillerWhaleFactory()
  : AnimalFactory(killerWhaleFactoryKey)
{
}

Animal* KillerWhaleFactory::build()
{
  return new KillerWhale;
}

void PlatypusFactory::initialize()
{
  sm_factory = new PlatypusFactory;
}
void PlatypusFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

PlatypusFactory::PlatypusFactory()
  : AnimalFactory(PlatypusFactoryKey)
{
}

Animal* PlatypusFactory::build()
{
  return new Platypus;
}

void MyrmecophagidaeFactory::initialize()
{
  sm_factory = new MyrmecophagidaeFactory;
}
void MyrmecophagidaeFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

MyrmecophagidaeFactory::MyrmecophagidaeFactory()
  : AnimalFactory(MyrmecophagidaeFactoryKey)
{
}

Animal* MyrmecophagidaeFactory::build()
{
  return new  Myrmecophagidae;
}
