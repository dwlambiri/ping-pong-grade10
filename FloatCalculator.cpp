
#include "FloatCalculator.hpp"
#include <stdlib.h>
#include <string.h>


Calculator::Calculator(){
    expression[exSize];
	strcpy(expression, "");
}

Calculator::~Calculator(){
	delete[] expression;
}

Calculator::Calculator(char userExpression[]){
	strcpy(expression, userExpression);
}

float Calculator::add(){
    float value1 = *top();
    pop();
    float value2 = *top();
    pop();
    push(value2 + value1);
    return *top();
}

float Calculator::subtract(){
    float value1 = *top();
    pop();
    float value2 = *top();
    pop();
    push(value2 - value1);
    return *top();
}

float Calculator::multiply(){
    float value1 = *top();
    pop();
    float value2 = *top();
    pop();
    push(value2 * value1);
    return *top();
}

float Calculator::divide(){
    float value1 = *top();
    pop();
    float value2 = *top();
    pop();
    //This is error checker for divide by zero
    if (value1 == 0){
        return 0;
    }
    push(value2 / value1);
    return *top();
}

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    Oct 26, 2017
   @mname   Calculator::Commands
   @details
	  \n
  --------------------------------------------------------------------------
 */
bool
Calculator::Commands(char term[]) {

    if (term[0] <= '0' && term[0] >= term[0]){
        push(atof(term));
    }
    else{
        switch(term[0]){
          case '+':
              add();
              break;
          case '-':
              subtract();
              break;
          case '*':
              multiply();
              break;
          case '/':
              divide();
              break;
          default: return false;
        }
    }

} // end-of-method Calculator::Commands


bool
Calculator::newExpression(char newE[]){
    if(exSize < strlen(newE)){
        return false;
    }
    strcpy(expression, newE);
    return true;
}

bool
Calculator::parse(){

}

