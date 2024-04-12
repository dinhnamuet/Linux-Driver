#ifndef __USB_SERIAL_H__
#define __USB_SERIAL_H__

#define IFC_ENABLE	        0x00
#define SET_BAUDDIV	        0x01
#define GET_BAUDDIV	        0x02
#define SET_LINE_CTL	    0x03
#define GET_LINE_CTL	    0x04
#define SET_BREAK	        0x05
#define IMM_CHAR		    0x06
#define SET_MHS		        0x07
#define GET_MDMSTS	        0x08
#define SET_XON		        0x09
#define SET_XOFF		    0x0A
#define SET_EVENTMASK	    0x0B
#define GET_EVENTMASK	    0x0C
#define SET_CHAR		    0x0D
#define GET_CHARS	        0x0E
#define GET_PROPS	        0x0F
#define GET_COMM_STATUS	    0x10
#define RESET		        0x11
#define PURGE		        0x12
#define SET_FLOW		    0x13
#define GET_FLOW		    0x14
#define EMBED_EVENTS	    0x15
#define GET_EVENTSTATE	    0x16
#define SET_CHARS	        0x19
#define GET_BAUDRATE	    0x1D
#define SET_BAUDRATE	    0x1E
#define VENDOR_SPECIFIC	    0xFF

#define REQTYPE_HOST_TO_INTERFACE	0x41
#define REQTYPE_INTERFACE_TO_HOST	0xc1
#define REQTYPE_HOST_TO_DEVICE		0x40
#define REQTYPE_DEVICE_TO_HOST		0xc0

#define UART_ENABLE			0x0001
#define UART_DISABLE		0x0000


#endif /* __USB_SERIAL_H__ */
