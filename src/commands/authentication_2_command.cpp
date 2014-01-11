# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/authentication_2_command.h>

namespace FreeliCa
{
	Authentication2Command::Authentication2Command(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		memset(challenge_2_response, 0, 8);

		parse(payload, payload_size);
	}
	Authentication2Command::~Authentication2Command()
	{
	}

	std::string Authentication2Command::toString()
		
	{
		std::stringstream builder;

		builder << "Authentication 2:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;
		builder << "\tChallenge 2 Response: " << bytes_to_hex(challenge_2_response, 8) << std::endl;

		return builder.str();
	}

	void Authentication2Command::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 16) { return; }

		memcpy(idm, payload, 8);
		memcpy(challenge_2_response, payload+8, 8);
	}

	uint8_t* Authentication2Command::encode(unsigned int *size)
	{
		*size = 17;
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);
		memcpy(encoded_command+9, challenge_2_response, 8);

		return encoded_command;
	}
}
