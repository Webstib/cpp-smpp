#ifndef SMPP_ALERT_NOTIFICATION_H
#define SMPP_ALERT_NOTIFICATION_H

#include "smpp/pdu/smpp_header.h"
#include "smpp/pdu/smpp_tlv_fields.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT AlertNotification : public Header, public TlvFields {
  uint8 source_addr_ton;
  uint8 source_addr_npi;
  std::string source_addr;
  uint8 esme_addr_ton;
  uint8 esme_addr_npi;
  std::string esme_addr;

public:

  AlertNotification(uint32 sequence_number = 0,
                    uint8 source_addr_ton = 0,
                    uint8 source_addr_npi = 0,
                    const std::string &source_addr = std::string(),
                    uint8 esme_addr_ton = 0,
                    uint8 esme_addr_npi = 0,
                    const std::string &esme_addr = std::string());

  AlertNotification(const Header &p);

  AlertNotification(const AlertNotification &p);

  inline void setSourceAddrTon(uint8 v) {
    source_addr_ton = v;
  }

  inline void setSourceAddrNpi(uint8 v) {
    source_addr_npi = v;
  }

  inline void setSourceAddr(const std::string &v) {
    source_addr = v;
  }

  inline void setEsmeAddrTon(uint8 v) {
    esme_addr_ton = v;
  }

  inline void setEsmeAddrNpi(uint8 v) {
    esme_addr_npi = v;
  }

  inline void setEsmeAddr(const std::string &v) {
    esme_addr = v;
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

  inline uint8 getEsmeAddrTon() const {
    return esme_addr_ton;
  }

  inline uint8 getEsmeAddrNpi() const {
    return esme_addr_npi;
  }

  inline const std::string &getEsmeAddr() const {
    return esme_addr;
  }

  void calcCommandLength();

  AlertNotification &operator =(const AlertNotification &p);

  bool operator ==(const AlertNotification &p) const;

  bool operator !=(const AlertNotification &p) const;

  static const uint32 MIN_LENGTH = 22;
};

} // namespace pdu

} // namespace smpp

#endif // ALERT_NOTIFICATION_H
