int main(void) {
    // pRCGCGPIO is a pointer to the General-Purpose Input/Output Run Mode Clock
    // Gating Control Register (p 340)
    unsigned int volatile *pRCGCGPIO = (unsigned int *) (0x400FE000 + 0x608);
    // pGPIOLOCK_PortE is a pointer to the GPIO Lock register for Port E (p 684)
    unsigned int volatile *pGPIOLOCK_PortE = (unsigned int *)(0x40024000 + 0x520);
    // pGPIOCR_PortE is a pointer to the GPIO Commit register for Port E (p 685)
    unsigned int volatile *pGPIOCR_PortE = (unsigned int *)(0x40024000 + 0x524);
    // pGPIODIR_PortE is a pointer to the GPIO Direction register for Port E (p 663)
    unsigned int volatile *pGPIODIR_PortE = (unsigned int *) (0x40024000 + 0x400);
    // pGPIOAFSEL is a pointer to the GPIO Alternate Function Select register for Port E (p 672)
    unsigned int volatile *pGPIOAFSEL_PortE = (unsigned int *) (0x40024000 + 0x420);
    // pGPIODEN is a pointer to the GPIO Digital Enable register for Port E (p 683)
    unsigned int volatile *pGPIODEN_PortE = (unsigned int *) (0x40024000 + 0x51C);
    // pGPIODATA is a pointer to the GPIO Data register for Port E (p 662)
    unsigned int volatile *pGPIODATA_PortE = (unsigned int *) (0x40024000 + 0x3FC);
    volatile int i; //Add this for the while loop later on

    volatile int switch_position;
    /*
    * Now step through the cook book of steps.
    */
    // Step 1a: Turn on the clocks for Port E.
    //Note: Port E clocks are controlled by bit 4 (R4) of *pRCGCGPIO.
    // In _binary_ this would correspond to a mask of 0b01.0000.
    // In _hex_ this would correspond to a mask of 0x10
    *pRCGCGPIO = *pRCGCGPIO | 0x0010;


    // Step 1b: Check to be sure the clocks have started.
    // This can take a few clock cycles.
    // Keep checking bit 5 until it is no longer 0.
    while ( (*pRCGCGPIO & 0x0010) == 0 ) ; // Good thing this is volatile!


    // Step 2a: Unlock Port E
    // Port E has NMI connected to PF0. The M4 thus has a lock on this.
    // The key to the lock is to write the value 0x4C4F434B to GPIOLOCK.
    *pGPIOLOCK_PortE = 0x4C4F434B;

    // Step 2b: Enable us to commit to all controls in Port E for PE[4:0]
    *pGPIOCR_PortE = *pGPIOCR_PortE | 0x1F;
    // Let's use PE3 to control the off board LED. Therefore it should be
    // an output.


    // Step 3: Set the direction of the pin to be used. The pins
    // can be configured as either input or output.
    // In _binary_ this would correspond to a mask of 0b1000
    // In _hex_ this would correspond to a mask of 0x08;
    *pGPIODIR_PortE = *pGPIODIR_PortE ~0x10;
    *pGPIODIR_PortE = *pGPIODIR_PortE | 0x08;
    // make pin 4 an input by clearing the bit



    // Step 4: Set the pin to be used as a general purpose I/O poin.
    // This means we clear the bit corresponding to PE3.
    *pGPIOAFSEL_PortE = *pGPIOAFSEL_PortE & ~0x18;
    //*pGPIOAFSEL_PortE = *pGPIOAFSEL_PortE & ~0x10;

    // Step 5: Enable the pin
    // Enable pin 3
    *pGPIODR_PortE = *pGPIODR_PortE | 0x10;

    //Enable pin 4
    *pGPIODEN_PortE = *pGPIODEN_PortE | 0x18;

    while(1)
    {
        switch_position = *pGPIODATA_PortE & 0x10;
        switch_position &= 0x10;

        if(switch_position != 0) {
            *pGPIODATA_PortE ^= ~0x08;
        }
        for (int i = 0; i < 200000; i++)
        {

        }

    }
    return 0;

}




