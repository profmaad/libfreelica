# ifndef LIBFREELICA_REQUEST_SERVICE_RESPONSE_COMMAND_H
# define LIBFREELICA_REQUEST_SERVICE_RESPONSE_COMMAND_H

# include <string>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class RequestServiceResponseCommand : public Command
	{
	public:
		RequestServiceResponseCommand(const uint8_t *payload, const unsigned int payload_size);
		~RequestServiceResponseCommand();

		std::string toString();

		CommandCode code() { return REQUEST_SERVICE_RESPONSE; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint8_t number_of_nodes;
		uint16_t *node_key_version_list;
	};
}

# endif /*LIBFREELICA_REQUEST_SERVICE_RESPONSE_COMMAND_H*/
