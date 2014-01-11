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

	FreeliCa::PollingCommand *polling_command = new FreeliCa::PollingCommand(0xffff, FreeliCa::PollingCommand::COMMUNICATION_PERFORMANCE_REQUEST, FreeliCa::PollingCommand::TIMESLOTS_1);
	FreeliCa::PollingResponseCommand *polling_response = dynamic_cast<FreeliCa::PollingResponseCommand*>(freelica_tag->executeCommand(polling_command));
	if(polling_response)
	{
		std::cerr << polling_response->toString() << std::endl;
	}
	delete polling_command;
	delete polling_response;

	FreeliCa::RequestSystemCodeCommand *system_code_command = new FreeliCa::RequestSystemCodeCommand(freelica_tag->getIDm());
	FreeliCa::RequestSystemCodeResponseCommand *system_code_response = dynamic_cast<FreeliCa::RequestSystemCodeResponseCommand*>(freelica_tag->executeCommand(system_code_command));
	if(system_code_response)
	{
		std::cerr << system_code_response->toString() << std::endl;
	}
	delete system_code_command;
	delete system_code_response;

	std::cerr << "Compiling system inventory for current system:" << std::endl;
	uint16_t index = 0;
	while(true)
	{
		FreeliCa::SearchServiceCodeCommand *service_code_command = new FreeliCa::SearchServiceCodeCommand(freelica_tag->getIDm(), index);
		FreeliCa::SearchServiceCodeResponseCommand *service_code_response = dynamic_cast<FreeliCa::SearchServiceCodeResponseCommand*>(freelica_tag->executeCommand(service_code_command));

		bool search_done = false;
		if(service_code_response)
		{
			if(service_code_response->getServiceCode() != 0xffff)
			{
				std::cerr << "\t"<< std::setw(5) << std::setfill(' ') << index << ": " << FreeliCa::format_node_code(service_code_response->getServiceCode()) << std::endl;
			}
		}
		delete service_code_command;
		delete service_code_response;

		if(service_code_response->isSearchDone()) { break; }
		else
		{
			index++;
		}
	}
	std::cerr << std::endl;

	FreeliCa::RequestResponseCommand *request_response_command = new FreeliCa::RequestResponseCommand(freelica_tag->getIDm());
	FreeliCa::RequestResponseResponseCommand *request_response_response = dynamic_cast<FreeliCa::RequestResponseResponseCommand*>(freelica_tag->executeCommand(request_response_command));
	if(request_response_response)
	{
		std::cerr << request_response_response->toString() << std::endl;
	}
	delete request_response_command;
	delete request_response_response;

	delete freelica_tag;
	nfc_close(nfc_device);
	nfc_exit(nfc_context);

	return 0;
}
