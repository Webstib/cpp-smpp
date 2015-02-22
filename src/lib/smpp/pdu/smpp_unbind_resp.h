#ifndef SMPP_UNBIND_RESP_H
#define SMPP_UNBIND_RESP_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT UnbindResp : public Header {
public:
  UnbindResp(uint32 command_status = 0, uint32 sequence_number = 0);

  UnbindResp(const Header &p);

  UnbindResp(const UnbindResp &p);

  virtual ~UnbindResp() {

  }
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_UNBIND_RESP_H
