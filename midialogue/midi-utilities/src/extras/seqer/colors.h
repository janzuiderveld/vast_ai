#ifndef COLORS_INCLUDED
#define COLORS_INCLUDED

class Colors;

#include <QtWidgets>

class Colors: public QObject
{
	Q_OBJECT

public:
	static QColor shade(QColor base_color, int light_theme_lightness, int dark_theme_lightness);
	static QColor buttonShade(int light_theme_lightness, int dark_theme_lightness);
	static QColor textShade(int light_theme_lightness, int dark_theme_lightness);

private:
	Colors() {}
};

#endif
