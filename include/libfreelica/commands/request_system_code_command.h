# ifndef LIBFREELICA_REQUEST_SYSTEM_CODE_COMMAND_H
# define LIBFREELICA_REQUEST_SYSTEM_CODE_COMMAND_H

# include <string>
# include <list>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class RequestSystemCodeCommand : public Command
	{
	public:
		RequestSystemCodeCommand(const uint8_t *payload, const unsigned int payload_size);
		RequestSystemCodeCommand(const uint8_t *idm);
		~RequestSystemCodeCommand();

		std::string toString();

		CommandCode code() { return REQUEST_SYSTEM_CODE; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
	};
}

# endif /*LIBFREELICA_REQUEST_SYSTEM_CODE_COMMAND_H*/
