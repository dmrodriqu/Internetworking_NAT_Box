#include <xinu.h>

void printPacket2(struct netpacket * );
//int32 charToHex(byte *, char *);



void    fillEthernet(struct netpacket *);
void    fillIPdatagram(struct base_header *);

//uint16	icmp_cksum (
//		struct	netpacket *pkt	/* Packet buffer pointer	*/
//		)
//{
//	uint32	cksum;	/* 32-bit sum of all shorts	*/
//	uint16	*ptr16;	/* Pointer to a short		*/
//	int32	i;	/* Index variable		*/

	/* This is the pseudo header */
/*	#pragma pack(1)
	struct	{
		byte	ipsrc[16];
		byte	ipdst[16];
		uint32	icmplen;
		byte	zero[3];
		byte	ipnh;
	} pseudo;
	#pragma pack()
*/
	/* Initialize the pseudo header */
/*	memset(&pseudo, 0, sizeof(pseudo));
	memcpy(pseudo.ipsrc, pkt->net_ipsrc, 16);
	memcpy(pseudo.ipdst, pkt->net_ipdst, 16);
	pseudo.icmplen = htonl(pkt->net_iplen);
	pseudo.ipnh = IP_ICMP;

	cksum = 0;
	ptr16 = (uint16 *)&pseudo;
*/
	/* First add all shorts in the pseudo header */
/*	for(i = 0; i < sizeof(pseudo); i = i + 2) {
		cksum += htons(*ptr16);
		ptr16++;
	}

	ptr16 = (uint16 *)pkt->net_ipdata;
*/
	/* Add all the shorts in the ICMP packet */
/*	for(i = 0; i < pkt->net_iplen; i = i + 2) {
		cksum += htons(*ptr16);
		ptr16++;
	}

	cksum = (uint16)cksum + (cksum >> 16);

	return (uint16)~cksum;
}*/



void    fillICMP(struct rsolicit * pkt) {
    kprintf("Filling icmp hdr\n");
    pkt->type = ROUTERS;
    //assumed code, checksum, and reserved were all set to 0 before coming in
    //but settting it again just in case
    pkt->code = 0;
    pkt->checksum = 0;
    pkt->reserved = 0;
    //void * pseudo = makePseudoHdr(pkt);
    uint16 sum = checksumv6(pkt, PSEUDOLEN);
    pkt->checksum = htons(sum);
    kprintf("cksum: %d\n", sum);
    //freemem(pseudo, PSEUDOLEN);
}

status  sendipv6pkt() {//byte[] destination, uint16 message) {
    struct netpacket * packet;
    if (ipv6bootstrap) {
        kprintf("bootstrapping for ipv6\n");

        //uint32 len = ETH_HDR_LEN + IPV6_HDR_LEN + ICMPSIZE;
        packet = (struct netpacket *) getmem(PACKLEN);//change this 
        memset((char *) packet, NULLCH, PACKLEN);

        fillEthernet(packet);
        fillIPdatagram((struct base_header *) ((char *) packet + ETH_HDR_LEN));
        fillICMP((struct rsolicit *) ((char *) packet + ETH_HDR_LEN + IPV6_HDR_LEN));

        if( write(ETHER0, (char *) packet, PACKLEN) == SYSERR) {
            kprintf("THE WORLD IS BURNING\n");
        }
        printPacket2(packet);
        ipv6bootstrap = 0;
        //TODO: figure out if packet buffer has to be freed
        //freebuf((char *) packet);
    }
    else {
        //normal packet sending
        packet = (struct netpacket *) getbuf(PACKLEN);
    }
    //printPacket(packet);
    return 0;
}

void    fillEthernet(struct netpacket * pkt) {
    kprintf("Filling ether hdr\n");
     
    pkt->net_dst[0] = 0xff;
    pkt->net_dst[1] = 0xff;
    pkt->net_dst[2] = 0xff;
    pkt->net_dst[3] = 0xff;
    pkt->net_dst[4] = 0xff;
    pkt->net_dst[5] = 0xff;

  /*
    pkt->net_dst[0] = 0x33;
    pkt->net_dst[1] = 0x33;
    pkt->net_dst[2] = 0x00;
    pkt->net_dst[3] = 0x00;
    pkt->net_dst[4] = 0x00;
    pkt->net_dst[5] = 0x02;
*/
    memcpy(&pkt->net_src, if_tab[ifprime].if_macucast, ETH_ADDR_LEN);
    pkt->net_type = htons(ETH_IPv6);
    //pkt->net_ethcrc = 0;
    //pkt->net_iface = 0;

}

void    fillIPdatagram(struct base_header * pkt) {
    kprintf("Filling ip hdr\n");
    pkt->info[0] = 0x60;
    pkt->info[1] = 0x00;
    pkt->info[2] = 0x00;
    pkt->info[3] = 0x00;
    pkt->payload_len = htons(ICMPSIZE);
    pkt->next_header = IPV6_ICMP;
    pkt->hop_limit = 255;
    //memcpy(&pkt->src, link_local, IPV6_ASIZE);
    pkt->dest[0] = 0xff;
    pkt->dest[1] = 0x02;
    pkt->dest[15] = 0x02;

}


/*
   int32 charToHex(byte * buf, char * string) {

   while (*string != 0) {
   printf("char: %c\n", *string);
   if (*string == ':') {
   string++;
   continue;
   }
   if (*string >= '0' && *string <= '9')
 *buf = (*string - '0') << 4;
 else if (*string >= 'A' && *string <= 'Z')
 *buf = (*string - 'A' + 10) << 4;
 else if (*string >= 'a' && *string <= 'z')
 *buf = (*string - 'a' + 10) << 4;
 else
 return 1;

 string++;

 if (*string >= '0' && *string <= '9')
 *buf |= *string - '0';
 else if (*string >= 'A' && *string <= 'Z')
 *buf |= *string - 'A' + 10;
 else if (*string >= 'a' && *string <= 'z')
 *buf |= *string - 'a' + 10;
 else
 return 1;

 string++;
 buf++;
 }
 return 0;
 }
 */
void printPacket2(struct netpacket * packet) {
    printf("Printing packet...\n");
    printf("Dest: %02x:%02x:%02x:%02x:%02x:%02x\n",
            packet->net_dst[0], 0xff & packet->net_dst[1], 0xff & packet->net_dst[2],
            0xff & packet->net_dst[3], 0xff & packet->net_dst[4], 0xff & packet->net_dst[5]);
    printf("Source: %02x:%02x:%02x:%02x:%02x:%02x\n",
            packet->net_src[0], 0xff & packet->net_src[1], 0xff & packet->net_src[2],
            0xff & packet->net_src[3], 0xff & packet->net_src[4], 0xff & packet->net_src[5]);
    printf("Type: 0x%04x\n", ntohs(packet->net_type));
    printf("Payload: %s\n", packet->net_payload);
}