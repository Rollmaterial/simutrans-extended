/*
 *  Copyright (c) 1997 - 2002 by Volker Meyer & Hansj�rg Malthaner
 *
 * This file is part of the Simutrans project under the artistic licence.
 */

#ifndef __SKIN_BESCH_H
#define __SKIN_BESCH_H

#include "../display/simimg.h"
#include "obj_besch_std_name.h"
#include "bildliste2d_besch.h"


/*
 *  Autor:
 *      Volker Meyer
 *
 *  Beschreibung:
 *      Skin ist im wesentlichen erstmal eine Bildliste.
 *
 *  Kindknoten:
 *	0   Name
 *	1   Copyright
 *	2   Image list
 */
class skin_besch_t : public obj_besch_std_name_t {
public:
	bild_besch_t const* get_image(int i) const { return get_child<bildliste_besch_t>(2)->get_image(i); }

	int get_count() const { return get_child<bildliste_besch_t>(2)->get_count(); }

	image_id get_bild_nr(int i) const
	{
		const bild_besch_t *image = get_image(i);
		return image != NULL ? image->get_nummer() : IMG_EMPTY;
	}
};

#endif
