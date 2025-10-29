/**
AT24CX.h
Library for using the EEPROM AT24C32/64
https://github.com/cyberp/AT24Cx
 */
#ifndef AT24CX_h
#define AT24CX_h

// includes
#include <Arduino.h>

// byte
typedef uint8_t byte;

// AT24Cx I2C adress
// 80
// 0x50
#define AT24CX_ID B1010000

// general class definition
class AT24CX {
public:
	AT24CX();
	AT24CX(byte index, byte pageSize);
	void write(unsigned int address, byte data);
	void write(unsigned int address, byte *data, int n);
	void writeInt(unsigned int address, unsigned int data);
	void writeLong(unsigned int address, unsigned long data);
	void writeFloat(unsigned int address, float data);
	void writeDouble(unsigned int address, double data);
	void writeChars(unsigned int address, char *data, int length);
	byte read(unsigned int address);
	void read(unsigned int address, byte *data, int n);
	unsigned int readInt(unsigned int address);
	unsigned long readLong(unsigned int address);
	float readFloat(unsigned int address);
	double readDouble(unsigned int address);
	void readChars(unsigned int address, char *data, int n);
protected:
	void init(byte index, byte pageSize);
private:
	void read(unsigned int address, byte *data, int offset, int n);
	void write(unsigned int address, byte *data, int offset, int n);
	int _id;
	byte _b[8];
	byte _pageSize;
};

// AT24C32 class definiton
class AT24C32 : public AT24CX {
public:
	AT24C32();
	AT24C32(byte index);
};

// AT24C64 class definiton
class AT24C64 : public AT24CX {
public:
	AT24C64();
	AT24C64(byte index);
};

// AT24C128 class definiton
class AT24C128 : public AT24CX {
public:
	AT24C128();
	AT24C128(byte index);
};

// AT24C256 class definiton
class AT24C256 : public AT24CX {
public:
	AT24C256();
	AT24C256(byte index);
};

// AT24C512 class definiton
class AT24C512 : public AT24CX {
public:
	AT24C512();
	AT24C512(byte index);
};

#endif