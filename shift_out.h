#ifndef SHIFT_OUT_H
#define SHIFT_OUT_H

#include <avr/io.h>


//#define USING_OUTPUT_ENABLE 	// Put to ground if not using
//#define USING_STOR_REG_CLR	// Put to Vcc if not using
#ifdef USING_OUTPUT_ENABLE
	#define OUTPUT_ENABLE 1	// Enable output or set to High-Z
	#define OUTPUT_ENABLE_PORT PORTD
	#define OUTPUT_ENABLE_PORT_DDR DDRD
#endif
#ifdef USING_STOR_REG_CLR
	#define SH_REG_CLR 3 // Clear all storage registers
	#define SH_REG_CLR_PORT PORTD
	#define SH_REG_CLR_PORT_DDR
#endif

#define STOR_REG_CLK 4  // Used to move data from shift reg to storage reg
		// should be set to (SRCLK)' or low to prevent metastability 
		// Called RCLK in datasheet
#define STOR_REG_CLK_PORT PORTC
#define STOR_REG_CLK_PORT_DDR DDRC

#define SER_DATA 5 	// Serial data
#define SER_DATA_PORT PORTC
#define SER_DATA_PORT_DDR DDRC

#define CLK 3	// The clock pin to control the shift reg
#define CLK_PORT PORTC
#define CLK_PORT_DDR DDRC

// Set up the DDRs and output port
// Clears all outputs (except Output Enable -> HIGH to disable output)
void initializeShiftReg();

// Push 8 bits of data LSB first to the shift register
void push(uint8_t data);

// using the push(uint8_t), push out 16 bits to the shift register
// this also does LSB first
void push16(uint16_t data);


#ifdef USING_OUTPUT_ENABLE
// This function disables the output (sets the OE to high)
// which sets the output pins to high-Z, effectively disconnecting them from
// the circuit
void disable();

// This function enables the output (sets the OE to low)
void enable();
#endif

#ifdef USING_SH_REG_CLR
// This function clears all the bits in the shift and storage register
void clear();
#endif

#endif
