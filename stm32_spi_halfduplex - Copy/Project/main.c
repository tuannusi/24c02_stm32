#include "stm32f10x.h"
#include "i2c_ee.h"
#include <stdio.h>
#include <string.h>
#include "setup.h"

#define  EEP_Firstpage      0x00
uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];
void I2C_Test(void);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

int main(void){
	/* cau hinh clock là 72M */
	Init_USART();
	Init_GPIO();
	
	/* Kh?i t?o ngo?i vi I2C (AT24C02) */
	I2C_EE_Init();
	printf("\r\n I2C (AT24C02) doc và ghi kiem tra \r\n");
	printf("\r\n ("__DATE__ " - " __TIME__ ") \r\n");
	//printf("bbb");
	I2C_Test();
while(1)
	{	
	}
}

void I2C_Test(void)
{
	uint16_t i;

	printf("Data kieu char \n\r");
    
	for ( i=0; i<=255; i++ ) //ghi vao bo dem
  {   
    I2c_Buf_Write[i] = i;

    printf("0x%02X ", I2c_Buf_Write[i]);
    if(i%16 == 15)    
        printf("\n\r");    
   }

  //Viet data tang dan theo thu tu trong I2c_Buf_Write vào EERPOM
	I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);	 
  
  printf("\n\r doc lai data \n\r");
  //Giu chuoi data doc EEPROM trong I2c_Buf_Read
	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256); 

  //In du lieu trong I2c_Buf_Read qua UART
	for (i=0; i<256; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			printf("0x%02X ", I2c_Buf_Read[i]);
			printf("´Error: Du lieu ghi và doc I2C EEPROM không giong nhau \n \r");
			return;
		}
    printf("0x%02X ", I2c_Buf_Read[i]);
    if(i%16 == 15)    
        printf("\n\r");
    
	}
  printf("Success\r\n");
}

