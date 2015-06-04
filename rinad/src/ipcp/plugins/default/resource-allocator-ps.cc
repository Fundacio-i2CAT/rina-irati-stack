//
// Default policy set for Resource Allocator
//
//    Eduard Grasa <eduard.grasa@i2cat.net>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#define IPCP_MODULE "resource-allocator-ps-default"
#include "../../ipcp-logging.h"

#include <string>

#include "ipcp/components.h"

namespace rinad {

class DefaultPDUFTGeneratorPs: public IPDUFTGeneratorPs {
public:
	DefaultPDUFTGeneratorPs(IResourceAllocator * ra);
	void routingTableUpdated(const std::list<rina::RoutingTableEntry*>& routing_table);
	int set_policy_set_param(const std::string& name, const std::string& value);
	virtual ~DefaultPDUFTGeneratorPs() {}

private:
        // Data model of the resource allocator component.
        IResourceAllocator * res_alloc;
};

DefaultPDUFTGeneratorPs::DefaultPDUFTGeneratorPs(IResourceAllocator * ra) : res_alloc(ra)
{ }

void DefaultPDUFTGeneratorPs::routingTableUpdated(
		const std::list<rina::RoutingTableEntry*>& rt)
{
	LOG_IPCP_DBG("Got %d entries in the routing table", rt.size());
	//Compute PDU Forwarding Table
	std::list<rina::PDUForwardingTableEntry *> pduft;
	std::list<rina::RoutingTableEntry *>::const_iterator it;
	rina::PDUForwardingTableEntry * entry;
	int port_id = 0;
	for (it = rt.begin(); it!= rt.end(); ++it){
		entry = new rina::PDUForwardingTableEntry();
		entry->address = (*it)->address;
		entry->qosId = (*it)->qosId;

		LOG_IPCP_DBG("Processing entry for destination %u", (*it)->address);
		LOG_IPCP_DBG("Next hop address %u", (*it)->nextHopAddresses.front());

		port_id = res_alloc->get_n_minus_one_flow_manager()->
				getManagementFlowToNeighbour((*it)->nextHopAddresses.front());

		if (port_id == -1) {
			delete entry;
		} else {
			LOG_IPCP_DBG("N-1 port-id: %u", port_id);
			entry->portIdAltlists.push_back(rina::PortIdAltlist(port_id));
			pduft.push_back(entry);
		}
	}

	try {
		rina::kernelIPCProcess->modifyPDUForwardingTableEntries(pduft, 2);
	} catch (rina::Exception & e) {
		LOG_IPCP_ERR("Error setting PDU Forwarding Table in the kernel: %s",
				e.what());
	}
}

int DefaultPDUFTGeneratorPs::set_policy_set_param(const std::string& name,
                                            	  const std::string& value)
{
        LOG_IPCP_DBG("No policy-set-specific parameters to set (%s, %s)",
                        name.c_str(), value.c_str());
        return -1;
}

extern "C" rina::IPolicySet *
createPDUFTGenPs(rina::ApplicationEntity * ctx)
{
	IResourceAllocator * ra = dynamic_cast<IResourceAllocator *>(ctx);

	if (!ra) {
		return NULL;
	}

	return new DefaultPDUFTGeneratorPs(ra);
}

extern "C" void
destroyPDUFTGenPs(rina::IPolicySet * ps)
{
        if (ps) {
                delete ps;
        }
}

}
