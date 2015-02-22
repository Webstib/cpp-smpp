#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_unbind_resp.h"

namespace smpp {

namespace pdu {

UnbindResp::UnbindResp(uint32 command_status,
                       uint32 sequence_number) :
  Header(Header::HEADER_LENGTH,
         parameters::command_id::UNBIND_RESP,
         command_status,
         sequence_number) {

}

UnbindResp::UnbindResp(const Header &p) : Header(p) {

}

UnbindResp::UnbindResp(const UnbindResp &p) : Header(p) {

}

} // namespace pdu

} // namespace smpp
