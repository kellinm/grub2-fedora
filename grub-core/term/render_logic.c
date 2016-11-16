/* render_logic.c - General supporting functionality for menus.  */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2003,2004,2005,2006,2007,2008,2009,2010  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/mm.h>
#include <grub/render_logic.h>

/* set up enum that we can map to the text in configuration file */
enum timeout_style {
  TIMEOUT_STYLE_UNSET,
  TIMEOUT_STYLE_MENU,
  TIMEOUT_STYLE_COUNTDOWN,
  TIMEOUT_STYLE_HIDDEN
};

struct timeout_style_name {
  const char *name;
  enum timeout_style style;
} timeout_style_list[] = {
  {"menu", TIMEOUT_STYLE_MENU},
  {"countdown", TIMEOUT_STYLE_COUNTDOWN},
  {"hidden", TIMEOUT_STYLE_HIDDEN},
  {NULL, 0}
};

static enum timeout_style chosen_timeout_style = TIMEOUT_STYLE_UNSET;

/* Set the timeout style per user configuration or to TIMEOUT_STYLE_MENU
   as default */
void grub_read_timeout_style_configuration()
{
  const char *timeout_style_configuration;
  struct timeout_style_name *style_name_ptr;

  /* short circuit if this is already done */
  if (chosen_timeout_style != TIMEOUT_STYLE_UNSET)
    return;

  /* set the default up front */
  chosen_timeout_style = TIMEOUT_STYLE_MENU;

  /* read the configuration file */
  timeout_style_configuration = grub_env_get ("timeout_style");
  if (!timeout_style_configuration)
    return;

  /* check for user configured timeout style */
  for (style_name_ptr = timeout_style_list; style_name_ptr->name; style_name_ptr++)
    if (grub_strcmp (style_name_ptr->name, timeout_style_configuration) == 0)
      chosen_timeout_style = style_name_ptr->style;

  return;
};

int grub_should_render_menu(void)
{
  grub_read_timeout_style_configuration();

  if (chosen_timeout_style == TIMEOUT_STYLE_COUNTDOWN
      || chosen_timeout_style == TIMEOUT_STYLE_HIDDEN)
    return 0;

  return 1;
};

int grub_should_render_countdown_only(void)
{
  grub_read_timeout_style_configuration();

  if (chosen_timeout_style == TIMEOUT_STYLE_COUNTDOWN)
    return 1;

  return 0;
}
