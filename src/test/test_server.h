#ifndef TEST_SERVER_H
#define TEST_SERVER_H

#include <QtNetwork/QTcpServer>
#include "smpp/sessions/smpp_server_async_session.h"
#include "test_client.h"

class TestServer : public QTcpServer {
  Q_OBJECT

public:

  explicit TestServer(QObject *parent = 0);

public slots:

  void start();

  void openSession();

  void closeSession();

  void errorSession(smpp::sessions::ErrorCode code);

  void bind(const smpp::pdu::BindTransceiver &p);

  void unbind(const smpp::pdu::Unbind &p);

  void submitSm(const smpp::pdu::SubmitSm &p);

  void deliverSmResp(const smpp::pdu::DeliverSmResp &p);

protected:

  void incomingConnection(int sock);

  void timerEvent(QTimerEvent *event);

private:

  int timer_id;

  smpp::sessions::ServerAsyncSession *session;

  TestClient *test_client;
};

#endif // TEST_SERVER_H
