#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H

#include "global.h"

class TestSmppClient {
  uint32 sequence_number;

  boost::asio::io_service *io;

  smpp::sessions::ClientAsyncSession *session;

  boost::asio::deadline_timer *send_timer;

  boost::asio::deadline_timer *console_update_timer;

  std::map<uint32, uint32> submit_sm_status;

  std::map<uint32, uint32> deliver_sm_status;

public:

  TestSmppClient();

private:

  void initConsoleUpdateTimer();

  void consoleUpdate();

  void initSendTimer();

  void sendMessage();

  void openedSession();

  void closedSession();

  void receivePacket(smpp::sessions::HeaderPtr);

  void outStatus(smpp::sessions::HeaderPtr, smpp::sessions::StatusMessage);

  void inStatus(smpp::sessions::HeaderPtr, smpp::sessions::StatusMessage);

  void stateChanged(smpp::sessions::SessionStates);

  void sessionInitTimeout();

  void enquireLinkTimeout();

  void inactivityTimeout();

  void error(const boost::system::error_code &);
};

#endif // TEST_CLIENT_H
