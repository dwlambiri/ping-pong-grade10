

#ifndef ASSIGNMENT4_STRINGSTACK_HPP_
#define ASSIGNMENT4_FLOATSTACK_HPP_

#include "FloatStack.hpp"
//#include "FloatStack.cpp"
#include <iostream>
#include <string>

/**
  ---------------------------------------------------------------------------
   @author     dwlambiri
   @date       Oct 26, 2017
   @classname  Calculator
   @brief
	 \n
   @details
	\n
  ---------------------------------------------------------------------------
 */
class Calculator : public FloatStack{

public:
	//--------------------------------------------------
	// Public Methods (External Type Interface)
	//--------------------------------------------------
	/**
	  --------------------------------------------------------------------------
	   @author  dwlambiri
	   @date    Oct 26, 2017
	   @name    Calculator::Calculator
	   @param   -
	   @return  -
	   @details
		Default constructor of Class Calculator \n
	  --------------------------------------------------------------------------
	 */
	Calculator();

	/**
	  --------------------------------------------------------------------------
	   @author  dwlambiri
	   @date    Oct 26, 2017
	   @name    Calculator::~Calculator
	   @param   -
	   @return  -
	   @details
		Default destructor of class Calculator \n
	  --------------------------------------------------------------------------
	 */
	~Calculator();

	 Calculator(char userExpression[]);

	 float add();

	 float subtract();

	 float multiply();

	 float divide();

	 /**
	  ---------------------------------------------------------------------------
	   @author  dwlambiri
	   @date    Oct 26, 2017
	   @mname   Commands
	   @details
		  \n
	  --------------------------------------------------------------------------
	 */
	bool Commands(char term[]);

	bool newExpression(char newE[]);

	bool parse();

private:
	//--------------------------------------------------
	// Data Members
	//--------------------------------------------------
	char* expression;
    int exSize = 200;

private:
	//--------------------------------------------------
	// Private Methods
	//--------------------------------------------------

}; //end-of-class Calculator

#endif /* ASSIGNMENT4_FLOATSTACK_HPP_ */
