# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/authentication_1_response_command.h>

namespace FreeliCa
{
	Authentication1ResponseCommand::Authentication1ResponseCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		memset(challenge_1_response, 0, 8);
		memset(challenge_2, 0, 8);

		parse(payload, payload_size);
	}
	Authentication1ResponseCommand::~Authentication1ResponseCommand()
	{
	}

	std::string Authentication1ResponseCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Authentication 1 Response:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;
		builder << "\tChallenge 1 Response: " << bytes_to_hex(challenge_1_response, 8) << std::endl;
		builder << "\tChallenge 2: " << bytes_to_hex(challenge_2, 8) << std::endl;

		return builder.str();
	}

	void Authentication1ResponseCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 24) { return; }

		memcpy(idm, payload, 8);
		memcpy(challenge_1_response, payload+8, 8);
		memcpy(challenge_2, payload+16, 8);
	}

	uint8_t* Authentication1ResponseCommand::encode(unsigned int *size)
	{
		*size = 25;
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);
		memcpy(encoded_command+9, challenge_1_response, 8);
		memcpy(encoded_command+17, challenge_2, 8);

		return encoded_command;
	}
}
