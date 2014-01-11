# include <iostream>

# include <cstdio>
# include <cstdlib>
# include <cmath>
# include <cstring>

# include <sys/types.h>

# include <nfc/nfc.h>

# include <libfreelica/libfreelica.h>

# define EXIT_NFC_INIT 1
# define EXIT_NFC_DEVICE_OPEN 2
# define EXIT_NFC_DEVICE_INITIATOR 3
# define EXIT_NFC_NO_TAG 4

int main(int argc, char **argv)
{
	nfc_context *nfc_context;
	nfc_device  *nfc_device;
	nfc_target  nfc_tag;
	FreeliCa::FreeliCaTag *freelica_tag;

	nfc_init(&nfc_context);
	if(nfc_context == NULL)
	{
		std::cerr << "Unable to initialize libnfc (malloc)" << std::endl;
		return EXIT_NFC_INIT;
	}

	const char *libnfc_version = nfc_version();
	std::cerr << "libnfc version: " << libnfc_version << std::endl;

	nfc_device = nfc_open(nfc_context, NULL);
	if(nfc_device == NULL)
	{
		std::cerr << "ERROR: Unable to open NFC device." << std::endl;
		return EXIT_NFC_DEVICE_OPEN;
	}

	nfc_device_set_property_bool(nfc_device, NP_HANDLE_CRC, true);
	nfc_device_set_property_bool(nfc_device, NP_HANDLE_PARITY, true);

	if(nfc_initiator_init(nfc_device) < 0)
	{
		nfc_perror(nfc_device, "nfc_initiator_init");
		return EXIT_NFC_DEVICE_INITIATOR;
	}
	std::cerr << "NFC reader '"<< nfc_device_get_name(nfc_device) << "' opened" << std::endl;

	if(FreeliCa::FreeliCaTag::libfreelica_get_tag(nfc_device, &freelica_tag) <= 0 || freelica_tag == NULL)
	{
		std::cerr << "No (NFC) Felica tag was found." << std::endl;
		std::cerr << "error returned was: " << nfc_strerror(nfc_device) << std::endl;
		return EXIT_NFC_NO_TAG;
	}
	else
	{
		std::cerr << freelica_tag->toString() << std::endl;
	}

	uint8_t *octopus_purse_block = freelica_tag->readWithoutEncryption(0x0117, 1, 0);
	if(octopus_purse_block)
	{
		uint32_t purse_data = (octopus_purse_block[0] << 24) + (octopus_purse_block[1] << 16) + (octopus_purse_block[2] << 8) + octopus_purse_block[3];
		uint32_t cashback_data = (octopus_purse_block[4] << 24) + (octopus_purse_block[5] << 16) + (octopus_purse_block[6] << 8) + octopus_purse_block[7];
		uint16_t execution_id = (octopus_purse_block[14] << 8) + octopus_purse_block[15];
		
		double purse_balance = static_cast<double>(-350 + purse_data)/10.0d;
		double cashback_balance = static_cast<double>(cashback_data)/10.0d;
		
		std::cerr << std::endl;
		std::cerr << "================================================" << std::endl;
		std::cerr << "Octopus Balance:" << std::endl;
		std::cerr << "\tCurrent Purse Balance: " << purse_balance << " HKD" << std::endl;
		std::cerr << "\tCashback Balance: " << cashback_balance << " HKD" << std::endl;
		std::cerr << "\tLast Execution ID: " << execution_id << std::endl;
		std::cerr << "\tUser Data: " << FreeliCa::bytes_to_hex(octopus_purse_block+8, 6) << std::endl;
		std::cerr << "================================================" << std::endl;
	}
	else
	{
		std::cerr << "Failed to read octopus balance." << std::endl;
	}

	nfc_close(nfc_device);
	nfc_exit(nfc_context);

	return 0;
}
