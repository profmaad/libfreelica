# ifndef LIBFREELICA_REQUEST_SPECIFICATION_VERSION_COMMAND_H
# define LIBFREELICA_REQUEST_SPECIFICATION_VERSION_COMMAND_H

# include <string>
# include <list>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class RequestSpecificationVersionCommand : public Command
	{
	public:
		RequestSpecificationVersionCommand(const uint8_t *payload, const unsigned int payload_size);
		RequestSpecificationVersionCommand(const uint8_t *idm);
		~RequestSpecificationVersionCommand();

		std::string toString();

		CommandCode code() { return REQUEST_SPECIFICATION_VERSION; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
	};
}

# endif /*LIBFREELICA_REQUEST_SPECIFICATION_VERSION_COMMAND_H*/
