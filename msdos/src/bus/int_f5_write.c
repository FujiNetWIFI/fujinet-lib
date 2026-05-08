/**
 * @brief   FujiNet INT F5 Interface - DL = 80 - WRITE Payload to FujiNet
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @param   device Device # (goes into AL)
 * @param   command The Command to send (goes into AH)
 * @param   aux1    Aux parameter 1 (goes into CL)
 * @param   aux2    Aux parameter 2 (goes into CH)
 * @param   buf     Pointer to source buffer
 * @param   len     # of bytes to send from source buffer
 * @return  Complete, Nak or Error  (returned value in AL)
 * @verbose Use this function when the PC needs to send a payload to FujiNet
 */

#include <dos.h>
#include <string.h>

unsigned char int_f5_write(unsigned char dev, unsigned char command, unsigned char aux1, unsigned char aux2, void *buf, unsigned short len)
{
    union REGS r;
    struct SREGS sr;

    memset(&r,0,sizeof(union REGS));
    memset(&sr,0,sizeof(struct SREGS));

    r.h.dl = 0x80;
    r.h.al = dev;
    r.h.ah = command;
    r.h.cl = aux1;
    r.h.ch = aux2;
    r.x.si = 0x00;

    sr.es  = FP_SEG(buf);
    r.x.bx = FP_OFF(buf);
    r.x.di = len;

    int86x(0xF5,&r,&r,&sr);

    return r.h.al;
}
