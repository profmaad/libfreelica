# ifndef LIBFREELICA_AUTHENTICATION_2_COMMAND_H
# define LIBFREELICA_AUTHENTICATION_2_COMMAND_H

# include <string>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class Authentication2Command : public Command
	{
	public:
		Authentication2Command(const uint8_t *payload, const unsigned int payload_size);
		~Authentication2Command();

		std::string toString();

		CommandCode code() { return AUTHENTICATION_2; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint8_t challenge_2_response[8]; //m4c
	};
}

# endif /*LIBFREELICA_AUTHENTICATION_2_COMMAND_H*/
