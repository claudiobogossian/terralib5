#include <iostream>


class Worker
{
  public:

    Worker(unsigned N, float guess, unsigned iter)
            : m_Number(N),
              m_Guess(guess),
              m_Iterations(iter){}  
           
    void operator()()
    {
      std::cout << "Worker: calculating sqrt(" << m_Number << "), itertations = " << m_Iterations << std::endl;  
               
      // Use Newton's Method
      float   x;  
      float   x_last = m_Guess;  
               
      for(unsigned i = 0; i < m_Iterations; i++)
      {
        x = x_last - (x_last*x_last-m_Number)/(2*x_last);
        x_last = x;

        std::cout << "Iter " << i << " = " << x << std::endl;  
      }
      
      std::cout << "Worker: Answer = " << x << std::endl;  
    }
       
    private:  
      unsigned    m_Number;  
      float       m_Guess;  
      unsigned    m_Iterations;  
};
