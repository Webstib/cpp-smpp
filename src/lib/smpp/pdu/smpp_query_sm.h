#ifndef SMPP_QUERY_SM_H
#define SMPP_QUERY_SM_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT QuerySm : public Header {
  std::string message_id;
  uint8 source_addr_ton;
  uint8 source_addr_npi;
  std::string source_addr;

public:

  QuerySm(uint32 sequence_number = 0,
          const std::string &message_id = std::string(),
          uint8 source_addr_ton = 0,
          uint8 source_addr_npi = 0,
          const std::string &source_addr = std::string());

  QuerySm(const Header &p);

  QuerySm(const QuerySm &p);

  virtual ~QuerySm() {

  }

  inline void setMessageId(const std::string &v) {
    message_id = v;
  }


  inline void setSourceAddrTon(uint8 v) {
    source_addr_ton = v;
  }

  inline void setSourceAddrNpi(uint8 v) {
    source_addr_npi = v;
  }

  inline void setSourceAddr(const std::string &v) {
    source_addr = v;
  }

  inline const std::string &getMessageId() const {
    return message_id;
  }

  inline uint8 getSourceAddrTon() const {
    return source_addr_ton;
  }

  inline uint8 getSourceAddrNpi() const {
    return source_addr_npi;
  }

  inline const std::string &getSourceAddr() const {
    return source_addr;
  }

  void calcCommandLength();

  QuerySm &operator =(const QuerySm &p);

  bool operator ==(const QuerySm &p) const;

  bool operator !=(const QuerySm &p) const;

  const static uint32 MIN_LENGTH = 20;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_QUERY_SM_H
