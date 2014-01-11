# ifndef LIBFREELICA_REQUEST_SPECIFICATION_VERSION_RESPONSE_COMMAND_H
# define LIBFREELICA_REQUEST_SPECIFICATION_VERSION_RESPONSE_COMMAND_H

# include <string>
# include <list>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class RequestSpecificationVersionResponseCommand : public Command
	{
	public:
		RequestSpecificationVersionResponseCommand(const uint8_t *payload, const unsigned int payload_size);
		~RequestSpecificationVersionResponseCommand();

		std::string toString();

		CommandCode code() { return REQUEST_SPECIFICATION_VERSION_RESPONSE; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint8_t status_flag_1;
		uint8_t status_flag_2;
		uint8_t format_version;
		uint16_t basic_version;
		uint8_t number_of_options;
		uint16_t *option_version_list;
	};
}

# endif /*LIBFREELICA_REQUEST_SPECIFICATION_VERSION_RESPONSE_COMMAND_H*/
