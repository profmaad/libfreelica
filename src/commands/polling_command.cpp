# include <iostream>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/polling_command.h>

namespace FreeliCa
{
	PollingCommand::PollingCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(system_code, 0, 2);
		request_code = 0x0;
		time_slot = 0x0;

		parse(payload, payload_size);
	}
	PollingCommand::PollingCommand(const uint16_t system_code, const RequestCode request_code, const TimeSlots timeslots) :
		Command(POLLING)
	{
		this->system_code[0] = (system_code & 0xff00) >> 8;
		this->system_code[1] = (system_code & 0x00ff);
		this->request_code = request_code;
		this->time_slot = timeslots;
	}
	PollingCommand::~PollingCommand()
	{
	}

	std::string PollingCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Polling:" << std::endl;
		builder << "\tSystem Code: " << bytes_to_hex(system_code, 2) << std::endl;
		builder << "\tRequest Code: ";
		switch(request_code)
		{
		case NO_REQUEST:
			builder << "no request (0x00)";
			break;
		case SYSTEM_CODE_REQUEST:
			builder << "system code request (0x01)";
			break;
		case COMMUNICATION_PERFORMANCE_REQUEST:
			builder << "communication performance request (0x02)";
			break;
		default:
			builder << "unknwon request (" << bytes_to_hex(&request_code, 1) << ")";
		}
		builder << std::endl;
		
		builder << "\tTime Slot: " << (time_slot+1) << " slots (" << bytes_to_hex(&time_slot, 1) << ")" << std::endl;

		return builder.str();
	}

	void PollingCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 4) { return; }

		memcpy(system_code, payload, 2);
		request_code = *(payload+2);
		time_slot = *(payload+3);
	}

	uint8_t* PollingCommand::encode(unsigned int *size)
	{
		*size = 5;
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, system_code, 2);
		encoded_command[3] = request_code;
		encoded_command[4] = time_slot;

		return encoded_command;
	}
}
