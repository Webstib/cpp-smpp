#ifndef SMPP_CANCEL_SM_H
#define SMPP_CANCEL_SM_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT CancelSm : public Header {
  std::string service_type;
  std::string message_id;
  uint8 source_addr_ton;
  uint8 source_addr_npi;
  std::string source_addr;
  uint8 dest_addr_ton;
  uint8 dest_addr_npi;
  std::string destination_addr;

public:

  CancelSm(uint32 sequence_number = 0,
           const std::string &service_type = std::string(),
           const std::string &message_id = std::string(),
           uint8 source_addr_ton = 0,
           uint8 source_addr_npi = 0,
           const std::string &source_addr = std::string(),
           uint8 dest_addr_ton = 0,
           uint8 dest_addr_npi = 0,
           const std::string &destination_addr = std::string());

  CancelSm(const Header &p);

  CancelSm(const CancelSm &p);

  virtual ~CancelSm() {

  }

  inline void setServiceType(const std::string &v) {
    service_type = v;
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

  inline void setDestAddrTon(uint8 v) {
    dest_addr_ton = v;
  }

  inline void setDestAddrNpi(uint8 v) {
    dest_addr_npi = v;
  }

  inline void setDestinationAddr(const std::string &v) {
    destination_addr = v;
  }

  inline const std::string &getServiceType() const {
    return service_type;
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

  inline uint8 getDestAddrTon() const {
    return dest_addr_ton;
  }

  inline uint8 getDestAddrNpi() const {
    return dest_addr_npi;
  }

  inline const std::string &getDestinationAddr() const {
    return destination_addr;
  }

  void calcCommandLength();

  CancelSm &operator =(const CancelSm &p);

  bool operator ==(const CancelSm &p) const;

  bool operator !=(const CancelSm &p) const;

  static const uint32 MIN_LENGTH = 24;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_CANCEL_SM_H
