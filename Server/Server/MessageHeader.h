//
//  Header.h
//  ShengJi
//
//  Created by duole_083 on 2020/7/16.
//

#ifndef Header_h
#define Header_h

const unsigned short DATA_TYPE = 0;
const unsigned short CREATE_CMD_TYPE = 1;
const unsigned short RM_CMD_TYPE = 2;
const unsigned short MOVE_CMD_TYPE = 3;

#pragma pack(push, 4)
typedef struct MessageHeader {
    unsigned short type;
    unsigned short length;
} MsgH;
#pragma pack(pop)

using Msg = std::pair<MsgH, std::string>;
using Msgs = std::vector<Msg>;

#endif /* Header_h */
