/*
 * Copyright (c) 2025 Kenneth Lu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <zephyr/shell/shell.h>
#include <wisblock/wb_sensor.h>

static int cmd_wisblock_fint(const struct shell *sh, size_t argc, char **argv)
{
	uint16_t interval = atoi(argv[1]);

	wisblock_set_fetching_interval(interval);

	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_wisblock,
			       SHELL_CMD_ARG(fint, NULL,
					     "Sensor fetch interval\n"
					     " Usage: fint <second>",
					     cmd_wisblock_fint, 2, 0),
			       SHELL_SUBCMD_SET_END /* Array terminated. */
);

SHELL_CMD_REGISTER(wisblock, &sub_wisblock, "WisBlock commands", NULL);
