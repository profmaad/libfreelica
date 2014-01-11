# include <string>
# include <sstream>

# include <stdint.h>
# include <cstdio>

# include <libfreelica/utils.h>

namespace FreeliCa
{
	std::string bytes_to_hex(const uint8_t *bytes, const unsigned int size, int bytes_per_line, int number_of_tabs)
	{
		std::stringstream builder;

		for(int i = 0; i < size; i++)		
		{
			if(bytes_per_line > 0 && i > 0 && i % bytes_per_line == 0)
			{
				builder << std::endl;

				if(number_of_tabs > 0)
				{
					for(int i = 0; i < number_of_tabs; i++)
					{
						builder << "\t";
					}
				}
			}

			builder << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(bytes[i]) << (i+1 == size ? "" : " ") << std::setw(0);
		}

		return builder.str();
	}
	void print_hex(const uint8_t *data, const unsigned int size)
	{
		size_t position;
		for(position = 0; position < size; position++)
		{
			printf("%02x  ", data[position]);
		}
		printf("\n");
	}

// TODO: these have to adapt to host byte order - currently they assume little-endiann
	uint16_t le_8to16(uint8_t first, uint8_t second)
	{
		uint16_t result = 0x0000;
		result |= (uint16_t)first << 8;
		result |= (uint16_t)second;
		return result;
	}
	uint16_t be_8to16(uint8_t first, uint8_t second)
	{
		uint16_t result = 0x0000;
		result |= (uint16_t)second << 8;
		result |= (uint16_t)first;
		return result;
	}
	uint16_t htole_16(uint16_t host_ordered)
	{
		return host_ordered;
	}
	uint16_t htobe_16(uint16_t host_ordered)
	{
		return letobe_16(host_ordered);
	}
	uint16_t betole_16(uint16_t be_ordered)
	{
		// its just a 2-bytes swap, so direction doesn't matter
		return letobe_16(be_ordered);
	}
	uint16_t letobe_16(uint16_t le_ordered)
	{
		uint16_t be_ordered = 0x0000;
		be_ordered |= (le_ordered & 0x00ff) << 8;
		be_ordered |= (le_ordered & 0xff00) >> 8;

		return be_ordered;
	}

	uint16_t decode_area_code(uint16_t raw_code, AreaAttribute *attribute)
	{
		uint16_t area_code = raw_code;
		*attribute = (AreaAttribute)(raw_code & 0x003F);

		return area_code;
	}
	uint16_t decode_service_code(uint16_t raw_code, ServiceAttribute *attribute)
	{
		uint16_t service_code = raw_code;
		*attribute = (ServiceAttribute)(raw_code & 0x003F);

		return service_code;
	}
	uint16_t decode_node_code(uint16_t raw_code, bool *type, AreaAttribute *area_attribute, ServiceAttribute *service_attribute)
	{
		if((raw_code & 0x003F) == 0x0000 || (raw_code & 0x003F) == 0x0001)
		{
			*type = false;
			*service_attribute = (ServiceAttribute)0xFF;
			return decode_area_code(raw_code, area_attribute);
		}
		else
		{
			*type = true;
			*area_attribute = (AreaAttribute)0xFF;
			return decode_service_code(raw_code, service_attribute);
		}
	}
	std::string format_node_code(uint16_t raw_code)
	{
		std::stringstream builder;

		if(raw_code == 0xFFFF)
		{
			builder << "System";
			return builder.str();
		}
	
		bool type = false;
		AreaAttribute area_attribute;
		ServiceAttribute service_attribute;
		uint16_t code = decode_node_code(raw_code, &type, &area_attribute, &service_attribute);
	
		if(type == false)
		{
			builder << "Area ";
			builder << "0x" << std::hex << std::setw(4) << std::setfill('0') << code << std::dec << std::setw(0) << std::setfill(' ');
			builder << " (";
			switch(area_attribute)
			{
			case CAN_CREATE_SUBAREA:
				builder << "can create sub-area";
				break;
			case CANNOT_CREATE_SUBAREA:
				builder << "cannot create sub-area";
				break;
			default:
				builder << "unknown attributes";
			}
			builder << ")";
		}
		else
		{
			builder << "Service ";
			builder << "0x" << std::hex << std::setw(4) << std::setfill('0') << code << std::dec << std::setw(0) << std::setfill(' ');
			builder << " (";
			switch(service_attribute)
			{
			case RANDOM_SERVICE_RW_AUTH:
				builder << "random service, r/w, auth required";
				break;
			case RANDOM_SERVICE_RW_NOAUTH:
				builder << "random service, r/w, no auth required";
				break;
			case RANDOM_SERVICE_RO_AUTH:
				builder << "random service, ro, auth required";
				break;
			case RANDOM_SERVICE_RO_NOAUTH:
				builder << "random service, ro, no auth required";
				break;

			case CYCLIC_SERVICE_RW_AUTH:
				builder << "cyclic service, r/w, auth required";
				break;
			case CYCLIC_SERVICE_RW_NOAUTH:
				builder << "cyclic service, r/w, no auth required";
				break;
			case CYCLIC_SERVICE_RO_AUTH:
				builder << "cyclic service, ro, auth required";
				break;
			case CYCLIC_SERVICE_RO_NOAUTH:
				builder << "cyclic service, ro, no auth required";
				break;

			case PURSE_SERVICE_DIRECT_AUTH:
				builder << "purse service, direct access, auth required";
				break;
			case PURSE_SERVICE_DIRECT_NOAUTH:
				builder << "purse service, direct access, no auth required";
				break;
			case PURSE_SERVICE_CASHBACK_AUTH:
				builder << "purse service, cashback/decrement access, auth required";
				break;
			case PURSE_SERVICE_CASHBACK_NOAUTH:
				builder << "purse service, cashback/decrement access, no auth required";
				break;
			case PURSE_SERVICE_DECREMENT_AUTH:
				builder << "purse service, decrement access, auth required";
				break;
			case PURSE_SERVICE_DECREMENT_NOAUTH:
				builder << "purse service, decrement access, no auth required";
				break;
			case PURSE_SERVICE_RO_AUTH:
				builder << "purse service, readonly access, auth required";
				break;
			case PURSE_SERVICE_RO_NOAUTH:
				builder << "purse service, readonly access, no auth required";
				break;
			default:
				builder << "unknown attributes";
			}
			builder << ")";
		}
	
		return builder.str();
	}

	std::string format_bcd_version(uint16_t bcd_be)
	{
		std::stringstream builder;

		builder << (unsigned int)((bcd_be & 0x0f00) >> 16) << ".";
		builder << (unsigned int)((bcd_be & 0x00f0) >>  8) << ".";
		builder << (unsigned int)(bcd_be & 0x000f);

		return builder.str();
	}

	std::string format_status_flag_1(uint8_t status_flag_1)
	{
		std::stringstream builder;

		builder << bytes_to_hex(&status_flag_1, 1) << " ";
		switch(status_flag_1)
		{
		case 0x00:
			builder << "(success)";
			break;
		case 0xff:
			builder << "(failure, no location specified)";
			break;
		default:
			builder << "(failure at location "<<status_flag_1<<" of node list)";
			break;
		}

		return builder.str();
	}
	std::string format_status_flag_2(uint8_t status_flag_2)
	{
		std::stringstream builder;

		builder << bytes_to_hex(&status_flag_2, 1) << " (";
		switch(status_flag_2)
		{
		case SUCCESS:
			builder << "Success";
			break;
		case INVALID_PURSE_CHANGE:
			builder << "The calculated result is either less than zero when the purse data is decremented, or exceeds 4 Bytes when the purse data is incremented.";
			break;
		case INVALID_CASHBACK:
			builder << "The specified data exceeds the value of cashback data at cashback of purse.";
			break;
		case FATAL_MEMORY_ERROR:
			builder << "Memory error [fatal error].";
			break;
		case MEMORY_REWRITE_LIMIT_REACHED:
			builder << "The number of memory rewrites exceeds the upper limit (this is only a warning; data writing is performed as normal). The maximum number of rewrites can differ, depending on the product being used. In addition, Status Flag1 is either 00h or FFh depending on the product being used.";
			break;
					
		case ILLEGAL_SERVICE_NUMBER:
			builder << "Number of Service or Number of Node specified by the command falls outside the range of the prescribed value.";
			break;
		case ILLEGAL_COMMAND_PACKET:
			builder << "Number of Block specified by the command falls outside the range of the prescribed values for the product.";
			break;
		case ILLEGAL_BLOCK_LIST:
			builder << "Service Code List Order specified by Block List Element falls outside the Number of Service specified by the command (or the Number of Service specified at the times of mutual authentication).";
			break;
		case ILLEGAL_SERVICE_TYPE:
			builder << "Area Attribute specified by the command or Service Attribute of Service Code is incorrect.";
			break;
		case ACCESS_NOT_ALLOWED:
			builder << "Area or Service specified by the command cannot be accessed. The parameter specified by the command does not satisfy the conditions for success.";
			break;
		case ILLEGAL_SERVICE_CODE_LIST:
			builder << "Target to be accessed, identified by Service Code List Order, specified by Block List Element does not exist. Or, Node specified by Node Code List does not exist.";
			break;
		case ILLEGAL_BLOCK_LIST_ACCESS_MODE:
			builder << "Access Mode specified by Block List Element is incorrect.";
			break;
		case ILLEGAL_BLOCK_NUMBER:
			builder << "Block Number specified by Block List Element exceeds the number of Blocks assigned to Service.";
			break;
		default:
			builder << "unknown: " << bytes_to_hex(&status_flag_2, 1);
		}
		builder << ")";

		return builder.str();
	}
	bool is_status_success(uint8_t status_flag_1, uint8_t status_flag_2)
	{
		return (status_flag_1 == 0x00 && status_flag_2 == 0x00);
	}
}
