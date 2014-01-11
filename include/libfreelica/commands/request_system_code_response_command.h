# ifndef LIBFREELICA_REQUEST_SYSTEM_CODE_RESPONSE_COMMAND_H
# define LIBFREELICA_REQUEST_SYSTEM_CODE_RESPONSE_COMMAND_H

# include <string>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class RequestSystemCodeResponseCommand : public Command
	{
	public:
		RequestSystemCodeResponseCommand(const uint8_t *payload, const unsigned int payload_size);
		~RequestSystemCodeResponseCommand();

		std::string toString();

		CommandCode code() { return REQUEST_SYSTEM_CODE_RESPONSE; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint8_t number_of_systems;
		uint16_t *system_code_list;
	};
}

# endif /*LIBFREELICA_REQUEST_SYSTEM_CODE_RESPONSE_COMMAND_H*/
