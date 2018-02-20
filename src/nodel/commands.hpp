#pragma once
#include <string>

class ofxPiMapper;
namespace op{
  struct Operation{
    virtual void run(ofxPiMapper &) = 0;
    virtual std::string get_type() = 0;
  };


};
