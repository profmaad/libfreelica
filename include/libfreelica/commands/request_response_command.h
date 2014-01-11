# ifndef LIBFREELICA_REQUEST_RESPONSE_COMMAND_H
# define LIBFREELICA_REQUEST_RESPONSE_COMMAND_H

# include <string>
# include <list>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class RequestResponseCommand : public Command
	{
	public:
		RequestResponseCommand(const uint8_t *payload, const unsigned int payload_size);
		RequestResponseCommand(const uint8_t *idm);
		~RequestResponseCommand();

		std::string toString();

		CommandCode code() { return REQUEST_RESPONSE; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
	};
}

# endif /*LIBFREELICA_REQUEST_RESPONSE_COMMAND_H*/
