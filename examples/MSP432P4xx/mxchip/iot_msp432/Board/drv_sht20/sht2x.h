#ifndef __SHT2x_H__
#define __SHT2x_H__


#ifdef __cplusplus
 extern "C" {
#endif

#define  I2C_ADDR_W 0x40
#define  I2C_ADDR_R 0x81

typedef enum {
    TRIG_TEMP_MEASUREMENT_HM   = 0xE3,
    TRIG_HUMI_MEASUREMENT_HM   = 0xE5,
    TRIG_TEMP_MEASUREMENT_POLL = 0xF3,
    TRIG_HUMI_MEASUREMENT_POLL = 0xF5,
    USER_REG_W                 = 0xE6,
    USER_REG_R                 = 0xE7,
    SOFT_RESET                 = 0xFE
} SHT2xCommand;

typedef enum {
    SHT2x_RES_12_14BIT         = 0x00,
    SHT2x_RES_8_12BIT          = 0x01,
    SHT2x_RES_10_13BIT         = 0x80,
    SHT2x_RES_11_11BIT         = 0x81,
    SHT2x_RES_MASK             = 0x81
} SHT2xResolution;

typedef enum {
    SHT2x_HEATER_ON            = 0x04,
    SHT2x_HEATER_OFF           = 0x00,
    SHT2x_HEATER_MASK          = 0x04
} SHT2xHeater;

typedef struct{
    float TEMP_POLL;
    float HUMI_POLL;    
} SHT2x_data;

extern SHT2x_data SHT20;

uint8_t SHT2x_Init(void); 
uint8_t SHT2x_SoftReset(void);
float SHT2x_GetTempPoll(void);
float SHT2x_GetHumiPoll(void);

#ifdef __cplusplus
}
#endif

#endif

