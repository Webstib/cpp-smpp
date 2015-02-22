#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_replace_sm_resp.h"

namespace smpp {

namespace pdu {

ReplaceSmResp::ReplaceSmResp(uint32 command_status,
                             uint32 sequence_number) :
  Header(HEADER_LENGTH,
         parameters::command_id::REPLACE_SM_RESP,
         command_status,
         sequence_number) {

}

ReplaceSmResp::ReplaceSmResp(const Header &p) : Header(p) {

}

ReplaceSmResp::ReplaceSmResp(const ReplaceSmResp &p) : Header(p) {

}

} // namespace pdu

} // namespace smpp
