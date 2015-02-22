#ifndef SMPP_SME_MULTI_ADDRESS_H
#define SMPP_SME_MULTI_ADDRESS_H

#include "smpp/pdu/smpp_multi_destination_address_base.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT SmeMultiAddress : public MultiDestinationAddressBase {
  uint8 dest_addr_ton;
  uint8 dest_addr_npi;
  std::string destination_addr;

public:

  SmeMultiAddress(uint8 dest_addr_ton = 0,
                  uint8 dest_addr_npi = 0,
                  const std::string &destination_addr = std::string());

  SmeMultiAddress(const SmeMultiAddress &p);

  virtual ~SmeMultiAddress() {

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

  inline uint8 getDestAddrTon() const {
    return dest_addr_ton;
  }

  inline uint8 getDestAddrNpi() const {
    return dest_addr_npi;
  }

  inline const std::string &getDestinationAddr() const {
    return destination_addr;
  }

  inline uint8 getDestFlag() const {
    return 0x01;
  }

  inline uint32 size() const {
    return 4 + destination_addr.size();
  }

  SmeMultiAddress &operator =(const SmeMultiAddress &p);

  bool operator ==(const SmeMultiAddress &p) const;

  bool operator !=(const SmeMultiAddress &p) const;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_SME_MULTI_ADDRESS_H
