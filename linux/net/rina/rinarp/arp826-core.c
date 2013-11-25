/*
 * ARP 826 (wonnabe) core
 *
 *    Sander Vrijders       <sander.vrijders@intec.ugent.be>
 *    Francesco Salvestrini <f.salvestrini@nextworks.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/if_ether.h>

/* FIXME: The following dependencies have to be removed */
#define RINA_PREFIX "arp826-core"

#include "logs.h"
#include "debug.h"
#include "utils.h"
/* FIXME: End of dependencies ... */

#include "arp826.h"
#include "arp826-utils.h"
#include "arp826-rxtx.h"
#include "arp826-arm.h"
#include "arp826-tables.h"

static struct packet_type arp826_packet_type __read_mostly = {
        .type = cpu_to_be16(ETH_P_ARP),
        .func = arp_receive,
};

static int protocol_add(struct net_device * device,
                        uint16_t            ptype,
                        size_t              hlen)
{
        LOG_DBG("Adding protocol (device = %pK, ptype = 0x%04X, hlen = %zd)",
                device, ptype, hlen);

        if (tbls_create(device, ptype, hlen)) {
                LOG_ERR("Cannot add (device = %pK, pype 0x%04X, hlen = %zd)",
                        device, ptype, hlen);
                return -1;
        }

        LOG_DBG("Protocol added successfully"
                "(device = %pK, ptype = 0x%04X, hlen = %zd)",
                device, ptype, hlen);

        return 0;
}

static void protocol_remove(struct net_device * device,
                            uint16_t            ptype)
{
        LOG_DBG("Removing protocol 0x%04X", ptype);

        tbls_destroy(device, ptype);
}

#ifdef CONFIG_ARP826_REGRESSION_TESTS
static bool regression_tests_gpa(void)
{
        struct gpa * a;
        struct gpa * b;
        struct gpa * c;
        uint8_t      name_tmp[] = { 0x01, 0x02, 0x03, 0x04 };
        size_t       len_a_1, len_a_2;
        size_t       len_b_1, len_b_2;
        uint8_t      gs_tmp[]   = {
                0x01, 0x02, 0x03, 0x04, 0x05,
                0x01, 0x02, 0x03, 0x04, 0x05,
                0x01, 0x02, 0x03, 0x04, 0x05,
                0x01, 0x02, 0x03, 0x04, 0x05,
                0x01, 0x02, 0x03, 0x04, 0x05,
                0x01, 0x02, 0x03, 0x04, 0x05
        };

        LOG_DBG("GPA regression tests");

        LOG_DBG("Regression test #1.1");
        a = gpa_create(name_tmp, sizeof(name_tmp));
        if (!a)
                return false;
        len_a_1 = gpa_address_length(a);

        LOG_DBG("Regression test #1.2");
        b = gpa_create(name_tmp, sizeof(name_tmp));
        if (!b)
                return false;
        len_b_1 = gpa_address_length(b);

        LOG_DBG("Regression test #2");
        if (!gpa_is_equal(a, b))
                return false;
        if (len_a_1 != len_b_1)
                return false;

        LOG_DBG("Regression test #3.1");
        if (gpa_address_grow(a, sizeof(name_tmp) * 2, 0xff))
                return false;
        len_a_2 = gpa_address_length(a);

        if (gpa_address_grow(b, sizeof(name_tmp) * 2, 0xff))
                return false;
        len_b_2 = gpa_address_length(b);

        if (!gpa_is_equal(a, b))
                return false;

        LOG_DBG("Regression test #3.2");
        if (len_a_2 != len_b_2)
                return false;

        LOG_DBG("Regression test #3.3");
        if (len_a_1 == len_a_2)
                return false;

        LOG_DBG("Regression test #3.4");
        if (len_b_1 == len_b_2)
                return false;

        LOG_DBG("Regression test #4.1");
        if (gpa_address_shrink(a, 0xff))
                return false;
        if (gpa_address_shrink(b, 0xff))
                return false;

        LOG_DBG("Regression test #4.2");
        if (gpa_address_length(a) != len_a_1)
                return false;
        LOG_DBG("Regression test #4.3");
        if (gpa_address_length(b) != len_b_1)
                return false;

        LOG_DBG("Regression test #5");
        if (!gpa_is_equal(a, a))
                return false;

        gpa_destroy(b);
        gpa_destroy(a);

        LOG_DBG("Regression test #6");

        LOG_DBG("Regression test #6.1");
        c = gpa_create(gs_tmp, sizeof(gs_tmp));
        if (!a)
                return false;

        LOG_DBG("Regression test #6.2");
        if (gpa_address_length(c) != 30)
                return false;

        LOG_DBG("Regression test #6.3");
        if (gpa_address_grow(c, 36, 0x00))
                return false;

        LOG_DBG("Regression test #6.4");
        if (gpa_address_length(c) != 36)
                return false;

        LOG_DBG("Regression test #6.5");
        if (gpa_address_shrink(c, 0x00))
                return false;

        LOG_DBG("Regression test #6.5");
        if (gpa_address_length(c) != 30)
                return false;

        gpa_destroy(c);

        return true;
}

static bool regression_tests_gha(void)
{
        uint8_t      mac_1[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
        struct gha * a;
        struct gha * b;
        uint8_t      mac_2[] = { 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };
        struct gha * c;

        LOG_DBG("GHA regression tests");

        LOG_DBG("Regression test #1");
        a = gha_create(MAC_ADDR_802_3, mac_1);
        if (!a)
                return false;
        b = gha_create(MAC_ADDR_802_3, mac_1);
        if (!b)
                return false;

        LOG_DBG("Regression test #2");
        if (!gha_is_equal(a, b))
                return false;

        LOG_DBG("Regression test #3");
        c = gha_create(MAC_ADDR_802_3, mac_2);
        if (!c)
                return false;
        if (gha_is_equal(a, c))
                return false;
        if (gha_is_equal(b, c))
                return false;

        LOG_DBG("Regression test #4");
        if (!gha_is_equal(c, c))
                return false;

        gha_destroy(a);
        gha_destroy(b);
        gha_destroy(c);

        return true;
}

#if 0
static bool regression_tests_table(void)
{
        struct table * x;

        LOG_DBG("Table regression tests");

        LOG_DBG("Regression test #1");
        if (tbls_init())
                return false;

        LOG_DBG("Regression test #2");

        LOG_DBG("Regression test #2.1");
        if (tbls_create(10, 3))
                return false;
        LOG_DBG("Regression test #2.2");
        if (tbls_create(21, 6))
                return false;
        LOG_DBG("Regression test #2.3");
        if (tbls_create(37, 31))
                return false;

        LOG_DBG("Regression test #3");

        LOG_DBG("Regression test #3.1");
        x = tbls_find(10);
        if (!x)
                return false;
        LOG_DBG("Regression test #3.2");
        x = tbls_find(21);
        if (!x)
                return false;
        LOG_DBG("Regression test #3.3");
        x = tbls_find(37);
        if (!x)
                return false;

        LOG_DBG("Regression test #4");

        LOG_DBG("Regression test #4.1");
        if (tbls_destroy(10))
                return false;
        LOG_DBG("Regression test #4.2");
        if (tbls_destroy(21))
                return false;
        LOG_DBG("Regression test #4.3");
        if (tbls_destroy(37))
                return false;

        LOG_DBG("Regression test #5");
        tbls_fini();

        return true;
}
#endif
#endif

#ifdef CONFIG_ARP826_REGRESSION_TESTS
static bool regression_tests(void)
{
        if (!regression_tests_gpa()) {
                LOG_ERR("GPA regression tests failed, bailing out");
                return false;
        }
        if (!regression_tests_gha()) {
                LOG_ERR("GHA regression tests failed, bailing out");
                return false;
        }
#if 0
        if (!regression_tests_table()) {
                LOG_ERR("Table regression tests failed, bailing out");
                return false;
        }
#endif
        return true;
}
#endif

static int __init mod_init(void)
{
        struct net_device * device;

#ifdef CONFIG_ARP826_REGRESSION_TESTS
        LOG_DBG("Starting regression tests");

        if (!regression_tests()) {
                return -1;
        }

        LOG_DBG("Regression tests completed successfully");
#endif

        LOG_DBG("Initializing");

        if (tbls_init())
                return -1;

        if (arm_init()) {
                tbls_fini();
                return -1;
        }

        /* FIXME: Replace with net-devices even-based behavior */
        read_lock(&dev_base_lock);
        device = first_net_device(&init_net);
        while (device) {
                if (protocol_add(device, ETH_P_RINA, 6)) {
                        tbls_fini();
                        arm_fini();
                        read_unlock(&dev_base_lock);
                        return -1;
                }

                device = next_net_device(device);
        }
        read_unlock(&dev_base_lock);

        dev_add_pack(&arp826_packet_type);

        LOG_DBG("Initialized successfully");

        return 0;
}

static void __exit mod_exit(void)
{
        struct net_device * device;

        LOG_DBG("Finalizing");

        dev_remove_pack(&arp826_packet_type);

        /* FIXME: Replace with net-devices even-based behavior */
        read_lock(&dev_base_lock);
        device = first_net_device(&init_net);
        while (device) {
                protocol_remove(device, ETH_P_RINA);
                
                device = next_net_device(device);
        }
        read_unlock(&dev_base_lock);

        arm_fini();
        tbls_fini();

        LOG_DBG("Finalized successfully");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_DESCRIPTION("Basic RFC 826 compliant ARP implementation");

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Francesco Salvestrini <f.salvestrini@nextworks.it>");
MODULE_AUTHOR("Sander Vrijders <sander.vrijders@intec.ugent.be>");
