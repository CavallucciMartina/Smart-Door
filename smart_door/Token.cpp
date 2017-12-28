#include "Token.h"
#include "config.h"

Token::Token(){
  state = AUTENTICATION_STATE;
}

String Token::getState(){
  return this->state;
}

void Token::setState(String state){
  this->state = state;
}
