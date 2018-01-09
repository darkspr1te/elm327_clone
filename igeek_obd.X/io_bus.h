

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */


#define RX_LED 4//PORTB, BIT4 (LED-RS232-CMD OFF)
#define TX_LED 5//PORTB, BIT5 (LED-WORK OFF)
#define BUSTX_LED 6//PORTB, BIT7 (LED-CAN-TX OFF)
#define BUSRX_LED 7//PORTB, BIT6 (LED-CAN-RX OFF)
