#ifndef SMPP_DATA_SM_RESP_H
#define SMPP_DATA_SM_RESP_H

#include "smpp/pdu/smpp_header.h"
#include "smpp/pdu/smpp_tlv_fields.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT DataSmResp : public Header, public TlvFields {
  std::string message_id;

public:

  DataSmResp(uint32 command_status = 0,
             uint32 sequence_number = 0,
             const std::string &message_id = std::string());

  DataSmResp(const Header &p);

  DataSmResp(const DataSmResp &p);

  virtual ~DataSmResp() {

  }

  inline void setMessageId(const std::string &v) {
    message_id = v;
  }

  inline const std::string &getMessageId() const {
    return message_id;
  }

  void calcCommandLength();

  DataSmResp &operator =(const DataSmResp &p);

  bool operator ==(const DataSmResp &p) const;

  bool operator !=(const DataSmResp &p) const;

  const static uint32 MIN_LENGTH = 17;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_DATA_SM_RESP_H
