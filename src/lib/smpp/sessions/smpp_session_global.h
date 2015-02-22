#ifndef SMPP_SESSION_GLOBAL_H
#define SMPP_SESSION_GLOBAL_H

#include <boost/signals2/signal.hpp>
#include "smpp/parameters/smpp_prameters_header.h"
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

namespace sessions {

enum StatusMessage {
  SENDED,
  RECEIVED,
  MAXSPEED,
  TRANSSTART,
  TRANSFULL,
  TRANSTIMEOUT,
  RESPONSED,
  ERRORCID,
  ERRORDECODE,
  NOTVALIDSTATE,
  NOTFOUNDTRANS,
  SESSERROR
};

enum SessionStates {
  CLOSED,
  OPEN,
  BOUND_TX,
  BOUND_RX,
  BOUND_TRX,
  UNBOUND,
  OUTBOUND
};

static const int DEFAULT_IN_TRANSACTIONS   = 10000;
static const int DEFAULT_OUT_TRANSACTIONS  = 10000;
static const int DEFAULT_IN_SPEED          = 1000;  // sms / sec
static const int DEFAULT_OUT_SPEED         = 1000;  // sms / sec
static const int DEFAULT_SESSION_INIT_TIME = 10000; // sec
static const int DEFAULT_ENQUIRE_LINK_TIME = 1000;  // sec
static const int DEFAULT_INACTIVITY_TIME   = 100;   // sec
static const int DEFAULT_TRANSACTION_TIME  = 1000;  // sec

typedef std::shared_ptr<smpp::pdu::Header> HeaderPtr;
typedef boost::signals2::signal<void ()> voidSig;
typedef boost::signals2::signal<void (const HeaderPtr &,
                                      smpp::sessions::StatusMessage)> statSig;
typedef boost::signals2::signal<void (smpp::sessions::SessionStates)> stateSig;
typedef boost::signals2::signal<void (const boost::system::error_code &)>
  errorSig;
typedef boost::signals2::signal<void (const HeaderPtr &)> headerSig;

} // namespace sessions

} // namespace smpp

#endif // SMPP_SESSION_GLOBAL_H
