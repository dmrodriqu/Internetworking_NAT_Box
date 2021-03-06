/* icmpv6.h - definintions for the Internet Control Message Protocol */

/*
 * must be preceded by an ipv6 extension header
 *
 * The structure for an ICMP message:
 *
 * The IPv6 header contains source and destination
 * information as well as information for an extension
 * header. This extension header contains the code, 58.
 * The IP header functions as the pseudo header for the
 * ICMPv6 header. The header for ICMPv6 contains the type,
 * code, and checksum. The payload then follows.
 *
 * Source: RFC 4443. Pg 3.
 * RFC 4861 pg 18-18.
 *
 * */

// check icmpsize
// make correct size
// network byte order
//
#define  IPLEN 16

#define ECHOREQSIZE     sizeof(icmpv6echoreq)
#define PSEUDOLEN       sizeof(pseudoHdr)
#define RSOLSIZE        sizeof(rsolicit)
#define RADVERTSIZE     sizeof(radvert)
#define NADSIZE         sizeof(nadvert)
#define NSOLSIZE        sizeof(nsolicit)
#define MAX_PAYLOAD     1472
#define DESTUNREACH     1
#define PKT_BIG         2
#define TIME_EX         3
#define PVT1            100
#define PVT2            100

/* ICMP Msg types */

#define ECHOREQ     0x80    //translates to 128
#define ECHORESP    0x81    //translates to 129
#define ROUTERS     0x85    //translates to 133
#define ROUTERA     0x86
#define NEIGHBS     0x87
#define NEIGHBA     0x88
#define NEIGHBASOLI     0x63

/*informational messages */
#define PVT3        200
#define PVT4        201


typedef struct idk{//TODO: Ask dylan what to name this
    uint32 type;
    uint32 length;
    uint32 prefixlength;
    uint32 L:1;
    uint32 A:1;
    uint32 res:6;
    uint32 validlifetime;
    uint32 preferredlifetime;
    byte res2[8];
    byte prefix[16];
}idk;

typedef struct lladdress{
    uint32 type;
    uint32 length;
    byte address[16];
}lladdress;

typedef struct icmpopt{
    byte type;
    byte length;
    byte payload[0];//length];TODO: what's length here dylan?
}icmpopt;

typedef struct icmpv6general{
    byte type;
    byte code;
    uint16 checksum;
    byte body[0]; //gives easy access to message body
}icmpv6general;

typedef struct rsolicit{
    byte type;
    byte code;
    uint16 checksum;
    uint32 reserved;
    byte opt[0];//easy access to options
}rsolicit;

typedef struct radvert{
    byte type;
    byte code;
    uint16 checksum;
    uint8  curhoplim;
    //byte M: 1;
    //byte O: 1;
    //byte reserved: 6;
    byte m_o_res;
    uint16 routerlifetime;
    uint32 reachabletime;
    uint32 retranstimer;
    //icmpopt opt;
}radvert;

extern struct radvert radvert_from_router;
// option of source link addr
typedef struct option_one{
    byte type; // should be 1
    byte length;
    byte payload[6];
}option_one;

// option of MTU
typedef struct option_MTU{
    byte type; // shoule be 5
    byte length; // should be 1
    byte reserved[4]; // all zeros
    uint16 payload;  // MTU
}option_MTU;

// option of prefix
typedef struct option_prefix{
    byte type; // should be 0x03;
    byte length; // shoule be 0x04;
    byte prefix_length; // most time it is 64 bits prefix
    byte LA_reserved;
    byte valid_lifetime[4];
    byte preferred_lifetime[4];
    byte reserved[4]; // should be zeros
    byte payload[16];  // for 64 bite prefix;
}option_prefix;

extern struct option_prefix option_prefix_default; //prefix from router
extern struct option_prefix option_prefix_oth1; //prefix for oth1
extern struct option_prefix option_prefix_oth2; //prefix for oth2

typedef struct nsolicit{
    byte type;
    byte code;
    uint16 checksum;
    byte reserved[4];
    byte target[IPV6_ASIZE];
    byte opt[0];//easy access to options
}nsolicit;

typedef struct nadvert{
    byte type;
    byte code;
    uint16 checksum;
    byte mosreserved[4];
    byte target[IPV6_ASIZE];
    byte opt[0];//easy access to options
}nadvert;


typedef struct pseudoHdr{
    byte    src[IPV6_ASIZE];
    byte    dest[IPV6_ASIZE];
    uint32  len;
    byte    zero[3];
    byte    next_header;
    byte    icmppayload[0];
}pseudoHdr;

typedef struct icmpv6echoreq{
    byte type;
    byte code;
    uint16 checksum;
    uint16 identifier;
    uint16 seqNumber;
    byte body[0]; //gives easy access to message body
} icmpv6echoreq;


