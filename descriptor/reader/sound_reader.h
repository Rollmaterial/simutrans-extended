/*
 * This file is part of the Simutrans-Extended project under the Artistic License.
 * (see LICENSE.txt)
 */

#ifndef DESCRIPTOR_READER_SOUND_READER_H
#define DESCRIPTOR_READER_SOUND_READER_H


#include "obj_reader.h"


class sound_reader_t : public obj_reader_t {
	static sound_reader_t the_instance;

	sound_reader_t() { register_reader(); }
protected:
	/// @copydoc obj_reader_t::register_obj
	void register_obj(obj_desc_t *&desc) OVERRIDE;

public:
	static sound_reader_t*instance() { return &the_instance; }

	/// @copydoc obj_reader_t::read_node
	obj_desc_t *read_node(FILE *fp, obj_node_info_t &node) OVERRIDE;

	obj_type get_type() const OVERRIDE { return obj_sound; }
	char const* get_type_name() const OVERRIDE { return "sound"; }
};

#endif
