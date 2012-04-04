#ifndef __ANIMALS_H
#define __ANIMALS_H

#include <string>

class Animal
{
  public:

    Animal() {}

    virtual ~Animal() {}

    virtual void printName() const = 0;
};

class KillerWhale : public Animal
{
  public:

    KillerWhale() {}

    ~KillerWhale() {}

    void printName() const;
};

class Platypus : public Animal
{
  public:

    Platypus() {}

    ~Platypus() {}

    void printName() const;
};

class Myrmecophagidae : public Animal
{
  public:

    Myrmecophagidae() {}

    ~Myrmecophagidae() {}

    void printName() const;
};

#endif  // __ANIMALS_H

