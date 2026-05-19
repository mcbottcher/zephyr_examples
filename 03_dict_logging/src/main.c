/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 * Copyright (c) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <inttypes.h>
#include <zephyr/kernel.h>
#include <string.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

LOG_MODULE_REGISTER(hello_world, LOG_LEVEL_DBG);

static const int32_t sleep_time_ms = 3000;

int main(void)
{

	uint32_t count = 0;

	while (1) {

		LOG_INF("Here comes a log entry!");
		LOG_INF("The count is at %" PRId32, count++);

		k_msleep(sleep_time_ms);
	}

	return 0;
}
