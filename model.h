#include <gtk/gtk.h>

#define KB_ENTER        65293
#define KB_UP           65362
#define KB_DOWN         65364
#define KB_LEFT         65361
#define KB_RIGHT        65363
#define KB_TAB          65289
#define KB_SHIFT_TAB    65056
#define KB_SHIFT        65509

enum
{
	POINT_ID_COLUMN,
	POINT_X_COLUMN,
	POINT_Y_COLUMN,
	N_COLUMNS,
};

const gchar *editVal;
gint editing;
