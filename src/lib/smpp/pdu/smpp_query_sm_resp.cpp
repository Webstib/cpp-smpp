#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_query_sm_resp.h"

namespace smpp {

namespace pdu {

QuerySmResp::QuerySmResp(uint32 command_status,
                         uint32 sequence_number,
                         const std::string &message_id,
                         const std::string &final_date,
                         uint8 message_state,
                         uint8 error_code) :
  Header(MIN_LENGTH,
         parameters::command_id::QUERY_SM_RESP,
         command_status,
         sequence_number),
  message_id(message_id),
  final_date(final_date),
  message_state(message_state),
  error_code(error_code) {

}

QuerySmResp::QuerySmResp(const Header &p) :
  Header(p),
  message_state(0),
  error_code(0) {

}

QuerySmResp::QuerySmResp(const QuerySmResp &p) :
  Header(p),
  message_id(p.message_id),
  final_date(p.final_date),
  message_state(p.message_state),
  error_code(p.error_code) {

}

void QuerySmResp::calcCommandLength() {
  setCommandLength(MIN_LENGTH + message_id.size() + final_date.size());
}

QuerySmResp &QuerySmResp::operator =(const QuerySmResp &p) {
  if(this != &p) {
    Header::operator=(p);
    message_id = p.message_id;
    final_date = p.final_date;
    message_state = p.message_state;
    error_code = p.error_code;
  }
  return *this;
}

bool QuerySmResp::operator ==(const QuerySmResp &p) const {
  return(Header::operator==(p) &&
         message_id == p.message_id &&
         final_date == p.final_date &&
         message_state == p.message_state &&
         error_code == p.error_code);
}

bool QuerySmResp::operator !=(const QuerySmResp &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
