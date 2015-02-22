#ifndef SMPP_BIND_TRANSMITTER_RESP_H
#define SMPP_BIND_TRANSMITTER_RESP_H

#include "smpp/pdu/smpp_header.h"
#include "smpp/pdu/smpp_tlv_fields.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT BindTransmitterResp : public Header, public TlvFields {
  std::string system_id;

public:

  BindTransmitterResp(uint32 command_status = 0,
                      uint32 sequence_number = 0,
                      const std::string &system_id = std::string());

  BindTransmitterResp(const Header &p);

  BindTransmitterResp(const BindTransmitterResp &p);

  virtual ~BindTransmitterResp() {

  }

  inline void setSystemId(const std::string &v) {
    system_id = v;
  }

  inline const std::string &getSystemId() const {
    return system_id;
  }

  void calcCommandLength();

  BindTransmitterResp &operator =(const BindTransmitterResp &p);

  bool operator ==(const BindTransmitterResp &p) const;

  bool operator !=(const BindTransmitterResp &p) const;

  const static uint32 MIN_LENGTH = 17;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_BIND_TRANSMITTER_RESP_H
