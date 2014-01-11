# ifndef LIBFREELICA_FREELICA_TAG_H
# define LIBFREELICA_FREELICA_TAG_H

# include <string>

# include <stdint.h>

# include <nfc/nfc.h>

namespace FreeliCa
{
	class FreeliCaTag
	{
		static const uint8_t LIBNFC_FELICA_POLLING_DATA[];

		static const nfc_modulation LIBNFC_FELICA_MODULATION_212;
		static const nfc_modulation LIBNFC_FELICA_MODULATION_424;

	public:
		static int libfreelica_get_tag(nfc_device *libnfc_device, FreeliCaTag **tag);
		static int libfreelica_get_tags(nfc_device *libnfc_device, FreeliCaTag **tags);

		FreeliCaTag(nfc_device *libnfc_device, nfc_target libnfc_target);
		~FreeliCaTag();

		std::string toString();
		uint8_t* getIDm();

		Command* executeCommand(Command *command);

		uint8_t* readWithoutEncryption(uint16_t service_code, unsigned int number_of_blocks, ...);
		ReadWithoutEncryptionResponseCommand* readWithoutEncryption(const std::list<ReadWithoutEncryptionCommand::ReadBlockListElement*> &abstract_block_list);
		
	protected:
		int transceiveCommand(Command *command, Command **response);

		nfc_device *libnfc_device;
		nfc_target libnfc_target;

		uint8_t idm[8];
	};
}
	
# endif /*LIBFREELICA_FREELICA_TAG_H*/
