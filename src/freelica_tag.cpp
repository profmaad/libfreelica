# include <iostream>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>
# include <cstdarg>

# include <nfc/nfc.h>

# include <libfreelica/commands.h>
# include <libfreelica/utils.h>

# include <libfreelica/freelica_tag.h>

# define LIBNFC_LIST_MAX_TARGETS 16
# define LIBFREELICA_FELICA_PAYLOAD_MAX_SIZE 256
# define LIBFREELICA_FELICA_BLOCK_SIZE 16

namespace FreeliCa
{
	const uint8_t FreeliCaTag::LIBNFC_FELICA_POLLING_DATA[] = {0x00, 0x80, 0x08, 0x01, 0x00};
	const nfc_modulation FreeliCaTag::LIBNFC_FELICA_MODULATION_212 = {
		.nmt = NMT_FELICA,
		.nbr = NBR_212,
	};
	const nfc_modulation FreeliCaTag::LIBNFC_FELICA_MODULATION_424 = {
		.nmt = NMT_FELICA,
		.nbr = NBR_424,
	};

	FreeliCaTag::FreeliCaTag(nfc_device *libnfc_device, nfc_target libnfc_target) :
		libnfc_device(libnfc_device),
		libnfc_target(libnfc_target)
	{
		memcpy((void*)idm, (void*)(libnfc_target.nti.nfi.abtId), 8);
	}
	FreeliCaTag::~FreeliCaTag()
	{
	}

	uint8_t* FreeliCaTag::getIDm()
	{
		return this->idm;
	}

	std::string FreeliCaTag::toString()
	{
		std::stringstream builder;

		builder << "FreeliCa Tag:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;

		return builder.str();
	}

	int FreeliCaTag::transceiveCommand(Command *command, Command **response)
	{
		unsigned int command_size = 0;
		uint8_t *encoded_command = command->encode(&command_size);
		if(command_size == 0 && encoded_command == NULL)
		{		
			return NFC_ESOFT;
		}
	
		// nfc_initiator_transceive_bytes requires the length to be prepended to the encoded command (as specified by the FeliCa data link layer)
		uint8_t *encoded_command_with_length = (uint8_t*)malloc(command_size+1);
		if(!encoded_command_with_length)
		{
			free(encoded_command);
			return NFC_ESOFT;
		}
		encoded_command_with_length[0] = (uint8_t)(command_size+1); // command length includes the length byte itself, hence "+1"
		memcpy(encoded_command_with_length+1, encoded_command, command_size);
		free(encoded_command);

		uint8_t response_buffer[LIBFREELICA_FELICA_PAYLOAD_MAX_SIZE];
		int response_size = 0;

		response_size = nfc_initiator_transceive_bytes(libnfc_device, encoded_command_with_length, command_size+1, response_buffer, 256, 0);
		free(encoded_command_with_length);

		if(response_size < 0)
		{
			// libnfc error
			return response_size;
		}

		Command *response_command = Command::create(response_buffer+1, response_size-1);
		*response = response_command;

		return NFC_SUCCESS;
	}
	Command* FreeliCaTag::executeCommand(Command *command)
	{
		int transceive_result = -1;
		Command *response = NULL;

		transceive_result = transceiveCommand(command, &response);

		if(transceive_result == NFC_SUCCESS && response != NULL)
		{
			return response;
		}
		else
		{
			delete response;

			return NULL;
		}
	}

	uint8_t* FreeliCaTag::readWithoutEncryption(uint16_t service_code, unsigned int number_of_blocks, ...)
	{
		std::list<ReadWithoutEncryptionCommand::ReadBlockListElement*> block_list;
		
		va_list block_numbers;
		va_start(block_numbers, number_of_blocks);
		for(int i = 0; i < number_of_blocks; i++)
		{
			ReadWithoutEncryptionCommand::ReadBlockListElement *ble = new ReadWithoutEncryptionCommand::ReadBlockListElement();
			ble->service_code = service_code;
			ble->is_purse_cashback_access = false;
			ble->block_number = (uint16_t)va_arg(block_numbers, unsigned int);

			block_list.push_back(ble);
		}
		va_end(block_numbers);

		ReadWithoutEncryptionResponseCommand *read_without_encryption_response = readWithoutEncryption(block_list);
		uint8_t *blocks = NULL;
		if(read_without_encryption_response && read_without_encryption_response->isSuccessful())
		{
			
			blocks = (uint8_t*)malloc(LIBFREELICA_FELICA_BLOCK_SIZE*read_without_encryption_response->getNumberOfBlocks());
			if(blocks)
			{
				for(int i = 0; i < read_without_encryption_response->getNumberOfBlocks(); i++)
				{
					memcpy((void*)(blocks+LIBFREELICA_FELICA_BLOCK_SIZE*i), (void*)read_without_encryption_response->getBlock(i), LIBFREELICA_FELICA_BLOCK_SIZE);
				}
			}
		}

		delete read_without_encryption_response;
		for(std::list<ReadWithoutEncryptionCommand::ReadBlockListElement*>::iterator i = block_list.begin(); i != block_list.end(); i++)
		{
			delete *i;
		}

		return blocks;
	}
	ReadWithoutEncryptionResponseCommand* FreeliCaTag::readWithoutEncryption(const std::list<ReadWithoutEncryptionCommand::ReadBlockListElement*> &abstract_block_list)
	{
		ReadWithoutEncryptionCommand read_without_encryption_command(idm, abstract_block_list);

		ReadWithoutEncryptionResponseCommand *read_without_encryption_response = dynamic_cast<ReadWithoutEncryptionResponseCommand*>(executeCommand(&read_without_encryption_command));
		return read_without_encryption_response;
	}


	int FreeliCaTag::libfreelica_get_tag(nfc_device *libnfc_device, FreeliCaTag **tag)
	{
		nfc_target libnfc_target;

		int libnfc_result = nfc_initiator_select_passive_target(libnfc_device, LIBNFC_FELICA_MODULATION_424, LIBNFC_FELICA_POLLING_DATA, sizeof(LIBNFC_FELICA_POLLING_DATA), &libnfc_target);
		if(libnfc_result <= 0)
		{
			libnfc_result = nfc_initiator_select_passive_target(libnfc_device, LIBNFC_FELICA_MODULATION_212, LIBNFC_FELICA_POLLING_DATA, sizeof(LIBNFC_FELICA_POLLING_DATA), &libnfc_target);
		}

		if(libnfc_result > 0)
		{
			FreeliCaTag *freelica_tag = new FreeliCaTag(libnfc_device, libnfc_target);

			*tag = freelica_tag;
		}

		return libnfc_result;
	}

	int FreeliCaTag::libfreelica_get_tags(nfc_device *libnfc_device, FreeliCaTag **tags)
	{
		nfc_target libnfc_targets[LIBNFC_LIST_MAX_TARGETS];

		uint8_t seen_424_idms[LIBNFC_LIST_MAX_TARGETS*8];
		unsigned int number_of_424_tags = 0;

		FreeliCaTag *freelica_tags[LIBNFC_LIST_MAX_TARGETS*2];
		unsigned int number_of_tags = 0;

		int libnfc_result = nfc_initiator_list_passive_targets(libnfc_device, LIBNFC_FELICA_MODULATION_424, libnfc_targets, LIBNFC_LIST_MAX_TARGETS);
		if(libnfc_result > 0)
		{
			for(int i = 0; i < libnfc_result; i++)
			{
				memcpy((void*)seen_424_idms[number_of_424_tags*8], (void*)(libnfc_targets[i].nti.nfi.abtId), 8);
				
				number_of_424_tags++;

				freelica_tags[number_of_tags] = new FreeliCaTag(libnfc_device, libnfc_targets[i]);
				number_of_tags++;
			}
		}

		libnfc_result = nfc_initiator_list_passive_targets(libnfc_device, LIBNFC_FELICA_MODULATION_212, libnfc_targets, LIBNFC_LIST_MAX_TARGETS);
		if(libnfc_result > 0)
		{
			for(int i = 0; i < libnfc_result; i++)
			{
				bool already_seen = false;
				for(int c = 0; c < number_of_424_tags; c++)
				{
					if(memcmp((void*)seen_424_idms[c*8], (void*)libnfc_targets[i].nti.nfi.abtId, 8) == 0)
					{
						already_seen = true;
						break;
					}
				}
				if(already_seen) { continue; }

				freelica_tags[number_of_tags] = new FreeliCaTag(libnfc_device, libnfc_targets[i]);
				number_of_tags++;
			}
		}

		*tags = (FreeliCaTag*)malloc(sizeof(FreeliCaTag*)*number_of_tags);
		if(!(*tags))
		{
			return NFC_ESOFT;
		}
		memcpy(*tags, freelica_tags, sizeof(FreeliCaTag*)*number_of_tags);

		if(number_of_tags > 0)
		{
			return number_of_tags;
		}
		else
		{
			return libnfc_result;
		}
	}
}
