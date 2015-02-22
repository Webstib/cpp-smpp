#ifndef SMPP_ENQUIRE_LINK_H
#define SMPP_ENQUIRE_LINK_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT EnquireLink : public Header {
public:
  EnquireLink(uint32 sequence_number = 0);

  EnquireLink(const Header &p);

  EnquireLink(const EnquireLink &p);

  virtual ~EnquireLink() {

  }
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_ENQUIRE_LINK_H
