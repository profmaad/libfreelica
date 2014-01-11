# ifndef LIBFREELICA_POLLING_RESPONSE_COMMAND_H
# define LIBFREELICA_POLLING_RESPONSE_COMMAND_H

# include <string>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

# define LIBFREELICA_MAXIMUM_RESPONSE_TIME_T 0.3020f

namespace FreeliCa
{
	class PollingResponseCommand : public Command
	{
		enum MaximumResponseTimeGroup
		{
			REQUEST_SERVICE_GROUP = 2,
			FIXED_GROUP = 3,
			AUTHENTICATION_GROUP = 4,
			DATA_READ_GROUP = 5,
			DATA_WRITE_GROUP = 6,
			ISSUANCE_GROUP = 7,
		};

	public:
		PollingResponseCommand(const uint8_t *payload, const unsigned int payload_size);
		~PollingResponseCommand();

		std::string toString();

		CommandCode code() { return POLLING_RESPONSE; }

		const uint8_t* getIDM() { return idm; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t maximumResponseTimeExponent(MaximumResponseTimeGroup group);
		uint8_t maximumResponseTimeRealA(MaximumResponseTimeGroup group);
		uint8_t maximumResponseTimeRealB(MaximumResponseTimeGroup group);
		std::string formatMaximumResponseTime(MaximumResponseTimeGroup group);

		uint8_t idm[8];
		uint8_t pmm[8];
		uint8_t request_data[2];
		bool request_data_received;

		uint8_t system_number;
		uint8_t manufacturer_code[2];
	};
}

# endif /*LIBFREELICA_POLLING_RESPONSE_COMMAND_H*/
