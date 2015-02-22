#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_enquire_link.h"

namespace smpp {

namespace pdu {

EnquireLink::EnquireLink(uint32 sequence_number) :
  Header(Header::HEADER_LENGTH,
         parameters::command_id::ENQUIRE_LINK,
         parameters::command_status::ESME_ROK,
         sequence_number) {

}

EnquireLink::EnquireLink(const Header &p) : Header(p) {

}

EnquireLink::EnquireLink(const EnquireLink &p) : Header(p) {

}

} // namespace pdu

} // namespace smpp
