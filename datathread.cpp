#include "datathread.h"

//#include <sys/stat.h>

#include <QProcess>

//新添加的

#if 1

/*
  Comple：make
  Run: ./bme280

  This Demo is tested on Raspberry PI 3B+
  you can use I2C or SPI interface to test this Demo
  When you use I2C interface,the default Address in this demo is 0X77
  When you use SPI interface,PIN 27 define SPI_CS
*/
#include "bme280.h"
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

//Raspberry 3B+ platform's default SPI channel
#define channel1 0

//Default write it to the register in one time
#define USESPISINGLEREADWRITE 0

//This definition you use I2C or SPI to drive the bme280
//When it is 1 means use I2C interface, When it is 0,use SPI interface
#define USEIIC 1


#if(USEIIC)
#include <string.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
//Raspberry 3B+ platform's default I2C device file
#define IIC_Dev  "/dev/i2c-1"

int fd;

void user_delay_ms(uint32_t period)
{
    usleep(period*1000);
}

int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    write(fd, &reg_addr,1);
    read(fd, data, len);
    return 0;
}

int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    int8_t *buf;
    buf = (int8_t*)malloc(len +1);
    buf[0] = reg_addr;
    memcpy(buf +1, data, len);
    write(fd, buf, len +1);
    free(buf);
    return 0;
}
#else

void SPI_BME280_CS_High(void)
{
    digitalWrite(27,1);
}

void SPI_BME280_CS_Low(void)
{
    digitalWrite(27,0);
}

void user_delay_ms(uint32_t period)
{
    usleep(period*1000);
}

int8_t user_spi_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0;

    SPI_BME280_CS_High();
    SPI_BME280_CS_Low();

    wiringPiSPIDataRW(channel,&reg_addr,1);

#if(USESPISINGLEREADWRITE)
    for(int i=0; i < len ; i++)
    {
        wiringPiSPIDataRW(channel,reg_data,1);
        reg_data++;
    }
#else
    wiringPiSPIDataRW(channel,reg_data,len);
#endif

    SPI_BME280_CS_High();

    return rslt;
}

int8_t user_spi_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0;

    SPI_BME280_CS_High();
    SPI_BME280_CS_Low();

    wiringPiSPIDataRW(channel,&reg_addr,1);

#if(USESPISINGLEREADWRITE)
    for(int i = 0; i < len ; i++)
    {
        wiringPiSPIDataRW(channel,reg_data,1);
        reg_data++;
    }
#else
    wiringPiSPIDataRW(channel,reg_data,len);
#endif

    SPI_BME280_CS_High();

    return rslt;
}
#endif

void print_sensor_data(struct bme280_data *comp_data)
{
#ifdef BME280_FLOAT_ENABLE
    printf("temperature:%0.2f*C   pressure:%0.2fhPa   humidity:%0.2f%%\r\n",comp_data->temperature, comp_data->pressure/100, comp_data->humidity);

    //直接显示到状态栏
//    QString str = "Temp：" + QString::number(comp_data->temperature);
//    QString str1 ="Hum：" +  QString::number(comp_data->humidity);
//    //aa->statusBar->showMessage(str);

//    aa->tLineEdit->setText(str);
//    aa->hLineEdit->setText(str1);



#else
    printf("temperature:%ld*C   pressure:%ldhPa   humidity:%ld%%\r\n",comp_data->temperature, comp_data->pressure/100, comp_data->humidity);
#endif
}



int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev)
{
    int8_t rslt;
    uint8_t settings_sel;
    struct bme280_data comp_data;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    rslt = bme280_set_sensor_settings(settings_sel, dev);

    printf("Temperature           Pressure             Humidity\r\n");
    /* Continuously stream sensor data */
    while (1) {
        rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
        /* Wait for the measurement to complete and print data @25Hz */
        dev->delay_ms(40);
        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        //print_sensor_data(&comp_data,this);
    }
    return rslt;
}


int8_t stream_sensor_data_normal_mode(struct bme280_dev *dev)
{
    int8_t rslt;
    uint8_t settings_sel;
    struct bme280_data comp_data;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;
    dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

    settings_sel = BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    settings_sel |= BME280_OSR_HUM_SEL;
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;
    rslt = bme280_set_sensor_settings(settings_sel, dev);
    rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);

    printf("Temperature           Pressure             Humidity\r\n");
    //while (1)
    {
        /* Delay while the sensor completes a measurement */
        dev->delay_ms(70);
        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        print_sensor_data(&comp_data);
    }

    return rslt;
}





#endif









#if 1

/*
 * ADS1256_test.c:
 *	Very simple program to test the serial port. Expects
 *	the port to be looped back to itself
 *
 */

/*
             define from bcm2835.h                       define from Board DVK511
                 3.3V | | 5V               ->                 3.3V | | 5V
    RPI_V2_GPIO_P1_03 | | 5V               ->                  SDA | | 5V
    RPI_V2_GPIO_P1_05 | | GND              ->                  SCL | | GND
       RPI_GPIO_P1_07 | | RPI_GPIO_P1_08   ->                  IO7 | | TX
                  GND | | RPI_GPIO_P1_10   ->                  GND | | RX
       RPI_GPIO_P1_11 | | RPI_GPIO_P1_12   ->                  IO0 | | IO1
    RPI_V2_GPIO_P1_13 | | GND              ->                  IO2 | | GND
       RPI_GPIO_P1_15 | | RPI_GPIO_P1_16   ->                  IO3 | | IO4
                  VCC | | RPI_GPIO_P1_18   ->                  VCC | | IO5
       RPI_GPIO_P1_19 | | GND              ->                 MOSI | | GND
       RPI_GPIO_P1_21 | | RPI_GPIO_P1_22   ->                 MISO | | IO6
       RPI_GPIO_P1_23 | | RPI_GPIO_P1_24   ->                  SCK | | CE0
                  GND | | RPI_GPIO_P1_26   ->                  GND | | CE1

::if your raspberry Pi is version 1 or rev 1 or rev A
RPI_V2_GPIO_P1_03->RPI_GPIO_P1_03
RPI_V2_GPIO_P1_05->RPI_GPIO_P1_05
RPI_V2_GPIO_P1_13->RPI_GPIO_P1_13
::
*/

#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>

//CS      -----   SPICS
//DIN     -----   MOSI
//DOUT  -----   MISO
//SCLK   -----   SCLK
//DRDY  -----   ctl_IO     data  starting
//RST     -----   ctl_IO     reset



//#define  DRDY  RPI_V2_GPIO_P1_11         //P0
#define  DRDY  17
//#define  RST  RPI_V2_GPIO_P1_12     //P1
#define  RST  18
//#define	SPICS	RPI_V2_GPIO_P1_15	//P3
#define  SPICS  22

#define CS_1() bcm2835_gpio_write(SPICS,HIGH)
#define CS_0()  bcm2835_gpio_write(SPICS,LOW)

#define DRDY_IS_LOW()	((bcm2835_gpio_lev(DRDY)==0))

#define RST_1() 	bcm2835_gpio_write(RST,HIGH)
#define RST_0() 	bcm2835_gpio_write(RST,LOW)



/* Unsigned integer types  */
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned long




//typedef enum {FALSE = 0, TRUE = !FALSE} bool;


/* gain channel?*/
typedef enum
{
    ADS1256_GAIN_1			= (0),	/* GAIN   1 */
    ADS1256_GAIN_2			= (1),	/*GAIN   2 */
    ADS1256_GAIN_4			= (2),	/*GAIN   4 */
    ADS1256_GAIN_8			= (3),	/*GAIN   8 */
    ADS1256_GAIN_16			= (4),	/* GAIN  16 */
    ADS1256_GAIN_32			= (5),	/*GAIN    32 */
    ADS1256_GAIN_64			= (6),	/*GAIN    64 */
}ADS1256_GAIN_E;

/* Sampling speed choice*/
/*
    11110000 = 30,000SPS (default)
    11100000 = 15,000SPS
    11010000 = 7,500SPS
    11000000 = 3,750SPS
    10110000 = 2,000SPS
    10100001 = 1,000SPS
    10010010 = 500SPS
    10000010 = 100SPS
    01110010 = 60SPS
    01100011 = 50SPS
    01010011 = 30SPS
    01000011 = 25SPS
    00110011 = 15SPS
    00100011 = 10SPS
    00010011 = 5SPS
    00000011 = 2.5SPS
*/
typedef enum
{
    ADS1256_30000SPS = 0,
    ADS1256_15000SPS,
    ADS1256_7500SPS,
    ADS1256_3750SPS,
    ADS1256_2000SPS,
    ADS1256_1000SPS,
    ADS1256_500SPS,
    ADS1256_100SPS,
    ADS1256_60SPS,
    ADS1256_50SPS,
    ADS1256_30SPS,
    ADS1256_25SPS,
    ADS1256_15SPS,
    ADS1256_10SPS,
    ADS1256_5SPS,
    ADS1256_2d5SPS,

    ADS1256_DRATE_MAX
}ADS1256_DRATE_E;

#define ADS1256_DRAE_COUNT = 15;

typedef struct
{
    ADS1256_GAIN_E Gain;		/* GAIN  */
    ADS1256_DRATE_E DataRate;	/* DATA output  speed*/
    int32_t AdcNow[8];			/* ADC  Conversion value */
    uint8_t Channel;			/* The current channel*/
    uint8_t ScanMode;	/*Scanning mode,   0  Single-ended input  8 channel， 1 Differential input  4 channel*/
}ADS1256_VAR_T;



/*Register definition： Table 23. Register Map --- ADS1256 datasheet Page 30*/
enum
{
    /*Register address, followed by reset the default values */
    REG_STATUS = 0,	// x1H
    REG_MUX    = 1, // 01H
    REG_ADCON  = 2, // 20H
    REG_DRATE  = 3, // F0H
    REG_IO     = 4, // E0H
    REG_OFC0   = 5, // xxH
    REG_OFC1   = 6, // xxH
    REG_OFC2   = 7, // xxH
    REG_FSC0   = 8, // xxH
    REG_FSC1   = 9, // xxH
    REG_FSC2   = 10, // xxH
};

/* Command definition： TTable 24. Command Definitions --- ADS1256 datasheet Page 34 */
enum
{
    CMD_WAKEUP  = 0x00,	// Completes SYNC and Exits Standby Mode 0000  0000 (00h)
    CMD_RDATA   = 0x01, // Read Data 0000  0001 (01h)
    CMD_RDATAC  = 0x03, // Read Data Continuously 0000   0011 (03h)
    CMD_SDATAC  = 0x0F, // Stop Read Data Continuously 0000   1111 (0Fh)
    CMD_RREG    = 0x10, // Read from REG rrr 0001 rrrr (1xh)
    CMD_WREG    = 0x50, // Write to REG rrr 0101 rrrr (5xh)
    CMD_SELFCAL = 0xF0, // Offset and Gain Self-Calibration 1111    0000 (F0h)
    CMD_SELFOCAL= 0xF1, // Offset Self-Calibration 1111    0001 (F1h)
    CMD_SELFGCAL= 0xF2, // Gain Self-Calibration 1111    0010 (F2h)
    CMD_SYSOCAL = 0xF3, // System Offset Calibration 1111   0011 (F3h)
    CMD_SYSGCAL = 0xF4, // System Gain Calibration 1111    0100 (F4h)
    CMD_SYNC    = 0xFC, // Synchronize the A/D Conversion 1111   1100 (FCh)
    CMD_STANDBY = 0xFD, // Begin Standby Mode 1111   1101 (FDh)
    CMD_RESET   = 0xFE, // Reset to Power-Up Values 1111   1110 (FEh)
};


ADS1256_VAR_T g_tADS1256;
static const uint8_t s_tabDataRate[ADS1256_DRATE_MAX] =
{
    0xF0,		/*reset the default values  */
    0xE0,
    0xD0,
    0xC0,
    0xB0,
    0xA1,
    0x92,
    0x82,
    0x72,
    0x63,
    0x53,
    0x43,
    0x33,
    0x20,
    0x13,
    0x03
};







void  bsp_DelayUS(uint64_t micros);
void ADS1256_StartScan(uint8_t _ucScanMode);
static void ADS1256_Send8Bit(uint8_t _data);
void ADS1256_CfgADC(ADS1256_GAIN_E _gain, ADS1256_DRATE_E _drate);
static void ADS1256_DelayDATA(void);
static uint8_t ADS1256_Recive8Bit(void);
static void ADS1256_WriteReg(uint8_t _RegID, uint8_t _RegValue);
static uint8_t ADS1256_ReadReg(uint8_t _RegID);
static void ADS1256_WriteCmd(uint8_t _cmd);
uint8_t ADS1256_ReadChipID(void);
static void ADS1256_SetChannal(uint8_t _ch);
static void ADS1256_SetDiffChannal(uint8_t _ch);
static void ADS1256_WaitDRDY(void);
static int32_t ADS1256_ReadData(void);

int32_t ADS1256_GetAdc(uint8_t _ch);
void ADS1256_ISR(void);
uint8_t ADS1256_Scan(void);




void  bsp_DelayUS(uint64_t micros)
{
    bcm2835_delayMicroseconds (micros);
}


/*
*********************************************************************************************************
*	name: bsp_InitADS1256
*	function: Configuration of the STM32 GPIO and SPI interface，The connection ADS1256
*	parameter: NULL
*	The return value: NULL
*********************************************************************************************************
*/


void bsp_InitADS1256(void)
{
#ifdef SOFT_SPI
    CS_1();
    SCK_0();
    DI_0();
#endif

    //ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_1000SPS);	/* 配置ADC参数： 增益1:1, 数据输出速率 1KHz */
}




/*
*********************************************************************************************************
*	name: ADS1256_StartScan
*	function: Configuration DRDY PIN for external interrupt is triggered
*	parameter: _ucDiffMode : 0  Single-ended input  8 channel， 1 Differential input  4 channe
*	The return value: NULL
*********************************************************************************************************
*/
void ADS1256_StartScan(uint8_t _ucScanMode)
{
    g_tADS1256.ScanMode = _ucScanMode;
    /* 开始扫描前, 清零结果缓冲区 */
    {
        uint8_t i;

        g_tADS1256.Channel = 0;

        for (i = 0; i < 8; i++)
        {
            g_tADS1256.AdcNow[i] = 0;
        }
    }

}

/*
*********************************************************************************************************
*	name: ADS1256_Send8Bit
*	function: SPI bus to send 8 bit data
*	parameter: _data:  data
*	The return value: NULL
*********************************************************************************************************
*/
static void ADS1256_Send8Bit(uint8_t _data)
{

    bsp_DelayUS(2);
    bcm2835_spi_transfer(_data);
}

/*
*********************************************************************************************************
*	name: ADS1256_CfgADC
*	function: The configuration parameters of ADC, gain and data rate
*	parameter: _gain:gain 1-64
*                      _drate:  data  rate
*	The return value: NULL
*********************************************************************************************************
*/
void ADS1256_CfgADC(ADS1256_GAIN_E _gain, ADS1256_DRATE_E _drate)
{
    g_tADS1256.Gain = _gain;
    g_tADS1256.DataRate = _drate;

    ADS1256_WaitDRDY();

    {
        uint8_t buf[4];		/* Storage ads1256 register configuration parameters */

        /*Status register define
            Bits 7-4 ID3, ID2, ID1, ID0  Factory Programmed Identification Bits (Read Only)

            Bit 3 ORDER: Data Output Bit Order
                0 = Most Significant Bit First (default)
                1 = Least Significant Bit First
            Input data  is always shifted in most significant byte and bit first. Output data is always shifted out most significant
            byte first. The ORDER bit only controls the bit order of the output data within the byte.

            Bit 2 ACAL : Auto-Calibration
                0 = Auto-Calibration Disabled (default)
                1 = Auto-Calibration Enabled
            When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
            the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
            values.

            Bit 1 BUFEN: Analog Input Buffer Enable
                0 = Buffer Disabled (default)
                1 = Buffer Enabled

            Bit 0 DRDY :  Data Ready (Read Only)
                This bit duplicates the state of the DRDY pin.

            ACAL=1  enable  calibration
        */
        //buf[0] = (0 << 3) | (1 << 2) | (1 << 1);//enable the internal buffer
        buf[0] = (0 << 3) | (1 << 2) | (0 << 1);  // The internal buffer is prohibited

        //ADS1256_WriteReg(REG_STATUS, (0 << 3) | (1 << 2) | (1 << 1));

        buf[1] = 0x08;

        /*	ADCON: A/D Control Register (Address 02h)
            Bit 7 Reserved, always 0 (Read Only)
            Bits 6-5 CLK1, CLK0 : D0/CLKOUT Clock Out Rate Setting
                00 = Clock Out OFF
                01 = Clock Out Frequency = fCLKIN (default)
                10 = Clock Out Frequency = fCLKIN/2
                11 = Clock Out Frequency = fCLKIN/4
                When not using CLKOUT, it is recommended that it be turned off. These bits can only be reset using the RESET pin.

            Bits 4-3 SDCS1, SCDS0: Sensor Detect Current Sources
                00 = Sensor Detect OFF (default)
                01 = Sensor Detect Current = 0.5 μ A
                10 = Sensor Detect Current = 2 μ A
                11 = Sensor Detect Current = 10μ A
                The Sensor Detect Current Sources can be activated to verify  the integrity of an external sensor supplying a signal to the
                ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.

            Bits 2-0 PGA2, PGA1, PGA0: Programmable Gain Amplifier Setting
                000 = 1 (default)
                001 = 2
                010 = 4
                011 = 8
                100 = 16
                101 = 32
                110 = 64
                111 = 64
        */
        buf[2] = (0 << 5) | (0 << 3) | (_gain << 0);
        //ADS1256_WriteReg(REG_ADCON, (0 << 5) | (0 << 2) | (GAIN_1 << 1));	/*choose 1: gain 1 ;input 5V/
        buf[3] = s_tabDataRate[_drate];	// DRATE_10SPS;

        CS_0();	/* SPI片选 = 0 */
        ADS1256_Send8Bit(CMD_WREG | 0);	/* Write command register, send the register address */
        ADS1256_Send8Bit(0x03);			/* Register number 4,Initialize the number  -1*/

        ADS1256_Send8Bit(buf[0]);	/* Set the status register */
        ADS1256_Send8Bit(buf[1]);	/* Set the input channel parameters */
        ADS1256_Send8Bit(buf[2]);	/* Set the ADCON control register,gain */
        ADS1256_Send8Bit(buf[3]);	/* Set the output rate */

        CS_1();	/* SPI  cs = 1 */
    }

    bsp_DelayUS(50);
}


/*
*********************************************************************************************************
*	name: ADS1256_DelayDATA
*	function: delay
*	parameter: NULL
*	The return value: NULL
*********************************************************************************************************
*/
static void ADS1256_DelayDATA(void)
{
    /*
        Delay from last SCLK edge for DIN to first SCLK rising edge for DOUT: RDATA, RDATAC,RREG Commands
        min  50   CLK = 50 * 0.13uS = 6.5uS
    */
    bsp_DelayUS(10);	/* The minimum time delay 6.5us */
}




/*
*********************************************************************************************************
*	name: ADS1256_Recive8Bit
*	function: SPI bus receive function
*	parameter: NULL
*	The return value: NULL
*********************************************************************************************************
*/
static uint8_t ADS1256_Recive8Bit(void)
{
    uint8_t read = 0;
    read = bcm2835_spi_transfer(0xff);
    return read;
}

/*
*********************************************************************************************************
*	name: ADS1256_WriteReg
*	function: Write the corresponding register
*	parameter: _RegID: register  ID
*			 _RegValue: register Value
*	The return value: NULL
*********************************************************************************************************
*/
static void ADS1256_WriteReg(uint8_t _RegID, uint8_t _RegValue)
{
    CS_0();	/* SPI  cs  = 0 */
    ADS1256_Send8Bit(CMD_WREG | _RegID);	/*Write command register */
    ADS1256_Send8Bit(0x00);		/*Write the register number */

    ADS1256_Send8Bit(_RegValue);	/*send register value */
    CS_1();	/* SPI   cs = 1 */
}

/*
*********************************************************************************************************
*	name: ADS1256_ReadReg
*	function: Read  the corresponding register
*	parameter: _RegID: register  ID
*	The return value: read register value
*********************************************************************************************************
*/
static uint8_t ADS1256_ReadReg(uint8_t _RegID)
{
    uint8_t read;

    CS_0();	/* SPI  cs  = 0 */
    ADS1256_Send8Bit(CMD_RREG | _RegID);	/* Write command register */
    ADS1256_Send8Bit(0x00);	/* Write the register number */

    ADS1256_DelayDATA();	/*delay time */

    read = ADS1256_Recive8Bit();	/* Read the register values */
    CS_1();	/* SPI   cs  = 1 */

    return read;
}

/*
*********************************************************************************************************
*	name: ADS1256_WriteCmd
*	function: Sending a single byte order
*	parameter: _cmd : command
*	The return value: NULL
*********************************************************************************************************
*/
static void ADS1256_WriteCmd(uint8_t _cmd)
{
    CS_0();	/* SPI   cs = 0 */
    ADS1256_Send8Bit(_cmd);
    CS_1();	/* SPI  cs  = 1 */
}

/*
*********************************************************************************************************
*	name: ADS1256_ReadChipID
*	function: Read the chip ID
*	parameter: _cmd : NULL
*	The return value: four high status register
*********************************************************************************************************
*/
uint8_t ADS1256_ReadChipID(void)
{
    uint8_t id;

    ADS1256_WaitDRDY();
    id = ADS1256_ReadReg(REG_STATUS);
    return (id >> 4);
}

/*
*********************************************************************************************************
*	name: ADS1256_SetChannal
*	function: Configuration channel number
*	parameter:  _ch:  channel number  0--7
*	The return value: NULL
*********************************************************************************************************
*/
static void ADS1256_SetChannal(uint8_t _ch)
{
    /*
    Bits 7-4 PSEL3, PSEL2, PSEL1, PSEL0: Positive Input Channel (AINP) Select
        0000 = AIN0 (default)
        0001 = AIN1
        0010 = AIN2 (ADS1256 only)
        0011 = AIN3 (ADS1256 only)
        0100 = AIN4 (ADS1256 only)
        0101 = AIN5 (ADS1256 only)
        0110 = AIN6 (ADS1256 only)
        0111 = AIN7 (ADS1256 only)
        1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are “don’t care”)

        NOTE: When using an ADS1255 make sure to only select the available inputs.

    Bits 3-0 NSEL3, NSEL2, NSEL1, NSEL0: Negative Input Channel (AINN)Select
        0000 = AIN0
        0001 = AIN1 (default)
        0010 = AIN2 (ADS1256 only)
        0011 = AIN3 (ADS1256 only)
        0100 = AIN4 (ADS1256 only)
        0101 = AIN5 (ADS1256 only)
        0110 = AIN6 (ADS1256 only)
        0111 = AIN7 (ADS1256 only)
        1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are “don’t care”)
    */
    if (_ch > 7)
    {
        return;
    }
    ADS1256_WriteReg(REG_MUX, (_ch << 4) | (1 << 3));	/* Bit3 = 1, AINN connection AINCOM */
}

/*
*********************************************************************************************************
*	name: ADS1256_SetDiffChannal
*	function: The configuration difference channel
*	parameter:  _ch:  channel number  0--3
*	The return value:  four high status register
*********************************************************************************************************
*/
static void ADS1256_SetDiffChannal(uint8_t _ch)
{
    /*
    Bits 7-4 PSEL3, PSEL2, PSEL1, PSEL0: Positive Input Channel (AINP) Select
        0000 = AIN0 (default)
        0001 = AIN1
        0010 = AIN2 (ADS1256 only)
        0011 = AIN3 (ADS1256 only)
        0100 = AIN4 (ADS1256 only)
        0101 = AIN5 (ADS1256 only)
        0110 = AIN6 (ADS1256 only)
        0111 = AIN7 (ADS1256 only)
        1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are “don’t care”)

        NOTE: When using an ADS1255 make sure to only select the available inputs.

    Bits 3-0 NSEL3, NSEL2, NSEL1, NSEL0: Negative Input Channel (AINN)Select
        0000 = AIN0
        0001 = AIN1 (default)
        0010 = AIN2 (ADS1256 only)
        0011 = AIN3 (ADS1256 only)
        0100 = AIN4 (ADS1256 only)
        0101 = AIN5 (ADS1256 only)
        0110 = AIN6 (ADS1256 only)
        0111 = AIN7 (ADS1256 only)
        1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are “don’t care”)
    */
    if (_ch == 0)
    {
        ADS1256_WriteReg(REG_MUX, (0 << 4) | 1);	/* DiffChannal  AIN0， AIN1 */
    }
    else if (_ch == 1)
    {
        ADS1256_WriteReg(REG_MUX, (2 << 4) | 3);	/*DiffChannal   AIN2， AIN3 */
    }
    else if (_ch == 2)
    {
        ADS1256_WriteReg(REG_MUX, (4 << 4) | 5);	/*DiffChannal    AIN4， AIN5 */
    }
    else if (_ch == 3)
    {
        ADS1256_WriteReg(REG_MUX, (6 << 4) | 7);	/*DiffChannal   AIN6， AIN7 */
    }
}

/*
*********************************************************************************************************
*	name: ADS1256_WaitDRDY
*	function: delay time  wait for automatic calibration
*	parameter:  NULL
*	The return value:  NULL
*********************************************************************************************************
*/
static void ADS1256_WaitDRDY(void)
{
    uint32_t i;

    for (i = 0; i < 400000; i++)
    {
        if (DRDY_IS_LOW())
        {
            break;
        }
    }
    if (i >= 400000)
    {
        printf("ADS1256_WaitDRDY() Time Out ...\r\n");
    }
}

/*
*********************************************************************************************************
*	name: ADS1256_ReadData
*	function: read ADC value
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/
static int32_t ADS1256_ReadData(void)
{
    uint32_t read = 0;
    static uint8_t buf[3];

    CS_0();	/* SPI   cs = 0 */

    ADS1256_Send8Bit(CMD_RDATA);	/* read ADC command  */

    ADS1256_DelayDATA();	/*delay time  */

    /*Read the sample results 24bit*/
    buf[0] = ADS1256_Recive8Bit();
    buf[1] = ADS1256_Recive8Bit();
    buf[2] = ADS1256_Recive8Bit();

    read = ((uint32_t)buf[0] << 16) & 0x00FF0000;
    read |= ((uint32_t)buf[1] << 8);  /* Pay attention to It is wrong   read |= (buf[1] << 8) */
    read |= buf[2];

    CS_1();	/* SPI片选 = 1 */

    /* Extend a signed number*/
    if (read & 0x800000)
    {
        read |= 0xFF000000;
    }

    return (int32_t)read;
}


/*
*********************************************************************************************************
*	name: ADS1256_GetAdc
*	function: read ADC value
*	parameter:  channel number 0--7
*	The return value:  ADC vaule (signed number)
*********************************************************************************************************
*/
int32_t ADS1256_GetAdc(uint8_t _ch)
{
    int32_t iTemp;

    if (_ch > 7)
    {
        return 0;
    }

    iTemp = g_tADS1256.AdcNow[_ch];

    return iTemp;
}

/*
*********************************************************************************************************
*	name: ADS1256_ISR
*	function: Collection procedures
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/
void ADS1256_ISR(void)
{
    if (g_tADS1256.ScanMode == 0)	/*  0  Single-ended input  8 channel， 1 Differential input  4 channe */
    {

        ADS1256_SetChannal(g_tADS1256.Channel);	/*Switch channel mode */
        bsp_DelayUS(5);

        ADS1256_WriteCmd(CMD_SYNC);
        bsp_DelayUS(5);

        ADS1256_WriteCmd(CMD_WAKEUP);
        bsp_DelayUS(25);

        if (g_tADS1256.Channel == 0)
        {
            g_tADS1256.AdcNow[7] = ADS1256_ReadData();
        }
        else
        {
            g_tADS1256.AdcNow[g_tADS1256.Channel-1] = ADS1256_ReadData();
        }

        if (++g_tADS1256.Channel >= 8)
        {
            g_tADS1256.Channel = 0;
        }
    }
    else	/*DiffChannal*/
    {

        ADS1256_SetDiffChannal(g_tADS1256.Channel);	/* change DiffChannal */
        bsp_DelayUS(5);

        ADS1256_WriteCmd(CMD_SYNC);
        bsp_DelayUS(5);

        ADS1256_WriteCmd(CMD_WAKEUP);
        bsp_DelayUS(25);

        if (g_tADS1256.Channel == 0)
        {
            g_tADS1256.AdcNow[3] = ADS1256_ReadData();
        }
        else
        {
            g_tADS1256.AdcNow[g_tADS1256.Channel-1] = ADS1256_ReadData();
        }

        if (++g_tADS1256.Channel >= 4)
        {
            g_tADS1256.Channel = 0;
        }
    }
}

/*
*********************************************************************************************************
*	name: ADS1256_Scan
*	function:
*	parameter:NULL
*	The return value:  1
*********************************************************************************************************
*/
uint8_t ADS1256_Scan(void)
{
    if (DRDY_IS_LOW())
    {
        ADS1256_ISR();
        return 1;
    }

    return 0;
}
/*
*********************************************************************************************************
*	name: Write_DAC8552
*	function:  DAC send data
*	parameter: channel : output channel number
*			   data : output DAC value
*	The return value:  NULL
*********************************************************************************************************
*/
void Write_DAC8552(uint8_t channel, uint16_t Data1)
{
    //uint8_t i;

    CS_1() ;
    CS_0() ;
    bcm2835_spi_transfer(channel);
    bcm2835_spi_transfer((Data1>>8));
    bcm2835_spi_transfer((Data1&0xff));
    CS_1() ;
}
/*
*********************************************************************************************************
*	name: Voltage_Convert
*	function:  Voltage value conversion function
*	parameter: Vref : The reference voltage 3.3V or 5V
*			   voltage : output DAC value
*	The return value:  NULL
*********************************************************************************************************
*/
uint16_t Voltage_Convert(float Vref, float voltage)
{
    uint16_t _D_;
    _D_ = (uint16_t)(65536 * voltage / Vref);

    return _D_;
}

/*
*********************************************************************************************************
*	name: main
*	function:
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/


#endif

DataThread::DataThread()
{
    m_isStop = false;

    m_isQTimerStart = false;

    m_isSend = true;

    m_isShow = false;

    m_isStopThread = 1;
}



void DataThread::threadStop()
{


    m_isStopThread = 0;
}


void DataThread::run()
{
        uint8_t id;
        int32_t adc[8];
        int32_t volt[8];
        uint8_t i;

        uint8_t ch_num;
        int32_t iTemp;
        uint8_t buf[3];


        if(!m_isQTimerStart)
        {
            if (!bcm2835_init())
                return;

            qDebug()  << "zhixingle1111111111111111111111111111111111111111111....................... ";

            /*
          bcm2835_spi_begin();
          bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST );      // The default
          bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                   // The default
          bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024); // The default
        */

            bcm2835_spi_begin();
            qDebug()  << "zhixingle3....................... ";
            bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);   //default
            qDebug()  << "zhixingle4....................... ";

            bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                //default
            qDebug()  << "zhixingle5...................... ";
            bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);//default
            qDebug()  << "zhixingle6...................... ";
            bcm2835_gpio_fsel(SPICS, BCM2835_GPIO_FSEL_OUTP);//
            bcm2835_gpio_write(SPICS, HIGH);
            bcm2835_gpio_fsel(DRDY, BCM2835_GPIO_FSEL_INPT);
            bcm2835_gpio_set_pud(DRDY, BCM2835_GPIO_PUD_UP);

            //qDebug()  << "zhixingle222222222222....................... ";

            //ADS1256_WriteReg(REG_MUX,0x01);
            //ADS1256_WriteReg(REG_ADCON,0x20);
            // ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_15SPS);
            id = ADS1256_ReadChipID();
            //printf("\r\n");
            //printf("ID=\r\n");
            if (id != 3)
            {
                //printf("Error, ASD1256 Chip ID = 0x%d\r\n", (int)id);
            }
            else
            {
                //printf("Ok, ASD1256 Chip ID = 0x%d\r\n", (int)id);
            }
            ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_15SPS);
            ADS1256_StartScan(0);
            //qDebug()  << "zhixingle7...................... ";
            ch_num = 4;
            //if (ADS1256_Scan() == 0)
            //{
            //continue;

            //}

            m_isQTimerStart = true;
        }



        //定义个数组，用来存当前的数据
        QList<float> intList;
        QStringList strLsit;
        //QString str5 ;
        float str4Flaot ;
        while(m_isStopThread)
        {
            //是否停止，如果停止，那么就直接跳出循环
            if(m_isStop)
            {
                // break;
            }

            while((ADS1256_Scan() == 0));
            for (i = 0; i < ch_num; i++)
            {
                adc[i] = ADS1256_GetAdc(i);
                volt[i] = (adc[i] * 100) / 167;

            }


                    for(i = 0; i < ch_num; i++)
                    {
                        qDebug()  << "volt[i]::"  << i << " ge ::" << volt[i];
                    }

            for (i = 0; i < ch_num; i++)
            {
                buf[0] = ((uint32_t)adc[i] >> 16) & 0xFF;
                buf[1] = ((uint32_t)adc[i] >> 8) & 0xFF;
                buf[2] = ((uint32_t)adc[i] >> 0) & 0xFF;
                //            printf("%d=%02X%02X%02X, %8ld", (int)i, (int)buf[0],
                //                    (int)buf[1], (int)buf[2], (long)adc[i]);

                iTemp = volt[i];	/* uV  */
                if (iTemp < 0)
                {
                    iTemp = -iTemp;
                    //printf(" (-%ld.%03ld %03ld Vaaaxxx) \r\n", iTemp /1000000, (iTemp%1000000)/1000, iTemp%1000);

                    QString str1 = QString::number(iTemp /1000000);
                    QString str2 = QString::number((iTemp%1000000)/1000);
                    QString str3 = QString::number(iTemp%1000);

                    QString str4 = str1 + "."  + str2 + str3;

                    str4 = str4.left(7);

                    //str5 = str4 + "::" +QString::number((int)ch_num);

                    str4Flaot  = str4.toFloat();

                }
                else
                {
                    //printf(" ( %ld.%03ld %03ld Vaaaxxx) \r\n", iTemp /1000000, (iTemp%1000000)/1000, iTemp%1000);

                    QString str1 = QString::number(iTemp /1000000);
                    QString str2 = QString::number((iTemp%1000000)/1000);
                    QString str3 = QString::number(iTemp%1000);

                    QString str4 = str1 + "."  + str2 + str3;

                    str4 = str4.left(7);

                    //str5 = str4 + "::" +QString::number((int)ch_num);

                    str4Flaot = str4.toFloat();
                }
                //把数据放到list容器里
                if(intList.count() < 17)
                {
                    intList.append(str4Flaot);
                    qDebug()  << "zhixingle7...................... ";
                    //strLsit.append(str5);
                }

                //如果说存够8个了，那就直接把数据发送出去
                if(intList.count() == 16)
                {
                    qDebug()  << "zhixingle8..................... ";


                    for(int i = 0;i < intList.count();i++)
                    {
                        qDebug()  << "shuju:::" << i << "  xxxx"        << intList[i];
                        //在这个里面进行判断，如果里面有数据等于0，那就不进行数据绘图
                        if(intList[i] < 0.1 || intList[i] > 5.5)
                        {
                            qDebug()  << "zhixingl9..................... ";
                            intList.clear();
                            strLsit.clear();
                            m_isSend = false;
                            break; //跳出循环
                        }
                        else
                        {
                            m_isSend = true;
                        }
                    }

                    if( m_isSend)
                    {
                        qDebug()  << "zhixingle10000..................... ";
                        m_isShow = true;
                        emit updateChartDataSignalFunction(intList[0],intList[1],intList[2],intList[3],intList[4],intList[5],intList[6],intList[7],intList[8],intList[9],intList[10],intList[11],intList[12],intList[13],intList[14],intList[15]);


                        //把容器清空
                        intList.clear();
                        strLsit.clear();
                        m_isSend = true;
                    }

                }
            }
            // printf("\33[%dA", (int)ch_num);
            bsp_DelayUS(100000);
        }



    bcm2835_spi_end();
    bcm2835_close();
    m_isStopThread = 1;
}
