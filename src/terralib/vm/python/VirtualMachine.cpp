/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

  This file is part of the TerraLib - a Framework for building GIS enabled applications.

  TerraLib is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License,
  or (at your option) any later version.

  TerraLib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with TerraLib. See COPYING. If not, write to
  TerraLib Team at <terralib-team@terralib.org>.
  */

/*!
  \file terralib/vm/python/VirtualMachine.cpp

  \brief Implementation of a virtual machine for the Python Programming Language.

  \author Frederico Augusto Bedê
  */

// TerraLib
#include "VirtualMachine.h"
#include "../../core/translator/Translator.h"
#include "../core/Exception.h"


struct te::vm::python::VirtualMachine::Impl
{
  Impl()
  {
  }

  ~Impl()
  {
  }
};

te::vm::python::VirtualMachine::VirtualMachine()
: pimpl_(nullptr)
{
  pimpl_ = new Impl;
}

te::vm::python::VirtualMachine::~VirtualMachine()
{
  delete pimpl_;
}

std::string
te::vm::python::VirtualMachine::getName() const
{
  return "Python";
}

std::string
te::vm::python::VirtualMachine::getTitle() const
{
  std::string msg(TE_TR("TerraLib Virtual Machine for the Python Programming Language"));

  return msg;
}

std::string
te::vm::python::VirtualMachine::getDescription() const
{
  std::string msg(TE_TR("This Virtual Machine allows applications to execute python scripts."
    "\nAll the TerraLib API will be available for TerraLib programmers "
    "in the Python environment"));

  return msg;
}

void
te::vm::python::VirtualMachine::build(const std::string& file)
{
}

void te::vm::python::VirtualMachine::execute()
{
}

void
te::vm::python::VirtualMachine::pause()
{
}

void
te::vm::python::VirtualMachine::stop()
{
}

void
te::vm::python::VirtualMachine::reset()
{
}
