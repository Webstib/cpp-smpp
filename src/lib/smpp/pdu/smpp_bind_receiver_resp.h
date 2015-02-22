#ifndef SMPP_BIND_RECEIVER_RESP_H
#define SMPP_BIND_RECEIVER_RESP_H

#include "smpp/pdu/smpp_header.h"
#include "smpp/pdu/smpp_tlv_fields.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT BindReceiverResp : public Header, public TlvFields {
  std::string system_id;

public:

  BindReceiverResp(uint32 command_status = 0,
                   uint32 sequence_number = 0,
                   const std::string &system_id = std::string());

  BindReceiverResp(const Header &p);

  BindReceiverResp(const BindReceiverResp &p);

  virtual ~BindReceiverResp() {

  }

  inline void setSystemId(const std::string &v) {
    system_id = v;
  }

  inline const std::string &getSystemId() const {
    return system_id;
  }

  void calcCommandLength();

  BindReceiverResp &operator =(const BindReceiverResp &p);

  bool operator ==(const BindReceiverResp &p) const;

  bool operator !=(const BindReceiverResp &p) const;

  const static uint32 MIN_LENGTH = 17;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_BIND_RECEIVER_RESP_H
