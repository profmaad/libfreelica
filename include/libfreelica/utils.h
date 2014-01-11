# ifndef LIBFREELICA_UTILS_H
# define LIBFREELICA_UTILS_H

# include <string>
# include <iomanip>

# include <stdint.h>

# include <libfreelica/constants.h>

namespace FreeliCa
{
	std::string bytes_to_hex(const uint8_t *bytes, const unsigned int size, int bytes_per_line = -1, int number_of_tags = -1);
	void print_hex(const uint8_t *data, const unsigned int size);

	uint16_t le_8to16(uint8_t first, uint8_t second);
	uint16_t be_8to16(uint8_t first, uint8_t second);
	uint16_t htole_16(uint16_t host_ordered);
	uint16_t htobe_16(uint16_t host_ordered);
	uint16_t betole_16(uint16_t host_ordered);
	uint16_t letobe_16(uint16_t host_ordered);

	uint16_t decode_area_code(uint16_t raw_code, FreeliCa::AreaAttribute *attribute);
	uint16_t decode_service_code(uint16_t raw_code, FreeliCa::ServiceAttribute *attribute);
	uint16_t decode_node_code(uint16_t raw_code, bool *type, FreeliCa::AreaAttribute *area_attribute, FreeliCa::ServiceAttribute *service_attribute);
	std::string format_node_code(uint16_t raw_code);

	std::string format_bcd_version(uint16_t bcd_be);

	std::string format_status_flag_1(uint8_t status_flag_1);
	std::string format_status_flag_2(uint8_t status_flag_2);
	bool is_status_success(uint8_t status_flag_1, uint8_t status_flag_2);
}

# endif /*LIBFREELICA_UTILS_H*/
