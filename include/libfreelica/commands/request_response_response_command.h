# ifndef LIBFREELICA_REQUEST_RESPONSE_RESPONSE_COMMAND_H
# define LIBFREELICA_REQUEST_RESPONSE_RESPONSE_COMMAND_H

# include <string>
# include <list>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class RequestResponseResponseCommand : public Command
	{
	public:
		enum Mode {
			MODE_0 = 0x00,
			MODE_1 = 0x01,
			MODE_2 = 0x02,
			MODE_3 = 0x03,
		};

		RequestResponseResponseCommand(const uint8_t *payload, const unsigned int payload_size);
		~RequestResponseResponseCommand();

		std::string toString();

		CommandCode code() { return REQUEST_RESPONSE_RESPONSE; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint8_t mode;
	};
}

# endif /*LIBFREELICA_REQUEST_RESPONSE_RESPONSE_COMMAND_H*/
