# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/request_response_command.h>

namespace FreeliCa
{
	RequestResponseCommand::RequestResponseCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);

		parse(payload, payload_size);
	}
	RequestResponseCommand::RequestResponseCommand(const uint8_t *idm) : Command(REQUEST_RESPONSE)
	{
		memcpy((void*)(this->idm), (void*)idm, 8);
	}
	RequestResponseCommand::~RequestResponseCommand()
	{
	}

	std::string RequestResponseCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Request Response:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;

		return builder.str();
	}

	void RequestResponseCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 8) { return; }

		memcpy(idm, payload, 8);
	}

	uint8_t* RequestResponseCommand::encode(unsigned int *size)
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
