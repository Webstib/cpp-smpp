#ifndef SMPP_HEX_ENCODER_H
#define SMPP_HEX_ENCODER_H

#include "smpp/pdu/smpp_bind_transmitter.h"
#include "smpp/pdu/smpp_bind_transmitter_resp.h"
#include "smpp/pdu/smpp_bind_receiver.h"
#include "smpp/pdu/smpp_bind_receiver_resp.h"
#include "smpp/pdu/smpp_bind_transceiver.h"
#include "smpp/pdu/smpp_bind_transceiver_resp.h"
#include "smpp/pdu/smpp_outbind.h"
#include "smpp/pdu/smpp_unbind.h"
#include "smpp/pdu/smpp_unbind_resp.h"
#include "smpp/pdu/smpp_generic_nack.h"
#include "smpp/pdu/smpp_submit_sm.h"
#include "smpp/pdu/smpp_submit_sm_resp.h"
#include "smpp/pdu/smpp_submit_multi.h"
#include "smpp/pdu/smpp_submit_multi_resp.h"
#include "smpp/pdu/smpp_deliver_sm.h"
#include "smpp/pdu/smpp_deliver_sm_resp.h"
#include "smpp/pdu/smpp_data_sm.h"
#include "smpp/pdu/smpp_data_sm_resp.h"
#include "smpp/pdu/smpp_query_sm.h"
#include "smpp/pdu/smpp_query_sm_resp.h"
#include "smpp/pdu/smpp_cancel_sm.h"
#include "smpp/pdu/smpp_cancel_sm_resp.h"
#include "smpp/pdu/smpp_replace_sm.h"
#include "smpp/pdu/smpp_replace_sm_resp.h"
#include "smpp/pdu/smpp_enquire_link.h"
#include "smpp/pdu/smpp_enquire_link_resp.h"
#include "smpp/pdu/smpp_alert_notification.h"

namespace smpp {

namespace encoders {

class SMPP_EXPORT HexEncoder {
  std::string data;
  const static char hex[];

  std::string byte_2_str(const int8 *bytes, uint32 size);

  void pack(uint8 p);

  void pack(uint16 p);

  void pack(uint32 p);

  void pack(const int8 *p, uint32 p_len);

  void pack(const std::string &p);

  void pack(const smpp::pdu::MultiDestinationAddresses &p);

  void pack(const smpp::pdu::DistributionListAddress &p);

  void pack(smpp::pdu::MultiDestinationAddressBase *p);

  void pack(const smpp::pdu::SmeMultiAddress &p);

  void pack(const smpp::pdu::UnsuccessSme &p);

  void pack(const smpp::pdu::Tlv &p);

  void pack(const smpp::pdu::TlvFields &p);

  void pack(const smpp::pdu::Header &p);

public:

  HexEncoder();

  ~HexEncoder() {

  }

  void clear();

  std::string getBuff() const;

  void pack(const smpp::pdu::BindTransmitter &p);

  void pack(const smpp::pdu::BindTransmitterResp &p);

  void pack(const smpp::pdu::BindReceiver &p);

  void pack(const smpp::pdu::BindReceiverResp &p);

  void pack(const smpp::pdu::BindTransceiver &p);

  void pack(const smpp::pdu::BindTransceiverResp &p);

  void pack(const smpp::pdu::Outbind &p);

  void pack(const smpp::pdu::Unbind &p);

  void pack(const smpp::pdu::UnbindResp &p);

  void pack(const smpp::pdu::GenericNack &p);

  void pack(const smpp::pdu::SubmitSm &p);

  void pack(const smpp::pdu::SubmitSmResp &p);

  void pack(const smpp::pdu::SubmitMulti &p);

  void pack(const smpp::pdu::SubmitMultiResp &p);

  void pack(const smpp::pdu::DeliverSm &p);

  void pack(const smpp::pdu::DeliverSmResp &p);

  void pack(const smpp::pdu::DataSm &p);

  void pack(const smpp::pdu::DataSmResp &p);

  void pack(const smpp::pdu::QuerySm &p);

  void pack(const smpp::pdu::QuerySmResp &p);

  void pack(const smpp::pdu::CancelSm &p);

  void pack(const smpp::pdu::CancelSmResp &p);

  void pack(const smpp::pdu::ReplaceSm &p);

  void pack(const smpp::pdu::ReplaceSmResp &p);

  void pack(const smpp::pdu::EnquireLink &p);

  void pack(const smpp::pdu::EnquireLinkResp &p);

  void pack(const smpp::pdu::AlertNotification &p);
};

} // namespace encoders

} // namespace smpp

#endif // SMPP_HEX_ENCODER_H
