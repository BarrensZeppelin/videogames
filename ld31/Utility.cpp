#include "Utility.hpp"


void CenterOrigin(bzsf::Drawable& drawable) {
	auto localBounds = drawable.getLocalBounds();
	drawable.setOrigin(localBounds.width / 2, localBounds.height / 2);
}

void CenterOrigin(sf::Sprite& sprite) {
	auto localBounds = sprite.getLocalBounds();
	sprite.setOrigin(localBounds.width / 2, localBounds.height / 2);
}

void CenterOrigin(sf::Text& text) {
	auto localBounds = text.getLocalBounds();
	text.setOrigin(localBounds.width / 2, localBounds.height / 2);
}

void CenterOrigin(sf::Shape& shape) {
	auto localBounds = shape.getLocalBounds();
	shape.setOrigin(localBounds.width / 2, localBounds.height / 2);
}


float Length(sf::Vector2f vector) {
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float PowLength(sf::Vector2f vector) {
	return vector.x * vector.x + vector.y * vector.y;
}

sf::Vector2f UnitVector(sf::Vector2f vector) {
	return vector / Length(vector);
}



// Skrevet ud fra pseudokode: http://en.wikipedia.org/wiki/Bresenham's_line_algorithm#Optimization
std::vector<sf::Vector2i> Bresenhams(sf::Vector2i start, sf::Vector2i end) {
	std::vector<sf::Vector2i> points;

	bool steep = abs(end.y - start.y) > abs(end.x - start.x);
	if(steep) {
		std::swap(start.x, start.y);
		std::swap(end.x, end.y);
	}

	bool reversed = false;
	if(start.x > end.x) {
		reversed = true;
		std::swap(start.x, end.x);
		std::swap(start.y, end.y);
	}

	sf::Int32 deltax = end.x - start.x;
	sf::Int32 deltay = abs(end.y - start.y);
	sf::Int32 error = deltax / 2;
	sf::Int8 ystep = (start.y < end.y ? 1 : -1);

	sf::Int32 y = start.y;
	for(sf::Int32 x = start.x; x <= end.x; ++x) {
		if(steep) { points.push_back(sf::Vector2i(y, x)); } else { points.push_back(sf::Vector2i(x, y)); }

		error -= deltay;
		if(error < 0) {
			y += ystep;
			error += deltax;
		}
	}

	// Preserve the ordering, that points near the <start> are first in the vector
	if(reversed) {
		std::reverse(points.begin(), points.end());
	}

	return points;
}