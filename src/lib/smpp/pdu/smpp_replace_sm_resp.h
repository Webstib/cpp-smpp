#ifndef SMPP_REPLACE_SM_RESP_H
#define SMPP_REPLACE_SM_RESP_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT ReplaceSmResp : public Header {
public:
  ReplaceSmResp(uint32 command_status = 0, uint32 sequence_number = 0);

  ReplaceSmResp(const Header &p);

  ReplaceSmResp(const ReplaceSmResp &p);

  virtual ~ReplaceSmResp() {

  }
};

} // namespace pdu

} // namespace smpp


#endif // SMPP_REPLACE_SM_RESP_H
