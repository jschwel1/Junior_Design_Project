#include <avr/io.h>
#include <avr/interrupt.h>
#include "shift_out.h"


void initializeShiftReg(){
	// Setup all DDRs
	#ifdef USING_OUTPUT_ENABLE
		OUTPUT_ENABLE_PORT_DDR |= (1<<OUTPUT_ENABLE);
	#endif
	#ifdef USING_SH_REG_CLR
		SH_REG_CLR_PORT_DDR |= (1 << SH_REG_CLR);
	#endif
	STOR_REG_CLK_PORT_DDR |= (1 << STOR_REG_CLK);
	SER_DATA_PORT_DDR |= (1 << SER_DATA);
	CLK_PORT_DDR |= (1 << CLK);

	// Clear all outputs

	#ifdef USING_OUTPUT_ENABLE
		OUTPUT_ENABLE_PORT |= (1 << OUTPUT_ENABLE); // Disable output
	#endif
	#ifdef USING_STOR_REG_CLR
		SH_REG_CLR_PORT &= ~(1 << SH_REG_CLR); // clear storage reg
	#endif

	STOR_REG_CLK_PORT &= ~(1 << STOR_REG_CLK);
	SER_DATA_PORT &= ~(1 << SER_DATA);
	CLK_PORT &= ~(1 << CLK);

}

void push(uint8_t data){
	// no interrupts while pushing data
	//cli();
	uint8_t i;

	// set the storage reg clk low when writing data
	STOR_REG_CLK_PORT &= ~(1 << STOR_REG_CLK); 

	// Only change data input while Serial data clk is low
	CLK_PORT &= ~(1 << CLK);

	for (i = 0; i < 8; i++){
		// Grab the ith bit drom data and put it onto the SER_DATA pin
		if ((data & (1 << i))) SER_DATA_PORT |= (1 << SER_DATA);
		else SER_DATA_PORT &= ~(1 << SER_DATA);
		CLK_PORT |= (1 << CLK);
		CLK_PORT &= ~(1 << CLK);
	}
	// all data pushed out, now move it from shift to storage reg
	STOR_REG_CLK_PORT |= (1 << STOR_REG_CLK);
	STOR_REG_CLK_PORT &= ~(1 << STOR_REG_CLK);
	
	// done--interrupts can occur
	// sei();
}
void push16(uint16_t data){
	push(data&0x00FF);	// push out the lower 8 bits
	push(data >> 8);	// push out the upper 8 bits
}

#ifdef USING_OUTPUT_ENABLE
void disable(){
	OUTPUT_ENABLE_PORT |= (1 << OUTPUT_ENABLE);
}
void enable(){
	OUTPUT_ENABLE_PORT &= ~(1 << OUTPUT_ENABLE);

}
#endif

#ifdef USING_SH_REG_CLR
void clear(){
	// Ensure the Storage Register CLK port is low
	STOR_REG_CLK_PORT &= ~(1 << STOR_REG_CLK);

	// Clear the shift register
	SH_REG_CLR_PORT &= ~(1 << SH_REG_CLR);

	// Move the new data into the storage register
	STOR_REG_CLK_PORT |= (1 << STOR_REG_CLK);
	STOR_REG_CLK_PORT &= ~(1 << STOR_REG_CLK);

	// Disable the clear pin
	SH_REG_CLR_PORT &= ~(1 << SH_REG_CLR);
	
}
#endif

