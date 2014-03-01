//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dtu234.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2008 DekTec
//

#ifndef _DTU234_H
#define _DTU234_H

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constants +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// DTU-234 I2C addresses
#define DTU234_IIC_ADDR_TUNER		0xC6
#define DTU234_IIC_ADDR_DEMOD		0x18
#define DTU234_IIC_ADDR_ADC			0x50
#define DTU234_IIC_ADDR_DAC			0x58
#define DTU234_IIC_ADDR_IO			0x4C
#define DTU234_IIC_ADDR_IO_EE		0xAC
#define DTU234_IIC_ADDR_EEPROM_1	0xAA
#define DTU234_IIC_ADDR_EEPROM_2	0xA8
#define DTU234_IIC_ADDR_EEPROM_3	0xAE

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helper Structs +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

typedef struct _Dtu234HexBlockStruct {
	Int  m_Address;
	Int  m_DataCount;
	UInt8  m_Data[4096];
} Dtu234HexBlockStruct;

typedef struct _Dtu234HexStruct
{
	Dtu234HexBlockStruct m_HexBlock[16];
	Int  m_HexBlockCount;
} Dtu234HexStruct;

#endif //#ifndef _DTU234_H
