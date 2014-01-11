# ifndef LIBFREELICA_SEARCH_SERVICE_CODE_COMMAND_H
# define LIBFREELICA_SEARCH_SERVICE_CODE_COMMAND_H

# include <string>
# include <list>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class SearchServiceCodeCommand : public Command
	{
	public:
		SearchServiceCodeCommand(const uint8_t *payload, const unsigned int payload_size);
		SearchServiceCodeCommand(const uint8_t *idm, const uint16_t index);
		~SearchServiceCodeCommand();

		std::string toString();

		CommandCode code() { return SEARCH_SERVICE_CODE; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint16_t index;
	};
}

# endif /*LIBFREELICA_SEARCH_SERVICE_CODE_COMMAND_H*/
