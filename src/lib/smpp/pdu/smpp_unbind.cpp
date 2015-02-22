#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_unbind.h"

namespace smpp {

namespace pdu {

Unbind::Unbind(uint32 sequence_number) :
  Header(Header::HEADER_LENGTH,
         parameters::command_id::UNBIND,
         parameters::command_status::ESME_ROK,
         sequence_number) {

}

Unbind::Unbind(const Header &p) : Header(p) {

}

Unbind::Unbind(const Unbind &p) : Header(p) {

}

} // namespace pdu

} // namespace smpp
