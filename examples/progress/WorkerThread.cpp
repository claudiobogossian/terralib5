#include "ProgressExamples.h"
#include "Worker.h"

// Boost
#include <boost/thread.hpp> 


void WorkerThread()
{
  std::cout << "main: startup" << std::endl;  
       
  Worker w(612, 10, 5);  
  //boost::thread workerThread(w);  
       
  std::cout << "main: waiting for thread" << std::endl;  
       
  //workerThread.join();  
       
  std::cout << "main: done" << std::endl;  
}