/**
 * @brief   FujiNet INT F5 Interface - AH = 00 - No Payload
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @param   device Device # (goes into AL)
 * @param   command The Command to send (goes into CL)
 * @param   aux1    Aux parameter 1 (goes into DL)
 * @param   aux2    Aux parameter 2 (goes into DH)
 * @return  Complete, Nak or Error  (returned value in AL)
 * @verbose Use this function when a command has no payload to send along with command.
 */

#include <dos.h>

unsigned char int_f5_ah_00(unsigned char dev, unsigned char command, unsigned char aux1, unsigned char aux2)
{
    union REGS r;
    
    r.h.ah = 0x00;
    r.h.al = dev;
    r.h.cl = command;
    r.h.dl = aux1;
    r.h.dh = aux2;

    int86(0xF5,&r,&r);

    return r.h.al;
}
