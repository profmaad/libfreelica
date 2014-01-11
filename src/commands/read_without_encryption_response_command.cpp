# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/read_without_encryption_response_command.h>

namespace FreeliCa
{
	ReadWithoutEncryptionResponseCommand::ReadWithoutEncryptionResponseCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		this->status_flag_1 = 0x00;
		this->status_flag_2 = 0x00;
		this->number_of_blocks = 0;
		this->block_data = NULL;

		parse(payload, payload_size);
	}
	ReadWithoutEncryptionResponseCommand::~ReadWithoutEncryptionResponseCommand()
	{
		free(block_data);
	}

	std::string ReadWithoutEncryptionResponseCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Read without Encryption Response:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;

		builder << "\tStatus Flag 1: " << format_status_flag_1(status_flag_1) << std::endl;
		if(!is_status_success(status_flag_1, status_flag_2))
		{
			builder << "\tError Code: " << format_status_flag_2(status_flag_2) << std::endl;
		}
		else
		{
			builder << "\tNumber of Blocks read: " << (unsigned int)number_of_blocks << std::endl;
			builder << "\tBlock Data:"<<std::endl;

			for(int i = 0; i < number_of_blocks; i++)
			{
				builder << "\t\tBlock " << std::setw(2) << std::setfill('0') << i <<": " << bytes_to_hex(block_data+16*i, 16) << std::endl;
			}
		}		

		return builder.str();
	}

	void ReadWithoutEncryptionResponseCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 10) { return; }

		free(block_data);

		memcpy(idm, payload, 8);
		status_flag_1 = payload[8];
		status_flag_2 = payload[9];

		if(status_flag_1 == 0x00)
		{
			number_of_blocks = payload[10];
			
			block_data = (uint8_t*)malloc(16*number_of_blocks);
			if(block_data)
			{
				memcpy(block_data, payload+11, 16*number_of_blocks);
			}
		}
	}

	uint8_t* ReadWithoutEncryptionResponseCommand::encode(unsigned int *size)
	{
		*size = 11 + (status_flag_1 == 0x00 ? 0 : 1+16*number_of_blocks);
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);
		encoded_command[9] = status_flag_1;
		encoded_command[10] = status_flag_2;

		if(status_flag_1 != 0x00)
		{
			encoded_command[11] = number_of_blocks;
			memcpy(encoded_command+12, block_data, 16*number_of_blocks);
		}

		return encoded_command;
	}

	bool ReadWithoutEncryptionResponseCommand::isSuccessful()
	{
		return is_status_success(status_flag_1, status_flag_2);
	}
	uint8_t ReadWithoutEncryptionResponseCommand::getNumberOfBlocks()
	{
		return number_of_blocks;
	}
	uint8_t* ReadWithoutEncryptionResponseCommand::getBlock(uint8_t block_number)
	{
		if(block_number >= number_of_blocks) { return NULL; }

		uint8_t *block = (uint8_t*)malloc(16);
		if(!block) { return NULL; }

		memcpy(block, block_data+2*block_number, 16);

		return block;
	}
}
