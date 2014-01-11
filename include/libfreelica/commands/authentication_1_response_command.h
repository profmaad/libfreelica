# ifndef LIBFREELICA_AUTHENTICATION_1_REPSONSE_COMMAND_H
# define LIBFREELICA_AUTHENTICATION_1_REPSONSE_COMMAND_H

# include <string>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class Authentication1ResponseCommand : public Command
	{
	public:
		Authentication1ResponseCommand(const uint8_t *payload, const unsigned int payload_size);
		~Authentication1ResponseCommand();

		std::string toString();

		CommandCode code() { return AUTHENTICATION_1_RESPONSE; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint8_t challenge_1_response[8]; //m2c
		uint8_t challenge_2[8]; //m3c
	};
}

# endif /*LIBFREELICA_AUTHENTICATION_1_RESPONSE_COMMAND_H*/
