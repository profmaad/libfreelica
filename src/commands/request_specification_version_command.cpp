# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/request_specification_version_command.h>

namespace FreeliCa
{
	RequestSpecificationVersionCommand::RequestSpecificationVersionCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);

		parse(payload, payload_size);
	}
	RequestSpecificationVersionCommand::RequestSpecificationVersionCommand(const uint8_t *idm) : Command(REQUEST_SPECIFICATION_VERSION)
	{
		memcpy((void*)(this->idm), (void*)idm, 8);
	}
	RequestSpecificationVersionCommand::~RequestSpecificationVersionCommand()
	{
	}

	std::string RequestSpecificationVersionCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Request Specification Version:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;

		return builder.str();
	}

	void RequestSpecificationVersionCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 8) { return; }

		memcpy(idm, payload, 8);

		// the command contains 2 fixed bytes after this, but they are of no significance
	}

	uint8_t* RequestSpecificationVersionCommand::encode(unsigned int *size)
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
		memset(encoded_command+9, 0, 2);

		return encoded_command;
	}
}
