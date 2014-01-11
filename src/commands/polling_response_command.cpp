# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cmath>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/polling_response_command.h>

namespace FreeliCa
{
	PollingResponseCommand::PollingResponseCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		memset(pmm, 0, 8);
		memset(request_data, 0, 2);
		
		parse(payload, payload_size);
	}
	PollingResponseCommand::~PollingResponseCommand()
	{
	}

	std::string PollingResponseCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Polling Response:" << std::endl;

		builder << "\tSystem Number: " << (unsigned int)system_number << std::endl;
		builder << "\tManufacturer Code: " << bytes_to_hex(manufacturer_code, 2) << std::endl;
		builder << "\tCard Identification Number: " << bytes_to_hex(idm+2, 6) << std::endl;
		builder << "\tIC Code (ROM Type): " << bytes_to_hex(pmm, 1) << std::endl;
		builder << "\tIC Code (IC Type): " << bytes_to_hex(pmm+1, 1) << std::endl;

		builder << "\tMaximum Response Times:" << std::endl;
		builder << "\t\tRequest Service: " << formatMaximumResponseTime(REQUEST_SERVICE_GROUP) << std::endl;
		builder << "\t\tFixed Time: " << formatMaximumResponseTime(FIXED_GROUP) << std::endl;
		builder << "\t\tMutual Authentication: " << formatMaximumResponseTime(AUTHENTICATION_GROUP) << std::endl;
		builder << "\t\tData Read: " << formatMaximumResponseTime(DATA_READ_GROUP) << std::endl;
		builder << "\t\tData Write: " << formatMaximumResponseTime(DATA_WRITE_GROUP) << std::endl;
		builder << "\t\tIssuance: " << formatMaximumResponseTime(ISSUANCE_GROUP) << std::endl;

		if(request_data_received)
		{
			// simple heuristic, fails for system codes like 0x00 0x01 :/
			if(request_data[0] == 0 && (request_data[1] & 0x7C) == 0)
			{
				// assuming this was a comm performance request
				builder << "\tCommunication Performance:" << std::endl;
				builder << "\tAvailable speeds: " << ((request_data[1] & 0x01) > 0 ? "212kbs, " : "") << ((request_data[1] & 0x02) > 0 ? "424kbs, " : "") << std::endl;
				builder << "\tCommunication Rate Autodetection Compliant?: " << ((request_data[1] & 0x80) > 0 ? "Yes" : "No") << std::endl;
			}
			else
			{
				// assuming this was a system code request
				builder << "\tSystem Code: " << bytes_to_hex(request_data, 2) << std::endl;
			}
		}

		return builder.str();
	}

	void PollingResponseCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 16) { return; }

		memcpy(idm, payload, 8);
		memcpy(pmm, payload+8, 8);
		if(payload_size >= 18)
		{
			memcpy(request_data, payload+16, 2);
			request_data_received = true;
		}
		else
		{
			memset(request_data, 0, 2);
			request_data_received = false;
		}

		system_number = ((idm[0] & 0xf0) >> 4);

		manufacturer_code[0] = (uint8_t)(idm[0] & 0x0f);
		manufacturer_code[1] = idm[1];
	}

	uint8_t PollingResponseCommand::maximumResponseTimeExponent(MaximumResponseTimeGroup group)
	{
		return (pmm[group] & 0xC0) >> 6;
	}
	uint8_t PollingResponseCommand::maximumResponseTimeRealB(MaximumResponseTimeGroup group)
	{
		return (pmm[group] & 0x38) >> 3;
	}
	uint8_t PollingResponseCommand::maximumResponseTimeRealA(MaximumResponseTimeGroup group)
	{
		return (pmm[group] & 0x07);
	}
	std::string PollingResponseCommand::formatMaximumResponseTime(MaximumResponseTimeGroup group)
	{
		std::stringstream builder;

		uint8_t exponent = maximumResponseTimeExponent(group);
		uint8_t real_a = maximumResponseTimeRealA(group);
		uint8_t real_b = maximumResponseTimeRealB(group);

		double b_factor = LIBFREELICA_MAXIMUM_RESPONSE_TIME_T*pow(4.0,(double)exponent)*(double)(real_b+1);
		double a_factor = LIBFREELICA_MAXIMUM_RESPONSE_TIME_T*pow(4.0,(double)exponent)*(double)(real_a+1);		
		
		builder << a_factor << "ms";

		if(group == REQUEST_SERVICE_GROUP || group == AUTHENTICATION_GROUP)
		{
			builder << " + " << b_factor << "ms * #node";
		}
		else if(group == DATA_READ_GROUP || group == DATA_WRITE_GROUP)
		{
			builder << " + " << b_factor << "ms * #block";
		}

		return builder.str();
	}

	uint8_t* PollingResponseCommand::encode(unsigned int *size)
	{
		*size = 17 + (request_data_received ? 2 : 0);
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);
		memcpy(encoded_command+9, pmm, 8);
		if(request_data_received)
		{
			memcpy(encoded_command+17, request_data, 2);
		}

		return encoded_command;
	}
}
