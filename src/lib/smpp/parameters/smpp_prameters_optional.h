#ifndef SMPP_PARAMETERS_OPTIONAL_H
#define SMPP_PARAMETERS_OPTIONAL_H

#include "smpp/smpp_global.h"

namespace smpp {

namespace parameters {

namespace tag {
  static const uint16 DEST_ADDR_SUBUNIT           = 0x0005;
  static const uint16 DEST_NETWORK_TYPE           = 0x0006;
  static const uint16 DEST_BEARER_TYPE            = 0x0007;
  static const uint16 DEST_TELEMATICS_ID          = 0x0008;
  static const uint16 SOURCE_ADDRSUBUINIT         = 0x000D;
  static const uint16 SOURCE_NETWORK_TYPE         = 0x000E;
  static const uint16 SOURCE_BEARER_TYPE          = 0x000F;
  static const uint16 SOURCE_TELEMATICS_ID        = 0x0010;
  static const uint16 QOS_TIME_TO_LIVE            = 0x0017;
  static const uint16 PAYLOAD_TYPE                = 0x0019;
  static const uint16 ADDITIONAL_STATUS_INFO_TEXT = 0x001D;
  static const uint16 RECEIPTED_MESSAGE_ID        = 0x001E;
  static const uint16 MS_MSG_WAIT_FACILITIES      = 0x0030;
  static const uint16 PRIVACY_INDICATOR           = 0x0201;
  static const uint16 SOURCE_SUBADDRESS           = 0x0202;
  static const uint16 DEST_SUBADDRESS             = 0x0203;
  static const uint16 USER_MESSAGE_REFERENCE      = 0x0204;
  static const uint16 USER_RESPONSE_CODE          = 0x0205;
  static const uint16 SOURCE_PORT                 = 0x020A;
  static const uint16 DESTINATION_PORT            = 0x020B;
  static const uint16 SAR_MSG_REF_NUM             = 0x020C;
  static const uint16 LANGUAGE_INDICATOR          = 0x020D;
  static const uint16 SAR_TOTAL_SEGMENTS          = 0x020E;
  static const uint16 SAR_SEGMENT_SEQNUM          = 0x020F;
  static const uint16 SC_INTERFACE_VERSION        = 0x0210;
  static const uint16 CALLBACK_NUM_PRES_IND       = 0x0302;
  static const uint16 CALLBACK_NUM_ATAG           = 0x0303;
  static const uint16 NUMBER_OF_MESSAGES          = 0x0304;
  static const uint16 CALLBACK_NUM                = 0x0381;
  static const uint16 DPF_RESULT                  = 0x0420;
  static const uint16 SET_DPF                     = 0x0421;
  static const uint16 MS_AVAILABILITY_STATUS      = 0x0422;
  static const uint16 NETWORK_ERROR_CODE          = 0x0423;
  static const uint16 MESSAGE_PAYLOAD             = 0x0424;
  static const uint16 DELIVERY_FAILURE_REASON     = 0x0425;
  static const uint16 MORE_MESSAGES_TO_SEND       = 0x0426;
  static const uint16 MESSAGE_STATE               = 0x0427;
  static const uint16 CONGESTION_STATE            = 0x0428;
  static const uint16 USSD_SERVICE_OP             = 0x0501;
  static const uint16 DISPLAY_TIME                = 0x1201;
  static const uint16 SMS_SIGNAL                  = 0x1203;
  static const uint16 MS_VALIDITY                 = 0x1204;
  static const uint16 ALERT_ON_MESSAGE_DELIVERY   = 0x130C;
  static const uint16 ITS_REPLY_TYPE              = 0x1380;
  static const uint16 ITS_SESSION_INFO            = 0x1383;
}

namespace additional_status_info_text {
  static const uint32 MAX_LENGTH = 256;
}

namespace callback_num {
  static const uint32 MIN_LENGTH = 4;
  static const uint32 MAX_LENGTH = 19;
}

namespace callback_num_atag {
  static const uint32 MAX_LENGTH = 65;
}

namespace delivery_failure_reason {
  static const uint8 UNAVAILABLE          = 0x00; // Destination unavailable
  static const uint8 INVALID              = 0x01; // Destination Address Invalid (e.g. suspended, no SMS capability, etc.)
  static const uint8 PREMANENT_NET_ERROR  = 0x02; // Permanent network error
  static const uint8 TEMPRORARY_NET_ERROR = 0x03; // Temporary network error
}

namespace addr_subunit {
  static const uint8 UNKNOWN          = 0x00; // Unknown (default)
  static const uint8 MS_DISPLAY       = 0x01; // MS Display
  static const uint8 MODILE_EQUIPMENT = 0x02; // Mobile Equipment
  static const uint8 SMART_CARD_1     = 0x03; // Smart Card 1 (expected to be a SIM exists in the MS)
  static const uint8 EXTERNAL_UNIT_1  = 0x04; // External Unit 1
}

namespace bearer_type {
  static const uint8 UNKNOWN     = 0x00; // Unknown
  static const uint8 SMS         = 0x01; // SMS
  static const uint8 CSD         = 0x02; // Circuit Switched Data (CSD)
  static const uint8 PACKET_DATA = 0x03; // Packet Data
  static const uint8 USSD        = 0x04; // USSD
  static const uint8 CDPD        = 0x05; // CDPD
  static const uint8 DATA_TAC    = 0x06; // DataTAC
  static const uint8 FLEX        = 0x07; // FLEX/ReFLEX
  static const uint8 CELLCAST    = 0x08; // Cell Broadcast (cellcast)
}

namespace network_type {
  static const uint8 UNKNOWN        = 0x00; // Unknown
  static const uint8 GSM            = 0x01; // GSM
  static const uint8 ANSI136        = 0x02; // ANSI-136/TDMA
  static const uint8 IS95           = 0x03; // IS-95/CDMA
  static const uint8 PDC            = 0x04; // PDC
  static const uint8 PHS            = 0x05; // PHS
  static const uint8 IDEN           = 0x06; // iDEN
  static const uint8 AMPS           = 0x07; // AMPS
  static const uint8 PAGING_NETWORK = 0x08; // Paging Network
}

namespace subaddress {
  static const uint32 MIN_LENGTH = 2;
  static const uint32 MAX_LENGTH = 23;
}

namespace display_time {
  static const uint8 TEMPORARY = 0x00; // Temporary
  static const uint8 DEFAULT   = 0x01; // Default (default)
  static const uint8 INVOKE    = 0x02; // Invoke
}

namespace dpf_result {
  static const uint8 DPF_NOT_SET = 0x00; // DPF not set
  static const uint8 DPF_SET     = 0x01; // DPF set
}

namespace its_reply_type {
  static const uint8 DIGIT          = 0x00;
  static const uint8 NUMBER         = 0x01;
  static const uint8 TELEPHONE_NO   = 0x02;
  static const uint8 PASSWORD       = 0x03;
  static const uint8 CHARACTER_LINE = 0x04;
  static const uint8 MENU           = 0x05;
  static const uint8 DATE           = 0x06;
  static const uint8 TIME           = 0x07;
  static const uint8 CONTINUE       = 0x08;
}

namespace language_indicator {
  static const uint8 UNSPECIFIED = 0x00; // unspecified (default)
  static const uint8 ENGLISH     = 0x01; // english
  static const uint8 FRENCH      = 0x02; // french
  static const uint8 SPANISH     = 0x03; // spanish
  static const uint8 GERMAN      = 0x04; // german
  static const uint8 PORTUGUESE  = 0x05; // Portuguese
}

namespace more_messages_to_send {
  static const uint8 NO_MORE = 0x00; // No more messages to follow
  static const uint8 MORE    = 0x01; // More messages to follow (default)
}

namespace ms_availability_status {
  static const uint8 AVAILABLE   = 0x00; // Available (Default)
  static const uint8 DENIED      = 0x01; // Denied (e.g. suspended, no SMS capability, etc.)
  static const uint8 UNAVAILABLE = 0x02; // Unavailable
}

namespace ms_validity {
  static const uint8 STORE_INDEFINITELY = 0x00; // Store Indefinitely (default)
  static const uint8 POWER_DOWN         = 0x01; // Power Down
  static const uint8 SID_BRA            = 0x02; // SID based registration area
  static const uint8 DISPLAY_ONLY       = 0x03; // Display Only
}

namespace network_error_code {
  static const uint8 ANSI136  = 0x00; // ANSI-136
  static const uint8 IS95     = 0x01; // IS-95
  static const uint8 GSM      = 0x02; // GSM
  static const uint8 RESERVED = 0x02; // Reserved
}

namespace payload_type {
  static const uint8 DFAULT = 0x00; // Default. In the case of a WAP
                                     // application, the default higher layer
                                     // message type is a WDP message.
                                     // See [WDP] for details.
  static const uint8 WCMP   = 0x01; // WCMP message. Wireless Control
                                     // Message Protocol formatted data.
                                     // See [WCMP] for details.
}

namespace privacy_indicator {
  static const uint8 LEVEL0 = 0x00; // Privacy Level 0 (Not Restricted) (default)
  static const uint8 LEVEL1 = 0x01; // Privacy Level 1 (Restricted)
  static const uint8 LEVEL2 = 0x02; // Privacy Level 2 (Confidential)
  static const uint8 LEVEL3 = 0x03; // Privacy Level 3 (Secret)
}

namespace set_dpf {
  static const uint8 DPF_NOT_REQUEST = 0x00; // Setting of DPF for delivery failure to MS not requested
  static const uint8 DPF_REQUEST     = 0x01; // Setting of DPF for delivery failure requested (default)
}

namespace ussd_service_op {
  static const uint8 PSSD_IND  = 0x00; // PSSD indication
  static const uint8 PSSR_IND  = 0x01; // PSSR indication
  static const uint8 USSR_REQ  = 0x02; // USSR request
  static const uint8 USSN_REQ  = 0x03; // USSN request
  // 4 to 15  = reserved
  static const uint8 PSSD_RESP = 0x10; // PSSD response
  static const uint8 PSSR_RESP = 0x11; // PSSR response
  static const uint8 USSR_CONF = 0x12; // USSR confirm
  static const uint8 USSN_CONF = 0x13; // USSN confirm
  // 20 to 31  = reserved
  // 32 to 255 = reserved for vendor s USSD operations
}

} // namespace parameters

} // namespace smpp

#endif // SMPP_PARAMETERS_OPTIONAL_H
