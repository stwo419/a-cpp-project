#include"protocol.h"
#include<cstring>

PDU* mkPDU(uint uiMsgLen)
{
    uint uiPDULen=sizeof(PDU)+uiMsgLen;
    PDU* pdu = (PDU*)malloc(uiPDULen);
    memset(pdu->caData,'\0',64);
    pdu->uiPDULen=uiPDULen;
    pdu->uiMsgLen=uiMsgLen;
    memset(pdu->caMsg,'\0',pdu->uiMsgLen);
    return pdu;
}
