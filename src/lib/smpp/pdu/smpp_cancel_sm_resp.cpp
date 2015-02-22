#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_cancel_sm_resp.h"

namespace smpp {

namespace pdu {

CancelSmResp::CancelSmResp(uint32 command_status,
                           uint32 sequence_number) :
  Header(HEADER_LENGTH,
         parameters::command_id::CANCEL_SM_RESP,
         command_status,
         sequence_number) {

}

CancelSmResp::CancelSmResp(const Header &p) : Header(p) {

}

CancelSmResp::CancelSmResp(const CancelSmResp &p) : Header(p) {

}

} // namespace pdu

} // namespace smpp
