/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/TerraLib.h

  \brief An utility class to control the startup and cleanup of the TerraLib Platform and its resources.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_TERRALIB_H
#define __TERRALIB_COMMON_INTERNAL_TERRALIB_H

// TerraLib
#include "Config.h"
#include "Singleton.h"

// STL
#include <string>
#include <vector>

/*!
  \class TerraLib

  \brief An utility class to control the startup and cleanup of the TerraLib Platform and its resources.

  This singleton will control the initialization and the cleanup of TerraLib
  modules and their resources. If you are creating a C++ application you may need to
  make an explicit initialization of the TerraLib Platform as showned in the code below:

  \code
  #include <terralib/common/TerraLib.h>

  ...

  int main(char** argv, int argc)
  {
    ... // other initializations goes here!

    TerraLib::getInstance().initialize();

    ... // now you can start using TerraLib!

    TerraLib::getInstance().finalize(); // call this when you have finished using TerraLib

    return EXIT_SUCCESS;
  }
  \endcode

  Notice the calling to the finalize function at the end of the example program,
  it is necessary in order to close/release any pending resource initialized by the modules.

  \ingroup common
*/
class TECOMMONEXPORT TerraLib : public te::common::Singleton<TerraLib>
{
  friend class te::common::Singleton<TerraLib>;

  public:

    /*!
      \typedef void (*StartupFptr)(void);
          
      \brief This is the type for call back functions that makes the startup of a module.
     */
    typedef void (*StartupFptr)(void);

    /*!
      \typedef void (*CleanupFptr)(void);
          
      \brief This is the type for call back functions that makes the cleanup of a module.
     */
    typedef void (*CleanupFptr)(void);

    /*!
      \struct Module
      
      \brief This internal structure is used to model the basic information about a TerraLib module.

      \sa TerraLib
    */
    struct Module
    {
      std::string m_name;         //!< The module name: an internal value used to identify the module in the system. Must be a unique value.
      StartupFptr m_startFptr;    //!< The module initialization routine.
      CleanupFptr m_cleanupFptr;  //!< The module finalization routine.
    };

    /*!
      \brief It initializes the TerraLib Platform.
           
      This method will perform the startup of all registered modules.
      If there isn't modules to be initialized this method has no effect.

      \exception Exception It may throws an exception.

      \note The registered module startup functions will be invoked in a direct order of registering (the first module to be registered will be the first to be initialized).

      \warning Not thread safe!

      \warning If this method throws an exception we recommend you to
               quit the program, don't try to resume it because you can have intermittent errors!
    */
    void initialize();

    /*!
      \brief It initializes the i-th registered module in the TerraLib Platform.
           
      \exception Exception It may throws an exception.

      \note If there is a registered module startup function it will be invoked.

      \warning Not thread safe!

      \warning Don't call this method in your code, it is used internally by TerraLib.
    */
    void initialize(const std::size_t i);

    /*!
      \brief It initializes the registered module in the TerraLib Platform.
           
      \exception Exception It may throws an exception.

      \note If there is a registered module startup function it will be invoked.

      \warning Not thread safe!

      \warning Don't call this method in your code, it is used internally by TerraLib.
    */
    void initialize(const std::string& moduleName);

    /*!
      \brief It finalizes the TerraLib Platform.

      \exception Exception It may throws an exception.

      \note The registered module shutdown functions will be invoked in an opposite order of registering (the last module to be registered will be the first module to be finalized).

      \warning Not thread safe!

      \warning If this method throws an exception we recommend you to
               quit the program, don't try to resume it because you can have intermittent errors!
    */
    void finalize();

    /*!
      \brief It finalizes the i-th registered module in the TerraLib Platform.
           
      \exception Exception It may throws an exception.

      \note If there is a registered module cleaunp function it will be invoked.

      \warning Not thread safe!

      \warning Don't call this method in your code, it is used internally by TerraLib.
    */
    void finalize(const std::size_t i);

    /*!
      \brief It finalizes the registered module in the TerraLib Platform.
           
      \exception Exception It may throws an exception.

      \note If there is a registered module cleaunp function it will be invoked.

      \warning Not thread safe!

      \warning Don't call this method in your code, it is used internally by TerraLib.
    */
    void finalize(const std::string& moduleName);

    /*!
      \brief It registers the module in the TerraLib Platform.
      
      Each registered module will be initialized when TerraLib Platform is explicitly started
      and then finalized when the application finalizes.

      \param m A TerraLib module.
    */
    void add(const Module& m);

    /*!
      \brief It removes the module entry identified by the given name.

      \param moduleName The TerraLib module to be removed from the platform control.
    */
    void remove(const std::string& moduleName);

    /*!
      \brief It removes the i-th module entry.

      \param i The i-th TerraLib module to be removed from the platform control.
    */
    void remove(const std::size_t i);

    /*!
      \brief It returns the number of registered modules.

      \return The number of registered modules.
    */
    std::size_t getNumRegModules() const;

    /*!
      \brief It returns the module index inside the internal data structure.

      \param moduleName The name of a valid module registered in this singleton.

      \return The module index inside the internal data structure.
    */
    std::size_t getModuleIdx(const std::string& moduleName) const;

    /*!
      \brief It return the i-th module name.

      \param i The module you are looking for its name.

      \return The module name for the i-th module.
    */
    const std::string& getModuleName(std::size_t i) const;

  protected:

    /*! \brief The singleton constructor is not callable outside the class. */
    TerraLib();

    /* \brief Destructor. */
    ~TerraLib();

  private:

    /*!
      \brief This static function is invoked automatically when the application finishes its execution.

      \exception Exception It may throws an exception.

      \note This function will be automatically called at the end of your program.

      \note The registered module shutdown functions will be invoked in an opposite order of registering (the last module to be registered will be the first module to be finalized).

      \warning Not thread safe!

      \warning If this method throws an exception we recommend you to
               quit the program, don't try to resume it because you can have intermittent errors!
    */
    //static void sfinalize();

  private:

    std::vector<Module> m_modules;    //!< The list of registered modules.
    std::vector<bool> m_initialized;  //!< The list of modules to be initialized when initialize is called.
};

#endif  // __TERRALIB_COMMON_INTERNAL_TERRALIB_H

