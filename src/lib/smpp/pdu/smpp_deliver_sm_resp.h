#ifndef SMPP_DELIVER_SM_RESP_H
#define SMPP_DELIVER_SM_RESP_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT DeliverSmResp : public Header {
  std::string message_id;

public:

  DeliverSmResp(uint32 command_status = 0,
                uint32 sequence_number = 0,
                const std::string &message_id = std::string());

  DeliverSmResp(const Header &p);

  DeliverSmResp(const DeliverSmResp &p);

  virtual ~DeliverSmResp() {

  }

  inline void setMessageId(const std::string &v) {
    message_id = v;
  }

  inline const std::string &getMessageId() const {
    return message_id;
  }

  void calcCommandLength();

  DeliverSmResp &operator =(const DeliverSmResp &p);

  bool operator ==(const DeliverSmResp &p) const;

  bool operator !=(const DeliverSmResp &p) const;

  const static uint32 MIN_LENGTH = 17;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_DELIVER_SM_RESP_H
