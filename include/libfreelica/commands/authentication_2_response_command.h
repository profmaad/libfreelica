# ifndef LIBFREELICA_AUTHENTICATION_2_REPSONSE_COMMAND_H
# define LIBFREELICA_AUTHENTICATION_2_REPSONSE_COMMAND_H

# include <string>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class Authentication2ResponseCommand : public Command
	{
	public:
		Authentication2ResponseCommand(const uint8_t *payload, const unsigned int payload_size);
		~Authentication2ResponseCommand();

		std::string toString();

		CommandCode code() { return AUTHENTICATION_2_RESPONSE; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint8_t idi[8]; // probably issuer ID
		uint8_t pmi[8]; // probably issuer parameters
	};
}

# endif /*LIBFREELICA_AUTHENTICATION_2_RESPONSE_COMMAND_H*/
