#pragma once

#include <stdint.h>
#include <stddef.h>
#include <util/twi.h>
#include <avr/pgmspace.h>

// Static class for single-master I2C driver.
// Each function returns I2c::Success (0) on success and TW_STATUS on failure.
// If the wait parameter is false, the function will not wait for operation completion.
// Hence no error checking is done and the return value is I2c::Waiting.
class I2c
{
public:
	static const uint8_t Success = 1;
	static const uint8_t Waiting = 2;

	// constant compile-time value for optimization and compile-time error checking
	template<uint8_t prescaler = 1, uint32_t frequency = 400000>
	uint8_t Initialize()
	{
		static_assert(prescaler == 1 || prescaler == 4 || prescaler == 16 || prescaler == 64, "Invalid prescaler value");
		switch (prescaler)
		{
			case 1:
				TWSR = 0;
				break;
			case 4:
				TWSR = 1;
				break;
			case 16:
				TWSR = 2;
				break;
			case 64:
				TWSR = 3;
				break;
		}
		
		const uint32_t twbr = (F_CPU / frequency - 16) / (2 * prescaler);
		static_assert(twbr < 256, "TWBR out of range");
			
		TWBR = (uint8_t)twbr;
		TWCR = (1 << TWEN);
		
		return Success;
	}
	
	uint8_t Start(bool wait = true)
	{
		// Wait until the TWI module is ready
		//while ((TWCR & (1 << TWINT)) == 0);
		
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
		
		if (!wait)
			return Waiting;
		
		// Wait until the TWI module is done
		while ((TWCR & (1 << TWINT)) == 0);
			
		if (TW_STATUS == TW_START || TW_STATUS == TW_REP_START)
			return Success;
		return TW_STATUS;
	}
	
	uint8_t Stop()
	{
		// Wait until the TWI module is ready
		//while ((TWCR & (1 << TWINT)) == 0);
		
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		
		return Success;
	}
	
	// address is the 7-bit slave address without the W/R bit
	static uint8_t WriteAddress(uint8_t address, bool write, bool wait = true)
	{
		// Wait until the TWI module is ready
		//while ((TWCR & (1 << TWINT)) == 0);
		
		if (write)
			TWDR = address << 1;
		else
			TWDR = (address << 1) | 1;
		TWCR = (1 << TWINT) | (1 << TWEN);
		
		if (!wait)
			return Waiting;
		
		// Wait until the TWI module is done
		while ((TWCR & (1 << TWINT)) == 0);
		
		if (write && TW_STATUS == TW_MT_SLA_ACK)
			return Success;
		if (!write && TW_STATUS == TW_MR_SLA_ACK)
			return Success;
		return TW_STATUS;
	}
	
	uint8_t Write(uint8_t data, bool wait = true)
	{
		// Wait until the TWI module is ready
		//while ((TWCR & (1 << TWINT)) == 0);
		
		TWDR = data;
		TWCR = (1 << TWINT) | (1 << TWEN);
		
		if (!wait)
			return Waiting;
		
		// Wait until the TWI module is done
		while ((TWCR & (1 << TWINT)) == 0);
		
		if (TW_STATUS == TW_MT_DATA_ACK)
			return Success;
		return TW_STATUS;
	}
	
	uint8_t Read(uint8_t& data, bool send_ack)
	{
		// Wait until the TWI module is ready
		//while ((TWCR & (1 << TWINT)) == 0);
		
		if (send_ack)
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		else
			TWCR = (1 << TWINT) | (1 << TWEN);
			
		// Wait until the TWI module is done
		while ((TWCR & (1 << TWINT)) == 0);
		
		data = TWDR;
		
		if (send_ack && TW_STATUS == TW_MT_DATA_ACK)
			return Success;
		if (!send_ack && TW_STATUS == TW_MT_DATA_NACK)
			return Success;
		return TW_STATUS;
	}
	
	// address is the 7-bit slave address without the W/R bit
	uint8_t Transmit(uint8_t address, const uint8_t* data, size_t length)
	{
		uint8_t status;
		
		status = Start();
		if (status != Success)
			return status;
			
		status = WriteAddress(address, true);
		if (status != Success)
			return status;
			
		for (size_t i = 0; i < length - 1; i++)
		{
			uint8_t status = Write(data[i]);
			if (status != Success)
				return status;
		}
		
		status = Stop();
		return Success;
	}
	
	// address is the 7-bit slave address without the W/R bit
	uint8_t TransmitFromFlash(uint8_t address, const uint8_t* data, size_t length)
	{
		uint8_t status;
		
		status = Start();
		if (status != Success)
			return status;
			
		status = WriteAddress(address, true);
		if (status != Success)
			return status;
			
		for (size_t i = 0; i < length - 1; i++)
		{
			uint8_t status = Write(pgm_read_byte(&data[i]));
			if (status != Success)
				return status;
		}
		
		status = Stop();
		return Success;
	}
};

extern I2c i2c;
