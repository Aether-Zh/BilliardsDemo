//
//  Header.h
//  ShengJi
//
//  Created by duole_083 on 2020/7/16.
//

#ifndef Header_h
#define Header_h

const unsigned short DATA_T = 1;
const unsigned short CMD_T = 2;

#pragma pack(push, 4)
typedef struct MessageHeader {
    unsigned short type;
    unsigned short length;
} MsgH;
#pragma pack(pop)

#endif /* Header_h */
