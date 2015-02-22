#ifndef SMPP_CANCEL_SM_RESP_H
#define SMPP_CANCEL_SM_RESP_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT CancelSmResp : public Header {
public:
  CancelSmResp(uint32 command_status = 0, uint32 sequence_number = 0);

  CancelSmResp(const Header &p);

  CancelSmResp(const CancelSmResp &p);

  virtual ~CancelSmResp() {

  }
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_CANCEL_SM_RESP_H
