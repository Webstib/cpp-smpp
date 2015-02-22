#ifndef SMPP_GENERIC_NACK_H
#define SMPP_GENERIC_NACK_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT GenericNack : public Header {
public:
  GenericNack(uint32 command_status = 0, uint32 sequence_number = 0);

  GenericNack(const Header &p);

  GenericNack(const GenericNack &p);

  virtual ~GenericNack() {

  }
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_GENERIC_NACK_H
