# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/request_specification_version_response_command.h>

namespace FreeliCa
{
	RequestSpecificationVersionResponseCommand::RequestSpecificationVersionResponseCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		this->status_flag_1 = 0x00;
		this->status_flag_2 = 0x00;
		this->format_version = 0x00;
		this->basic_version = 0x0000;
		this->number_of_options = 0;
		this->option_version_list = NULL;

		parse(payload, payload_size);
	}
	RequestSpecificationVersionResponseCommand::~RequestSpecificationVersionResponseCommand()
	{
		free(option_version_list);
	}

	std::string RequestSpecificationVersionResponseCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Request Specification Version Response:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;

		builder << "\tStatus Flag 1: " << format_status_flag_1(status_flag_1) << std::endl;
		if(!is_status_success(status_flag_1, status_flag_2))
		{
			builder << "\tError Code: " << format_status_flag_2(status_flag_2) << std::endl;
		}
		else
		{
			builder << "\tFormat Version: " << format_version << std::endl;
			builder << "\tBasic Version: " << format_bcd_version(basic_version) << std::endl;

			if(number_of_options == 0)
			{
				builder << "\tCard Type: AES Card" << std::endl;
			}
			else if(number_of_options == 1)
			{
				builder << "\tCard Type: AES/DES Card" << std::endl;
				builder << "\tDES Option Version: " << format_bcd_version(option_version_list[0]) << std::endl;
			}
			else
			{
				builder << "\tCard Type: Unknown (" << bytes_to_hex(&number_of_options, 1) << ")" << std::endl;
			}
		}
		

		return builder.str();
	}

	void RequestSpecificationVersionResponseCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 10) { return; }

		free(option_version_list);

		memcpy(idm, payload, 8);
		status_flag_1 = payload[8];
		status_flag_2 = payload[9];

		if(status_flag_1 == 0x00 && payload_size >= 14)
		{
			format_version = payload[10];

			// data on the wire is little endian, but we want to store it as big endian (even on a LE system, since this is the way we parse it			
			basic_version = le_8to16(*(payload+11+1), *(payload+11));

			number_of_options = payload[13];
			if(payload_size >= 14 + 2*number_of_options)
			{
				option_version_list = (uint16_t*)malloc(2*number_of_options);
				if(option_version_list)
				{
					for(int i = 0; i < number_of_options; i++)
					{
						// data on the wire is little endian, but we want to store it as big endian (even on a LE system, since this is the way we parse it			
						option_version_list[i] = le_8to16(*(payload+14+2*i+1), *(payload+14+2*i));
						
					}
				}
			}
		}
	}

	uint8_t* RequestSpecificationVersionResponseCommand::encode(unsigned int *size)
	{
		*size = 11 + (status_flag_1 == 0x00 ? 0 : 4+2*number_of_options);
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
			encoded_command[11] = format_version;

			uint16_t basic_version_le = betole_16(basic_version);
			encoded_command[12] = (uint8_t)((basic_version_le & 0xff00) >> 8);
			encoded_command[13] = (uint8_t)(basic_version_le & 0x00ff);

			encoded_command[14] = number_of_options;

			for(int i = 0; i < number_of_options; i++)
			{
				uint16_t option_version_le = betole_16(option_version_list[i]);
				encoded_command[14+2*i] = (uint8_t)((option_version_le & 0xff00) >> 8);
				encoded_command[14+2*i+1] = (uint8_t)(option_version_le & 0x00ff);
			}
		}

		return encoded_command;
	}
}
