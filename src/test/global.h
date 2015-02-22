#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <map>
#include "boost/asio.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "smpp/sessions/smpp_client_async_session.h"

static const std::string host = "10.5.5.10";
static const uint16 port = 6666;
static const int32 console_update = 1000;
static const int32 send_speed = 10;
static const int32 max_iter = 1000000;
static const int32 in_speed = 100000;
static const int32 out_speed = 100000;
static const int32 session_init_time = 60;
static const int32 enquire_link_time = 10;
static const int32 inactivity_time = 180;
static const int32 transaction_time = 60;
static const int32 max_in_trans = 100000;
static const int32 max_out_trans = 100000;

#endif // GLOBAL_H
