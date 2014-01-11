# include <stdint.h>

# include <libfreelica/constants.h>

# include <libfreelica/commands/command.h>
# include <libfreelica/commands.h>

namespace FreeliCa
{
	Command* Command::create(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 1) { return NULL; }

		uint8_t command_code = payload[0];
		return create(command_code, payload+1, payload_size-1);
	}
	Command* Command::create(uint8_t command_code, const uint8_t *payload, const unsigned int payload_size)
	{
		Command *command = NULL;

		switch(command_code)
		{
		case FreeliCa::POLLING:
			command = new PollingCommand(payload, payload_size);
			break;
		case FreeliCa::REQUEST_SERVICE:
			command = new RequestServiceCommand(payload, payload_size);
			break;
		case FreeliCa::REQUEST_RESPONSE:
			command = new RequestResponseCommand(payload, payload_size);
			break;
		case FreeliCa::READ_WITHOUT_ENCRYPTION:
			command = new ReadWithoutEncryptionCommand(payload, payload_size);
			break;
		case FreeliCa::WRITE_WITHOUT_ENCRYPTION:
		case FreeliCa::SEARCH_SERVICE_CODE:
			command = new SearchServiceCodeCommand(payload, payload_size);
			break;
		case FreeliCa::REQUEST_SYSTEM_CODE:
			command = new RequestSystemCodeCommand(payload, payload_size);
			break;
		case FreeliCa::AUTHENTICATION_1:
			command = new Authentication1Command(payload, payload_size);
			break;
		case FreeliCa::AUTHENTICATION_2:
			command = new Authentication2Command(payload, payload_size);
			break;
		case FreeliCa::READ:
		case FreeliCa::WRITE:
		case FreeliCa::REQUEST_SERVICE_V2:
		case FreeliCa::GET_SYSTEM_STATUS:
		case FreeliCa::REQUEST_SPECIFICATION_VERSION:
			command = new RequestSpecificationVersionCommand(payload, payload_size);
			break;
		case FreeliCa::RESET_MODE:
		case FreeliCa::AUTHENTICATION_1_V2:
		case FreeliCa::AUTHENTICATION_2_V2:
		case FreeliCa::READ_V2:
		case FreeliCa::WRITE_V2:
		case FreeliCa::UPDATE_RANDOM_ID:
			command = new Command(payload, payload_size);
			break;

		case FreeliCa::POLLING_RESPONSE:
			command = new PollingResponseCommand(payload, payload_size);
			break;
		case FreeliCa::REQUEST_SERVICE_RESPONSE:
			command = new RequestServiceResponseCommand(payload, payload_size);
			break;
		case FreeliCa::REQUEST_RESPONSE_RESPONSE:
			command = new RequestResponseResponseCommand(payload, payload_size);
			break;
		case FreeliCa::READ_WITHOUT_ENCRYPTION_RESPONSE:
			command = new ReadWithoutEncryptionResponseCommand(payload, payload_size);
			break;
		case FreeliCa::WRITE_WITHOUT_ENCRYPTION_RESPONSE:
		case FreeliCa::SEARCH_SERVICE_CODE_RESPONSE:
			command = new SearchServiceCodeResponseCommand(payload, payload_size);
			break;
		case FreeliCa::REQUEST_SYSTEM_CODE_RESPONSE:
			command = new RequestSystemCodeResponseCommand(payload, payload_size);
			break;
		case FreeliCa::AUTHENTICATION_1_RESPONSE:
			command = new Authentication1ResponseCommand(payload, payload_size);
			break;
		case FreeliCa::AUTHENTICATION_2_RESPONSE:
			command = new Authentication2ResponseCommand(payload, payload_size);
			break;
		case FreeliCa::READ_RESPONSE:
		case FreeliCa::WRITE_RESPONSE:
		case FreeliCa::REQUEST_SERVICE_V2_RESPONSE:
		case FreeliCa::GET_SYSTEM_STATUS_RESPONSE:
		case FreeliCa::REQUEST_SPECIFICATION_VERSION_RESPONSE:
			command = new RequestSpecificationVersionResponseCommand(payload, payload_size);
			break;
		case FreeliCa::RESET_MODE_RESPONSE:
		case FreeliCa::AUTHENTICATION_1_V2_RESPONSE:
		case FreeliCa::AUTHENTICATION_2_V2_RESPONSE:
		case FreeliCa::READ_V2_RESPONSE:
		case FreeliCa::WRITE_V2_RESPONSE:
		case FreeliCa::UPDATE_RANDOM_ID_RESPONSE:
			command = new Command(payload, payload_size);
			break;
		default:
			command = new Command(payload, payload_size);
			break;
		}

		command->setCode((CommandCode)command_code);

		return command;
	}
}
