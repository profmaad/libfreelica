# ifndef LIBFREELICA_READ_WITHOUT_ENCRYPTION_COMMAND_H
# define LIBFREELICA_READ_WITHOUT_ENCRYPTION_COMMAND_H

# include <string>

# include <stdint.h>

# include <libfreelica/constants.h>
# include <libfreelica/commands/command.h>

namespace FreeliCa
{
	class ReadWithoutEncryptionCommand : public Command
	{
	public:
		struct ReadBlockListElement
		{
			uint16_t service_code;
			uint16_t block_number;
			bool is_purse_cashback_access;
		};

		ReadWithoutEncryptionCommand(const uint8_t *payload, const unsigned int payload_size);
		ReadWithoutEncryptionCommand(const uint8_t *idm, const std::list<ReadBlockListElement*> &abstract_block_list);
		~ReadWithoutEncryptionCommand();

		std::string toString();

		CommandCode code() { return READ_WITHOUT_ENCRYPTION; }

		uint8_t* encode(unsigned int *size);

	protected:
		void parse(const uint8_t *payload, const unsigned int payload_size);

		uint8_t idm[8];
		uint8_t number_of_services;
		uint16_t *service_code_list;
		uint8_t number_of_blocks;
		uint8_t *block_headers_list;
		uint16_t *block_list;
	};
}

# endif /*LIBFREELICA_READ_WITHOUT_ENCRYPTION_COMMAND_H*/
