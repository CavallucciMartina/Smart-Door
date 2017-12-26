#include "Token.h"

Token::Token(){
  state = "TOKEN_AUTENTICATION";
}

String Token::getState(){
  return this->state;
}

void Token::setState(String state){
  this->state = state;
}
