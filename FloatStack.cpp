/*****************************************************************************
 *	Name:   Darius W Lambiri (dwlambiri)
 *	File:   FloatStackStack.cpp
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

#include "FloatStack.hpp"

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    Oct 23, 2017
   @mname   FloatStack::FloatStack
   @details
	  \n
  --------------------------------------------------------------------------
 */

FloatStack::FloatStack() {
	mStackSize = 50;
	mStack = new float[mStackSize];
	mElementsInStack = 0;

} // end-of-method FloatStack::FloatStack

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    Oct 23, 2017
   @mname   FloatStack::~FloatStack
   @details
	  \n
  --------------------------------------------------------------------------
 */

FloatStack::~FloatStack() {

	if (mStackSize > 0) {
		delete [] mStack;
	} //end-of-if

} // end-of-method FloatStack::~FloatStack

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    Oct 23, 2017
   @mname   FloatStack::FloatStack
   @details
	  \n
  --------------------------------------------------------------------------
 */

FloatStack::FloatStack(FloatStack& param) {

	mStackSize = param.mStackSize;
	mStack = new float[mStackSize];
	mElementsInStack = param.mElementsInStack;
	for (unsigned int i = 0; i < mElementsInStack; i++ ) {
		mStack[i] = param.mStack[i];
	} //end-of-for
} // end-of-method FloatStack::FloatStack



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    Oct 23, 2017
   @mname   FloatStack::FloatStack
   @details
	  \n
  --------------------------------------------------------------------------
 */

FloatStack::FloatStack(unsigned int stackSize) {

	if (stackSize == 0) {
		mStackSize = 1;
	} else {
		mStackSize = stackSize;
	} //end-of-if
	mStack = new float[mStackSize];
	mElementsInStack = 0;
} // end-of-method FloatStack::FloatStack


/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    Oct 23, 2017
   @mname   FloatStack::top
   @details
	  \n
  --------------------------------------------------------------------------
 */
float*
FloatStack::top() const{
	if (empty()) {
		return nullptr;
	} //end-of-if
	return &(mStack[mElementsInStack - 1]);
} // end-of-method FloatStack::top

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    Oct 23, 2017
   @mname   FloatStack::push
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
FloatStack::push(float param) {
	if (full()) {
		expand();
	} //end-of-if
	mStack[mElementsInStack++] = param;
} // end-of-method FloatStack::push


/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    Oct 23, 2017
   @mname   FloatStack::pop
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
FloatStack::pop() {

	if (empty()) {
		return;
	} else {
		mElementsInStack--;
	} //end-of-if
} // end-of-method FloatStack::pop


/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    Oct 23, 2017
   @mname   FloatStack::expand
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
FloatStack::expand() {
	mStackSize *= 2;
	float* tmp = new float[mStackSize];
	for (unsigned int i = 0; i < mElementsInStack; i++ ) {
		tmp[i] = mStack[i];
	} //end-of-for
	delete [] mStack;
	mStack = tmp;

} // end-of-method FloatStack::expand




