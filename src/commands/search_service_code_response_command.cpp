# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/search_service_code_response_command.h>

namespace FreeliCa
{
	SearchServiceCodeResponseCommand::SearchServiceCodeResponseCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		this->service_code = 0x0000;
		this->area_end_code = 0x0000;
		this->is_area = false;

		parse(payload, payload_size);
	}
	SearchServiceCodeResponseCommand::~SearchServiceCodeResponseCommand()
	{
	}

	std::string SearchServiceCodeResponseCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Search Service Code Response:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;
		builder << "\tService Code: " << format_node_code(service_code);
		if(is_area)
		{
			uint8_t area_end_code_bytes[] = {
				(area_end_code & 0xff00) >> 8,
				(area_end_code & 0x00ff)
			};
			builder << "\tArea End: "<< bytes_to_hex(area_end_code_bytes, 2) << std::endl;
		}

		return builder.str();
	}

	void SearchServiceCodeResponseCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 10) { return; }

		memcpy(idm, payload, 8);
		service_code = le_8to16(*(payload+9), *(payload+8));

		if(payload_size >= 12)
		{
			area_end_code = le_8to16(*(payload+11), *(payload+10));
			is_area = true;
		}
		else
		{
			is_area = false;
		}
	}

	uint8_t* SearchServiceCodeResponseCommand::encode(unsigned int *size)
	{
		*size = 11 + (is_area ? 2 : 0);
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);

		uint16_t service_code_le = betole_16(service_code);
		encoded_command[9] = (uint8_t)((service_code_le & 0xff00) >> 8);
		encoded_command[10] = (uint8_t)(service_code_le & 0x00ff);

		if(is_area)
		{			
			uint16_t area_end_code_le = betole_16(area_end_code);
			encoded_command[11] = (uint8_t)((area_end_code_le & 0xff00) >> 8);
			encoded_command[12] = (uint8_t)(area_end_code_le & 0x00ff);
		}

		return encoded_command;
	}

	bool SearchServiceCodeResponseCommand::isSearchDone()
	{
		return !is_area && service_code == 0xffff;
	}
}
