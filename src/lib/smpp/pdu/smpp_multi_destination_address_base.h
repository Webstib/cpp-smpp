#ifndef SMPP_MULTI_DESTINATION_ADDRESS_BASE_H
#define SMPP_MULTI_DESTINATION_ADDRESS_BASE_H

#include "smpp/smpp_global.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT MultiDestinationAddressBase {
public:
  MultiDestinationAddressBase() {}

  MultiDestinationAddressBase(const MultiDestinationAddressBase &) {}

  virtual ~MultiDestinationAddressBase() {}

  virtual uint8 getDestFlag() const = 0;

  virtual uint32 size() const = 0;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_MULTI_DESTINATION_ADDRESS_BASE_H
