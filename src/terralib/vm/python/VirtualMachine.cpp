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
#include "../../core/lib/Library.h"
#include "../../core/translator/Translator.h"
#include "../../core/utils/Platform.h"
#include "../core/Exception.h"

// Python
#include <node.h>
#include <functional>

#include <iostream>

void PythonPrinting(std::string inputStr){
  std::string stdOutErr = 
    "import sys\n\
class CatchOut:\n\
  def __init__(self):\n\
    self.value = ''\n\
  def write(self, txt):\n\
    self.value += txt\n\
catchOut = CatchOut()\n\
sys.stdout = catchOut\n\
sys.stderr = catchOut\n\
"; //this is python code to redirect stdouts/stderr

  PyObject *pModule = PyImport_AddModule("__main__"); //create main module
  PyRun_SimpleString(stdOutErr.c_str()); //invoke code to redirect

  PyRun_SimpleString(inputStr.c_str());
  PyObject *catcher = PyObject_GetAttrString(pModule, "catchOut");

  PyObject *output = PyObject_GetAttrString(catcher, "value");
  std::cout <<PyString_AsString(output);
}

std::string GetBindingModuleName()
{
  std::string mName = "_terralib_mod_binding_python";

  return "_" + mName;
}


te::vm::python::VirtualMachine::VirtualMachine()
{
  const char *scriptDirectoryName = te::core::FindInTerraLibPath("bin/terraview.exe").c_str();

  Py_Initialize();

  PyObject *sysPath = PySys_GetObject("path");

  PyObject *path = PyString_FromString(scriptDirectoryName);

  int result = PyList_Insert(sysPath, 0, path);

  // Import TL5 binding on interpreter
  m_pyModule = PyImport_ImportModule("_terralib_mod_binding_python");
}

te::vm::python::VirtualMachine::~VirtualMachine()
{
  Py_Finalize();
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
  // aqui é necessário usar Py_file_input ou Py_eval_input
  // https://docs.python.org/2/c-api/veryhigh.html#c.Py_eval_input
  PythonPrinting("");
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
