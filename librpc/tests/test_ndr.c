/*
 * Tests for librpc ndr functions
 *
 * Copyright (C) Catalyst.NET Ltd 2020
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 * from cmocka.c:
 * These headers or their equivalents should be included prior to
 * including
 * this header file.
 *
 * #include <stdarg.h>
 * #include <stddef.h>
 * #include <setjmp.h>
 *
 * This allows test applications to use custom definitions of C standard
 * library functions and types.
 *
 */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "librpc/ndr/libndr.h"

/*
 * Test NDR_PULL_NEED_BYTES integer overflow handling.
 */
static enum ndr_err_code wrap_NDR_PULL_NEED_BYTES(
	struct ndr_pull *ndr,
	uint32_t bytes) {

	NDR_PULL_NEED_BYTES(ndr, bytes);
	return NDR_ERR_SUCCESS;
}

static void test_NDR_PULL_NEED_BYTES(void **state)
{
	struct ndr_pull ndr = {0};
	enum ndr_err_code err;

	ndr.data_size = UINT32_MAX;
	ndr.offset = UINT32_MAX -1;

	/*
	 * This will not cause an overflow
	 */
	err = wrap_NDR_PULL_NEED_BYTES(&ndr, 1);
	assert_int_equal(NDR_ERR_SUCCESS, err);

	/*
	 * This will cause an overflow
	 * and (offset + n) will be less than data_size
	 */
	err = wrap_NDR_PULL_NEED_BYTES(&ndr, 2);
	assert_int_equal(NDR_ERR_BUFSIZE, err);
}

int main(int argc, const char **argv)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_NDR_PULL_NEED_BYTES),
	};

	cmocka_set_message_output(CM_OUTPUT_SUBUNIT);
	return cmocka_run_group_tests(tests, NULL, NULL);
}