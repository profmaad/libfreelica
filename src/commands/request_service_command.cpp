# include <iostream>
# include <iomanip>
# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdlib>
# include <cstring>

# include <libfreelica/utils.h>

# include <libfreelica/commands/request_service_command.h>

namespace FreeliCa
{
	RequestServiceCommand::RequestServiceCommand(const uint8_t *payload, const unsigned int payload_size) : Command(payload, payload_size)
	{
		memset(idm, 0, 8);
		number_of_nodes = 0x0;
		node_code_list = NULL;

		parse(payload, payload_size);
	}
	RequestServiceCommand::RequestServiceCommand(const uint8_t *idm, std::list<uint16_t> &node_codes) : Command(REQUEST_SERVICE)
	{
		memcpy((void*)(this->idm), (void*)idm, 8);
		
		this->number_of_nodes = node_codes.size();
		this->node_code_list = (uint16_t*)malloc(number_of_nodes*2);
		if(node_code_list == NULL) { return; }
		
		int array_index = 0;
		for(std::list<uint16_t>::iterator i = node_codes.begin(); i != node_codes.end(); i++)
		{
			node_code_list[array_index++] = htobe_16(*i);
		}
	}
	RequestServiceCommand::~RequestServiceCommand()
	{
		free(node_code_list);
	}

	std::string RequestServiceCommand::toString()
		
	{
		std::stringstream builder;

		builder << "Request Service:" << std::endl;
		builder << "\tIDm: " << bytes_to_hex(idm, 8) << std::endl;
		builder << "\tNumber of Nodes in Request: " << (unsigned int)number_of_nodes << std::endl;
		if(number_of_nodes > 0)
		{
			builder << "\tRequested Nodes:" << std::endl;
			for(int i = 0; i < number_of_nodes; i++)
			{
				builder << "\t\t"<< std::setw(2) << std::setfill('0') << i <<": ";
				if(node_code_list[i] == 0xFFFF)
				{
					builder << "System" << std::endl;
				}
				else
				{
					builder << format_node_code(node_code_list[i]) << std::dec << std::setw(0) << std::setfill(' ') << std::endl;
				}
			}
		}

		return builder.str();
	}

	void RequestServiceCommand::parse(const uint8_t *payload, const unsigned int payload_size)
	{
		if(payload_size < 11) { return; }

		memcpy(idm, payload, 8);
		number_of_nodes = *(payload+8);
		
		if(number_of_nodes < 1 || number_of_nodes > 32) { return; }
		free(node_code_list);
		node_code_list = (uint16_t*)malloc(number_of_nodes*2);

		for(int i = 0; i < number_of_nodes; i++)
		{			
			// data on the wire is little endian, but we want to store it as big endian (even on a LE system, since this is the way we parse it
			node_code_list[i] = le_8to16(*(payload+9+2*i+1), *(payload+9+2*i));
		}
	}

	uint8_t* RequestServiceCommand::encode(unsigned int *size)
	{
		*size = 10+2*number_of_nodes;
		uint8_t *encoded_command = (uint8_t*)malloc(*size);
		if(!encoded_command)
		{
			*size = 0;
			return NULL;
		}

		encoded_command[0] = (uint8_t)code();
		memcpy(encoded_command+1, idm, 8);
		encoded_command[9] = number_of_nodes;
		for(int i = 0; i < number_of_nodes; i++)
		{
			uint16_t node_le = betole_16(node_code_list[i]);
			encoded_command[10+2*i] = (uint8_t)((node_le & 0xff00) >> 8);
			encoded_command[10+2*i+1] = (uint8_t)(node_le & 0x00ff);
		}

		return encoded_command;
	}
}
