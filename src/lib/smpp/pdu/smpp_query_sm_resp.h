#ifndef SMPP_QUERY_SM_RESP_H
#define SMPP_QUERY_SM_RESP_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT QuerySmResp : public Header {
  std::string message_id;
  std::string final_date;
  uint8 message_state;
  uint8 error_code;

public:

  QuerySmResp(uint32 command_status = 0,
              uint32 sequence_number = 0,
              const std::string &message_id = std::string(),
              const std::string &final_date = std::string(),
              uint8 message_state = 0,
              uint8 error_code = 0);

  QuerySmResp(const Header &p);

  QuerySmResp(const QuerySmResp &p);

  virtual ~QuerySmResp() {

  }

  inline void setMessageId(const std::string &v) {
    message_id = v;
  }

  inline void setFinalDate(const std::string &v) {
    final_date = v;
  }

  inline void setMessageState(uint8 v) {
    message_state = v;
  }

  inline void setErrorCode(uint8 v) {
    error_code = v;
  }

  inline const std::string &getMessageId() const {
    return message_id;
  }

  inline const std::string &getFinalDate() const {
    return final_date;
  }

  inline uint8 getMessageState() const {
    return message_state;
  }

  inline uint8 getErrorCode() const {
    return error_code;
  }

  void calcCommandLength();

  QuerySmResp &operator =(const QuerySmResp &p);

  bool operator ==(const QuerySmResp &p) const;

  bool operator !=(const QuerySmResp &p) const;

  const static uint32 MIN_LENGTH = 20;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_QUERY_SM_RESP_H
