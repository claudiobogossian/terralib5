#ifndef __ANIMALSFACTORY_H
#define __ANIMALSFACTORY_H

#include <terralib/common/AbstractFactory.h>

#include <string>

class Animal;

class AnimalFactory : public te::common::AbstractFactory<Animal, std::string>
{
  public:   

    /* Note: destructor must be public! */
    virtual ~AnimalFactory() {}

  protected:

    /* Note: constructor must be protected! */
    AnimalFactory(const std::string& factoryKey);
};

class KillerWhaleFactory : public AnimalFactory
{
  public:

    /* Note: destructor must be public! */
    ~KillerWhaleFactory() {}

    /* Note: create a static member to initialize the factory in some place of your application! */
    static void initialize();

    /* Note: create a static member to finalize the factory in some place of your application! */
    static void finalize();

  protected:

    /* Note: constructor must be protected! */
    KillerWhaleFactory();

    /* Note: This is the mandatory method for all subclasses of AnimalFactory! */
    Animal* build();

  private:

    /* try to keep a singleton concrete factory for KillerWhale object */
    static KillerWhaleFactory* sm_factory;
};

class PlatypusFactory : public AnimalFactory
{
  public:    

    /* Note: destructor must be public! */
    ~PlatypusFactory() {}

    /* Note: create a static member to initialize the factory in some place of your application! */
    static void initialize();

    /* Note: create a static member to finalize the factory in some place of your application! */
    static void finalize();

  protected:

    /* Note: constructor must be protected! */
    PlatypusFactory();

    /* Note: This is the mandatory method for all subclasses of AnimalFactory! */
    Animal* build();

  private:

    /* try to keep a singleton concrete factory for Platypus object */
    static PlatypusFactory* sm_factory;
};

class MyrmecophagidaeFactory : public AnimalFactory
{
  public:

    /* Note: destructor must be public! */
    ~MyrmecophagidaeFactory() {}

    /* Note: create a static member to initialize the factory in some place of your application! */
    static void initialize();

    /* Note: create a static member to finalize the factory in some place of your application! */
    static void finalize();

  protected:

    /* Note: constructor must be protected! */
    MyrmecophagidaeFactory();

    /* Note: This is the mandatory method for all subclasses of AnimalFactory! */
    Animal* build();

  private:

    /* try to keep a singleton concrete factory for Myrmecophagidae object */
    static MyrmecophagidaeFactory* sm_factory;
};

#endif  // __ANIMALSFACTORY_H

