#ifndef SMPP_UNSUCCESS_SME_H
#define SMPP_UNSUCCESS_SME_H

#include "smpp/smpp_global.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT UnsuccessSme {
  uint8 dest_addr_ton;
  uint8 dest_addr_npi;
  std::string destination_addr;
  uint32 error_status_code;

public:

  UnsuccessSme(uint8 dest_addr_ton = 0,
               uint8 dest_addr_npi = 0,
               const std::string &destination_addr = std::string(),
               uint32 error_status_code = 0);

  UnsuccessSme(const UnsuccessSme &p);

  virtual ~UnsuccessSme() {

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

  inline void setErrorStatusCode(uint32 v) {
    error_status_code = v;
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

  inline uint32 getErrorStatusCode() const {
    return error_status_code;
  }

  inline uint32 size() const {
    return 6 + destination_addr.size();
  }

  UnsuccessSme &operator =(const UnsuccessSme &p);

  bool operator ==(const UnsuccessSme &p) const;

  bool operator !=(const UnsuccessSme &p) const;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_UNSUCCESS_SME_H
