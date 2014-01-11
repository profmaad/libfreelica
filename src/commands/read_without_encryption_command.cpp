# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>
# include <list>
# include <vector>
# include <algorithm>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/read_without_encryption_command.h>

namespace FreeliCa
{
	ReadWithoutEncryptionCommand::ReadWithoutEncryptionCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		number_of_services = 0x0;
		service_code_list = NULL;
		number_of_blocks = 0x0;
		block_list = NULL;
		block_headers_list = NULL;

		parse(payload, payload_size);
	}
	ReadWithoutEncryptionCommand::ReadWithoutEncryptionCommand(const uint8_t *idm, const std::list<ReadBlockListElement*> &abstract_block_list) : Command(READ_WITHOUT_ENCRYPTION)
	{
		memcpy((void*)(this->idm), (void*)idm, 8);
		
		number_of_services = 0x0;
		service_code_list = NULL;

		number_of_blocks = abstract_block_list.size();
		block_list = (uint16_t*)malloc(2*number_of_blocks);
		block_headers_list = (uint8_t*)malloc(number_of_blocks);
		if(!block_list || !block_headers_list) { return; }

		std::vector<uint16_t> abstract_service_code_list;

		int block_position = 0;
		for(std::list<ReadBlockListElement*>::const_iterator i = abstract_block_list.begin(); i != abstract_block_list.end(); i++)
		{
			ReadBlockListElement *block_list_element = *i;

			std::vector<uint16_t>::iterator service_code_position_iterator = std::find(abstract_service_code_list.begin(), abstract_service_code_list.end(), block_list_element->service_code);
			uint8_t service_code_position = 0;
			if(service_code_position_iterator == abstract_service_code_list.end())
			{
				abstract_service_code_list.push_back(block_list_element->service_code);
				service_code_position = (uint8_t)(abstract_service_code_list.size() - 1);
			}
			else
			{
				service_code_position = (uint8_t)(service_code_position_iterator - abstract_service_code_list.begin());
			}
			
			if(block_list_element->is_purse_cashback_access)
			{
				block_headers_list[block_position] = (0x01 << 4) | service_code_position;
			}
			else
			{
				block_headers_list[block_position] = service_code_position;
			}

			block_list[block_position] = block_list_element->block_number;
			block_position++;
		}

		number_of_services = abstract_service_code_list.size();
		service_code_list = (uint16_t*)malloc(2*number_of_services);
		if(!service_code_list) { return; }

		int service_code_position = 0;
		for(std::vector<uint16_t>::const_iterator i = abstract_service_code_list.begin(); i != abstract_service_code_list.end(); i++)
		{
			service_code_list[service_code_position] = *i;
			service_code_position++;
		}
	}
	ReadWithoutEncryptionCommand::~ReadWithoutEncryptionCommand()
	{
		free(service_code_list);
		free(block_list);
		free(block_headers_list);
	}

	std::string ReadWithoutEncryptionCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Read without Encryption:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;
		builder << "\tNumber of Services to read: " << (unsigned int)number_of_services << std::endl;
		if(number_of_services > 0)
		{
			builder << "\tServices to read:" << std::endl;
			for(int i = 0; i < number_of_services; i++)
			{
				builder << "\t\t"<< std::setw(2) << std::setfill('0') << i <<": ";
				builder << format_node_code(service_code_list[i]) << std::dec << std::setw(0) << std::setfill(' ') << std::endl;
			}
		}
		
		builder << "\tNumber of Blocks to read: " << (unsigned int)number_of_blocks << std::endl;
		if(number_of_blocks > 0)
		{
			builder << "\tBlocks to read: " << std::endl;
			for(int i = 0; i < number_of_blocks; i++)
			{
				unsigned int service_code_list_order = block_headers_list[i] & 0x0f;
				unsigned int access_mode = (block_headers_list[i] & 0x70) >> 4;

				builder << "\t\t" << std::setw(2) << std::setfill('0') << i <<": ";
				builder << "Service #" << service_code_list_order << ", Block #" << (unsigned int)block_list[i] << " (" << (access_mode == 1 ? "purse cashback access" : "r/w operation") << ")" << std::endl;
			}
		}

		return builder.str();
	}

	void ReadWithoutEncryptionCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 12) { return; }

		memcpy(idm, payload, 8);

		number_of_services = *(payload+8);		
		if(number_of_services < 1 || number_of_services > 16) { return; }
		free(service_code_list);
		service_code_list = (uint16_t*)malloc(number_of_services*2);

		for(int i = 0; i < number_of_services; i++)
		{			
			// data on the wire is little endian, but we want to store it as big endian (even on a LE system, since this is the way we parse it
			service_code_list[i] = le_8to16(*(payload+9+2*i+1), *(payload+9+2*i));
		}

		number_of_blocks = *(payload+9+2*number_of_services);		
		free(block_list);
		free(block_headers_list);
		block_list = (uint16_t*)malloc(2*number_of_blocks);
		block_headers_list = (uint8_t*)malloc(number_of_blocks);

		unsigned int payload_offset = 9 + 2*number_of_services + 1;
		for(int i = 0; i < number_of_blocks && payload_offset < payload_size; i++)
		{
			block_headers_list[i] = payload[payload_offset];

			if((block_headers_list[i] & 0x80) == 0)
			{
				// we store block number in native endianess in memory, it is little endian on the wire
				block_list[i] = le_8to16(*(payload+payload_offset+1), *(payload+payload_offset+2));
				payload_offset += 3;
			}
			else
			{
				block_list[i] |= le_8to16(*(payload+payload_offset+1), 0x00);
				payload_offset += 2;
			}
		}
	}


	uint8_t* ReadWithoutEncryptionCommand::encode(unsigned int *size)
	{
		// we always generate 3-byte block list elements, for simplicity

		*size = 1+8+1+2*number_of_services+1+3*number_of_blocks;
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);
		encoded_command[9] = number_of_services;
		for(int i = 0; i < number_of_services; i++)
		{
			uint16_t service_code_le = betole_16(service_code_list[i]);
			encoded_command[10+2*i] = (uint8_t)((service_code_le & 0xff00) >> 8);
			encoded_command[10+2*i+1] = (uint8_t)(service_code_le & 0x00ff);
		}
		encoded_command[10+2*number_of_services] = number_of_blocks;
		for(int i = 0; i < number_of_blocks; i++)
		{
			encoded_command[10+2*number_of_services+1+2*i] = block_headers_list[i];

			uint16_t block_list_element_le = htole_16(block_list[i]);
			encoded_command[10+2*number_of_services+1+2*i+1] = (uint8_t)((block_list_element_le & 0xff00) >> 8);
			encoded_command[10+2*number_of_services+1+2*i+2] = (uint8_t)(block_list_element_le & 0x00ff);
		}

		return encoded_command;
	}
}
