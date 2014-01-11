# ifndef LIBFREELICA_AUTHENTICATION_1_COMMAND_H
# define LIBFREELICA_AUTHENTICATION_1_COMMAND_H

# include <string>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class Authentication1Command : public Command
	{
	public:
		Authentication1Command(const uint8_t *payload, const unsigned int payload_size);
		~Authentication1Command();

		std::string toString();

		CommandCode code() { return AUTHENTICATION_1; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint8_t number_of_areas;
		uint16_t *area_code_list;
		uint8_t number_of_services;
		uint16_t *service_code_list;
		uint8_t challenge_1[8]; //m1c
	};
}

# endif /*LIBFREELICA_AUTHENTICATION_1_COMMAND_H*/
