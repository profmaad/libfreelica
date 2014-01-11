# ifndef LIBFREELICA_READ_WITHOUT_ENCRYPTION_REPSONSE_COMMAND_H
# define LIBFREELICA_READ_WITHOUT_ENCRYPTION_REPSONSE_COMMAND_H

# include <string>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class ReadWithoutEncryptionResponseCommand : public Command
	{
	public:
		ReadWithoutEncryptionResponseCommand(const uint8_t *payload, const unsigned int payload_size);
		~ReadWithoutEncryptionResponseCommand();

		std::string toString();

		CommandCode code() { return READ_WITHOUT_ENCRYPTION_RESPONSE; }

		bool isSuccessful();
		uint8_t getNumberOfBlocks();
		uint8_t* getBlock(uint8_t block_number);

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint8_t status_flag_1;
		uint8_t status_flag_2;
		uint8_t number_of_blocks;
		uint8_t *block_data;
	};
}

# endif /*LIBFREELICA_READ_WITHOUT_ENCRYPTION_RESPONSE_COMMAND_H*/
