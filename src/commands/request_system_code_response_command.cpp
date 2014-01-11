# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/request_system_code_response_command.h>

namespace FreeliCa
{
	RequestSystemCodeResponseCommand::RequestSystemCodeResponseCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		number_of_systems = 0x0;
		system_code_list = NULL;

		parse(payload, payload_size);
	}
	RequestSystemCodeResponseCommand::~RequestSystemCodeResponseCommand()
	{
		free(system_code_list);
	}

	std::string RequestSystemCodeResponseCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Request System Code Response:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;
		builder << "\tNumber of Systems in Response: " << (unsigned int)number_of_systems << std::endl;
		if(number_of_systems > 0)
		{
			builder << "\tSystem Codes:" << std::endl;
			for(int i = 0; i < number_of_systems; i++)
			{
				uint8_t system_code[2] = {
					(system_code_list[i] & 0xff00) >> 8,
					(system_code_list[i] & 0x00ff)
				};

				builder << "\t\t"<< std::setw(2) << std::setfill('0') << i << ": " << bytes_to_hex(system_code, 2) << std::endl;
			}
		}

		return builder.str();
	}

	void RequestSystemCodeResponseCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 9) { return; }

		memcpy(idm, payload, 8);
		number_of_systems = *(payload+8);
		
		free(system_code_list);
		if(number_of_systems < 1) { return; }
 		system_code_list = (uint16_t*)malloc(number_of_systems*2);

		for(int i = 0; i < number_of_systems; i++)
		{
			system_code_list[i] = (*(payload+9+2*i) << 8) | *(payload+9+2*i+1);
		}
	}

	uint8_t* RequestSystemCodeResponseCommand::encode(unsigned int *size)
	{
		*size = 10+2*number_of_systems;
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);
		encoded_command[9] = number_of_systems;
		for(int i = 0; i < number_of_systems; i++)
		{
			encoded_command[10+2*i] = (uint8_t)((system_code_list[i] & 0xff00) >> 8);
			encoded_command[10+2*i+1] = (uint8_t)(system_code_list[i] & 0x00ff);
		}

		return encoded_command;
	}
}
