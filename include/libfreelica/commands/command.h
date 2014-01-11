# ifndef LIBFREELICA_COMMAND_H
# define LIBFREELICA_COMMAND_H

# include <string>

# include <stdint.h>

# include <libfreelica/constants.h>

namespace FreeliCa
{
	class Command
	{
	public:
		static Command* create(const uint8_t *payload, const unsigned int payload_size);
		static Command* create(const uint8_t command_code, const uint8_t *payload, const unsigned int payload_size);

		Command(const uint8_t *payload, const unsigned int payload_size);
		Command(CommandCode command_code);
		~Command();

		virtual std::string toString();

		virtual CommandCode code() { return this->command_code; }
		void setCode(CommandCode code) { this->command_code = code; }

		virtual uint8_t* encode(unsigned int *size);

	protected:
		virtual void parse(const uint8_t *payload, const unsigned int payload_size);

		const uint8_t *payload;
		const unsigned int payload_size;
		CommandCode command_code;
	};
}

# endif /*LIBFREELICA_COMMAND_H*/
