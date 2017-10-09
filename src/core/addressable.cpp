#include "src/core/include/addressable.h"

namespace Core
{
	Addressable::Addressable()
	{

	}

	/* @brief Slot will be called when the assigned virtual address has been changed.
	*
	*
	*/
	void Addressable::connect_slot_virtual_address_changed(std::function<void(int64_t)> const& slot)
	{
		virtual_address_changed_signal.connect(slot);
	}

	/* @brief Update assigned virtual address
	*  Will update the slots connected to "virtual_address_changed_signal".
	*
	*  @param virtual_address the new virtual address.
	*  @throws std::invalid_argument if virtual_address is smaller than zero.
	*
	*/
	void Addressable::update_virtual_address(int64_t virtual_address)
	{
		if (virtual_address < 0) throw std::invalid_argument("virtual address < 0");

		_virtual_address = virtual_address;

		virtual_address_changed_signal(_virtual_address);
	}
}
