void MemoryTestv1(void)
{
    unsigned int *RamPtr, counter1=1, i;
    register unsigned int testValue=0;
    unsigned int Start, End, last_addr, test_size_b=4;
    short alignValue;
    char c ;

    printf("\r\nStart Address: ") ;
    Start = Get8HexDigits(0) ;
    printf("\r\nEnd Address: ") ;
    End = Get8HexDigits(0) ;

    // align Start and End addresses
    Start = Start + (Start % 2); // make sure starting on an even address
   // it = end - start / size_b;
    last_addr = (End - Start) / test_size_b;
    printf("\r\nFirst Addr Written: %08X", Start);
    printf("\r\nlast_addr: %08x", last_addr);
    // add your code to test memory here using 32 bit reads and writes of data between the start and end of memory
    for(i = 0; i < last_addr; i++)
    {
        RamPtr = Start + i * test_size_b;
        *RamPtr = testValue;
        if (*RamPtr == testValue)
        {
            printf("\r\nPassed @addr: %08x = %08x)" RamPtr, *RamPtr);
            testValue++;
        }
        else
        {
            printf("\r\nFailed at %08X = %08x", RamPtr, *RamPtr);
            go();
        }
    }
    printf("\r\nLast Addr Written: %08X", RamPtr);
    printf("\r\nLast Value Read: %08X", testValue--);
    printf("\r\nPass");
}

void MemoryTestv2(void)
{
    unsigned int *RamPtr, i;
    register unsigned int testValue=0;
    unsigned int Start, End, num_iter, test_size_b=4;
    char c ;

    printf("\r\nStart Address: ") ;
    Start = Get8HexDigits(0) ;
    printf("\r\nEnd Address: ") ;
    End = Get8HexDigits(0) ;

    // align Start and End addresses
    Start = Start + (Start % 2); // make sure starting on an even address
   // it = end - start / size_b;
    num_iter = (End - Start + 1) / test_size_b;
    // add your code to test memory here using 32 bit reads and writes of data between the start and end of memory
    for(i = 0; i < num_iter; i++)
    {
        RamPtr = Start + i * test_size_b;
        *RamPtr = testValue | (unsigned int)((PortB << 8) | (PortA));
        if (*RamPtr == testValue)
        {
            //printf("\r\nPassed @addr: %08x = %08x" RamPtr, *RamPtr);
            testValue++;
        }
        else
        {
            printf("\r\nFailed at %08X = %08x", RamPtr, *RamPtr);
            go();
        }
    }
    printf("\r\nFirst Addr Written To: %08X", Start);
    printf("\r\nnumber of writes: %d", num_iter);
    printf("\r\nLast Addr Written From: %08X", RamPtr);
    printf("\r\nLast Addr Written To: %08X", (unsigned int)RamPtr + 3);
    printf("\r\nLast Value Read: %08X", *RamPtr);
    printf("\r\nPass");
    go();
}

void SwitchTest(void)
{
    int i, switches = 0 ;

    while(1)    {
        switches = (PortB << 8) | (PortA) ;
        printf("\rSwitches SW[15-0] = ") ;
        for( i = (int)(0x00008000); i > 0; i = i >> 1)  {
            if((switches & i) == 0)
                printf("0") ;
            else
                printf("1") ;
        }
    }
}

void TestLEDS(void)
{
    int delay ;
    unsigned char count = 0 ;

    while(1)    {
        PortA = PortB = PortC = PortD = HEX_A = HEX_B = HEX_C = HEX_D = count++ ;
        for(delay = 0; delay < 100000; delay ++)
            ;
    }
}