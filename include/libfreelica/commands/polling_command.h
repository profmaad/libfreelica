# ifndef LIBFREELICA_POLLING_COMMAND_H
# define LIBFREELICA_POLLING_COMMAND_H

# include <string>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class PollingCommand : public Command
	{
	public:
		enum RequestCode
		{
			NO_REQUEST = 0x00,
			SYSTEM_CODE_REQUEST = 0x01,
			COMMUNICATION_PERFORMANCE_REQUEST = 0x02,
		};
		enum TimeSlots
		{
			TIMESLOTS_1  = 0x00,
			TIMESLOTS_2  = 0x01,
			TIMESLOTS_4  = 0x03,
			TIMESLOTS_8  = 0x07,
			TIMESLOTS_16 = 0x0f,
		};

		PollingCommand(const uint8_t *payload, const unsigned int payload_size);
		PollingCommand(const uint16_t system_code, const RequestCode request_code, const TimeSlots timeslots);
		~PollingCommand();

		std::string toString();

		CommandCode code() { return POLLING; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t system_code[2];
		uint8_t request_code;
		uint8_t time_slot;
	};
}

# endif /*LIBFREELICA_POLLING_COMMAND_H*/
