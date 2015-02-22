#ifndef SMPP_SERVER_ASYNC_SESSION_H
#define SMPP_SERVER_ASYNC_SESSION_H

#include <QtNetwork/QHostAddress>
#include "smpp/sessions/smpp_session_global.h"

namespace smpp {

namespace sessions {

class ServerAsyncSessionPrivate;

class SMPP_EXPORT ServerAsyncSession : public QObject {
  Q_OBJECT
public:
  ServerAsyncSession(
    int sock_descriptor,
    int max_in_speed = DEFAULT_IN_SPEED, // sms / sec
    int max_out_speed = DEFAULT_OUT_SPEED, // sms / sec
    int session_init_time = DEFAULT_SESSION_INIT_TIME,
    int enquire_link_time = DEFAULT_ENQUIRE_LINK_TIME,
    int inactivity_time = DEFAULT_INACTIVITY_TIME,
    int transaction_time = DEFAULT_TRANSACTION_TIME,
    int max_in_trans = DEFAULT_IN_TRANSACTIONS,
    int max_out_trans = DEFAULT_OUT_TRANSACTIONS,
    bool threaded = true);

  ~ServerAsyncSession();

  void setMaxInSpeed(int in_speed);

  void setMaxOutSpeed(int out_speed);

  void setSessionInitTime(int session_init_time);

  void setEnquireLinkTime(int enquire_link_time);

  void setInactivityTime(int inactivity_time);

  void setTransactionTime(int transaction_time);

  void setMaxInTrans(int max_in_trans);

  void setMaxOutTrans(int max_out_trans);

  smpp::sessions::SessionStates state() const;

  QHostAddress peerAddress() const;

  uint16 peerPort() const;

  QHostAddress localAddress() const;

  uint16 localPort() const;

  int getMaxInSpeed() const;

  int getMaxOutSpeed() const;

  int getNowInSpeed() const;

  int getNowOutSpeed() const;

  int getSessionInitTime() const;

  int getEnquireLinkTime() const;

  int getInactivityTime() const;

  int getTransactionTime() const;

  int getMaxInTrans() const;

  int getMaxOutTrans() const;

  int getOpenedInTrans() const;

  int getOpenedOutTrans() const;

  smpp::sessions::ErrorCode error() const;

  std::string errorString() const;

public slots:

  void closeSession();

  void send(const smpp::pdu::AlertNotification &p);

  void send(const smpp::pdu::BindTransmitterResp &p);

  void send(const smpp::pdu::BindReceiverResp &p);

  void send(const smpp::pdu::BindTransceiverResp &p);

  void send(const smpp::pdu::UnbindResp &p);

  void send(const smpp::pdu::Outbind &p);

  void send(const smpp::pdu::GenericNack &p);

  void send(const smpp::pdu::SubmitSmResp &p);

  void send(const smpp::pdu::SubmitMultiResp &p);

  void send(const smpp::pdu::DeliverSm &p);

  void send(const smpp::pdu::DataSm &p);

  void send(const smpp::pdu::DataSmResp &p);

  void send(const smpp::pdu::QuerySmResp &p);

  void send(const smpp::pdu::ReplaceSmResp &p);

  void send(const smpp::pdu::CancelSmResp &p);

  void send(const smpp::pdu::EnquireLink &p);

  void send(const smpp::pdu::EnquireLinkResp &p);

private slots:

  void startProcess();

signals:

  void openedSession();

  void closedSession();

  void receive(const smpp::pdu::GenericNack &p);

  void receive(const smpp::pdu::BindTransmitter &p);

  void receive(const smpp::pdu::BindReceiver &p);

  void receive(const smpp::pdu::BindTransceiver &p);

  void receive(const smpp::pdu::Unbind &p);

  void receive(const smpp::pdu::SubmitSm &p);

  void receive(const smpp::pdu::SubmitMulti &p);

  void receive(const smpp::pdu::DeliverSmResp &p);

  void receive(const smpp::pdu::DataSm &p);

  void receive(const smpp::pdu::DataSmResp &p);

  void receive(const smpp::pdu::QuerySm &p);

  void receive(const smpp::pdu::ReplaceSm &p);

  void receive(const smpp::pdu::CancelSm &p);

  void receive(const smpp::pdu::EnquireLink &p);

  void receive(const smpp::pdu::EnquireLinkResp &p);

  void stateChanged(smpp::sessions::SessionStates state);

  void sessionInitTimeout();

  void enquireLinkTimeout();

  void inactivityTimeout();

  void outStatus(const smpp::pdu::Header &p,
                 smpp::sessions::StatusMessage status);

  void inStatus(const smpp::pdu::Header &p,
                smpp::sessions::StatusMessage status);

  void error(smpp::sessions::ErrorCode code);

private:

  friend class ServerAsyncSessionPrivate;

  ServerAsyncSessionPrivate *p;
};

} // namespace sessions

} // namespace smpp

#endif // SMPP_SERVER_ASYNC_SESSION_H
