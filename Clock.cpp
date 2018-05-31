// 
// 
// 

#include "Clock.h"


namespace {

	enum Register {
		kSecondReg = 0,
		kMinuteReg = 1,
		kHourReg = 2,
		kDateReg = 3,
		kMonthReg = 4,
		kDayReg = 5,
		kYearReg = 6,
		kWriteProtectReg = 7,

		// The RAM register space follows the clock register space.
		kRamAddress0 = 32
	};

	enum Command {
		kClockBurstRead = 0xBF,
		kClockBurstWrite = 0xBE,
		kRamBurstRead = 0xFF,
		kRamBurstWrite = 0xFE
	};

	// Establishes and terminates a three-wire SPI session.
	class SPISession {
	public:
		SPISession(const uint8_t rs_pin, const uint8_t dat_pin, const uint8_t clk_pin)
			: rst_pin(rs_pin)//, dat_pin(d_pin), clk_pin(k_pin)
		{
			digitalWrite(clk_pin, LOW);
			digitalWrite(rs_pin, HIGH);
			delayMicroseconds(4);  // tCC
		}
		~SPISession() {
			digitalWrite(rst_pin, LOW);
			delayMicroseconds(4);  // tCWH
		}

	private:
		const uint8_t rst_pin;
		//const int dat_pin;
		//const int clk_pin;
	};

	// Returns the decoded decimal value from a binary-coded decimal (BCD) byte.
	// Assumes 'bcd' is coded with 4-bits per digit, with the tens place digit in
	// the upper 4 MSBs.
	uint8_t bcdToDec(const uint8_t bcd) {
		return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
	}

	// Returns the binary-coded decimal of 'dec'. Inverse of bcdToDec.
	uint8_t decToBcd(const uint8_t dec) {
		const uint8_t tens = dec / 10;
		const uint8_t ones = dec % 10;
		return (tens << 4) | ones;
	}

	// Returns the hour in 24-hour format from the hour register value.
	uint8_t hourFromRegisterValue(const uint8_t value) {
		uint8_t adj;
		if (value & 128)  // 12-hour mode
			adj = 12 * ((value & 32) >> 5);
		else           // 24-hour mode
			adj = 10 * ((value & (32 + 16)) >> 4);
		return (value & 15) + adj;
	}

}  // namespace


Time_::Time_(const uint16_t yr, const uint8_t mon, const uint8_t date,
	const uint8_t hr, const uint8_t min, const uint8_t sec, const uint8_t day) {
	Time_(yr, mon, date, hr, min, sec);
	this->day = day;
}

Time_::Time_(const uint16_t yr, const uint8_t mon, const uint8_t date,
	const uint8_t hr, const uint8_t min, const uint8_t sec) {
	this->year = yr >= 2000 ? yr - 2000 : yr;
	this->mon = mon;
	this->date = date;
	this->hour = hr;
	this->min = min;
	this->sec = sec;
	this->day = 1;
}

char * Clock::get_date_str() {
	//const SPISession s(RST_PIN, DAT_PIN, CLK_PIN);

	//writeOut(kClockBurstRead, true);
	//readIn();
	//readIn();
	//readIn();
	//uint8_t date = bcdToDec(readIn());
	//uint8_t mon = bcdToDec(readIn());
	uint8_t date = bcdToDec(readRegister(kDateReg));
	uint8_t mon = bcdToDec(readRegister(kMonthReg));
	uint16_t year = 2000+bcdToDec(readRegister(kYearReg));

	char buf[11];
	snprintf(buf, sizeof(buf), "%02d-%02d-%04d", date, mon, year);
	return buf;
}

char * Clock::get_time_str() {
	const SPISession s(RST_PIN, DAT_PIN, CLK_PIN);
	writeOut(kClockBurstRead, true);
	uint8_t sec = bcdToDec(readIn() & 0x7F);
	uint8_t min = bcdToDec(readIn());
	uint8_t hour = hourFromRegisterValue(readIn());
	char buf[9];
	snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hour, min, sec);
	return buf;
}

void Clock::setup(const uint8_t ce_pin, const uint8_t io_pin, const uint8_t sclk_pin) {
	RST_PIN = ce_pin;
	DAT_PIN = io_pin;
	CLK_PIN = sclk_pin;

	pinMode(RST_PIN, OUTPUT);
	digitalWrite(RST_PIN, LOW);
	pinMode(DAT_PIN, INPUT);
	pinMode(CLK_PIN, OUTPUT);
	digitalWrite(CLK_PIN, LOW);
}

void Clock::writeOut(const uint8_t value, bool readAfter) {
	pinMode(DAT_PIN, OUTPUT);

	for (int i = 0; i < 8; ++i) {
		digitalWrite(DAT_PIN, (value >> i) & 1);
		delayMicroseconds(1);
		digitalWrite(CLK_PIN, HIGH);
		delayMicroseconds(1);

		if (readAfter && i == 7) {
			// We're about to read data -- ensure the pin is back in input mode
			// before the clock is lowered.
			pinMode(DAT_PIN, INPUT);
		}
		else {
			digitalWrite(CLK_PIN, LOW);
			delayMicroseconds(1);
		}
	}
}

uint8_t Clock::readIn() {
	uint8_t input_value = 0;
	uint8_t bit = 0;
	pinMode(DAT_PIN, INPUT);

	// Bits from the DS1302 are output on the falling edge of the clock
	// cycle. This is called after readIn (which will leave the clock low) or
	// writeOut(..., true) (which will leave it high).
	for (int i = 0; i < 8; ++i) {
		digitalWrite(CLK_PIN, HIGH);
		delayMicroseconds(1);
		digitalWrite(CLK_PIN, LOW);
		delayMicroseconds(1);

		bit = digitalRead(DAT_PIN);
		input_value |= (bit << i);  // Bits are read LSB first.
	}

	return input_value;
}

uint8_t Clock::readRegister(const uint8_t reg) {
	const SPISession s(RST_PIN, DAT_PIN, CLK_PIN);

	const uint8_t cmd_byte = (0x81 | (reg << 1));
	writeOut(cmd_byte, true);
	return readIn();
}

void Clock::writeRegister(const uint8_t reg, const uint8_t value) {
	const SPISession s(RST_PIN, DAT_PIN, CLK_PIN);

	const uint8_t cmd_byte = (0x80 | (reg << 1));
	writeOut(cmd_byte);
	writeOut(value);
}

void Clock::writeProtect(const bool enable) {
	writeRegister(kWriteProtectReg, (enable << 7));
}

void Clock::halt(const bool enable) {
	uint8_t sec = readRegister(kSecondReg);
	sec &= ~(1 << 7);
	sec |= (enable << 7);
	writeRegister(kSecondReg, sec);
}

Time_ Clock::time() {
	const SPISession s(RST_PIN, DAT_PIN, CLK_PIN);

	Time_ t(2099, 1, 1, 0, 0, 0, 1);
	writeOut(kClockBurstRead, true);
	t.sec = bcdToDec(readIn() & 0x7F);
	t.min = bcdToDec(readIn());
	t.hour = hourFromRegisterValue(readIn());
	t.date = bcdToDec(readIn());
	t.mon = bcdToDec(readIn());
	t.day = bcdToDec(readIn());
	t.year = bcdToDec(readIn());
	return t;
}

void Clock::time(const Time_ t) {
	// We want to maintain the Clock Halt flag if it is set.
	const uint8_t ch_value = readRegister(kSecondReg) & 0x80;

	const SPISession s(RST_PIN, DAT_PIN, CLK_PIN);

	writeOut(kClockBurstWrite);
	writeOut(ch_value | decToBcd(t.sec));
	writeOut(decToBcd(t.min));
	writeOut(decToBcd(t.hour));
	writeOut(decToBcd(t.date));
	writeOut(decToBcd(t.mon));
	writeOut(decToBcd(t.day));
	writeOut(decToBcd(t.year));
	// All clock registers *and* the WP register have to be written for the time
	// to be set.
	writeOut(0);  // Write protection register.
}

void Clock::setDate(uint8_t d, uint8_t m, uint8_t y) {

}

//RAM OPERATION

void Clock::writeRam(const uint8_t address, const uint8_t value) {
	if (address >= kRamSize) {
		return;
	}

	writeRegister(kRamAddress0 + address, value);
}

uint8_t Clock::readRam(const uint8_t address) {
	if (address >= kRamSize) {
		return 0;
	}

	return readRegister(kRamAddress0 + address);
}

void Clock::writeRamBulk(const uint8_t* const data, int len) {
	if (len <= 0) {
		return;
	}
	if (len > kRamSize) {
		len = kRamSize;
	}

	const SPISession s(RST_PIN, DAT_PIN, CLK_PIN);

	writeOut(kRamBurstWrite);
	for (int i = 0; i < len; ++i) {
		writeOut(data[i]);
	}
}

void Clock::readRamBulk(uint8_t* const data, int len) {
	if (len <= 0) {
		return;
	}
	if (len > kRamSize) {
		len = kRamSize;
	}

	const SPISession s(RST_PIN, DAT_PIN, CLK_PIN);

	writeOut(kRamBurstRead, true);
	for (int i = 0; i < len; ++i) {
		data[i] = readIn();
	}
}
