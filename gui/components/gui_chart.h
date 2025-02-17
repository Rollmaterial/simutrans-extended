/*
 * This file is part of the Simutrans-Extended project under the Artistic License.
 * (see LICENSE.txt)
 */

#ifndef GUI_COMPONENTS_GUI_CHART_H
#define GUI_COMPONENTS_GUI_CHART_H


#include "../../simtypes.h"
#include "gui_component.h"
#include "../../tpl/slist_tpl.h"


enum chart_marker_t {
	none    = 0,
	square  = 1,
	cross   = 2,
	diamond = 3,
	round_box=4,
	MAX_CHART_MARKER_TYPE,
	MARKER_TYPE_MASK=127,
	draw_horizontal_line = 1<<7
};

static void display_chart_marker_rgb(scr_coord_val xp, scr_coord_val yp, PIXVAL color, chart_marker_t marker_type, bool dirty  CLIP_NUM_DEFAULT_ZERO)
{
	switch (marker_type)
	{
	case chart_marker_t::square:
		display_fillbox_wh_clip_rgb(xp, yp, 5, 5, color, true);
		break;
	case chart_marker_t::cross:
		display_direct_line_rgb(xp, yp, xp + 4, yp + 4, color);
		display_direct_line_rgb(xp + 4, yp, xp, yp + 4, color);
		break;
	case chart_marker_t::diamond:
		for (int j = 0; j < 5; j++) {
			display_vline_wh_clip_rgb(xp + j, yp + abs(2 - j), 5 - 2 * abs(2 - j), color, false);
		}
		break;
	case chart_marker_t::round_box:
		display_filled_roundbox_clip(xp, yp, 5, 5, color, true);
		break;
	case chart_marker_t::none:
	default:
		break;
	}
}

/**
 * Draws a group of curves.
 */
class gui_chart_t : public gui_component_t
{
public:
	// NOTE: KMPH and FORCE hacks drawing accuracy and should not be mixed with other types
	// CURVE TYPES
	enum chart_suffix_t { STANDARD = 0, MONEY, PERCENT, DISTANCE, KMPH, FORCE, PAX_KM, KG_KM, TON_KM, TON_KM_MAIL, KW,/* WATT,*/ TONNEN, TIME };

	/**
	 * Set background color. -1 means no background
	 */
	void set_background(FLAGGED_PIXVAL color);

	gui_chart_t();

	/**
	 * paint chart
	 */
	void draw(scr_coord offset) OVERRIDE;

	bool infowin_event(event_t const*) OVERRIDE;

	/**
	 * set dimension
	 */
	void set_dimension(int x, int y) {
		x_elements = x;
		y_elements = y;
	}

	/**
	 * Pointer to function which converts supplied values before use
	 */
	typedef sint64 (*convert_proc) (const sint64);

	/**
	 * Adds a curve to the graph
	 * @param color    color for this curve; default 0
	 * @param values   reference to values
	 * @param size     elements to skip before next valid entry (only useful in multidimensional arrays)
	 * @param offset   element to start with
	 * @param elements elements in values
	 * @param proc     conversion procedure to be applied to supplied values
	 * @returns curve's id
	 */
	uint32 add_curve(PIXVAL color, const sint64 *values, int size, int offset, int elements, int type, bool show, bool show_value, int precision, convert_proc proc=NULL, chart_marker_t marker=square);

	void remove_curves() { curves.clear(); }

	/**
	 * Hide a curve of the set
	 */
	void hide_curve(unsigned int id);

	/**
	 * Show a curve of the set
	 */
	void show_curve(unsigned int id);

	/**
	 * set starting value for x-axis of chart
	 * example: set_seed(1930) will make a graph starting in year 1930; use set_seed(-1) to display nothing
	 */
	void set_seed(int seed) { this->seed = seed; }

	// x-axis number increase factor. dx=2 then 0, 2, 4, 6...
	void set_x_axis_span(sint32 dx = 1) { x_axis_span = dx; }
	// X-axis boundary that aborts the curve drawing.
	void set_abort_display_x(uint8 abort_x = 0) { abort_display_x = abort_x; }

	void set_show_x_axis(bool yesno) { show_x_axis = yesno; }

	void set_show_y_axis(bool yesno) { show_y_axis = yesno; }

	// Whether the chart follows the left_to_right_graphs setting or not
	//  0: Does not follow the settings. the graph is right to left.
	// *1: Follow the settings (default). Out-of-range values are treated as 1.
	//  2: Does not follow the settings, but the graphs is left to right
	void set_ltr(uint8 value) { ltr = value; }

	int get_curve_count() { return curves.get_count(); }

	scr_size get_max_size() const OVERRIDE;

	scr_size get_min_size() const OVERRIDE;

	void set_min_size(scr_size);
private:
	void calc_gui_chart_values(sint64 *baseline, double *scale, char *, char *, int precision ) const;

	/*
	 * curve struct
	 */
	struct curve_t {
		PIXVAL color;
		const sint64 *values;
		int size;
		int offset;
		int elements;
		bool show;
		bool show_value;      // show first value of curve as number on chart?
		int type;             // 0 = standard, 1 = money, 2 = percent
		const char* suffix;
		chart_marker_t marker_type;
		int precision;        // how many numbers ...
		convert_proc convert; // procedure for converting supplied values before use
	};

	slist_tpl <curve_t> curves;

	int x_elements, y_elements;
	uint8 abort_display_x = 0;

	int seed;
	sint32 x_axis_span;

	scr_coord tooltipcoord;

	uint8 ltr;
	bool show_x_axis, show_y_axis;

	/**
	 * Background color, -1 for transparent background
	 */
	FLAGGED_PIXVAL background;

	// TODO do something smarter here
	scr_size min_size;
};

#endif
