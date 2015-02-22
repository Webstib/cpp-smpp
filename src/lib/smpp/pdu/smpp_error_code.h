#ifndef SMPP_ERROR_CODE_H
#define SMPP_ERROR_CODE_H

#include "smpp_integer1.h"

namespace smpp {

namespace pdu {

namespace query_sm {

class SMPP_EXPORT ErrorCode : public Integer1 {
public:
  ErrorCode() : Integer1() {}

  ErrorCode(const Integer1 &p) : Integer1(p) {}

  ErrorCode(const ErrorCode &p) : Integer1(p) {}
};

} // namespace query_sm

} // namespace pdu

} // namespace smpp

#endif // SMPP_ERROR_CODE_H
