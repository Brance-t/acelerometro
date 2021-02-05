/*! @file : sdk_hal.c
 * @author  Brance Albeiro torres Pimienta
 * @version 1.0.0
 * @date    4 feb. 2021
 * @brief   Driver para
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"



#include "sdk_hal_uart0.h"
#include "sdk_hal_i2c0.h"
/***************************
 * Definitions
 **************************/
#define MMA851_I2C_DEVICE_ADDRESS	0x1D

#define MMA8451_WHO_AM_I_MEMORY_ADDRESS		0x0D

#define DIR_REG1 0x2A
#define ULT_BIT_REG1 0x01


/***************************
 * Private Prototypes
 **************************/


/***************************
 * External vars
 **************************/


/***************************
 * Local vars
 **************************/


/***************************
 * Private Source Code
 **************************/


/***************************
 * Public Source Code
 **************************/
int main(void) {
	status_t status;
	uint8_t nuevo_byte_uart;


	uint16_t nuevo_dato_i2c;
	uint16_t nuevo_dato_i2c_REG1;

	uint16_t dato_x;
	uint16_t nuevo_dato_i2c_x_UP;
	uint16_t nuevo_dato_i2c_x_LOW;

	uint16_t dato_y;
	uint16_t nuevo_dato_i2c_y_UP;
	uint16_t nuevo_dato_i2c_y_LOW;

	uint16_t dato_z;
	uint16_t nuevo_dato_i2c_z_UP;
	uint16_t nuevo_dato_i2c_z_LOW;


  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    (void)uart0Inicializar(115200);	//115200bps
    (void)i2c0MasterInit(100000);	//100kbps

    PRINTF("Usar teclado para controlar LEDs\r\n");
    PRINTF("r-R led ROJO\r\n");
    PRINTF("v-V led VERDE\r\n");
    PRINTF("a-A led AZUL\r\n");
    PRINTF("M buscar acelerometro\r\n");


    while(1) {
    	if(uart0CuantosDatosHayEnBuffer()>0){
    		status=uart0LeerByteDesdeBuffer(&nuevo_byte_uart);
    		if(status==kStatus_Success){
    			printf("dato:%c\r\n",nuevo_byte_uart);

    			switch (nuevo_byte_uart) {


				case 'M':

					i2c0MasterReadByte(&nuevo_dato_i2c, MMA851_I2C_DEVICE_ADDRESS, MMA8451_WHO_AM_I_MEMORY_ADDRESS, 1);



					if(nuevo_dato_i2c==0x1A)
						printf("MMA8451 encontrado!!\r\n");
					else
						printf("MMA8451 error\r\n");

					break;

				case 'x':
				case 'X':

					i2c0MasterWriteByte(&nuevo_dato_i2c_REG1, MMA851_I2C_DEVICE_ADDRESS,DIR_REG1,ULT_BIT_REG1);


					i2c0MasterReadByte(&nuevo_dato_i2c_x_UP, MMA851_I2C_DEVICE_ADDRESS, OUT_X_MSB, 1);
					i2c0MasterReadByte(&nuevo_dato_i2c_x_LOW, MMA851_I2C_DEVICE_ADDRESS, OUT_X_LSB, 1);

					printf("Valor eje x MSB: %d\r\n", nuevo_dato_i2c_x_UP);
					printf("Valor eje x LSB: %d\r\n", nuevo_dato_i2c_x_LOW);

					nuevo_dato_i2c_x_UP <<=8;
										dato_x = nuevo_dato_i2c_x_UP | nuevo_dato_i2c_x_LOW;

									    printf("Valor eje x: %d\r\n", dato_x);
					break;

				case 'y':
				case 'Y':

					i2c0MasterWriteByte(&nuevo_dato_i2c_REG1, MMA851_I2C_DEVICE_ADDRESS,DIR_REG1, ULT_BIT_REG1);



										i2c0MasterReadByte(&nuevo_dato_i2c_y_UP, MMA851_I2C_DEVICE_ADDRESS, OUT_Y_MSB, 1);
										i2c0MasterReadByte(&nuevo_dato_i2c_y_LOW, MMA851_I2C_DEVICE_ADDRESS, OUT_Y_LSB, 1);

										printf("Valor eje y MSB: %d\r\n", nuevo_dato_i2c_y_UP);
										printf("Valor eje y LSB: %d\r\n", nuevo_dato_i2c_y_LOW);

										nuevo_dato_i2c_y_UP <<=8;
																		dato_y = nuevo_dato_i2c_y_UP | nuevo_dato_i2c_y_LOW;

																	    printf("Valor eje y: %d\r\n", dato_y);
				    break;

				case 'z':
				case 'Z':

					i2c0MasterWriteByte(&nuevo_dato_i2c_REG1, MMA851_I2C_DEVICE_ADDRESS,DIR_REG1, ULT_BIT_REG1);



										i2c0MasterReadByte(&nuevo_dato_i2c_z_UP, MMA851_I2C_DEVICE_ADDRESS, OUT_Z_MSB, 1);
										i2c0MasterReadByte(&nuevo_dato_i2c_z_LOW, MMA851_I2C_DEVICE_ADDRESS, OUT_Z_LSB, 1);

										printf("Valor eje z MSB: %d\r\n", nuevo_dato_i2c_z_UP);
										printf("Valor eje z LSB: %d\r\n", nuevo_dato_i2c_z_LOW);

										nuevo_dato_i2c_z_UP <<=8;
																			dato_z = nuevo_dato_i2c_z_UP | nuevo_dato_i2c_z_LOW;

																		    printf("Valor eje z: %d\r\n", dato_z);
					break;

				}
    		}else{
    			printf("error\r\n");
    		}
    	}
    }

    return 0 ;
}
