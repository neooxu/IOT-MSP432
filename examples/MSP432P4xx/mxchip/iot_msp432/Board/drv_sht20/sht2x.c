
#include <stdio.h>


#include "mx_hal.h"
#include "sht2x.h"


static void *i2c = NULL;

int ht20_i2c_bus_write(uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
	int ret = 0;

	ret = mx_hal_i2c_cmd_write(i2c, I2C_ADDR_W, reg_addr, reg_data, cnt);
	require_noerr(ret, exit);

exit:	
	return ret;
}


int ht20_i2c_bus_read(uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
	int ret = 0;

	ret = i2c_m_sync_cmd_read(i2c, I2C_ADDR_R, reg_addr, reg_data, cnt);
	require_noerr(ret, exit);

exit:	
	return ret;
}


uint8_t SHT2x_Init(void)
{
	uint8_t err = kNoErr;
   	i2c = mx_hal_i2c_init(NULL);
	   
	err = SHT2x_SoftReset();
	mx_hal_delay_ms(100);

	return err;
}

uint8_t SHT2x_SoftReset(void) //SHT20 Soft Reset
{
	return ht20_i2c_bus_write(SOFT_RESET, NULL, 0);
}


float SHT2x_GetTempPoll(void)
{
	float TEMP;
	uint8_t tmp[2];
	uint16_t ST;
	
	tmp[0]=0;
	tmp[1]=0;

	ht20_i2c_bus_read(TRIG_TEMP_MEASUREMENT_POLL, tmp, 2);
	ST = (tmp[0] << 8) | (tmp[1] << 0);

	printf("SHT2x_GetTempPoll: %x, %x\r\n",tmp[0],tmp[1]);
	
#if 0
    u8 ack, tmp1, tmp2;
    u16 i=0;
    IIC_Start();
    IIC_Send_Byte(I2C_ADDR_W);
		ack = IIC_Wait_Ack();	
		IIC_Send_Byte(TRIG_TEMP_MEASUREMENT_POLL);
		ack = IIC_Wait_Ack();

    do {
        delay_ms(100);               
        IIC_Start();
        IIC_Send_Byte(I2C_ADR_R);	
			  i++;
			  ack = IIC_Wait_Ack();
			  if(i==1000)break;
    } while(ack!=0);
    tmp1 = IIC_Read_Byte(1); // MSB 
    tmp2 = IIC_Read_Byte(1); // LSB
		 IIC_Read_Byte(0); // Checksun
    IIC_Stop();

    ST = (tmp1 << 8) | (tmp2 << 0);
#endif	
    ST &= ~0x0003;
    TEMP = ((float)ST * 0.00268127) - 46.85;

    return (TEMP);	  

}

float SHT2x_GetHumiPoll(void)
{
	float HUMI;
	uint8_t tmp[2];
	uint16_t SRH;

	tmp[0]=0;
	tmp[1]=0;
		
	ht20_i2c_bus_read(TRIG_HUMI_MEASUREMENT_POLL, tmp, 2);
	SRH = (tmp[0] << 8) | (tmp[1] << 0);

	printf("SHT2x_GetHumiPoll: %x, %x\r\n",tmp[0],tmp[1]);
		
#if 0
    float HUMI;
    u8 ack, tmp1, tmp2;
    u16 SRH;
		u16 i=0;
    
    IIC_Start();
    IIC_Send_Byte(I2C_ADDR_W);
		ack = IIC_Wait_Ack();	
		IIC_Send_Byte(TRIG_HUMI_MEASUREMENT_POLL);
		ack = IIC_Wait_Ack();    
    do {
       delay_ms(100);               
        IIC_Start();
        IIC_Send_Byte(I2C_ADR_R);	
			  i++;
			  ack = IIC_Wait_Ack();
			  if(i==100)break;
    } while(ack!=0);
    
    tmp1 = IIC_Read_Byte(1);
   
    tmp2 = IIC_Read_Byte(1);
     IIC_Read_Byte(0);
    IIC_Stop();
    
    SRH = (tmp1 << 8) | (tmp2 << 0);
#endif

    SRH &= ~0x0003;
    HUMI = ((float)SRH * 0.00190735) - 6;

    return (HUMI);
}


