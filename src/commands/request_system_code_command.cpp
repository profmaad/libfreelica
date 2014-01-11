# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/request_system_code_command.h>

namespace FreeliCa
{
	RequestSystemCodeCommand::RequestSystemCodeCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);

		parse(payload, payload_size);
	}
	RequestSystemCodeCommand::RequestSystemCodeCommand(const uint8_t *idm) : Command(REQUEST_SYSTEM_CODE)
	{
		memcpy((void*)(this->idm), (void*)idm, 8);
	}
	RequestSystemCodeCommand::~RequestSystemCodeCommand()
	{
	}

	std::string RequestSystemCodeCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Request System Code:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;

		return builder.str();
	}

	void RequestSystemCodeCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 8) { return; }

		memcpy(idm, payload, 8);
	}

	uint8_t* RequestSystemCodeCommand::encode(unsigned int *size)
	{
		*size = 9;
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);

		return encoded_command;
	}
}
