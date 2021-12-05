#include <bzsf/SFML_Snips.hpp>

void CenterOrigin(bzsf::Drawable& drawable);
void CenterOrigin(sf::Sprite& sprite);
void CenterOrigin(sf::Text& text);
void CenterOrigin(sf::Shape& shape);

float Length(sf::Vector2f vector);
float PowLength(sf::Vector2f vector);

sf::Vector2f UnitVector(sf::Vector2f vector);



std::vector<sf::Vector2i> Bresenhams(sf::Vector2i start, sf::Vector2i end);