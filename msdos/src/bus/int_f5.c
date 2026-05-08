/**
 * @brief   FujiNet INT F5 Interface - DL = 00 - No Payload
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @param   device Device # (goes into AL)
 * @param   command The Command to send (goes into ah)
 * @param   aux1    Aux parameter 1 (goes into cl)
 * @param   aux2    Aux parameter 2 (goes into ch)
 * @return  Complete, Nak or Error  (returned value in AL)
 * @verbose Use this function when a command has no payload to send along with command.
 */

#include <dos.h>
#include <string.h>

unsigned char int_f5(unsigned char dev, unsigned char command, unsigned char aux1, unsigned char aux2)
{
    union REGS r;

    memset(&r,0,sizeof(union REGS));

    r.h.dl = 0x00;
    r.h.al = dev;
    r.h.ah = command;
    r.h.cl = aux1;
    r.h.ch = aux2;
    r.x.si = 0x00;

    int86(0xF5,&r,&r);

    return r.h.al;
}
