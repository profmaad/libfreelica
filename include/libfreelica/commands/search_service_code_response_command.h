# ifndef LIBFREELICA_SEARCH_SERVICE_CODE_RESPONSE_COMMAND_H
# define LIBFREELICA_SEARCH_SERVICE_CODE_RESPONSE_COMMAND_H

# include <string>
# include <list>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class SearchServiceCodeResponseCommand : public Command
	{
	public:
		SearchServiceCodeResponseCommand(const uint8_t *payload, const unsigned int payload_size);
		~SearchServiceCodeResponseCommand();

		std::string toString();

		CommandCode code() { return SEARCH_SERVICE_CODE_RESPONSE; }

		uint16_t getServiceCode() { return service_code; }
		bool isSearchDone();

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint16_t service_code;
		uint16_t area_end_code;
		bool is_area;
	};
}

# endif /*LIBFREELICA_SEARCH_SERVICE_CODE_RESPONSE_COMMAND_H*/
