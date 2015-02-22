#ifndef SMPP_SUBMIT_SM_RESP_H
#define SMPP_SUBMIT_SM_RESP_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT SubmitSmResp : public Header {
  std::string message_id;

public:

  SubmitSmResp(uint32 command_status = 0,
               uint32 sequence_number = 0,
               const std::string &message_id = std::string());

  SubmitSmResp(const Header &p);

  SubmitSmResp(const SubmitSmResp &p);

  virtual ~SubmitSmResp() {

  }

  inline void setMessageId(const std::string &v) {
    message_id = v;
  }

  inline const std::string &getMessageId() const {
    return message_id;
  }

  void calcCommandLength();

  SubmitSmResp &operator =(const SubmitSmResp &p);

  bool operator ==(const SubmitSmResp &p) const;

  bool operator !=(const SubmitSmResp &p) const;

  const static uint32 MIN_LENGTH = 16;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_SUBMIT_SM_RESP_H
