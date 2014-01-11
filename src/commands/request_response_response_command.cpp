# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/request_response_response_command.h>

namespace FreeliCa
{
	RequestResponseResponseCommand::RequestResponseResponseCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		this->mode = MODE_0;

		parse(payload, payload_size);
	}
	RequestResponseResponseCommand::~RequestResponseResponseCommand()
	{
	}

	std::string RequestResponseResponseCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Request Response Response:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;
		builder << "\tMode: ";
		switch(mode)
		{
		case MODE_0:
			builder << "Mode0 (unauthenticated)";
			break;
		case MODE_1:
			builder << "Mode1 (authentication underway)";
			break;
		case MODE_2:
			builder << "Mode2 (authenticated)";
			break;
		case MODE_3:
			builder << "Mode3 (issuance)";
			break;
		default:
			builder << bytes_to_hex(&mode, 1) << " (unknown mode)";
		}
		builder << std::endl;

		return builder.str();
	}

	void RequestResponseResponseCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 9) { return; }

		memcpy(idm, payload, 8);
		mode = payload[8];
	}

	uint8_t* RequestResponseResponseCommand::encode(unsigned int *size)
	{
		*size = 10;
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);
		encoded_command[9] = mode;

		return encoded_command;
	}
}
