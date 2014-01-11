# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/authentication_1_command.h>

namespace FreeliCa
{
	Authentication1Command::Authentication1Command(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		number_of_areas = 0x0;
		area_code_list = NULL;
		number_of_services = 0x0;
		service_code_list = NULL;
		memset(challenge_1, 0, 8);

		parse(payload, payload_size);
	}
	Authentication1Command::~Authentication1Command()
	{
		free(area_code_list);
		free(service_code_list);
	}

	std::string Authentication1Command::toString()
		
	{
		std::stringstream builder;

		builder << "Authentication 1:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;
		builder << "\tNumber of Areas in Request: " << (unsigned int)number_of_areas << std::endl;
		if(number_of_areas > 0)
		{
			builder << "\tRequested areas:" << std::endl;
			for(int i = 0; i < number_of_areas; i++)
			{
				builder << "\t\t"<< std::setw(2) << std::setfill('0') << i <<": ";
				builder << format_node_code(area_code_list[i]) << std::dec << std::setw(0) << std::setfill(' ') << std::endl;
			}
		}
		builder << "\tNumber of Services in Request: " << (unsigned int)number_of_services << std::endl;
		if(number_of_services > 0)
		{
			builder << "\tRequested services:" << std::endl;
			for(int i = 0; i < number_of_services; i++)
			{
				builder << "\t\t"<< std::setw(2) << std::setfill('0') << i <<": ";
				builder << format_node_code(service_code_list[i]) << std::dec << std::setw(0) << std::setfill(' ') << std::endl;
			}
		}
		builder << "\tChallenge 1: " << bytes_to_hex(challenge_1, 8) << std::endl;

		return builder.str();
	}

	void Authentication1Command::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 22) { return; }

		memcpy(idm, payload, 8);

		number_of_areas = *(payload+8);		
		if(number_of_areas < 1 || number_of_areas > 32) { return; }
		free(area_code_list);
		area_code_list = (uint16_t*)malloc(number_of_areas*2);

		for(int i = 0; i < number_of_areas; i++)
		{			
			// data on the wire is little endian, but we want to store it as big endian (even on a LE system, since this is the way we parse it
			area_code_list[i] = le_8to16(*(payload+9+2*i+1), *(payload+9+2*i));
		}

		number_of_services = *(payload+9+2*number_of_areas);		
		if(number_of_services < 1 || number_of_services > 32) { return; }
		free(service_code_list);
		service_code_list = (uint16_t*)malloc(number_of_services*2);

		for(int i = 0; i < number_of_services; i++)
		{			
			// data on the wire is little endian, but we want to store it as big endian (even on a LE system, since this is the way we parse it
			service_code_list[i] = le_8to16(*(payload+10+2*number_of_areas+2*i+1), *(payload+10+2*number_of_areas+2*i));
		}

		memcpy(challenge_1, payload+8+1+2*number_of_areas+1+2*number_of_services, 8);
	}


	uint8_t* Authentication1Command::encode(unsigned int *size)
	{
		*size = 1+8+1+2*number_of_areas+1+2*number_of_services+8;
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);
		encoded_command[9] = number_of_areas;
		for(int i = 0; i < number_of_areas; i++)
		{
			uint16_t area_le = betole_16(area_code_list[i]);
			encoded_command[10+2*i] = (uint8_t)((area_le & 0xff00) >> 8);
			encoded_command[10+2*i+1] = (uint8_t)(area_le & 0x00ff);
		}
		encoded_command[10+2*number_of_areas] = number_of_services;
		for(int i = 0; i < number_of_services; i++)
		{
			uint16_t service_le = betole_16(service_code_list[i]);
			encoded_command[10+2*number_of_areas+1+2*i] = (uint8_t)((service_le & 0xff00) >> 8);
			encoded_command[10+2*number_of_areas+1+2*i+1] = (uint8_t)(service_le & 0x00ff);
		}
		memcpy(encoded_command+10+2*number_of_areas+1+2*number_of_services, challenge_1, 8);

		return encoded_command;
	}
}
