#include "src/core/include/addressable.h"

namespace Core
{
	VirtualAddressChangedSignal::VirtualAddressChangedSignal():
		_virtual_address(0)
	{

	}

	/* @brief Slot will be called when the assigned virtual address has been changed.
	*
	*/
	void VirtualAddressChangedSignal::connect_slot_virtual_address_changed(std::function<void(int64_t)> const& slot)
	{
		_virtual_address_changed_signal.connect(slot);
	}

	/* @brief Update assigned virtual address
	*  Will update the slots connected to "virtual_address_changed_signal".
	*  If there is no change (updating with the existing value) the slots are not updated.
	*
	*  @param virtual_address the new virtual address.
	*  @throws std::invalid_argument if virtual_address is smaller than zero.
	*
	*  @TODO: Can check here for alignment?
	*/
	void VirtualAddressChangedSignal::update_virtual_address(int64_t virtual_address)
	{
		//if (virtual_address % destination_bus_size) != 0
		if (virtual_address == _virtual_address) return;
		if (virtual_address < 0) throw std::invalid_argument("virtual address < 0");

		_virtual_address = virtual_address;

		_virtual_address_changed_signal(_virtual_address);
	}
}
