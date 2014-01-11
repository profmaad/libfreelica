# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/authentication_2_response_command.h>

namespace FreeliCa
{
	Authentication2ResponseCommand::Authentication2ResponseCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		memset(idi, 0, 8);
		memset(pmi, 0, 8);

		parse(payload, payload_size);
	}
	Authentication2ResponseCommand::~Authentication2ResponseCommand()
	{
	}

	std::string Authentication2ResponseCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Authentication 2 Response:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;
		builder << "\tIDi: " << bytes_to_hex(idi, 8) << std::endl;
		builder << "\tPMi: " << bytes_to_hex(pmi, 8) << std::endl;

		return builder.str();
	}

	void Authentication2ResponseCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 24) { return; }

		memcpy(idm, payload, 8);
		memcpy(idi, payload+8, 8);
		memcpy(pmi, payload+16, 8);
	}


	uint8_t* Authentication2ResponseCommand::encode(unsigned int *size)
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
		memcpy(encoded_command+9, idi, 8);
		memcpy(encoded_command+17, pmi, 8);

		return encoded_command;
	}
}
