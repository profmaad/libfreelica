# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/constants.h>
# include <libfreelica/utils.h>

# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	Command::Command(const uint8_t *payload, const unsigned int payload_size) :
		payload(NULL),
		payload_size(payload_size)
	{
		this->payload = (const uint8_t*)malloc(payload_size);
		memcpy((void*)this->payload, (void*)payload, payload_size);
	}
	Command::Command(CommandCode command_code) :
		payload(NULL),
		payload_size(0)
	{
		setCode(command_code);
	}

	Command::~Command()
	{
		free((void*)this->payload);
	}

	std::string Command::toString()
	{
		std::stringstream builder;

		uint8_t command_code_uint8 = (uint8_t)command_code;
		builder << "Unknown Command " << bytes_to_hex(&command_code_uint8, 1) << " (" << payload_size << " bytes): " << bytes_to_hex(payload, payload_size);

		return builder.str();
	}

	void Command::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		// NOOP
	}

	uint8_t* Command::encode(unsigned int *size)
	{
		// of course, this only works if payload is set properly... (and it kind of doesn't really make sense, anyway)
		uint8_t *encoded_command = (uint8_t*)malloc(1+payload_size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)command_code;
		memcpy((void*)(encoded_command+1), (void*)this->payload, payload_size);

		*size = 1 + payload_size;
		return encoded_command;
	}
}
