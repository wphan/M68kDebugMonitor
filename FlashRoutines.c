#include "DebugMonitor.h"

/* erase chip by writing to address with data*/
void EraseFlashChip(void)
{
    unsigned char *FlashPtr = FlashStart;//(unsigned char*)FlashStart;

    FlashPtr[0xAAA << 1] = 0xAA;
    FlashPtr[0x555 << 1] = 0x55;
    FlashPtr[0xAAA << 1] = 0x80;
    FlashPtr[0xAAA << 1] = 0xAA;
    FlashPtr[0x555 << 1] = 0x55;
    FlashPtr[0xAAA << 1] = 0x10;

    // poll 
    PortD = 1;
    while ((FlashPtr[0] & 0x80) == 0)  // Poll DQ7 for erase 
    {
        PortD = 2;
    }
    PortD = 1;
}

void FlashReset(void)
{
    unsigned char *FlashPtr = (unsigned char*)FlashStart;
    FlashPtr[0xAAA] = 0xF0;
}

/* erase sector by writing to address with data*/
void FlashSectorErase(int SectorAddress)
{
    unsigned char *FlashPtr = (unsigned char*)FlashStart;
    // For sectors 0-7, Shift 12 for 0 -> 12, then shift 1 for byte logic, then shift 1 for sector address mapping.
    unsigned int SectorAddressStart =  SectorAddress << 14; //(SectorAddress < 8) ? ((SectorAddress) << 14) : ((SectorAddress - 7) << 17);

    printf("\r\nStart Address: %i, %x",SectorAddressStart,SectorAddressStart);
  //  SectorAddress = (SectorAddress < 8) ?  SectorAddress : SectorAddress;
    {
        FlashPtr[0xAAA << 1] = 0xAA;
        FlashPtr[0x555 << 1] = 0x55;
        FlashPtr[0xAAA << 1] = 0x80;
        FlashPtr[0xAAA << 1] = 0xAA;
        FlashPtr[0x555 << 1] = 0x55;
        FlashPtr[SectorAddressStart] = 0x30;

        // Poll 
        PortD = 1; // turn on LEDG0, turn off LEDG1
        while ((FlashPtr[SectorAddressStart] & 0x80) == 0)  // poll DQ7
        {
            PortD = 2; // turn on LEDG1, turn off LEDG0
        }
        PortD = 1; // turn on LEDG0, turn off LEDG1
    }
}


/* program chip by writing to address with data*/
void FlashProgram(unsigned int AddressOffset, int ByteData)		// write a byte to the specified address (assumed it has been erased first)
{
    unsigned char *FlashPtr = FlashStart;
    unsigned int readData;

    FlashPtr[0xAAA << 1] = 0xAA;
    FlashPtr[0x555 << 1] = 0x55;
    FlashPtr[0xAAA << 1] = 0xA0;
    FlashPtr[AddressOffset << 1] = ByteData;

    // Poll
    PortD = 1; // turn on LEDG0, turn off LEDG1
    do // check DQ5
    {
        readData = FlashRead(AddressOffset); 

        if ((readData & 0x80) == (ByteData & 0x80))
        {
            goto PROG_PASS;
        }
        PortD = 2; // turn on LEDG1, turn off LEDG0
        //printf("\r\nreadData:%x  ByteData:%x", readData, ByteData);
    } while ((readData & 0x20) == 0);

    readData = FlashRead(AddressOffset);
    if ((readData & 0x80) != (ByteData & 0x80))
    {
        goto PROG_PASS;
    }
    else
    {
        PortD |= 0x4;
        printf("\r\nHow the fuck did you fail");
    }

PROG_PASS:
    PortD = 1; // turn on LEDG0, turn off LEDG1
    //printf("\r\nWrote to address: %x", AddressOffset);
}

/* program chip to read a byte */
unsigned char FlashRead(unsigned int AddressOffset)		// read a byte from the specified address (assumed it has been erased first)
{
    unsigned char *FlashPtr = FlashStart;

	return FlashPtr[AddressOffset << 1];
}