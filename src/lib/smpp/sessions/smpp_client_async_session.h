#ifndef SMPP_CLIENT_ASYNC_SESSION_H
#define SMPP_CLIENT_ASYNC_SESSION_H

#include "smpp/sessions/smpp_session_global.h"

namespace smpp {

namespace sessions {

class ClientAsyncSessionPrivate;

class SMPP_EXPORT ClientAsyncSession {
public:
  ClientAsyncSession(
    boost::asio::io_service &service,
    int32 max_in_speed = DEFAULT_IN_SPEED, // sms / sec
    int32 max_out_speed = DEFAULT_OUT_SPEED, // sms / sec
    int32 session_init_time = DEFAULT_SESSION_INIT_TIME,
    int32 enquire_link_time = DEFAULT_ENQUIRE_LINK_TIME,
    int32 inactivity_time = DEFAULT_INACTIVITY_TIME,
    int32 transaction_time = DEFAULT_TRANSACTION_TIME,
    int32 max_in_trans = DEFAULT_IN_TRANSACTIONS,
    int32 max_out_trans = DEFAULT_OUT_TRANSACTIONS);

  virtual ~ClientAsyncSession();

  void setMaxInSpeed(uint32 in_speed);

  void setMaxOutSpeed(uint32 out_speed);

  void setSessionInitTime(uint32 session_init_time);

  void setEnquireLinkTime(uint32 enquire_link_time);

  void setInactivityTime(uint32 inactivity_time);

  void setTransactionTime(uint32 transaction_time);

  void setMaxInTrans(uint32 max_in_trans);

  void setMaxOutTrans(uint32 max_out_trans);

  smpp::sessions::SessionStates state() const;

  boost::asio::ip::tcp::endpoint remoteEndpoint() const;

  boost::asio::ip::tcp::endpoint localEndpoint() const;

  uint32 getMaxInSpeed() const;

  uint32 getMaxOutSpeed() const;

  uint32 getNowInSpeed() const;

  uint32 getNowOutSpeed() const;

  uint32 getSessionInitTime() const;

  uint32 getEnquireLinkTime() const;

  uint32 getInactivityTime() const;

  uint32 getTransactionTime() const;

  uint32 getMaxInTrans() const;

  uint32 getMaxOutTrans() const;

  uint32 getOpenedInTrans() const;

  uint32 getOpenedOutTrans() const;

  const boost::system::error_code &getLastError() const;

  void openSession(const boost::asio::ip::tcp::endpoint &ep);

  void closeSession();

  void sendPacket(const HeaderPtr &packet);

// signals

  boost::signals2::connection openedSession(const voidSig::slot_type &slot);

  boost::signals2::connection closedSession(const voidSig::slot_type &slot);

  boost::signals2::connection receivePacket(const headerSig::slot_type &slot);

  boost::signals2::connection outStatus(const statSig::slot_type &slot);

  boost::signals2::connection inStatus(const statSig::slot_type &slot);

  boost::signals2::connection stateChanged(const stateSig::slot_type &slot);

  boost::signals2::connection sessionInitTimeout(
    const voidSig::slot_type &slot);

  boost::signals2::connection enquireLinkTimeout(
    const voidSig::slot_type &slot);

  boost::signals2::connection inactivityTimeout(const voidSig::slot_type &slot);

  boost::signals2::connection error(const errorSig::slot_type &slot);

private:

  friend class ClientAsyncSessionPrivate;

  ClientAsyncSessionPrivate *p;
};

} // namespace sessions

} // namespace smpp

#endif // SMPP_CLIENT_ASYNC_SESSION_H
