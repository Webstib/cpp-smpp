#ifndef SMPP_PARAMETERS_HEADER_H
#define SMPP_PARAMETERS_HEADER_H

#include "smpp/smpp_global.h"

namespace smpp {

namespace parameters {

namespace command_id {
  static const uint32 GENERIC_NACK          = 0x80000000;
  static const uint32 BIND_RECEIVER         = 0x00000001;
  static const uint32 BIND_RECEIVER_RESP    = 0x80000001;
  static const uint32 BIND_TRANSMITTER      = 0x00000002;
  static const uint32 BIND_TRANSMITTER_RESP = 0x80000002;
  static const uint32 QUERY_SM              = 0x00000003;
  static const uint32 QUERY_SM_RESP         = 0x80000003;
  static const uint32 SUBMIT_SM             = 0x00000004;
  static const uint32 SUBMIT_SM_RESP        = 0x80000004;
  static const uint32 DELIVER_SM            = 0x00000005;
  static const uint32 DELIVER_SM_RESP       = 0x80000005;
  static const uint32 UNBIND                = 0x00000006;
  static const uint32 UNBIND_RESP           = 0x80000006;
  static const uint32 REPLACE_SM            = 0x00000007;
  static const uint32 REPLACE_SM_RESP       = 0x80000007;
  static const uint32 CANCEL_SM             = 0x00000008;
  static const uint32 CANCEL_SM_RESP        = 0x80000008;
  static const uint32 BIND_TRANSCEIVER      = 0x00000009;
  static const uint32 BIND_TRANSCEIVER_RESP = 0x80000009;
  static const uint32 OUTBIND               = 0x0000000B;
  static const uint32 ENQUIRE_LINK          = 0x00000015;
  static const uint32 ENQUIRE_LINK_RESP     = 0x80000015;
  static const uint32 SUBMIT_MULTI          = 0x00000021;
  static const uint32 SUBMIT_MULTI_RESP     = 0x80000021;
  static const uint32 ALERT_NOTIFICATION    = 0x00000102;
  static const uint32 DATA_SM               = 0x00000103;
  static const uint32 DATA_SM_RESP          = 0x80000103;
}

namespace command_status {
  static const uint32 ESME_ROK              = 0x00000000; // No Error
  static const uint32 ESME_RINVMSGLEN       = 0x00000001; // Message Length is invalid
  static const uint32 ESME_RINVCMDLEN       = 0x00000002; // Command Length is invalid
  static const uint32 ESME_RINVCMDID        = 0x00000003; // Invalid Command ID
  static const uint32 ESME_RINVBNDSTS       = 0x00000004; // Incorrect BIND Status for given command
  static const uint32 ESME_RALYBND          = 0x00000005; // ESME Already in Bound State
  static const uint32 ESME_RINVPRTFLG       = 0x00000006; // Invalid Priority Flag
  static const uint32 ESME_RINVREGDLVFLG    = 0x00000007; // Invalid Registered Delivery Flag
  static const uint32 ESME_RSYSERR          = 0x00000008; // System Error
  //                   Reserved                0x00000009  // Reserved
  static const uint32 ESME_RINVSRCADR       = 0x0000000A; // Invalid Source Address
  static const uint32 ESME_RINVDSTADR       = 0x0000000B; // Invalid Dest Addr
  static const uint32 ESME_RINVMSGID        = 0x0000000C; // Message ID is invalid
  static const uint32 ESME_RBINDFAIL        = 0x0000000D; // Bind Failed
  static const uint32 ESME_RINVPASWD        = 0x0000000E; // Invalid Password
  static const uint32 ESME_RINVSYSID        = 0x0000000F; // Invalid System ID
  //                   Reserved                0x00000010  // Reserved
  static const uint32 ESME_RCANCELFAIL      = 0x00000011; // Cancel SM Failed
  //                   Reserved                0x00000012  // Reserved
  static const uint32 ESME_RREPLACEFAIL     = 0x00000013; // Replace SM Failed
  static const uint32 ESME_RMSGQFUL         = 0x00000014; // Message Queue Full
  static const uint32 ESME_RINVSERTYP       = 0x00000015; // Invalid Service Type
  //                   Reserved                0x00000016-
  //                                           0x00000032  // Reserved
  static const uint32 ESME_RINVNUMDESTS     = 0x00000033; // Invalid number of destinations
  static const uint32 ESME_RINVDLNAME       = 0x00000034; // Invalid Distribution List name
  //                   Reserved                0x00000035-
  //                                           0x0000003F  // Reserved
  static const uint32 ESME_RINVDESTFLAG     = 0x00000040; // Destination flag is invalid (submit_multi)
  //                   Reserved                0x00000041  // Reserved
  static const uint32 ESME_RINVSUBREP       = 0x00000042; // Invalid ‘submit with replace’ request (i.e.  submit_smwith replace_if_present_flagset)
  static const uint32 ESME_RINVESMCLASS     = 0x00000043; // Invalid esm_classfield data
  static const uint32 ESME_RCNTSUBDL        = 0x00000044; // Cannot Submit to Distribution List
  static const uint32 ESME_RSUBMITFAIL      = 0x00000045; // submit_sm or submit_multi failed
  //                   Reserved                0x00000046-
  //                                           0x00000047  // Reserved
  static const uint32 ESME_RINVSRCTON       = 0x00000048; // Invalid Source address TON
  static const uint32 ESME_RINVSRCNPI       = 0x00000049; // Invalid Source address NPI
  static const uint32 ESME_RINVDSTTON       = 0x00000050; // Invalid Destination address TON
  static const uint32 ESME_RINVDSTNPI       = 0x00000051; // Invalid Destination address NPI
  //                   Reserved                0x00000052  // Reserved
  static const uint32 ESME_RINVSYSTYP       = 0x00000053; // Invalid system_typefield
  static const uint32 ESME_RINVREPFLAG      = 0x00000054; // Invalid replace_if_present flag
  static const uint32 ESME_RINVNUMMSGS      = 0x00000055; // Invalid number of messages
  //                   Reserved                0x00000056-
  //                                           0x00000057  // Reserved
  static const uint32 ESME_RTHROTTLED       = 0x00000058; // Throttling error (ESME has exceeded allowed message limits)
  //                   Reserved                0x00000059-
  //                                           0x00000060  // Reserved
  static const uint32 ESME_RINVSCHED        = 0x00000061; // Invalid Scheduled Delivery Time
  static const uint32 ESME_RINVEXPIRY       = 0x00000062; // Invalid message validity period (Expiry time)
  static const uint32 ESME_RINVDFTMSGID     = 0x00000063; // Predefined Message Invalid or Not Found
  static const uint32 ESME_RX_T_APPN        = 0x00000064; // ESME Receiver Temporary App Error Code
  static const uint32 ESME_RX_P_APPN        = 0x00000065; // ESME Receiver Permanent App Error Code
  static const uint32 ESME_RX_R_APPN        = 0x00000066; // ESME Receiver Reject Message Error Code
  static const uint32 ESME_RQUERYFAIL       = 0x00000067; // query_smrequest failed
  //                   Reserved                0x00000068-
  //                                           0x000000BF  // Reserved
  static const uint32 ESME_RINVOPTPARSTREAM = 0x000000C0; // Error in the optional part of the PDU Body.
  static const uint32 ESME_ROPTPARNOTALLWD  = 0x000000C1; // Optional Parameter not allowed
  static const uint32 ESME_RINVPARLEN       = 0x000000C2; // Invalid Parameter Length.
  static const uint32 ESME_RMISSINGOPTPARAM = 0x000000C3; // Expected Optional Parameter missing
  static const uint32 ESME_RINVOPTPARAMVAL  = 0x000000C4; // Invalid Optional Parameter Value
  //                   Reserved                0x000000C5-
  //                                           0x000000FD  // Reserved
  static const uint32 ESME_RDELIVERYFAILURE = 0x000000FE; // Delivery Failure (used for data_sm_resp)
  static const uint32 ESME_RUNKNOWNERR      = 0x000000FF; // Unknown Error
  //                   Reserved for            0x00000100-
  //                   SMPP extension          0x000003FF  // Reserved for SMPP
  //                   Reserved for SMSC       0x00000400- // extension
  //                   vendor specific errors  0x000004FF  // Reserved for SMSC
  //                   Reserved                0x00000500- // vendor specific errors
  //                                           0xFFFFFFFF  // Reserved
}

namespace sequence_number {
  static const uint32 MIN = 0x00000000;
  static const uint32 MAX = 0x7FFFFFFF;
}

} // namespace parameters

} // namespace smpp

#endif // SMPP_PARAMETERS_HEADER_H
