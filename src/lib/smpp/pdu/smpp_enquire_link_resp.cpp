#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_enquire_link_resp.h"

namespace smpp {

namespace pdu {

EnquireLinkResp::EnquireLinkResp(uint32 command_status,
                                 uint32 sequence_number) :
  Header(Header::HEADER_LENGTH,
         parameters::command_id::ENQUIRE_LINK_RESP,
         command_status,
         sequence_number) {

}

EnquireLinkResp::EnquireLinkResp(const Header &p) : Header(p) {

}

EnquireLinkResp::EnquireLinkResp(const EnquireLinkResp &p) : Header(p) {

}

} // namespace pdu

} // namespace smpp

