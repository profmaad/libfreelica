# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/search_service_code_command.h>

namespace FreeliCa
{
	SearchServiceCodeCommand::SearchServiceCodeCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		this->index = 0;

		parse(payload, payload_size);
	}
	SearchServiceCodeCommand::SearchServiceCodeCommand(const uint8_t *idm, const uint16_t index) : Command(SEARCH_SERVICE_CODE)
	{
		memcpy((void*)(this->idm), (void*)idm, 8);
		this->index = index;
	}
	SearchServiceCodeCommand::~SearchServiceCodeCommand()
	{
	}

	std::string SearchServiceCodeCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Search Service Code:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;
		builder << "\tIndex: " << (unsigned int)index << std::endl;

		return builder.str();
	}

	void SearchServiceCodeCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 10) { return; }

		memcpy(idm, payload, 8);

		this->index = le_8to16(payload[8], payload[9]);
	}

	uint8_t* SearchServiceCodeCommand::encode(unsigned int *size)
	{
		*size = 11;
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);

		uint16_t index_le = htobe_16(index);
		encoded_command[9] = (index_le & 0xff00) >> 8;
		encoded_command[10] = (index_le & 0x00ff);

		return encoded_command;
	}
}
