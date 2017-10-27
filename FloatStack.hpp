/*****************************************************************************
 *	Name:   Darius W Lambiri (dwlambiri)
 *	File:   FloatStack.hpp
 *	Course: ICS4U
 *	Date:   Oct 23, 2017
 *
 *	Purpose:
 *
 *	Usage:
 *
 *	Revision History:
 *
 *	Known Issues:
 *
 *****************************************************************************/

#ifndef ASSIGNMENT4_STRINGSTACK_HPP_
#define ASSIGNMENT4_FLOATSTACK_HPP_

#include <string>

/**
  ---------------------------------------------------------------------------
   @author     dwlambiri
   @date       Oct 23, 2017
   @classname  FloatStack
   @brief
	 \n
   @details
	 This is a string stack class that dynamically resizes itself once full.
	 \n
  ---------------------------------------------------------------------------
 */
class FloatStack {

public:
	//--------------------------------------------------
	// Public Methods (External Type Interface)
	//--------------------------------------------------
	/**
	  --------------------------------------------------------------------------
	   @author  dwlambiri
	   @date    Oct 23, 2017
	   @name    FloatStack::FloatStack
	   @param   -
	   @return  -
	   @details
		Default constructor of Class FloatStack \n
	  --------------------------------------------------------------------------
	 */
	FloatStack();

	/**
	  -------------------------------------------------------------------------
	   @name  FloatStack
	   @param  int
	   @return
	   @details
		  \n
	  -------------------------------------------------------------------------
	 */
	 FloatStack(unsigned int);


	 /**
	  -------------------------------------------------------------------------
	   @name  FloatStack
	   @param  FloatStack&
	   @return
	   @details
		  \n
	  -------------------------------------------------------------------------
	 */
	 FloatStack(FloatStack&);

	/**
	  --------------------------------------------------------------------------
	   @author  dwlambiri
	   @date    Oct 23, 2017
	   @name    FloatStack::~FloatStack
	   @param   -
	   @return  -
	   @details
		Default destructor of class FloatStack \n
	  --------------------------------------------------------------------------
	 */
	~FloatStack();

	/**
	  -------------------------------------------------------------------------
	   @name  getStackSize
	   @param
	   @return  int
	   @details
		  This is an accessor method that returns the maximum number of
		  elements that can be stored in the object. \n
	  -------------------------------------------------------------------------
	 */
	int getStackSize() const { return mStackSize; }

	/**
	  -------------------------------------------------------------------------
	   @name  top
	   @param
	   @return  std::string&
	   @details
		  It is an accessor method that returns the top element in the
		  stack or a nullptr if no elements are present.
		  This method does NOT remove the top element.
		  This interface is inspired by the std::stack class.
		  See here http://www.cplusplus.com/reference/stack/stack/\n
	  -------------------------------------------------------------------------
	 */
	float* top() const;

	/**
	  -------------------------------------------------------------------------
	   @name  push
	   @param  std::string&
	   @return  void
	   @details
		  This is a modifier method that adds an element to the stack
		  if the stack is not full. Otherwise it doubles the maximum size
		  of the stack, copies all of the original elements and adds the new
		  element.\n
	  -------------------------------------------------------------------------
	 */
	void push(float);

	/**
	  -------------------------------------------------------------------------
	   @name  pop
	   @param
	   @return  void
	   @details
		  If the stack is not empty, this modifier removes the top element
		  in the stack. It does not return the element to the user.
		  This behaviour is inspired by the std::stack.pop() method.\n
	  -------------------------------------------------------------------------
	 */
	void pop();

	/**
	  -------------------------------------------------------------------------
	   @name  isEmpty
	   @param
	   @return  bool
	   @details
		  This method checks checks if the stack is empty and returns true
		  if it is. \n
	  -------------------------------------------------------------------------
	 */
	bool empty() const {return (mElementsInStack == 0);}

	/**
	  ---------------------------------------------------------------------------
	   @author  dwlambiri
	   @date    Oct 23, 2017
	   @mname   full
	   @details
		  This method checks if the stack is full and returns true if it is. \n
	  --------------------------------------------------------------------------
	 */
	bool full() {return mElementsInStack >= mStackSize;} // end-of-method full

protected:

	/*
	 * @author   dwlambiri
	 * @date     Oct 23, 2017
	 *  mStack is dynamically allocated when the object is constructed,
	 *  and deleted when the object is destroyed.
	 *  It stores the strings passed to the object.
	 */
	float*    mStack;


private:
	//--------------------------------------------------
	// Data Members
	//--------------------------------------------------

	/*
	 * @author   dwlambiri
	 * @date     Oct 23, 2017
	 *  mStackSize remembers the maximum number of elements that mStack can store.
	 */
	unsigned int    mStackSize;

	/*
	 * @author   dwlambiri
	 * @date     Oct 23, 2017
	 *   mElementsInStack represents the number of elements in the stack.
	 *   The index of the top of the stack is mElementsInStack - 1, if
	 *     mElementsInStack > 0.
	 */
	unsigned int    mElementsInStack;

private:
	//--------------------------------------------------
	// Private Methods
	//--------------------------------------------------
	/**
	  -------------------------------------------------------------------------
	   @author  dwlambiri
	   @date    Oct 23, 2017
	   @name    FloatStack::expand
	   @param
	   @return  type
	   @details
		  \n
	  -------------------------------------------------------------------------
	 */
	void expand();

}; //end-of-class FloatStack


#endif /* ASSIGNMENT4_FLOATSTACK_HPP_ */



