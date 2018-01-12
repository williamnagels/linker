#pragma once
#include "src/include/core/locatable.h"

#include <boost/signals2/signal.hpp>

#include <vector>
namespace N_Core
{
	/*@brief An addressable entity is an object that will be 
	* assigned an address at some point to which it will be loaded when
	* the linked binary is ran.
	*
	* Other objects interested in updates of the assigned virtual address can
	* connect to the "virtual_address_changed_signal"-signal. This callback
	* will be called whenever the assigned address has been changed. 
	*
	* NOTE: ignoring the existance of physical address. It will always be set to equal
	* the virtual address.
	*
	*/
	class VirtualAddressChangedSignal
	{

	public:
		VirtualAddressChangedSignal();
		void connect_slot_virtual_address_changed(std::function<void(int64_t)> const& slot); ///< Connect for updates if the address changes
		void update_virtual_address(int64_t virtual_address); ///< Change the address; will update connected slots

	private:
		int64_t _virtual_address; ///< Destination address
		boost::signals2::signal<void(int64_t)> _virtual_address_changed_signal;
	};
}
