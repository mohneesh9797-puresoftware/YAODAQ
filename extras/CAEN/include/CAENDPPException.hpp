#pragma once

#include "Exception.hpp"

namespace CAEN
{
  
class CAENDPPException: public Exception
{
public:
#if experimental_have_source_location == 1
  CAENDPPException(const int& code = 0, std::experimental::source_location loc = std::experimental::source_location::current());
#elif have_source_location == 1
  CAENDPPException(const int& code = 0, std::source_location loc  = std::source_location::current());
#else
  CAENDPPException(const int& code = 0)
#endif
  virtual std::string toString() const final;
private:
  CAENDPPException() = delete;
  virtual const char* errorStrings(const int& code) final;
};

}  // namespace CAEN
